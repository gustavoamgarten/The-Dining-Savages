#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <curses.h>
 
#define MAX_SERVINGS 5
#define SAVAGES_NUMBER 3
 
volatile int servings = 0;
volatile int savagesInit = 0;
 
sem_t emptyPot;
sem_t fullPot;
 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void putServingsInPot(){
	//clear();
	printw("Servindo o pote.");
	refresh();
	sleep(1);
}

void getServingFromPot(int id){
	//clear();
	printw("Selvagem %d pegando comida do pote.", id);
	refresh();
	sleep(1);
}
 
void* cook(void *v) {
        while (1) {
                sem_wait(&emptyPot);
 
                putServingsInPot();        /*ANIMAÇÃO DE ENCHER O POTE AQUI (FAZER EM FUNÇÃO PARA FICAR MAIS ORGANIZADO O CÓDIGO)*/
 
                sem_post(&fullPot);
        }
}
 
void* savage(void *v) {
		int id = *(int *)v;
        while (1) {
                pthread_mutex_lock(&mutex);
                if (servings == 0) {
                        sem_post(&emptyPot);
                        sem_wait(&fullPot);
                        servings = MAX_SERVINGS;
 
                }
 
                servings--;
 
                getServingFromPot(id); /*ANIMAÇÃO DE ESVAZIAR O POTE AQUI (FAZER EM FUNÇÃO PARA FICAR MAIS ORGANIZADO O CÓDIGO)*/
 
                pthread_mutex_unlock(&mutex);
 
                //eat(); /*ANIMAÇÃO DE COMER, ALGO BEM SIMPLES SÓ PARA INDICAR...*/
        }
 
}
 
int main() {
	int i;
	pthread_t thr_savages[SAVAGES_NUMBER];
	pthread_t thr_cook;
	
	int savagesIds[SAVAGES_NUMBER];
	for(i = 0; i < SAVAGES_NUMBER; i++){
		savagesIds[i] = i;
	}
	
	initscr(); /* Start curses mode  */

	sem_init(&emptyPot, 0, 0);
	sem_init(&fullPot, 0, 0);

	pthread_create(&thr_cook, NULL, cook, NULL);

	for (i = 0; i < SAVAGES_NUMBER; i++) {
		pthread_create(&thr_savages[i], NULL, savage, (void *)&savagesIds[i]);
	}
	
	getch(); // Se o usuário teclar qualquer tecla, o programa encerra (não precisa do join)

	endwin(); /* End curses mode */

	return 0;
}
