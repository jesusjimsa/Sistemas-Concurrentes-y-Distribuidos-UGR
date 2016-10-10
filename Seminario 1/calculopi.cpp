// *********************************************************************
// SCD. Ejemplos del seminario 1.
//
// Plantilla para el ejercicio de cálculo de PI
// *********************************************************************

#include <iostream>
#include <pthread.h>
#include <cmath>
#include "fun_tiempo.h"

using namespace std ;

// ---------------------------------------------------------------------
// constante y variables globales (compartidas entre hebras)

const unsigned long m = long(1024)*long(1024) ;  // número de muestras (un millón)
const unsigned n      = 4 ;                      // número de hebras

double resultado_parcial[n] ; // tabla de sumas parciales (una por hebra)

// ---------------------------------------------------------------------
// implementa función $f$

double f( double x ){
	return 4.0/(1 + x*x) ;     // $~~~~f(x)\,=\,4/(1+x^2)$
}
// ---------------------------------------------------------------------
// cálculo secuencial

double calcular_integral_secuencial( )
{
	double suma = 0.0 ;                      // inicializar suma
	
	for( unsigned long i = 0 ; i < m ; i++ ) // para cada $i$ entre $0$ y $m-1$
		suma += f( (i+0.5)/m );               // $~~~~~$ añadir $f(x_i)$ a la suma actual
	
	return suma/m ;                          // devolver valor promedio de $f$
}
// ---------------------------------------------------------------------
// función que ejecuta cada hebra

void * funcion_hebra( void * ih_void ){
	unsigned long ih = (unsigned long) ih_void ; // número o índice de esta hebra
	double sumap = 0.0 ;
	
	// calcular suma parcial en "sumap"
	for (int i = 0; i < m ; i++){
		if (ih == (i % n)){
			sumap = sumap + f((i + 0.5) / m);
		}
	}
	
	resultado_parcial[ih] = sumap ; // guardar suma parcial en vector.
	
	return NULL ;
}
// ---------------------------------------------------------------------
// cálculo concurrente

double calcular_integral_concurrente( ){
	double suma = 0.0;
	
	// crear y lanzar $n$ hebras, cada una ejecuta "funcion\_concurrente"
	
	pthread_t id_hebra[n] ; // vector de identificadores de hebra
	
	// lanzar hebras
	
	for( unsigned long i = 0 ; i < n ; i++ ){	//n es el número de hebras
		pthread_create( &(id_hebra[i]), NULL, funcion_hebra, NULL );
	}
	
	double resultado;
	void **ptr_res = (void **) &resultado;
	
	// esperar (join) a que termine cada hebra, sumar su resultado
	
	for (int i = 0; i < n; i++){
		pthread_join(id_hebra[i], ptr_res);
		suma = suma + resultado;
	}
	
	// devolver resultado completo
	
	
	return suma / m;
}

// ---------------------------------------------------------------------

int main(int argc, char *argv[]){
	if (argc < 2){
		cerr << "Debe escribir el numero de muestras y el numero de hebras";
	}
	else{
		cout << "Ejemplo 4 (cálculo de PI)" << endl ;
		
		double pi_sec = 0.0, pi_conc = 0.0 ;
		
		struct timespec inicio_sec = ahora();
		pi_sec  = calcular_integral_secuencial() ;
		struct timespec fin_sec = ahora();
		
		struct timespec inicio_con = ahora();
		pi_conc = calcular_integral_concurrente() ;
		struct timespec fin_con = ahora();
		
		cout << "Valor de PI (calculado secuencialmente)  == " << pi_sec << " en " << duracion(inicio_sec, fin_sec) << endl;
		cout << "Valor de PI (calculado concurrentemente) == " << pi_conc << " en " << duracion(inicio_con, fin_con) << endl ;
	}
	
	
	
}