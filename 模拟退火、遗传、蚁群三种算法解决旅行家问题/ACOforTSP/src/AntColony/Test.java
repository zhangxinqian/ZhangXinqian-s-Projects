package AntColony;

import java.io.IOException;

public class Test {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		ACO aco = new ACO(48, 10, 100, 1.f, 8.f, 0.25f);
		try {
			aco.init("data.txt");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		aco.solve();
	}

}
