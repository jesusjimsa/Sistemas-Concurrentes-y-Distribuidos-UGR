#include <mpi.h>
#include <iostream>
#include <math.h>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"

#define Buffer 10
#define Pagar 1
#define Completa 2

using namespace std;

// ---------------------------------------------------------------------

void cliente(int rank){
	int intermedio, var;
	MPI_Status status;

	while(true){
		cout << "Cliente " << rank << " esta comprando" << endl << flush;
		usleep(rand() % 3);	//Compra en la tienda

		MPI_Ssend(NULL, 0, MPI_INT, Buffer, Pagar, MPI_COMM_WORLD);	//Solicita pago
		cout << "Cliente " << rank << " solicita pagar" << endl << flush;
	
		MPI_Recv(NULL, 0, MPI_INT, Buffer, Pagar, MPI_COMM_WORLD, &status);	//Puede ir a una caja
		usleep(rand() % 2);	//Pago en la caja

		MPI_Ssend(NULL, 0, MPI_INT, Buffer, Completa, MPI_COMM_WORLD); //Compra completada
		cout << "Cliente " << rank << " ha completado la compra" << endl << flush;

		usleep(rand() % 5);	//El cliente se va a casa, ya volverá
	}
}

void intermedio(){
	int cajas, buf;
	MPI_Status status;

	cajas = 0;

	while(true){
		if(cajas < 3){
			//Espera a que llegue algún cliente a una caja
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		}
		else{
			//Espera a que haya una caja libre
			MPI_Probe(MPI_ANY_SOURCE, Completa, MPI_COMM_WORLD, &status);
		}

		if(status.MPI_TAG == Pagar){
			buf = status.MPI_SOURCE;
			
			MPI_Recv(NULL, 0, MPI_INT, buf, Pagar, MPI_COMM_WORLD, &status);
			MPI_Ssend(NULL, 0, MPI_INT, buf, Pagar, MPI_COMM_WORLD);
			cajas++;

			cout << "Hay " << cajas << " cajas ocupadas" << endl;
		}

		if(status.MPI_TAG == Completa){
			buf = status.MPI_SOURCE;

			MPI_Recv(NULL, 0, MPI_INT, buf, Pagar, MPI_COMM_WORLD, &status);
			cajas--;

			cout << "Hay " << cajas << " cajas ocupadas" << endl;
		}
	}
}

int main(int argc, char **argv){
	int rank, size;

	srand(time(0));		//Inicializa la semilla aleatoria

	//Inicializar MPI, leer identificador de proceso y número de procesos
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(size != 11){
		if(rank == 0){
			cout << "El numero de procesos debe ser 11" << endl << flush ;
		}

		MPI_Finalize();

		return 0;
	}


	if(rank == 10){
		intermedio();
	}
	else{
		cliente(rank);
		
	}

	MPI_Finalize();

	return 0;
}

