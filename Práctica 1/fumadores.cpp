#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"

using namespace std ;

sem_t estanquero, fumador[3];	//Declaracion de los semáforos

int ingredientes;	//0 = cerillas, 1 = tabaco, 2 = papel

// ---------------------------------------------------------------------
// introduce un retraso aleatorio de duración comprendida entre
// 'smin' y 'smax' (dados en segundos)

void retraso_aleatorio( const float smin, const float smax ){
	static bool primera = true ;
	
	if ( primera ){        // si es la primera vez:
		srand(time(NULL)); //   inicializar la semilla del generador
		primera = false ;  //   no repetir la inicialización
	}
	
	// calcular un número de segundos aleatorio, entre {\ttbf smin} y {\ttbf smax}
	
	const float tsec = smin+(smax-smin)*((float)random()/(float)RAND_MAX);
	
	// dormir la hebra (los segundos se pasan a microsegundos, multiplicándos por 1 millón)
	
	usleep( (useconds_t) (tsec*1000000.0)  );
}

// ----------------------------------------------------------------------------
// función que simula la acción de fumar, como un retardo aleatorio de la hebra.
// recibe como parámetro el numero de fumador
// el tiempo que tarda en fumar está entre dos y ocho décimas de segundo.

void* fumar( int* num_fumador ){
	
	while(true){
		sem_wait(&fumador[num_fumador]);
		
		cout << "Fumador número " << num_fumador << ": comienza a fumar." << endl << flush ;
		
		retraso_aleatorio( 0.2, 0.8 );
		
		cout << "Fumador número " << num_fumador << ": termina de fumar." << endl << flush ;
		
		cout << "Fumador número " << num_fumador << " está esperando: ";
		
		if (num_fumador == 1){
			cout << "cerillas.\n";
		}
		else{
			if(num_fumador == 2){
				cout << "tabaco\n";
			}
			else{
				cout << "papel\n";
			}
		}
		
		sem_post(&estanquero);
	}
}

// ----------------------------------------------------------------------------
//Función que simula al estanco
//Crea aleatoriamente los ingredientes

void* estanco(){
	srand(time(NULL));
	
	while (true){
		sem_wait(estanquero);
		
		ingredientes = rand()%3;
		
		if (ingredientes == 0){
			cout << "Se producen cerillas\n";
			
			sem_post(&fumador[1]);
		}
		
		if (ingredientes == 1){
			cout << "Se produce tabaco\n";
			
			sem_post(&fumador[2]);
		}
		
		if(ingredientes == 3){
			cout << "Se produce papel\n";
			
			sem_post(&fumador[3]);
		}
	}
}

int main(){
	srand( time(NULL) ); // inicializa semilla aleatoria para selección aleatoria de fumador
	pthread_t fumador_1, fumador_2, fumador_3, estanquero_th;	//Declaración de las diferentes hebras
	
	//Inicialización de los semáforos
	
	for (int i = 0; i < 3; i++){
		sem_init(&fumador[i], 0, 0);
	}
	sem_init(estanquero, 0, 1);
	
	//Creación de hebras
	
	pthread_create(&fumador_1, NULL, fumar, 1);
	pthread_create(&fumador_2, NULL, fumar, 2);
	pthread_create(&fumador_3, NULL, fumar, 3);
	pthread_create(&estanquero_th, NULL, estanco, NULL);
	
	return 0 ;
}
