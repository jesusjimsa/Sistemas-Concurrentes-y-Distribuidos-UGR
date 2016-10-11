#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"

using namespace std ;

sem_t estanquero, fumador[3];	//Declaracion de los semáforos

enum class Ingredientes{tabaco, papel, cerillas};	//Declaración de los ingredientes con un enumerado

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

void fumar( int num_fumador ){
	
	while(true){
		sem_wait(&fumador[num_fumador]);
		
		cout << "Fumador número " << num_fumador << ": comienza a fumar." << endl << flush ;
		
		retraso_aleatorio( 0.2, 0.8 );
		
		cout << "Fumador número " << num_fumador << ": termina de fumar." << endl << flush ;
		
		sem_post(&estanquero);
	}
}

// ----------------------------------------------------------------------------
//Función que simula al estanco
//Crea aleatoriamente los ingredientes

void estanco(){
	srand(time(NULL));
	Ingredientes creado;
	
	while (true){
		sem_wait(estanquero);
		
		creado = Ingredientes::rand()%3;
		
		if (creado == Ingredientes::cerillas){
			sem_post(&fumador[1]);
		}
		
		if (creado == Ingredientes::tabaco){
			sem_post(&fumador[2]);
		}
		
		if(creado == Ingredientes::papel){
			sem_post(&fumador[3]);
		}
	}
}

int main(){
	srand( time(NULL) ); // inicializa semilla aleatoria para selección aleatoria de fumador
	pthread_t fumador_1, fumador_2, fumador_3, estanquero;	//Declaración de las diferentes hebras
	
	//Inicialización de los semáforos
	
	for (int i = 0; i < 3; i++){
		sem_init(&fumador[i], 0, 0);
	}
	sem_init(estanquero, 0, 1);
	
	//Creación de hebras
	//aquí
	
	
	return 0 ;
}
