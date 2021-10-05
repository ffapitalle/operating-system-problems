#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define FREESEATS 10
#define CUSTOMERS 50

sem_t barbers, customers;
pthread_mutex_t mutex;
unsigned int freeseats = FREESEATS;

void cut_hair()
{
    printf("[barber] Cutting hair\n");
    sleep(5);
    printf("[barber] Done.\n");
}

void get_hair_cut(int c) {
    printf("[customer %02d] Getting hair cut.\n", c);
    sleep(5);
    printf("[customer %02d] Done.\n", c);
}

void *barber(void *data)
{
    printf("[barber] Enter the shop\n");
    while (1) {
        printf("[barber] Wait for customer ...\n");
        sem_wait(&customers);
        pthread_mutex_lock(&mutex);
        freeseats+=1;
        sem_post(&barbers);
        pthread_mutex_unlock(&mutex);
        printf("[barber] %d free seat(s) left.\n", freeseats);

        cut_hair();
    }
}

void *customer(void *data)
{
    printf("[customer %02d] Enter barber shop\n", (int) data);
    pthread_mutex_lock(&mutex);
    if (freeseats > 0) {
        freeseats-=1;
        printf("[customer %02d] Sit and wait. %d free seat(s) left.\n", (int) data, freeseats);
        sem_post(&customers);
        pthread_mutex_unlock(&mutex);
        sem_wait(&barbers);
        get_hair_cut((int) data);
        printf("[customer %02d] Exit barber shop.\n", (int) data);
    } else {
        pthread_mutex_unlock(&mutex);
        printf("[customer %02d] Leaving ... barber shop is full.\n", (int) data);
    }
    
}

int main()
{
    pthread_t barber_th, customer_th[CUSTOMERS];

    srand(time(NULL));
    
    sem_init(&customers, 0, 0);
    sem_init(&barbers, 0, 0);

    pthread_create(&barber_th, NULL, barber, NULL);

    for (int c = 0; c<CUSTOMERS; c++) {
        sleep(rand() % (5 + 1 - 0) + 0);
        pthread_create(&customer_th[c], NULL, customer, (void *) c);
    }

    pthread_join(barber_th, NULL);
    for (int c = 0; c<CUSTOMERS; c++) {
        pthread_join(customer_th[c], NULL);
    }

    sem_destroy(&customers);
    sem_destroy(&barbers);
}