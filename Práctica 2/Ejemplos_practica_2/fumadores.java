import monitor.*;
import java.util.Random;

class Estanco extends AbstractMonitor{
	private int ingrediente_producido;
	private Boolean puede_consumir;

	private Condition producir = makeCondition();
	private Condition fumadores[] = new Condition[3];

	//Constructor del monitor
	public Estanco(){
		ingrediente_producido = -1;
		puede_consumir = false;

		for(int i = 0; i < 3; i++){
			fumadores[i] = makeCondition();
		}
	}

	//Invocado por cada fumador, indicando su ingrediente o número
	public void obtenerIngrediente(int miIngrediente){
		enter();

		if(miIngrediente != ingrediente_producido){
			fumadores[miIngrediente].await();
		}

		puede_consumir = false;

		System.out.println("El fumador " + miIngrediente + " esta fumando\n");

		producir.signal();

		leave();
	}

	//Invocado por el estanquero, indicando el ingrediente que pone
	public void ponerIngrediente(int ingrediente){
		enter();

		ingrediente_producido = ingrediente;
		puede_consumir = true;

		if(ingrediente == 0){
			System.out.println("Se han producido cerillas.\n");
		}

		if(ingrediente == 1){
			System.out.println("Se ha producido tabaco.\n");
		}

		if(ingrediente == 2){
			System.out.println("Se ha producido papel.\n");
		}

		fumadores[ingrediente].signal();

		leave();
	}

	//Invocado por el estanquero
	public void esperarRecogidaIngrediente(){
		enter();

		if(puede_consumir){
			producir.await();
		}

		leave();
	}
}

//Cada instancia de la hebra fumador guarda su número de fumador(el ingrediente que necesita)
class Fumador implements Runnable{
	private int miIngrediente;
	public Thread thr;
	private Estanco estanco;

	public Fumador(int p_miIngrediente, Estanco p_estanco){
		miIngrediente = p_miIngrediente;
		estanco = p_estanco;
		thr = new Thread(this, "Fumador " + miIngrediente);
	}

	public void run(){
		while(true){
			estanco.obtenerIngrediente(miIngrediente);

			aux.dormir_max(2000);

			System.out.println("El fumador " + miIngrediente + " ha terminado de fumar\n");
		}
	}
}

//El estanquero continuamente produce ingredientes y espera a que se recojan
class Estanquero implements Runnable{
	public Thread thr;
	private Estanco estanco;

	public Estanquero(Estanco p_estanco){
		estanco = p_estanco;
		thr = new Thread(this, "Estanquero");
	}

	public void run(){
		int ingrediente;

		while(true){
			ingrediente = (int)(Math.random() * 3.0);   //0, 1 o 2
			estanco.ponerIngrediente(ingrediente);
			estanco.esperarRecogidaIngrediente();
		}
	}
}

class aux{
  static Random genAlea = new Random() ;

  static void dormir_max( int milisecsMax ){
	try{
	  Thread.sleep( genAlea.nextInt( milisecsMax ) ) ;
	}

	catch( InterruptedException e ){
	  System.err.println("sleep interumpido en 'aux.dormir_max()'");
	}
  }
}

class MainFumadores{
	public static void main(String [] args){
		Estanco estanco = new Estanco();
		Estanquero estanquero = new Estanquero(estanco);
		Fumador fumador[] = new Fumador[3];

		for(int i = 0; i < 3; i++){
			fumador[i] = new Fumador(i, estanco);
		}

		estanquero.thr.start();

		for(int i = 0; i < 3; i++){
			fumador[i].thr.start();
		}
	}
}
