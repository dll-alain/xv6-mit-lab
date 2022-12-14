//
// Created by Alain Dong on 12/13/22.
//
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

int main(int argc, char *argv[]) {
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);
    int size;
    char buf[5];
    int pid = fork();
    if (pid == 0) {
        //child process
        //receive ping
        //0 read 1 write
        close(p1[1]);
        if ((size = read(p1[0], buf, sizeof buf)) > 0) {
            printf("%d: receive ping", getpid());
            write(1, buf, size);
        }
        else {
            printf("%d: receive failed", getpid());
        }
        //send pong
        close(p2[0]);
        write(p2[1], "pong\n", 5);
        exit(0);
    }
    else if (pid > 0) {
        //parent process
        //receive pong
        close(p2[1]);
        if ((size = read(p2[0], buf, sizeof buf)) > 0) {
            printf("%d: receive pong", getpid());
            write(1, buf, size);
        }
        else {
            printf("%d: receive failed", getpid());
        }
        //send ping
        close(p1[0]);
        write(p1[1], "ping\n", 5);
        exit(0);
    }
    else {
        printf("fork error\n");
    }
    exit(0);
}