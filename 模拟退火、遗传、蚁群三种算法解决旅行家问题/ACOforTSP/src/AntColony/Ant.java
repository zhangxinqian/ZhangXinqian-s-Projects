package AntColony;

import java.util.LinkedList;
import java.util.Random;

public class Ant{

	private LinkedList<Integer> arrivedCities; 			// 已到达的城市
	private LinkedList<Integer> allowedCities; 			// 允许搜索的城市
	private float[][] delta; 							// 信息素变化矩阵
	private int[][] distance; 							// 距离矩阵
	private float alpha;
	private float beta;
	private int tourLength; 							// 路径长度
	private int cityNum; 								// 城市数量
	private int firstCity; 								// 起始城市
	private int currentCity; 							// 当前城市
	private Random random;

	 /**
	  * Constructor of Ant
	  * @param num 蚂蚁数量
	  */
	 public Ant(int num){
		 cityNum = num;
		 tourLength = 0;
	 }

	 /**
	  * 初始化蚂蚁，随机选择起始位置
	  * @param distance 距离矩阵
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
		 //随机生成0至cityNum（不包括cityNum）的整数
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
	  * 选择下一个城市
	  * @param pheromone 信息素矩阵
	  */
	 public void selectNextCity(float[][] pheromone) {
		 //概率
		 float[] p = new float[cityNum];
		 float sum = 0.0f;
		 //计算概率的分母部分
		 for (Integer i:allowedCities) {
			 sum += Math.pow(pheromone[currentCity][i.intValue()], alpha) * Math.pow(1.0 / distance[currentCity][i.intValue()], beta);
		 }

		 //计算概率矩阵
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

		 //轮盘赌选择下一个城市
		 //随机生成范围为0至1的浮点数
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

		 //从允许选择的城市中去除select city
		 for (Integer i:allowedCities) {
			 if (i.intValue() == selectCity) {
				 allowedCities.remove(i);
				 break;
			 }
		 }
		 //在禁忌表中添加select city
		 arrivedCities.add(Integer.valueOf(selectCity));
		 //将当前城市改为选择的城市
		 currentCity = selectCity;
		 
	 }

	 /**
	  * 计算路径长度
	  * @return 路径长度
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
