
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cputempd.h"

int main(int argc, char *argv[]) {
    pid_t pid = getpid();

    if (access(FIFO_PATH, W_OK) != 0) {
        fprintf(stderr, "can't write to %s\n", FIFO_PATH);
        exit(EXIT_FAILURE);
    }

    printf("%d\n", pid);

    FILE *req_fifo = fopen(FIFO_PATH, "w");
    fprintf(req_fifo, "%d", pid);
    fclose(req_fifo);

    char res_fifo_path[BUFSIZ]; 
    snprintf(res_fifo_path, BUFSIZ, FIFO_CLIENT_PATH"%d", pid);

    printf("%s\n", res_fifo_path);

    char temp[BUFSIZ];

//    sleep(1); // :(
    usleep(1000); // :|

    FILE *res_fifo = fopen(res_fifo_path, "r");
    if (res_fifo == NULL) {
        fprintf(stderr, "unable to open %s\nerrno: %s", res_fifo_path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    fgets(temp, BUFSIZ, res_fifo);
    fclose(res_fifo);

    printf("cpu temp: %s", temp);

    exit(EXIT_SUCCESS);
}

