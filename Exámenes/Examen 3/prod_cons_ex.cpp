#include <mpi.h>
#include <iostream>
#include <math.h>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"

#define Productor 3
#define Buffer 8
#define ITERS 10
#define TAM 5

using namespace std;

// ---------------------------------------------------------------------

void producir(int rank){
	int value;

	for(unsigned int i = 0; i < ITERS; i++){
		value = rand() % 10;
		cout << "Productor " << rank << " produce valor " << value << endl << flush ;

		// espera bloqueado durante un intervalo de tiempo aleatorio
		// (entre una décima de segundo y un segundo)
		usleep(1000U * (100U + (rand() % 900U)));

		// enviar 'value'
		MPI_Ssend(&value, 1, MPI_INT, Buffer, Productor, MPI_COMM_WORLD);
	}
}

// ---------------------------------------------------------------------

void buffer(){
	int value, peticion;
	bool opcion = true;
	MPI_Status status;

	for(unsigned int i = 0; i < ITERS * 5; i++){
		//MPI_Probe(MPI_ANY_SOURCE, Productor, MPI_COMM_WORLD, &status);	//Compruebo de qué productor viene el elemento
		MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, Productor, MPI_COMM_WORLD, &status);	//Guardo el valor recibido en value

		//Se envía el valor al consumidor
		if(value % 2 == 0){
			MPI_Ssend(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		}
		else{
			if(opcion){
				MPI_Ssend(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
				opcion = false;
			}
			else{
				MPI_Ssend(&value, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
				opcion = true;
			}
		}
		
		

		cout << "Buffer recibe " << value << " de un productor y lo envía a un consumidor" << endl << flush;
		
	}
}

// ---------------------------------------------------------------------

void consumir(int rank){
	int value;
	float raiz;
	MPI_Status status;

	//for (unsigned int i = 0; i < ITERS/4; i++){
	while(true){
		MPI_Recv(&value, 1, MPI_INT, Buffer, 0, MPI_COMM_WORLD,&status);
		
		cout << "Consumidor " << rank << " recibe valor " << value << " de Buffer " << endl << flush ;

		// espera bloqueado durante un intervalo de tiempo aleatorio
		// (entre una décima de segundo y un segundo)
		usleep(1000U * (100U + (rand() % 900U)));

		raiz = sqrt(value);
	}
}

// ---------------------------------------------------------------------

int main(int argc, char *argv[]){
	int rank,size;

	// inicializar MPI, leer identif. de proceso y número de procesos
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	// inicializa la semilla aleatoria:
	srand(time(NULL));

	// comprobar el número de procesos con el que el programa
	// ha sido puesto en marcha (debe ser 9)
	if(size != 9){
		cout << "El numero de procesos debe ser 9 " << endl;

		MPI_Finalize();

		return 0;
	}

	// verificar el identificador de proceso (rank), y ejecutar la
	// operación apropiada a dicho identificador
	if(rank < Buffer)
		producir(rank);
	else if (rank == Buffer)
		buffer();
	else
		consumir(rank);

	// al terminar el proceso, finalizar MPI
	MPI_Finalize();

	return 0;
}
