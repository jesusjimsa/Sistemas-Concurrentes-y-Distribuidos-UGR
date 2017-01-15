#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"

using namespace std;

//Variables globales
int v1[9], v2[9], x, y, z;
bool terminar = false;
sem_t sem_proc[2], mutex;

void P1(){
	for (int i = 0; i < 9; i++){
		sem_wait(&mutex);

		x = v1[i];
		
		sem_post(&sem_proc[0]);
	}
}

void P2(){
	for (int i = 0; i < 9; i++){
		sem_wait(&sem_proc[0]);
		
		y = v[i] + x;
		
		if((y % 2) == 0){
			sem_post(&sem_proc[1]);
			sem_wait(&sem_proc[0]);
		}

		sem_post(&mutex);
	}
	
	terminar = true;
}

void P3(){
	z = 0;

	while(!terminar){
		sem_wait(&sem_proc[1]);
		
		z = z + y;
		
		sem_post(&sem_proc[1]);
		sem_post(&mutex);
	}
}

int main(){
	srand( time(NULL) ); // Inicializa semilla aleatoria
	pthread_t proceso[3];

	for (int i = 0; i < 9; i++){
		v1[i] = rand()%7+1;
		v2[i] = rand()%7+1;
	}

	
	sem_init(&smutex, 0, 1);

	for (int i = 0; i < 2; i++){
		sem_init(&sem_proc[i], 0, 0);
	}

	
	pthread_create(&proceso[0], NULL, P1, NULL);
	pthread_create(&proceso[1], NULL, P2, NULL);
	pthread_create(&proceso[2], NULL, P3, NULL);

	for (int i = 0; i < 3; i++){
		pthread_join(&proceso[i], NULL);
	}

	cout << "z vale " << z;
}
