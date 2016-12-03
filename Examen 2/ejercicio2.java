import monitor.*;
import java.util.Random;

class Peaje extends AbstractMonitor{
	private boolean primero;
	private int tam_cola[2];

	private Condition coches = makeCondition();

	public Peaje(){
		primero = true;
		
		for(int i = 0; i < 2; i++){
			tam_cola[i] = 0;
		}
	}

	public int llegada_peaje(int num_coche){
		enter();
		
		int cola_mas_corta;

		if(tam_cola[0] == tam_cola[1] || tam_cola[0] < tam_cola[1]){
			cola_mas_corta = 0;
			tam_cola[0] += 1;
		}
		else{
			cola_mas_corta = 1;
			tam_cola[1] += 1;
		}

		if(tam_cola[cola_mas_corta] > 1){
			coches.await();
			primero = false;
		}
		else{
			primero = true;
		}

		System.out.println("El coche " + num_coche + " se ha puesto en la cola de la cabina " + cola_mas_corta + "\n");

		leave();
		
		return cola_mas_corta;
	}

	public void pagado(int cabina){
		enter();
		
		tam_cola[cabina] -= 1;
		coches.signal();
		
		leave();
	}
}

class aux{
	static Random genAlea = new Random() ;

	static void dormir_max(int milisecsMax){
    	try{
			Thread.sleep( genAlea.nextInt( milisecsMax ) ) ;
    	}

		catch(InterruptedException e){
			System.err.println("sleep interumpido en 'aux.dormir_max()'");
		}
	}
}

class Hebra_coche implements Runnable{
	public Thread thr;
	private Peaje peaje;
	private int cabina;
	private int coche;

	public Hebra_coche(int num_coche, Peaje nuevo){
		peaje = nuevo;
		coche = num_coche;
		thr = new Thread(this, "Coche " + num_coche);
	}

	public void run(){
		while(true){
			cabina = peaje.llegada_peaje(coche);

			aux.dormir_max(2000);

			peaje.pagado(cabina);
			
			System.out.println("El coche" + coche + " acaba de pagar\n");
		}
	}
}

class ejercicio2{
	public static void main(){
		Peaje peaje = new Peaje();
		Hebra_coche coches[] = new Hebra_coche[6];

		for(int i = 0; i < 6; i++){
			coches[i] = new Hebra_coche(i, peaje);
		}

		for(int i = 0; i < 6; i++){
			coches[i].thr.start();
		}
	}
}

