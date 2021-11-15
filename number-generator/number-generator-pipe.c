#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define NUMGEN 2
#define NUMWRITERS 2

int pfdgen[2], pfdw1[2], pfdw2[2], pfdctrl[2];
int pidgen[NUMGEN];
int pidwriter[NUMWRITERS];
int pidctrl;

void generate_numbers() {
    close(pfdgen[0]);

    srand(getpid());

    while (1)
    {
        sleep(rand() % 10);
        int nbr = rand() % 100;
        dprintf(pfdgen[1], "%02d ", nbr);
        printf("[G %d] %d\n", getpid(), nbr);
    }
    
}

void write_numbers(int fdin, int id) {
    char format[13] = "Salida%d.txt";
    char filename[12];

    sprintf(filename, format, id);

    FILE *fp = fopen(filename, "w");

    while (1)
    {
        char where[3];
        read(fdin, where, 3);
        fprintf(fp, "%d ", atoi(where));
        printf("[W %d] %d\n", getpid(), atoi(where));
        fflush(fp);
    }

    fclose(fp);
}

void control_output() {

    srand(getpid());
    close(pfdctrl[0]);
    while (1)
    {
        int delay = rand() % 10;
        
        sleep(delay);
        write(pfdctrl[1], ".", 1);
    }
    
}

void sync_output() {
    int output = 0;
    fcntl(pfdctrl[0], F_SETFL, O_NONBLOCK);

    while (1)
    {
        char where[3];
        read(pfdgen[0], where, 3);
        printf("[S %d] %d\n", getpid(), atoi(where));

        char toggle;

        if (read(pfdctrl[0], &toggle, 1) > 0) {
            output = (++output) % 2;
            printf("[S %d] toggle output (%d)\n", getpid(), output);
        }

        if (output == 0) {
            write(pfdw1[1], where, 3);
        } else {
            write(pfdw2[1], where, 3);
        }
    }
    
}

void sig_handler() {
    for (int i = 0; i < NUMGEN ; i++) {
        waitpid(pidgen[i], NULL, 0);
    }

    for (int i = 0; i < NUMWRITERS ; i++) {
        waitpid(pidwriter[i], NULL, 0);
    }

    waitpid(pidctrl, NULL, 0);

    exit(0);
}

int main() {
    pipe(pfdgen);
    pipe(pfdw1);
    pipe(pfdw2);
    pipe(pfdctrl);

    for (int i = 0; i < NUMGEN ; i++) {
        pidgen[i] = fork();
        if (pidgen[i] == 0) {
            generate_numbers();
        }
    }

    for (int i = 0; i < NUMWRITERS ; i++) {
        pidwriter[i] = fork();
        if (pidwriter[i] == 0) {
            if (i == 0) {
                write_numbers(pfdw1[0], i);
            } else {
                write_numbers(pfdw2[0], i);
            }
        }
    }

    pidctrl = fork();

    if (pidctrl == 0) {
        control_output();
    }

    signal(SIGINT, sig_handler);

    sync_output();
}