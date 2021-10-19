#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define INGREDIENTS 3

enum INGREDIENT {
    PAPER = 0,
    TOBACCO = 1,
    MATCHES = 2
};

sem_t sem_table, sem_smoker[INGREDIENTS];

void smoke_cigarette(int index) {
    switch (index) {
        case PAPER:
            printf("[PAPER - smoker %02d] Smoking cigarette ... \n", index);
            break;
        case TOBACCO:
            printf("[TOBACCO - smoker %02d] Smoking cigarette ... \n", index);
            break;
        case MATCHES:
            printf("[MATCHES - smoker %02d] Smoking cigarette ... \n", index);
            break;
    }
    sleep(3);
}

void *agent(void *data) {
    while (1) {
        sem_wait(&sem_table);
        switch (rand() % INGREDIENTS)
        {
        case PAPER:
            printf("[agent] Puts tobacco and matches\n");
            sem_post(&sem_smoker[PAPER]);
            break;
        case TOBACCO:
            printf("[agent] Puts paper and matches\n");
            sem_post(&sem_smoker[TOBACCO]);
            break;
        case MATCHES:
            printf("[agent] Puts paper and tobacco\n");
            sem_post(&sem_smoker[MATCHES]);
            break;        
        default:
            break;
        } 
    }
}

void *smoker(void *data) {
    while (1) {
        int index = (int) data;
        sem_wait(&sem_smoker[index]);
        smoke_cigarette(index);
        sem_post(&sem_table);
    }
}

int main()
{
    pthread_t agent_th, smoker_th[INGREDIENTS];

    srand(time(NULL));

    sem_init(&sem_table, 0, 1);
    for (int i = 0; i<INGREDIENTS; i++) {
        sem_init(&sem_smoker[i], 0, 0);
    }

    pthread_create(&agent_th, NULL, agent, NULL);
    for (int i = 0; i<INGREDIENTS; i++) {
        pthread_create(&smoker_th[i], NULL, smoker, (void *) i);
    }

    pthread_join(agent_th, NULL);
    for (int i = 0; i<INGREDIENTS; i++) {
        pthread_join(smoker_th[i], NULL);
    }

    sem_destroy(&sem_table);
    for (int i = 0; i<INGREDIENTS; i++) {
        sem_destroy(&sem_smoker[i]);
    }
}