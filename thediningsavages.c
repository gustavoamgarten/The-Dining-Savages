#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <curses.h>
 
#define MAX_SERVINGS 30
#define SAVAGES_NUMBER 30
 
volatile int servings = 0;
 
sem_t emptyPot;
sem_t fullPot;
 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 
void* cook(void *v) {
        while (true) {
                sem_wait(&emptyPot);
 
                //putServingsInPot();        /*ANIMAÇÃO DE ENCHER O POTE AQUI (FAZER EM FUNÇÃO PARA FICAR MAIS ORGANIZADO O CÓDIGO)*/
 
                sem_post(&fullPot);
        }
}
 
void* savage(void *v) {
        while (true) {
                pthread_mutex_lock(&mutex);
                if (servings == 0) {
                        sem_post(&emptyPot);
                        sem_wait(&fullPot);
                        servings = MAX_SERVINGS;
 
                }
 
                servings--;
 
                //getServingFromPot(); /*ANIMAÇÃO DE ESVAZIAR O POTE AQUI (FAZER EM FUNÇÃO PARA FICAR MAIS ORGANIZADO O CÓDIGO)*/
 
                pthread_mutex_unlock(&mutex);
 
                //eat(); /*ANIMAÇÃO DE COMER, ALGO BEM SIMPLES SÓ PARA INDICAR...*/
        }
 
}
 
int main() {
		int i;
		pthread_t thr_savages[SAVAGES_NUMBER];
        pthread_t thr_cook;
 
        sem_init(&emptyPot, 0, 0);
        sem_init(&fullPot, 0, 0);
 
        pthread_create(&thr_cook, NULL, cook, NULL);
 
        for (i = 0; i < SAVAGES_NUMBER; i++) {
			pthread_create(&thr_savages[i], NULL, savage, NULL);
		}
 
        return 0;
}
