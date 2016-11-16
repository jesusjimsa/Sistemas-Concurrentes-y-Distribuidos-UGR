import monitor.*;
import java.util.Random;

class Barberia extends AbstractMonitor{
	private Boolean pelando, durmiendo;
	private int esperando;

	private Condition barbero = makeCondition();
	private Condition clientes[] = new Condition[8];

	public Barberia(){
		pelando = false;	//Al principio no se está pelando a nadie
		durmiendo = true;	//El barbero comienza dormido
		esperando = 0;	//Al principio no hay clientes esperando

		for(int i = 0; i < 8; i++){
			clientes[i] = makeCondition();
		}
	}

	//Invcado por los clientes para cortarse el pelo
	public void cortarPelo(int num_cliente){
		enter();

		if(durmiendo){
			durmiendo = false;

			System.out.println("El barbero se ha despertado\n");
		}

		if(pelando){
			clientes[num_cliente].await();
		}

		pelando = true;

		System.out.println("El cliente " + num_cliente + " ha comenzado a pelarse\n");

		barbero.signal();

		leave();
	}

	//Invocado por el barbero para esperar (si procede) a un nuevo cliente y sentarlo para el corte
	public void siguienteCliente(){

	}

	//Invocado por el barbero para indicar que ha terminado de cortar el pelo
	public void finCliente(){

	}
}
