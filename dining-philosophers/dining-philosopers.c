#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define PHILOSOPHERS 5

pthread_mutex_t mutex;
pthread_mutex_t forks[PHILOSOPHERS];
pthread_t philosopher[PHILOSOPHERS];

void think(int i) {
    printf("[%d] Thinking ...\n", i); fflush(stdout);
    sleep(rand() % 10);
}

void get_forks(int left, int right) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&forks[left]);
    pthread_mutex_lock(&forks[right]);
    pthread_mutex_unlock(&mutex);
}

void eat(int i) {
    printf("[%d] Eating ...\n", i); fflush(stdout);
    sleep(rand() % 10);
    printf("[%d] done ...\n", i); fflush(stdout);
}

void put_forks(int left, int right) {
    pthread_mutex_unlock(&forks[left]);
    pthread_mutex_unlock(&forks[right]);
}

void *dine(uint i) {
    while (1)
    {
        think(i);
        get_forks(i, (i+1)%PHILOSOPHERS);
        eat(i);
        put_forks(i, (i+1)%PHILOSOPHERS);
    }   
}

int main() {

    srand(time(NULL));

    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i<PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    for (int i = 0; i<PHILOSOPHERS; i++) {
        pthread_create(&philosopher[i], NULL, (void *)dine, (int *)i);
    }

    for (int i = 0; i<PHILOSOPHERS; i++) {
        pthread_join(philosopher[i], NULL);
    }

    return 0;
}