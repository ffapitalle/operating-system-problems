// Implementacion con semaforos de impresion de secuencia
// AB(C|D)...

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

sem_t semA, semB, semCD; 

void * A() {
    while(1) {
        sem_wait(&semA); 
        printf("A");
        sleep(1); 
        sem_post(&semB); 
    }
}

void * B() {
    while(1) {
        sem_wait(&semB); 
        printf("B"); 
        sleep(1); 
        sem_post(&semCD); 
    }
}

void * C() {
    while(1) {
        sem_wait(&semCD); 
        printf("C\n"); 
        sleep(1); 
        sem_post(&semA); 
    }
}

void * D() {
    while(1) {
        sem_wait(&semCD); 
        printf("D\n"); 
        sleep(1); 
        sem_post(&semA); 
    }
}

int main() {
    sem_init(&semA,1,1);
    sem_init(&semB,1,0);
    sem_init(&semCD,1,0);

    pthread_t thread[4];

    pthread_create(&(thread[0]),NULL,(void*)A,NULL); 
    pthread_create(&(thread[1]),NULL,(void*)B,NULL); 
    pthread_create(&(thread[2]),NULL,(void*)C,NULL); 
    pthread_create(&(thread[3]),NULL,(void*)D,NULL); 
    
    for (int i=0;i<4;i++) {
        pthread_join(thread[i],NULL);
    }

}