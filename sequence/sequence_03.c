// Implementacion con pipe de impresion de secuencia
// AB(C|D)EA(C|D)F...

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_PROCESS 6

int pipe_a[2];
int pipe_b[2];
int pipe_cd[2];
int pipe_e[2];
int pipe_f[2];
int pipe_t[2];

void * A() {
    char token;

    while(1) {
        read(pipe_a[0],&token,sizeof(char));
        printf("A"); fflush(stdout);
        write(pipe_b[1],&token,sizeof(char));
        write(pipe_t[1],&token,sizeof(char));
    }
}

void * B() {
    char token;

    while(1) {
        read(pipe_b[0],&token,sizeof(char));
        read(pipe_b[0],&token,sizeof(char));
        printf("B"); fflush(stdout);
        write(pipe_cd[1],&token,sizeof(char));
    }
}

void * C() {
    char token;

    while(1) {
        read(pipe_cd[0],&token,sizeof(char));
        read(pipe_t[0],&token,sizeof(char));
        printf("C"); fflush(stdout);
        write(pipe_e[1],&token,sizeof(char));
        write(pipe_f[1],&token,sizeof(char));
    }
}

void * D() {
    char token;

    while(1) {
        read(pipe_cd[0],&token,sizeof(char));
        read(pipe_t[0],&token,sizeof(char));
        printf("D"); fflush(stdout);
        write(pipe_e[1],&token,sizeof(char));
        write(pipe_f[1],&token,sizeof(char));
    }
}

void * E() {
    char token;

    while(1) {

        read(pipe_e[0],&token,sizeof(char));
        read(pipe_e[0],&token,sizeof(char));
        printf("E"); fflush(stdout);
        write(pipe_a[1],&token,sizeof(char));
        write(pipe_cd[1],&token,sizeof(char));
    }
}

void * F() {
    char token;

    while(1) {
        read(pipe_f[0],&token,sizeof(char));
        read(pipe_f[0],&token,sizeof(char));
        printf("F\n"); fflush(stdout);
        write(pipe_a[1],&token,sizeof(char));
    }
}

int main() {

    if (pipe(pipe_a) == -1) {
       exit(EXIT_FAILURE);
    }
    if (pipe(pipe_b) == -1) {
       exit(EXIT_FAILURE);
    }
    if (pipe(pipe_cd) == -1) {
       exit(EXIT_FAILURE);
    }
    if (pipe(pipe_e) == -1) {
       exit(EXIT_FAILURE);
    }
    if (pipe(pipe_f) == -1) {
       exit(EXIT_FAILURE);
    }
    if (pipe(pipe_t) == -1) {
       exit(EXIT_FAILURE);
    }

    char token = '*';
    write(pipe_a[1],&token,sizeof(char));
    write(pipe_b[1],&token,sizeof(char));
    write(pipe_e[1],&token,sizeof(char));

    pid_t pids[NUM_PROCESS];
    for (int i = 0; i<NUM_PROCESS; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            switch (i) {
                case 0: A();
                    break;
                case 1: B();
                    break;
                case 2: C();
                    break;
                case 3: D();
                    break;
                case 4: E();
                    break;
                case 5: F();
                    break;
            }
        }
    }

    int status;
    for (int i = 0; i<NUM_PROCESS; i++) {
        waitpid(pids[i], &status, 0);
    }

}