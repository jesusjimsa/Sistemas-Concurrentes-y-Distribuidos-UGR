#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"

using namespace std ;

sem_t estanquero, fumadores[3];	//Declaracion de los semáforos

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

void* fumar( void * arg_ptr ){
	
	unsigned long arg_ent = (unsigned long) arg_ptr;	//Convertir puntero a entero
	
	while(true){
		sem_wait(&fumadores[arg_ent]);
		
		cout << "Fumador número " << arg_ent << ": comienza a fumar." << endl << flush ;
		
		retraso_aleatorio( 0.2, 0.8 );
		
		cout << "Fumador número " << arg_ent << ": termina de fumar." << endl << flush ;
		
		cout << "Fumador número " << arg_ent << " está esperando: ";
		
		if (arg_ent == 0){
			cout << "cerillas.\n";
		}
		else{
			if(arg_ent == 1){
				cout << "tabaco.\n";
			}
			else{
				cout << "papel.\n";
			}
		}
		
		sem_post(&estanquero);
	}
}

// ----------------------------------------------------------------------------
//Función que simula al estanco
//Crea aleatoriamente los ingredientes

void* estanco(void * num){
	srand(time(NULL));
	int dos_seguidos[2];
	bool primera_iteracion = true;
	unsigned long num_est = (unsigned long) num;
	
	while (true){
		sem_wait(&estanquero);
		
		ingredientes = rand()%3;
		
		if (ingredientes == 0){
			cout << "\nSe producen cerillas.\n";
			
			sem_post(&fumadores[0]);
		}
		
		if (ingredientes == 1){
			cout << "\nSe produce tabaco.\n";
			
			sem_post(&fumadores[1]);
		}
		
		if(ingredientes == 2){
			cout << "\nSe produce papel.\n";
			
			sem_post(&fumadores[2]);
		}

		if (primera_iteracion){
			primera_iteracion = false;
			dos_seguidos[0] = ingredientes;
		}
		else{
			dos_seguidos[1] = ingredientes;
			
			if (dos_seguidos[0] == dos_seguidos[1]){
				cout << "\nEl estanquero " << num_est << " ha producido dos veces seguidas el mismo ingrediente: ";
				
				if (ingredientes == 0)
					cout << "cerillas\n\n";
				else
					if(ingredientes == 1)
						cout << "tabaco\n\n";
					else
						if(ingredientes == 2)
							cout << "papel\n\n";
				
			}
			
			dos_seguidos[0] = dos_seguidos[1];
		}
	}
}

int main(){
	srand( time(NULL) ); // inicializa semilla aleatoria para selección aleatoria de fumador
	pthread_t fumador[3], estanquero_th[2];	//Declaración de las diferentes hebras
	int comienza;
	
	//Inicialización de los semáforos
	
	for (int i = 0; i < 3; i++){
		sem_init(&fumadores[i], 0, 0);
	}
	sem_init(&estanquero, 0, 1);
	
	//Creación de hebras
	
	for (int i = 0; i < 3; i++){
		void * arg_ptr = (void *) i;	//Convertir entero a puntero
		pthread_create(&fumador[i], NULL, fumar, arg_ptr);
	}

	comienza = rand()%2;	//Se elige aleatoriamente qué estanquero se iniciará primero
	void * num_ptr1 = (void *) comienza;

	pthread_create(&estanquero_th[comienza], NULL, estanco, num_ptr1);

	if (comienza == 0){
		comienza = 1;
		void * num_ptr2 = (void *) comienza;
		pthread_create(&estanquero_th[1], NULL, estanco, num_ptr2);
	}
	else{
		comienza = 0;
		void * num_ptr2 = (void *) comienza;
		pthread_create(&estanquero_th[0], NULL, estanco, num_ptr2);
	}
	
	for (int i = 0; i < 3; i++){
		pthread_join(fumador[i], NULL);
	}
	
	for (int i = 0; i < 2; i++){
		pthread_join(estanquero_th[i], NULL);
	}
	
	
	return 0 ;
}
