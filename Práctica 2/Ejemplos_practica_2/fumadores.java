import monitor.*;

class Estanco extends AbstractMonitor{
    //Invocado por cada fumador, indicando su ingrediente o número
    public void obtenerIngrediente(int miIngrediente){
        /////////////////////
    }

    //Invocado por el estanquero, indicando el ingrediente que pone
    public void ponerIngrediente(int ingrediente){
        ////////////////////
    }

    //Invocado por el estanquero
    public void esperarRecogidaIngrediente(){
        ///////////////////
    }
}

//Cada instancia de la hebra fumador guarda su número de fumador(el ingrediente que necesita)
class Fumador implements Runnable{
    int miIngrediente;
    public Thread thr;
    /////////////////

    public Fumador(int p_miIngrediente, ...){
        ////////////////
    }

    public void run(){
        while(true){
            estanco.obtenerIngrediente(miIngrediente);
            aux.dormir_max(2000);
        }
    }
}

//El estanquero continuamente produce ingredientes y espera a que se recojan
class Estanquero implements Runnable{
    public Thread thr;
    //////////////////
    public void run(){
        int ingrediente;

        while(true){
            ingrediente = (int)(Math.random() * 3.0);   //0, 1 o 2
            estanco.ponerIngrediente(ingrediente);
            estanco.esperarRecogidaIngrediente();
        }
    }
}
