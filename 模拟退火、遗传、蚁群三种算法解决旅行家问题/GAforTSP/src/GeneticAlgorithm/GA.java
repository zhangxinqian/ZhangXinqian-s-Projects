package GeneticAlgorithm;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Random;

public class GA {

	private int cityNum;							//��������
	private int population;							//��Ⱥ��ģ
	private int	maxGen;								//���ɵĴ���
	private int bestGen;							//��ѳ��ֵĴ���
	private int[][] distance; 						//�������
	private int[] bestTour; 						//���·��
	private int bestLength; 						//��ѳ���
	  
    private int[][] oldPopulation;  				// ��ʼ��Ⱥ��������Ⱥ��������ʾ��Ⱥ��ģ��һ�д���һ�����壬��Ⱦɫ�壬�б�ʾȾɫ�����Ƭ��
    private int[][] newPopulation;					// �µ���Ⱥ���Ӵ���Ⱥ  
    private int[] fitness;							// ��Ⱥ��Ӧ�ȣ���ʾ��Ⱥ�и����������Ӧ�ȣ���·�̳��ȣ�ԽС��Ӧ��Խ��
	
	private double[] p;								// ��Ⱥ�и���������ۼƸ��� 
	private double pc;								//Ⱦɫ�彻�����crossover
	private double pm;								//Ⱦɫ��������mutation
	private int t;									// ��ǰ����
	private Random random;							//���������
	
	public GA(int cityNum, int population, int gen, double pc, double pm) {
		this.cityNum = cityNum;
		this.population = population;
		this.maxGen = gen;
		this.pc = pc;
		this.pm = pm;
	}
	
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
		bestTour = new int[cityNum];
		bestGen = 0;
		t = 0;
		newPopulation = new int[population][cityNum];  
        oldPopulation = new int[population][cityNum];  
        fitness = new int[population];  
        p = new double[population];
		
		random = new Random(System.currentTimeMillis());
		
	}
	
	private void initPopulation() {
		int i, j, k;
		for (k = 0; k < population; k++) {// ��Ⱥ��
			oldPopulation[k][0] = random.nextInt(65535) % cityNum;
			for (i = 1; i < cityNum;) { // Ⱦɫ�峤�� 
				oldPopulation[k][i] = random.nextInt(65535) % cityNum;
				for (j = 0; j < i; j++) {
					if (oldPopulation[k][i] == oldPopulation[k][j]) {
						break;
					}
				}
				if (j == i) {
					i++;
				}
			}
		}
	}
	
	private int calculateTourLength(int[] chromosome) {
        int len = 0;  
        // Ⱦɫ�壬��ʼ����,����1,����2...����n  
        for (int i = 1; i < cityNum; i++) {  
            len += distance[chromosome[i-1]][chromosome[i]];  
        }  
        // ����n,��ʼ����  
        len += distance[chromosome[cityNum-1]][chromosome[0]];  
        
        return len;  
    }  
	
	// ������Ⱥ�и���������ۻ����ʣ�ǰ�����Ѿ�����������������Ӧ��fitness[max]����Ϊ����ѡ�����һ����  
    private void countRate() {  
        int k;  
        double sumFitness = 0;// ��Ӧ���ܺ�  
  
        double[] tempDbl = new double[population];  
  
        for (k = 0; k < population; k++) {  
            tempDbl[k] = 10.0 / fitness[k];  
            sumFitness += tempDbl[k];  
        }  
  
        p[0] = (double) (tempDbl[0] / sumFitness);  
        for (k = 1; k < population; k++) {  
            p[k] = (double) (tempDbl[k] / sumFitness + p[k-1]);
        }
  
    }  
	
    // ����Ⱦɫ�壬k��ʾ��Ⱦɫ������Ⱥ�е�λ�ã�kk��ʾ�ɵ�Ⱦɫ������Ⱥ�е�λ��  
    private void copyGh(int k, int kk) {  
        int i;  
        for (i = 0; i < cityNum; i++) {  
            newPopulation[k][i] = oldPopulation[kk][i];  
        }  
    }  
    
    // ��ѡĳ����Ⱥ����Ӧ����ߵĸ��壬ֱ�Ӹ��Ƶ��Ӵ���  
    // ǰ�����Ѿ�����������������Ӧ��Fitness[max]  
    private void selectBestGh() {  
        int k, i, maxid;  
        int maxevaluation;  //ԽС��Ӧ��Խ��
  
        maxid = 0;  
        maxevaluation = fitness[0];  
        for (k = 1; k < population; k++) {  
            if (maxevaluation > fitness[k]) {  
                maxevaluation = fitness[k];  
                maxid = k;  
            }  
        }  
  
        if (bestLength > maxevaluation) {  
            bestLength = maxevaluation;  
            bestGen = t;// ��õ�Ⱦɫ����ֵĴ���;  
            for (i = 0; i < cityNum; i++) {  
                bestTour[i] = oldPopulation[maxid][i];
            }
        }  
  
        // ����Ⱦɫ�壬k��ʾ��Ⱦɫ������Ⱥ�е�λ�ã�kk��ʾ�ɵ�Ⱦɫ������Ⱥ�е�λ��  
        copyGh(0, maxid);// ��������Ⱥ����Ӧ����ߵ�Ⱦɫ��k���Ƶ�����Ⱥ�У����ڵ�һλ0  
    }  
    
    // ����ѡ�������ѡ  
    private void select() {  
        int k, i, selectId;  
        double ran1; 
        for (k = 1; k < population; k++) {  
            ran1 = (double) (random.nextInt(65535) % 1000 / 1000.0);  
            // ������ʽ  
            for (i = 0; i < population; i++) {  
                if (ran1 <= p[i]) {  
                    break;  
                }  
            }  
            selectId = i; 
            copyGh(k, selectId);  
        }  
    }
    
    //���������������������  
    private void evolution() {  
        int k;  
        // ��ѡĳ����Ⱥ����Ӧ����ߵĸ���  
        selectBestGh();  
        // ����ѡ�������ѡscale-1����һ������  
        select();  
   
        double r;  
        // ���淽��  
        for (k = 0; k < population; k = k + 2) {  
            r = random.nextDouble();// /��������  
            if (r < pc) {   
                //OXCross(k, k + 1);// ���н���  
                OXCross1(k, k + 1);  
            } else {  
                r = random.nextDouble();// /��������  
                // ����  
                if (r < pm) {    
                    OnCVariation(k);  
                }  
                r = random.nextDouble();// /��������    
                // ����  
                if (r < pm) {    
                    OnCVariation(k + 1);  
                }  
            }  
  
        }  
    }  
  
    //�����������������Ⱦɫ�岻���н������  
    private void evolution1() {  
        int k;  
        // ��ѡĳ����Ⱥ����Ӧ����ߵĸ���  
        selectBestGh();  
        // ����ѡ�������ѡscale-1����һ������  
        select();  
    
        double r;  
  
        //k=0�����ţ����ֱ�������Ӵ�
        for (k = 1; k + 1 < population / 2; k = k + 2) {  
            r = random.nextDouble();// /��������  
            if (r < pc) {  
                OXCross1(k, k + 1);// ���н���  
                //OXCross(k,k+1);//���н���  
            } else {  
                r = random.nextDouble();// /��������  
                // ����  
                if (r < pm) {  
                    OnCVariation(k);  
                }  
                r = random.nextDouble();// /��������  
                // ����  
                if (r < pm) {  
                    OnCVariation(k + 1);  
                }  
            }  
        }  
        if (k == population / 2 - 1)// ʣ���һ��Ⱦɫ��û�н���L-1  
        {  
            r = random.nextDouble();// /��������  
            if (r < pm) {  
                OnCVariation(k);  
            }  
        }  
  
    }  
  
    // ��OX��������  
    private void OXCross(int k1, int k2) {  
        int i, j, k, flag;  
        int ran1, ran2, temp;  
        int[] Gh1 = new int[cityNum];  
        int[] Gh2 = new int[cityNum];  
  
        ran1 = random.nextInt(65535) % cityNum;  
        ran2 = random.nextInt(65535) % cityNum;  
  
        while (ran1 == ran2) {  
            ran2 = random.nextInt(65535) % cityNum;  
        }  
  
        if (ran1 > ran2)// ȷ��ran1<ran2  
        {  
            temp = ran1;  
            ran1 = ran2;  
            ran2 = temp;  
        }  
       
        flag = ran2 - ran1 + 1;// ɾ���ظ�����ǰȾɫ�峤��  
        for (i = 0, j = ran1; i < flag; i++, j++) {  
            Gh1[i] = newPopulation[k2][j];  
            Gh2[i] = newPopulation[k1][j];  
        }  
        // �Ѿ���ֵi=ran2-ran1������  
  
        for (k = 0, j = flag; j < cityNum;)// Ⱦɫ�峤��  
        {  
            Gh1[j] = newPopulation[k1][k++];  
            for (i = 0; i < flag; i++) {  
                if (Gh1[i] == Gh1[j]) {  
                    break;  
                }  
            }  
            if (i == flag) {  
                j++;  
            }  
        }  
  
        for (k = 0, j = flag; j < cityNum;)// Ⱦɫ�峤��  
        {  
            Gh2[j] = newPopulation[k2][k++];  
            for (i = 0; i < flag; i++) {  
                if (Gh2[i] == Gh2[j]) {  
                    break;  
                }  
            }  
            if (i == flag) {  
                j++;  
            }  
        }  
  
        for (i = 0; i < cityNum; i++) {  
            newPopulation[k1][i] = Gh1[i];// ������ϷŻ���Ⱥ  
            newPopulation[k2][i] = Gh2[i];// ������ϷŻ���Ⱥ  
        }  
  
    }  
  
    // ��������,��ͬȾɫ�彻�������ͬ�Ӵ�Ⱦɫ��  
    private void OXCross1(int k1, int k2) {  
        int i, j, k, flag;  
        int ran1, ran2, temp;  
        int[] Gh1 = new int[cityNum];  
        int[] Gh2 = new int[cityNum];  
  
        ran1 = random.nextInt(65535) % cityNum;  
        ran2 = random.nextInt(65535) % cityNum;  
        while (ran1 == ran2) {  
            ran2 = random.nextInt(65535) % cityNum;  
        }  
  
        if (ran1 > ran2)// ȷ��ran1<ran2  
        {  
            temp = ran1;  
            ran1 = ran2;  
            ran2 = temp;  
        }  
  
        // ��Ⱦɫ��1�еĵ��������Ƶ�Ⱦɫ��2���ײ�  
        for (i = 0, j = ran2; j < cityNum; i++, j++) {  
            Gh2[i] = newPopulation[k1][j];  
        }  
  
        flag = i;// Ⱦɫ��2ԭ����ʼλ��  
  
        for (k = 0, j = flag; j < cityNum;)// Ⱦɫ�峤��  
        {  
            Gh2[j] = newPopulation[k2][k++];  
            for (i = 0; i < flag; i++) {  
                if (Gh2[i] == Gh2[j]) {  
                    break;  
                }  
            }  
            if (i == flag) {  
                j++;  
            }  
        }  
  
        flag = ran1;  
        for (k = 0, j = 0; k < cityNum;)// Ⱦɫ�峤��  
        {  
            Gh1[j] = newPopulation[k1][k++];  
            for (i = 0; i < flag; i++) {  
                if (newPopulation[k2][i] == Gh1[j]) {  
                    break;  
                }  
            }  
            if (i == flag) {  
                j++;  
            }  
        }  
  
        flag = cityNum - ran1;  
  
        for (i = 0, j = flag; j < cityNum; j++, i++) {  
            Gh1[j] = newPopulation[k2][i];  
        }  
  
        for (i = 0; i < cityNum; i++) {  
            newPopulation[k1][i] = Gh1[i];// ������ϷŻ���Ⱥ  
            newPopulation[k2][i] = Gh2[i];// ������ϷŻ���Ⱥ  
        }  
    }  
  
    // ��ζԻ���������  
    private void OnCVariation(int k) {  
        int ran1, ran2, temp;  
        int count;// �Ի�����  
  
        count = random.nextInt(65535) % cityNum;  
  
        for (int i = 0; i < count; i++) {  
  
            ran1 = random.nextInt(65535) % cityNum;  
            ran2 = random.nextInt(65535) % cityNum;  
            while (ran1 == ran2) {  
                ran2 = random.nextInt(65535) % cityNum;  
            }  
            temp = newPopulation[k][ran1];  
            newPopulation[k][ran1] = newPopulation[k][ran2];  
            newPopulation[k][ran2] = temp;  
        }  

    }  
    
    private void printOptimal() {
		System.out.println("The optimal length is: " + bestLength);
		System.out.println("The optimal length exists in No." + bestGen + " generations");  
		System.out.println("The optimal tour is: ");
		for (int i = 0; i < cityNum; i++) {
			System.out.print(bestTour[i] + " ");
		}
		System.out.println(bestTour[0]);
	}
    
	public void solve() {
		int i;  
        int k;  
  
        // ��ʼ����Ⱥ  
        initPopulation();  
        
        // �����ʼ����Ⱥ��Ӧ�ȣ�Fitness[max]  
        for (k = 0; k < population; k++) {  
            fitness[k] = calculateTourLength(oldPopulation[k]);   
        }  
        // �����ʼ����Ⱥ�и���������ۻ�����  
        countRate();   
          
        for (t = 0; t < maxGen; t++) {  
            evolution1();  
            //evolution();  
            // ������ȺnewGroup���Ƶ�����ȺoldGroup�У�׼����һ������  
            for (k = 0; k < population; k++) {  
                for (i = 0; i < cityNum; i++) {  
                    oldPopulation[k][i] = newPopulation[k][i];  
                }  
            }  
            // ������Ⱥ��Ӧ��  
            for (k = 0; k < population; k++) {  
                fitness[k] = calculateTourLength(oldPopulation[k]);  
            }  
            // ������Ⱥ�и���������ۻ�����  
            countRate();  
        }  
 
        printOptimal();
        
	}
}
