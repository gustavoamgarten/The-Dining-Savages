#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <curses.h>
 
#define MAX_SERVINGS 3
#define SAVAGES_NUMBER 6
 
volatile int servings = 0;
volatile int savagesNumber = 0;
volatile int maxServings = 0;
 
sem_t emptyPot;
sem_t fullPot;
 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void getInput(){
	clear();
	move(0, 0);
	printw("Digite o numero maximo de pratos: ");
	refresh();
	scanw("%d", &maxServings);
	printw("Digite o numero de selvagens: ");
	refresh();
	scanw("%d", &savagesNumber);
}

void drawPanel(int id){
	int i;
	move(0, 0);
	printw("##################################################"); //50 chars
	for(i = 1; i < 5; i++){
		move(i, 0);
		printw("#");
	}
	move(i, 0);
	printw("##################################################"); //50 chars
	move(0, 0);
	for(i = 1; i < 5; i++){
		move(i, 49);
		printw("#");
	}

	move(1, 3);
	printw("Pratos disponiveis: %d", servings);
	move(2, 3);
	printw("Total de selvagens: %d", savagesNumber);
	move(3, 3);
	printw("Selvagem %d esta pegando comida", id + 1);
	move(4, 3);
	printw("PRESSIONE QUALQUER TECLA PARA SAIR");
}

void drawIdleSavages(int id){
	clear();
	drawPanel(id);
	int i;
	int pos;
	for(i = 0; i < savagesNumber; i++){
		pos = 10 + (i * 4);
		move(pos, 0);
		printw("  O (%d)", i + 1);
		pos++;
		move(pos, 0);
		printw(" /|\\");
		pos++;
		move(pos, 0);
		printw(" < >");
	}
	
	move(8, 18);
	if(servings == 0){
		printw("| |");
		move(9, 19);
		printw("T");
	} else {
		printw("|o|");
		move(9, 19);
		printw("T");
	}
}

void moveSavage(int id){
	int i;
	int pos;
	int frame;
	int posIdRow = (10 + (id * 4));
	int posIdCol = 0;
	for(frame = 0; frame < ((id * 4) + 25); frame++){
		
		move(posIdRow, posIdCol);
		clrtoeol();
		printw("  O (%d)", id + 1);
		move(posIdRow + 1, posIdCol);
		clrtoeol();
		printw(" /|\\");
		move(posIdRow + 2, posIdCol);
		clrtoeol();
		printw(" / \\");
		move(posIdRow + 3, posIdCol);
		clrtoeol();
		
		if(posIdCol == 21){
			posIdRow--;
		} else {
			posIdCol++;
		}

		refresh();
		usleep(100000);
	}
}

void putServingsInPot(){
	
	move(3, 3);
	clrtoeol();
	printw("O pote esta sendo enchido");
	move(3, 49);
	printw("#");
	
	int i;
	
	for(i = 0; i < maxServings; i++){
		move(6, 19);
		printw("o");
		move(7, 19);
		printw(" ");
		refresh();
		usleep(100000);
		move(6, 19);
		printw(" ");
		move(7, 19);
		printw("o");
		refresh();
		usleep(100000);
		
		move(8, 19);
		printw("o");
		refresh();
		usleep(100000);
	}
	move(7, 19);
	printw(" ");
	sleep(1);
}

void getServingFromPot(int id){
	drawIdleSavages(id);
	moveSavage(id);
	servings--;
	drawIdleSavages(id);
	refresh();
}

void eatAnimation(int id){
	int pos = 10 + (id * 4);
	move(pos + 1, 4);
	printw("CHOMP..");
	refresh();
	usleep(100000);
	move(pos + 1, 4);
	printw("       ");
	refresh();
	usleep(100000);
	move(pos + 1, 4);
	printw("CHOMP..");
	refresh();
	usleep(100000);
	move(pos + 1, 4);
	printw("       ");
	refresh();
	usleep(100000);
	move(pos + 1, 4);
	printw("CHOMP..");
	refresh();
	usleep(100000);
	move(pos + 1, 4);
	printw("       ");
	refresh();
	usleep(100000);
	move(pos + 1, 4);
	printw("CHOMP..");
	refresh();
	usleep(100000);
	move(pos + 1, 4);
	printw("       ");
	refresh();
	usleep(100000);
}

void eatSleep(){
	sleep(3);
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
                        servings = maxServings;
 
                }
 
                
                getServingFromPot(id); /*ANIMAÇÃO DE ESVAZIAR O POTE AQUI (FAZER EM FUNÇÃO PARA FICAR MAIS ORGANIZADO O CÓDIGO)*/
                
                //servings--;
                
                eatAnimation(id);
 
                pthread_mutex_unlock(&mutex);
 
                eatSleep(); /*ANIMAÇÃO DE COMER, ALGO BEM SIMPLES SÓ PARA INDICAR...*/
        }
 
}
 
int main() {
	
	initscr(); /* Start curses mode  */
	
	getInput();
	
	servings = maxServings;
	
	int i;
	pthread_t thr_savages[savagesNumber];
	pthread_t thr_cook;
	
	int savagesIds[savagesNumber];
	for(i = 0; i < savagesNumber; i++){
		savagesIds[i] = i;
	}
	
	curs_set(0);

	sem_init(&emptyPot, 0, 0);
	sem_init(&fullPot, 0, 0);

	pthread_create(&thr_cook, NULL, cook, NULL);

	for (i = 0; i < savagesNumber; i++) {
		pthread_create(&thr_savages[i], NULL, savage, (void *)&savagesIds[i]);
	}
	
	getch(); // Se o usuário teclar qualquer tecla, o programa encerra (não precisa do join)

	endwin(); /* End curses mode */

	return 0;
}
