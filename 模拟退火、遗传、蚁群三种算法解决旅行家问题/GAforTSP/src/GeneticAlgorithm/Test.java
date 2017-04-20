package GeneticAlgorithm;

import java.io.IOException;

public class Test {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		GA ga = new GA(48, 300, 1000, 0.8f, 0.2f);
		try {
			ga.init("data.txt");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		ga.solve();
	}

}
