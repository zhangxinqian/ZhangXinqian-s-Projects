package SimulatedAnnealing;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Random;

public class SA {

	Random random;									//生成随机数
	private int  cityNum;							//城市数量
	private double initTem;							//初始温度
	private double finTem;							//冷却温度
	private double rate;							//降温速率
	private int[][] distance; 						//距离矩阵
	private int bestLength; 						//最佳长度
	private int tourLength;							//一次退火产生的路径长度
	private int[] bestTour; 						//最佳路径	
	private int[] tour;								//一次退火产生的路径
	private int[] tempTour;							//临时路径
	private double k;								//Boltzmann常数
	private int iterations;							//迭代次数
	
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
	 * 初始化SA算法类
	 * @param filename 数据文件名，该文件存储所有城市节点坐标数据
	 * @throws IOException
	 */
	public void init(String filename) throws IOException {
		
		// 读取数据
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

		// 计算距离矩阵，针对具体问题，距离计算方法也不一样，此处用的是att48作为案例，它有48个城市，距离计算方法为伪欧氏距离，最优值为10628
		for (int i = 0; i < cityNum - 1; i++) {
			distance[i][i] = 0; // 对角线为0
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
		
		//初始生成一条随机路线
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
		//进行迭代
		for (int n = 0; n < iterations; n++) {
			double T = initTem;
			double dE = 0.0;
			int newLength = 0;
			//退火过程
			while (T > finTem) {
				generateNewTour();
				newLength = calculateTourLength();
				dE = tourLength - newLength;
				if (dE >= 0) {//总是接受新的路径
					tourLength = newLength;
					for (int i = 0; i < cityNum; i++) {
						tour[i] = tempTour[i];
					}
				} else {//以一定概率接受新的路径规划
					 //随机生成范围为0至1的浮点数
					 double p = random.nextDouble();
					 if (Math.exp(dE/(k*T)) > p) {
						 tourLength = newLength;
						for (int i = 0; i < cityNum; i++) {
							tour[i] = tempTour[i];
						}
					 }
				}
				T *= rate;//温度以速率rate减少
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
		//退火过程
		while (T > finTem) {
			//迭代次数
			for (int n = 0; n < iterations; n++) {
				generateNewTour();
				newLength = calculateTourLength();
				dE = tourLength - newLength;
				if (dE >= 0) {//总是接受新的路径
					tourLength = newLength;
					for (int i = 0; i < cityNum; i++) {
						tour[i] = tempTour[i];
					}
				} else {//以一定概率接受新的路径规划
					 //随机生成范围为0至1的浮点数
					 double p = random.nextDouble();
					 if (Math.exp(dE/(k*T)) > p) {
						 tourLength = newLength;
						for (int i = 0; i < cityNum; i++) {
							tour[i] = tempTour[i];
						}
					 }
				}
			}
			
			T *= rate;//温度以速率rate减少
			
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
