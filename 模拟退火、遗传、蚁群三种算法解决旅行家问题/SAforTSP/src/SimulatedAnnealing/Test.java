package SimulatedAnnealing;

import java.io.IOException;

public class Test {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		SA sa = new SA(48, 1000, 10, 0.9, 10, 1000);
		try {
			sa.init("data.txt");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		sa.solve();
	}

}
