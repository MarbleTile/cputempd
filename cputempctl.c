
#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cputempd.h"

void get_temp(int pid) {
    FILE *req_fifo = fopen(FIFO_PATH, "w");
    fprintf(req_fifo, "%d", pid);
    fclose(req_fifo);

    char res_fifo_path[BUFSIZ]; 
    snprintf(res_fifo_path, BUFSIZ, FIFO_CLIENT_PATH"%d", pid);

    char temp[BUFSIZ];

    pause();

    FILE *res_fifo = fopen(res_fifo_path, "r");
    if (res_fifo == NULL) {
        fprintf(stderr, "unable to open %s\nerrno: %s\n", res_fifo_path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    fgets(temp, BUFSIZ, res_fifo);
    fclose(res_fifo);

    printf("%s\n", temp);
}

void sigusr1_handler(int signo) {
    return;
}

int main(int argc, char *argv[]) {
    if (signal(SIGUSR1, sigusr1_handler) == SIG_ERR) {
        fprintf(stderr, "can't catch SIGUSR1\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = getpid();

    if (access(FIFO_PATH, W_OK) != 0) {
        fprintf(stderr, "can't write to %s\n", FIFO_PATH);
        exit(EXIT_FAILURE);
    }

    if (argc > 1) {
        if (argc > 2 || strncmp(argv[1], "-p", 2) != 0) {
            fprintf(stderr, "%s: usage:\n%s\tprint CPU temperature\n%s -p\tprint CPU temperature every second\n", 
                    argv[0], argv[0], argv[0]);
            exit(EXIT_FAILURE);
        } else if (strncmp(argv[1], "-h", 2) == 0 || strncmp(argv[1], "--help", 6) == 0) {
            fprintf(stderr, "%s: usage:\n%s\tprint CPU temperature\n%s -p\tprint CPU temperature every second\n", 
                    argv[0], argv[0], argv[0]);
            exit(EXIT_SUCCESS);
        }
        while (1) {
            get_temp(pid);
            sleep(1);
        }
    } 
    
    get_temp(pid);

    exit(EXIT_SUCCESS);
}

