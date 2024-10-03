
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <syslog.h>
#include <unistd.h>

#include <sensors/sensors.h>

#include "cputempd.h"

// from apue3
void daemonize(const char *name) {
    int fd0, fd1, fd2;
    pid_t pid;
    struct sigaction sa;
    struct rlimit rl;

    umask(0);

    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        fprintf(stderr, "%s: can't query resource limit", name);
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) < 0) {
        fprintf(stderr, "%s: can't fork", name);
        exit(EXIT_FAILURE);
    } else if (pid != 0) {
        exit(EXIT_SUCCESS);
    }

    setsid();

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        fprintf(stderr, "%s: can't ignore SIGHUP", name);
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) < 0) {
        fprintf(stderr, "%s: can't fork", name);
        exit(EXIT_FAILURE);
    } else if (pid != 0) {
        exit(EXIT_SUCCESS);
    }

    if (chdir("/") < 0) {
        fprintf(stderr, "%s: cant chdir to /", name);
        exit(EXIT_FAILURE);
    }

    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = FD_NUM_REASONABLE;
    for (int i = 0; i < rl.rlim_max; i++)
        close(i);
    
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(FD_STDOUT);
    fd2 = dup(FD_STDOUT);

    openlog(name, LOG_CONS, LOG_DAEMON);
    if (fd0 != FD_STDOUT || fd1 != FD_STDIN || fd2 != FD_STDERR) {
        syslog(LOG_ERR, "unexpected fd: %d %d %d", fd0, fd1, fd2);
        exit(EXIT_FAILURE);
    }
    syslog(LOG_INFO, "successfully daemonized!");
}

int sensors_search_cpu(const sensors_chip_name **ret_name, int *ret_subfeat_nr) {
    int result = CPU_SEARCH_FAIL;
    int do_search = CPU_SEARCH_GO;

    const sensors_chip_name *name;
    int n_chip = 0;
    const sensors_feature *feat;
    int n_feat = 0;
    const sensors_subfeature *subfeat;
    int n_subfeat = 0;
    char *label;

    while (((name = sensors_get_detected_chips(NULL, &n_chip)) != NULL) && do_search) {
//        syslog(LOG_INFO, "chip:\t%d\t%s\t%s", name->addr, name->path, name->prefix);
        while (((feat = sensors_get_features(name, &n_feat)) != NULL) && do_search) {
//            syslog(LOG_INFO, "feature:\t%s", feat->name);
            label = sensors_get_label(name, feat);
//            syslog(LOG_INFO, "label:\t%s", label);
            if (strncmp(label, CPU_SEARCH_0, CPU_SEARCH_LEN_0) == 0 || \
                    strncmp(label, CPU_SEARCH_1, CPU_SEARCH_LEN_1) == 0) {
                do_search = CPU_SEARCH_STOP;
                subfeat = sensors_get_subfeature(name, feat, CPU_TEMP_SUBFEAT);
                *ret_name = name;
                *ret_subfeat_nr = subfeat->number;
                result = CPU_SEARCH_SUCCESS;
//                syslog(LOG_INFO, "cpu temp subfeature number: %d", subfeat->number);
            }
            free(label);
            n_subfeat = 0;
        }
        n_feat = 0;
    }
    return result;
}

int main(int argc, char *argv[]) {
    daemonize(argv[0]);

    if (atexit(sensors_cleanup) != 0) {
        syslog(LOG_ERR, "couldn't register exit function");
        exit(EXIT_FAILURE);
    }

    if (sensors_init(NULL) != 0) {
        syslog(LOG_ERR, "sensors_init fail\n");
        exit(EXIT_FAILURE);
    }
    
    const sensors_chip_name *name;
    int subfeat_nr;
    if (sensors_search_cpu(&name, &subfeat_nr) != CPU_SEARCH_SUCCESS) {
        syslog(LOG_ERR, "couldn't find cpu temp");
        exit(EXIT_FAILURE);
    }

    if (access(FIFO_PATH, R_OK) != 0) {
        syslog(LOG_INFO, "creating %s", FIFO_PATH);
        if (mkfifo(FIFO_PATH, 0) != 0) {
            syslog(LOG_ERR, "couldn't open fifo at %s", FIFO_PATH);
            exit(EXIT_FAILURE);
        }
        chmod(FIFO_PATH, S_IWOTH); // :^)
    }

    pid_t pid;

    int fifo_fd = open(FIFO_PATH, O_RDWR); //RDWR to prevent closing when client disconnects
    char *fifo_pid_in = malloc(PIPE_BUF);
    pid_t client_pid;
    size_t client_pid_len;

    double temp = 0.0;

    // fifo and loop
    while (1) {
        syslog(LOG_INFO, "begin loop");

        client_pid_len = read(fifo_fd, fifo_pid_in, PIPE_BUF);
        client_pid = (pid_t) atol(fifo_pid_in);

        syslog(LOG_INFO, "recv pid %d", client_pid);

        if ((pid = fork()) < 0) {
            syslog(LOG_ERR, "fork error");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { //child
            syslog(LOG_INFO, "child created");

            sensors_get_value(name, subfeat_nr, &temp);
            char client_path[BUFSIZ];
            snprintf(client_path, BUFSIZ, FIFO_CLIENT_PATH"%d", client_pid);

            syslog(LOG_INFO, "child client path: %s", client_path);

            if (access(client_path, W_OK) != 0) {
                syslog(LOG_INFO, "creating %s", client_path);
                mkfifo(client_path, 0);
                chmod(client_path, S_IROTH);
            }

            int client_fifo_fd = open(client_path, O_WRONLY);
            char temp_str[TEMP_STR_LEN];
            int b_to_write = snprintf(temp_str, TEMP_STR_LEN, "%4.2f", temp);
            syslog(LOG_INFO, "cpu temp: %s (%f)", temp_str, temp);

            int write_bytes;
            if ((write_bytes = write(client_fifo_fd, temp_str, b_to_write)) != b_to_write) {
                syslog(LOG_ERR, "write error, %d bytes, %s", write_bytes, strerror(errno));
            }

            close(client_fifo_fd);

            syslog(LOG_INFO, "exiting child");
            exit(EXIT_SUCCESS);
        }
//        syslog(LOG_INFO, "waiting for child"); // shouldnt have to :)
//        wait(NULL);
        syslog(LOG_INFO, "parent end loop");
    }

    exit(EXIT_SUCCESS);
}

