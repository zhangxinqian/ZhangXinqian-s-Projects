package AntColony;

import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.IOException;

public class ACO {

	private Ant[] ants;				//����
	private int antNum; 			//��������
	private int cityNum; 			//��������
	private int MAX_GEN; 			//��������
	private float[][] pheromone; 	//��Ϣ�ؾ���
	private int[][] distance; 		//�������
	private int bestLength; 		//��ѳ���
	private int[] bestTour; 		//���·��

	//��������
	private float alpha;
	private float beta;
	private float rho;				//��Ϣ�ػӷ���

	/**
	 * Constructor of Ant Colony Optimization Algorithm
	 * @param cn 
	 * @param an
	 * @param gn
	 * @param a
	 * @param b
	 * @param r
	 */
	public ACO(int cn, int an, int gn, float a, float b, float r) {
		cityNum = cn;
		antNum = an;
		ants = new Ant[antNum];
		MAX_GEN = gn;
		alpha = a;
		beta = b;
		rho = r;
	}

	/**
	 * ��ʼ��ACO�㷨��
	 * @param filename �����ļ��������ļ��洢���г��нڵ���������
	 * @throws IOException
	 */
	public void init(String filename) throws IOException {
		//��ȡ����  
		int[] x;  
		int[] y;  
		String strbuff;
		BufferedReader data = new BufferedReader(
				new InputStreamReader(new FileInputStream(filename))
				);  

		distance = new int[cityNum][cityNum];  
		x = new int[cityNum];  
		y = new int[cityNum];  

		for (int i = 0; i < cityNum; i++) {  
			strbuff = data.readLine(); 
			String[] strcol = strbuff.split(" ");  
			x[i] = Integer.valueOf(strcol[1]);  
			y[i] = Integer.valueOf(strcol[2]);  
		}  

		//���������� ����Ծ������⣬������㷽��Ҳ��һ�����˴��õ���att48��Ϊ����������48�����У�������㷽��Ϊαŷ�Ͼ��룬����ֵΪ10628
		for (int i = 0; i < cityNum - 1; i++) {
			distance[i][i] = 0;  //�Խ���Ϊ0
			for (int j = i + 1; j < cityNum; j++) {  
				double rij = Math.sqrt(((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j])) / 10.0);
				int tij = (int) Math.round(rij);
				if (tij < rij) {
					distance[i][j] = tij + 1;
				}else {
					distance[i][j] = tij;
				}
				distance[j][i] = distance[i][j];
			}
		}
		distance[cityNum-1][cityNum-1] = 0;  
		
		bestLength = Integer.MAX_VALUE;  
		bestTour = new int[cityNum];

		//��ʼ����Ϣ�ؾ���  
		pheromone = new float[cityNum][cityNum];  
		for(int i = 0; i < cityNum; i++)  {  
			for(int j = 0; j < cityNum; j++){  
				pheromone[i][j] = 0.1f;  //��ʼ��Ϊ0.1
			}  
		}  
		
		//�����������  
		for(int i = 0; i < antNum; i++)	{  
			ants[i] = new Ant(cityNum);  
			ants[i].init(distance, alpha, beta);  
		}
	}

	
	//������Ϣ��
	private void updatePheromone() {
		
		//��Ϣ�ػӷ� ����rho�����ʼ���
		for(int i = 0; i < cityNum; i++) {
			for(int j = 0; j < cityNum; j++) {
				pheromone[i][j] = pheromone[i][j] * (1 - rho);
			}
		}
		
		//��Ϣ�ظ���  
		for(int i = 0; i < cityNum; i++) {  
			for(int j = 0; j < cityNum; j++) {  
				for (int k = 0; k < antNum; k++) {
					//����i��j֮�����Ϣ��Ϊ��������������������֮�����µ���Ϣ��֮��
					pheromone[i][j] += ants[k].getDelta()[i][j];
				} 
			}  
		}  
	}

	private void printOptimal() {
		System.out.println("The optimal length is: " + bestLength);
		System.out.println("The optimal tour is: ");
		for (int i = 0; i < cityNum; i++) {
			System.out.print(bestTour[i] + " ");
		}
		System.out.println(bestTour[0]);
	}
	
	//��⺯��
	public void solve() {
		
		for (int g = 0; g < MAX_GEN; g++) {
			
			for (int i = 0; i < antNum; i++) {
				
				for (int j = 1; j < cityNum; j++) {
					ants[i].selectNextCity(pheromone);
				}
				
				//��¼Ŀǰ������·
				int tourLength = ants[i].getTourLength();
				if (tourLength < bestLength) {
					bestLength = tourLength;
					for (int k = 0; k < cityNum; k++) {//cityNum+1�����һ����firstCity
						bestTour[k] = ants[i].getArrivedCities().get(k).intValue();
					}
				}

				//������Ϣ��
				for (int j = 0; j < cityNum - 1; j++) {
			        //�ܾ���Խ�̣���Ϣ��ԽŨ
					ants[i].getDelta()[ants[i].getArrivedCities().get(j).intValue()][ants[i].getArrivedCities().get(j+1).intValue()] = (float) (1.0 / tourLength);
					//��Ϊ�ǶԳ�TSP������i������j֮�����Ϣ����j��i����ͬ
					ants[i].getDelta()[ants[i].getArrivedCities().get(j+1).intValue()][ants[i].getArrivedCities().get(j).intValue()] = (float) (1.0 / tourLength);
				}
				ants[i].getDelta()[ants[i].getArrivedCities().get(cityNum-1).intValue()][ants[i].getArrivedCities().get(0).intValue()] = (float) (1.0 / tourLength);
				ants[i].getDelta()[ants[i].getArrivedCities().get(0).intValue()][ants[i].getArrivedCities().get(cityNum-1).intValue()] = (float) (1.0 / tourLength);
				
			}

			//������Ϣ��
			updatePheromone();
			
			//���³�ʼ������
			for(int i = 0; i < antNum; i++){  
				ants[i].init(distance, alpha, beta);
			}  
		}

		//��ӡ��ѽ��
		printOptimal();
	}
	
	public Ant[] getAnts() {
		return ants;
	}

	public void setAnts(Ant[] ants) {
		this.ants = ants;
	}

	public int getAntNum() {
		return antNum;
	}

	public void setAntNum(int m) {
		this.antNum = m;
	}

	public int getCityNum() {
		return cityNum;
	}

	public void setCityNum(int cityNum) {
		this.cityNum = cityNum;
	}

	public int getMAX_GEN() {
		return MAX_GEN;
	}

	public void setMAX_GEN(int mAX_GEN) {
		MAX_GEN = mAX_GEN;
	}

	public float[][] getPheromone() {
		return pheromone;
	}

	public void setPheromone(float[][] pheromone) {
		this.pheromone = pheromone;
	}

	public int[][] getDistance() {
		return distance;
	}

	public void setDistance(int[][] distance) {
		this.distance = distance;
	}

	public int getBestLength() {
		return bestLength;
	}
	
	public void setBestLength(int bestLength) {
		this.bestLength = bestLength;
	}

	public int[] getBestTour() {
		return bestTour;
	}

	public void setBestTour(int[] bestTour) {
		this.bestTour = bestTour;
	}

	public float getAlpha() {
		return alpha;
	}

	public void setAlpha(float alpha) {
		this.alpha = alpha;
	}

	public float getBeta() {
		return beta;
	}

	public void setBeta(float beta) {
		this.beta = beta;
	}

	public float getRho() {
		return rho;
	}

	public void setRho(float rho) {
		this.rho = rho;
	}

}
