// Implementacion con semaforos de impresion de secuencia
// AB(C|D)EA(C|D)F...

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define NUMTHREAD 6

sem_t semA, semB, semCD, semE, semF, semT; 

void * A() {
    while(1) {
        sem_wait(&semA); 
        printf("A");
        sem_post(&semB); 
        sem_post(&semT);
    }
}

void * B() {
    while(1) {
        sem_wait(&semB); 
        sem_wait(&semB); 
        printf("B"); 
        sem_post(&semCD); 
    }
}

void * C() {
    while(1) {
        sem_wait(&semCD); 
        sem_wait(&semT); 
        printf("C"); 
        sem_post(&semE); 
        sem_post(&semF); 
    }
}

void * D() {
    while(1) {
        sem_wait(&semCD); 
        sem_wait(&semT); 
        printf("D"); 
        sleep(1); 
        sem_post(&semE); 
        sem_post(&semF); 
    }
}

void * E() {
    while(1) {
        sem_wait(&semE); 
        sem_wait(&semE); 
        printf("E"); 
        sem_post(&semA);
        sem_post(&semCD); 
    }
}

void * F() {
    while(1) {
        sem_wait(&semF); 
        sem_wait(&semF); 
        printf("F\n"); 
        sem_post(&semA);
    }
}

int main() {
    sem_init(&semA,1,1);
    sem_init(&semB,1,1);
    sem_init(&semCD,1,0);
    sem_init(&semE,1,1);
    sem_init(&semF,1,0);
    sem_init(&semT,1,0);

    pthread_t thread[NUMTHREAD];

    pthread_create(&(thread[0]),NULL,(void*)A,NULL); 
    pthread_create(&(thread[1]),NULL,(void*)B,NULL); 
    pthread_create(&(thread[2]),NULL,(void*)C,NULL); 
    pthread_create(&(thread[3]),NULL,(void*)D,NULL); 
    pthread_create(&(thread[4]),NULL,(void*)E,NULL); 
    pthread_create(&(thread[5]),NULL,(void*)F,NULL); 
    
    for (int i=0;i<NUMTHREAD;i++) {
        pthread_join(thread[i],NULL);
    }

}