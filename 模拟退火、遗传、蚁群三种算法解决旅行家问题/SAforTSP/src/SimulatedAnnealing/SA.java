package SimulatedAnnealing;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Random;

public class SA {

	Random random;									//���������
	private int  cityNum;							//��������
	private double initTem;							//��ʼ�¶�
	private double finTem;							//��ȴ�¶�
	private double rate;							//��������
	private int[][] distance; 						//�������
	private int bestLength; 						//��ѳ���
	private int tourLength;							//һ���˻������·������
	private int[] bestTour; 						//���·��	
	private int[] tour;								//һ���˻������·��
	private int[] tempTour;							//��ʱ·��
	private double k;								//Boltzmann����
	private int iterations;							//��������
	
	public SA(int cityNum, double initTem, double finTem, double rate, double k, int iterations)
	{
		this.cityNum = cityNum;
		this.initTem = initTem;
		this.finTem = finTem;
		this.rate = rate;
		this.k = k;
		this.iterations = iterations;
	}
	
	/**
	 * ��ʼ��SA�㷨��
	 * @param filename �����ļ��������ļ��洢���г��нڵ���������
	 * @throws IOException
	 */
	public void init(String filename) throws IOException {
		
		// ��ȡ����
		int[] x;
		int[] y;
		String strbuff;
		BufferedReader data = new BufferedReader(new InputStreamReader(
				new FileInputStream(filename)));

		distance = new int[cityNum][cityNum];
		x = new int[cityNum];
		y = new int[cityNum];

		for (int i = 0; i < cityNum; i++) {
			strbuff = data.readLine();
			String[] strcol = strbuff.split(" ");
			x[i] = Integer.valueOf(strcol[1]);
			y[i] = Integer.valueOf(strcol[2]);
		}

		// ������������Ծ������⣬������㷽��Ҳ��һ�����˴��õ���att48��Ϊ����������48�����У�������㷽��Ϊαŷ�Ͼ��룬����ֵΪ10628
		for (int i = 0; i < cityNum - 1; i++) {
			distance[i][i] = 0; // �Խ���Ϊ0
			for (int j = i + 1; j < cityNum; j++) {
				double rij = Math.sqrt(((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j])) / 10.0);
				int tij = (int) Math.round(rij);
				if (tij < rij) {
					distance[i][j] = tij + 1;
				} else {
					distance[i][j] = tij;
				}
				distance[j][i] = distance[i][j];
			}
		}
		distance[cityNum-1][cityNum-1] = 0;
		
		bestLength = Integer.MAX_VALUE;
		tourLength = 0;
		bestTour = new int[cityNum];
		tempTour = new int[cityNum];
		tour = new int[cityNum];
		
		random = new Random(System.currentTimeMillis());
		
	}
	
	private void printOptimal() {
		System.out.println("The optimal length is: " + bestLength);
		System.out.println("The optimal tour is: ");
		for (int i = 0; i < cityNum; i++) {
			System.out.print(bestTour[i] + " ");
		}
		System.out.println(bestTour[0]);
	}
	
	private void generateNewTour() {
		int i = random.nextInt(cityNum);
		int j = random.nextInt(cityNum);
		while (i == j) {
			j = random.nextInt(cityNum);
		}
		
		//swap
		tempTour[i] = tempTour[i] + tempTour[j];
		tempTour[j] = tempTour[i] - tempTour[j];
		tempTour[i] = tempTour[i] - tempTour[j];
	}
	
	private int calculateTourLength() {
		 int len = 0;
		 for (int i = 0; i < cityNum - 1; i++) {
			 len += distance[tempTour[i]][tempTour[i+1]];
		 }
		 len += distance[tempTour[cityNum-1]][tempTour[0]];
	 	return len;
	 } 
	
	public void solve() {
		
		//��ʼ����һ�����·��
		for (int i = 0; i < cityNum; i++) {
			tempTour[i] = i;
		}
		for (int i = 0; i < cityNum; i++) {
			int j = random.nextInt(cityNum);
			//swap
			tempTour[i] = tempTour[i] + tempTour[j];
			tempTour[j] = tempTour[i] - tempTour[j];
			tempTour[i] = tempTour[i] - tempTour[j];
		}
		tourLength = calculateTourLength();
		for (int i = 0; i < cityNum; i++) {
			tour[i] = tempTour[i];
		}
		
		/*
		//���е���
		for (int n = 0; n < iterations; n++) {
			double T = initTem;
			double dE = 0.0;
			int newLength = 0;
			//�˻����
			while (T > finTem) {
				generateNewTour();
				newLength = calculateTourLength();
				dE = tourLength - newLength;
				if (dE >= 0) {//���ǽ����µ�·��
					tourLength = newLength;
					for (int i = 0; i < cityNum; i++) {
						tour[i] = tempTour[i];
					}
				} else {//��һ�����ʽ����µ�·���滮
					 //������ɷ�ΧΪ0��1�ĸ�����
					 double p = random.nextDouble();
					 if (Math.exp(dE/(k*T)) > p) {
						 tourLength = newLength;
						for (int i = 0; i < cityNum; i++) {
							tour[i] = tempTour[i];
						}
					 }
				}
				T *= rate;//�¶�������rate����
			}
			
			if (bestLength > tourLength) {
				bestLength = tourLength;
				for (int i = 0; i < cityNum; i++) {
					bestTour[i] = tour[i];
				}
			}
			
		}
		*/
		
		double T = initTem;
		double dE = 0.0;
		int newLength = 0;
		//�˻����
		while (T > finTem) {
			//��������
			for (int n = 0; n < iterations; n++) {
				generateNewTour();
				newLength = calculateTourLength();
				dE = tourLength - newLength;
				if (dE >= 0) {//���ǽ����µ�·��
					tourLength = newLength;
					for (int i = 0; i < cityNum; i++) {
						tour[i] = tempTour[i];
					}
				} else {//��һ�����ʽ����µ�·���滮
					 //������ɷ�ΧΪ0��1�ĸ�����
					 double p = random.nextDouble();
					 if (Math.exp(dE/(k*T)) > p) {
						 tourLength = newLength;
						for (int i = 0; i < cityNum; i++) {
							tour[i] = tempTour[i];
						}
					 }
				}
			}
			
			T *= rate;//�¶�������rate����
			
			if (bestLength > tourLength) {
				bestLength = tourLength;
				for (int i = 0; i < cityNum; i++) {
					bestTour[i] = tour[i];
				}
			}
			
		}
		
		printOptimal();
		
	}
		
}
