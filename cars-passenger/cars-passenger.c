#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define SEATS_NUMBER 4
#define PASSENGERS 20

sem_t seats_available, passenger_arrived, vehicle_full, mutex;

void *passenger() {
    while(1) {
        sem_wait(&seats_available);
        printf("Passenger %u get into the vehicle\n", pthread_self()); fflush(stdout);

        sem_wait(&mutex);
        if (sem_trywait(&seats_available) < 0) { // vehicle full
            sem_post(&vehicle_full);
        } else {
            sem_post(&seats_available);
        }
        sem_post(&mutex);

        sem_wait(&passenger_arrived);
        printf("Passenger %u steps out of the vehicle\n", pthread_self()); fflush(stdout);
    }
}

void *vehicle() {
    while (1) {
        sem_wait(&vehicle_full);
        printf("Vehicle starts ....");
        sleep(1);
        printf("and stops.\n"); fflush(stdout);

        sem_wait(&mutex);
        for (int i=0; i<SEATS_NUMBER; i++) {
            sem_post(&passenger_arrived);
            sem_post(&seats_available);
        }
        sem_post(&mutex);
   }
    
}

int main() {
    sem_init(&seats_available, 0, SEATS_NUMBER);
    sem_init(&passenger_arrived, 0, 0);
    sem_init(&vehicle_full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_t vehicle_t;
    pthread_create(&vehicle_t, NULL, &vehicle, NULL);

    pthread_t passenger_t[PASSENGERS];
    for (int i=0; i<PASSENGERS; i++) {
        pthread_create(&passenger_t[i], NULL, &passenger, NULL);
    }

    pthread_join(vehicle_t, NULL);
    for (int i=0; i<PASSENGERS; i++) {
        pthread_join(passenger_t[i], NULL);
    }

    return 0;
}