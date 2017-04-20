package AntColony;

import java.util.LinkedList;
import java.util.Random;

public class Ant{

	private LinkedList<Integer> arrivedCities; 			// �ѵ���ĳ���
	private LinkedList<Integer> allowedCities; 			// ���������ĳ���
	private float[][] delta; 							// ��Ϣ�ر仯����
	private int[][] distance; 							// �������
	private float alpha;
	private float beta;
	private int tourLength; 							// ·������
	private int cityNum; 								// ��������
	private int firstCity; 								// ��ʼ����
	private int currentCity; 							// ��ǰ����
	private Random random;

	 /**
	  * Constructor of Ant
	  * @param num ��������
	  */
	 public Ant(int num){
		 cityNum = num;
		 tourLength = 0;
	 }

	 /**
	  * ��ʼ�����ϣ����ѡ����ʼλ��
	  * @param distance �������
	  * @param a alpha
	  * @param b beta
	  */
	 public void init(int[][] distance, float a, float b){
		 alpha = a;
		 beta = b;
		 allowedCities = new LinkedList<Integer>();
		 arrivedCities = new LinkedList<Integer>();
		 this.distance = distance;
		 delta = new float[cityNum][cityNum];
		 
		 for (int i = 0; i < cityNum; i++) {
			 Integer integer = new Integer(i);
		     allowedCities.add(integer);
		     for (int j = 0; j < cityNum; j++) {
		    	 delta[i][j] = 0.f;
		     }
		 }

		 random = new Random(System.currentTimeMillis());
		 //�������0��cityNum��������cityNum��������
		 firstCity = random.nextInt(cityNum);
		 for (Integer i:allowedCities) {
			 if (i.intValue() == firstCity) {
				 allowedCities.remove(i);
				 break;
			 }
		 }

		arrivedCities.add(Integer.valueOf(firstCity));
		currentCity = firstCity;
	 }
	 
	 /**
	  * ѡ����һ������
	  * @param pheromone ��Ϣ�ؾ���
	  */
	 public void selectNextCity(float[][] pheromone) {
		 //����
		 float[] p = new float[cityNum];
		 float sum = 0.0f;
		 //������ʵķ�ĸ����
		 for (Integer i:allowedCities) {
			 sum += Math.pow(pheromone[currentCity][i.intValue()], alpha) * Math.pow(1.0 / distance[currentCity][i.intValue()], beta);
		 }

		 //������ʾ���
		 for (int i = 0; i < cityNum; i++) {
			 boolean flag = false;
			 for (Integer j:allowedCities) {
				 if (i == j.intValue()) {
					 p[i] = (float) (Math.pow(pheromone[currentCity][i], alpha) * Math.pow(1.0 / distance[currentCity][i], beta)) / sum;
					 flag = true;
					 break;
				 }
			 }
			 if (flag == false) {
				 p[i] = 0.f;
			 }
		 }

		 //���̶�ѡ����һ������
		 //������ɷ�ΧΪ0��1�ĸ�����
		 float selectP = random.nextFloat();
		 int selectCity = 0;
		 float sum1 = 0.f;
		 
		 //
		 for (int i = 0; i < cityNum; i++) {
			 sum1 += p[i];
			 if (sum1 >= selectP) {
				 selectCity = i;
				 break;
			 }
		 }

		 //������ѡ��ĳ�����ȥ��select city
		 for (Integer i:allowedCities) {
			 if (i.intValue() == selectCity) {
				 allowedCities.remove(i);
				 break;
			 }
		 }
		 //�ڽ��ɱ������select city
		 arrivedCities.add(Integer.valueOf(selectCity));
		 //����ǰ���и�Ϊѡ��ĳ���
		 currentCity = selectCity;
		 
	 }

	 /**
	  * ����·������
	  * @return ·������
	  */
	 private int calculateTourLength() {
		 int len = 0;
		 for (int i = 0; i < cityNum - 1; i++) {
			 len += distance[this.arrivedCities.get(i).intValue()][this.arrivedCities.get(i+1).intValue()];
		 }
		 len += distance[this.arrivedCities.get(cityNum-1).intValue()][this.arrivedCities.get(0).intValue()];
	 	return len;
	 }  

	 public LinkedList<Integer> getAllowedCities() {
		 return allowedCities;
	 }

	 public void setAllowedCities(LinkedList<Integer> allowedCities) {
		 this.allowedCities = allowedCities;
	 }

	 public int getTourLength() {
		 tourLength = calculateTourLength();
		 return tourLength;
	 }

	 public void setTourLength(int tourLength) {
		 this.tourLength = tourLength;
	 }
	 
	 public int getCityNum() {
		 return cityNum;
	 }

	 public void setCityNum(int cityNum) {
		 this.cityNum = cityNum;
	 }

	 public LinkedList<Integer> getArrivedCities() {
		 return arrivedCities;
	 }

	 public void setArrivedCities(LinkedList<Integer> tabu) {
		 this.arrivedCities = tabu;
	 }

	 public float[][] getDelta() {
		 return delta;
	 }
	 
	 public void setDelta(float[][] delta) {
		 this.delta = delta;
	 }

	 public int getFirstCity() {
		 return firstCity;
	 }

	 public void setFirstCity(int firstCity) {
		 this.firstCity = firstCity;
	 }

}
