class Contador{
	private volatile int actual;
	
	public Contador(int inicial){
		actual = inicial;
	}
	
	public synchronized void inc(){
		actual++;
	}
	
	public synchronized void dec(){
		actual--;
	}
	
	public synchronized int valor(){
		return actual;
	}
}
