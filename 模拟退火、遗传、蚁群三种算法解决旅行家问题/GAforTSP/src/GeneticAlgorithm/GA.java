package GeneticAlgorithm;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Random;

public class GA {

	private int cityNum;							//城市数量
	private int population;							//种群规模
	private int	maxGen;								//生成的代数
	private int bestGen;							//最佳出现的代数
	private int[][] distance; 						//距离矩阵
	private int[] bestTour; 						//最佳路径
	private int bestLength; 						//最佳长度
	  
    private int[][] oldPopulation;  				// 初始种群，父代种群，行数表示种群规模，一行代表一个个体，即染色体，列表示染色体基因片段
    private int[][] newPopulation;					// 新的种群，子代种群  
    private int[] fitness;							// 种群适应度，表示种群中各个个体的适应度，即路程长度，越小适应度越高
	
	private double[] p;								// 种群中各个个体的累计概率 
	private double pc;								//染色体交叉概率crossover
	private double pm;								//染色体变异概率mutation
	private int t;									// 当前代数
	private Random random;							//生成随机数
	
	public GA(int cityNum, int population, int gen, double pc, double pm) {
		this.cityNum = cityNum;
		this.population = population;
		this.maxGen = gen;
		this.pc = pc;
		this.pm = pm;
	}
	
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
		for (k = 0; k < population; k++) {// 种群数
			oldPopulation[k][0] = random.nextInt(65535) % cityNum;
			for (i = 1; i < cityNum;) { // 染色体长度 
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
        // 染色体，起始城市,城市1,城市2...城市n  
        for (int i = 1; i < cityNum; i++) {  
            len += distance[chromosome[i-1]][chromosome[i]];  
        }  
        // 城市n,起始城市  
        len += distance[chromosome[cityNum-1]][chromosome[0]];  
        
        return len;  
    }  
	
	// 计算种群中各个个体的累积概率，前提是已经计算出各个个体的适应度fitness[max]，作为赌轮选择策略一部分  
    private void countRate() {  
        int k;  
        double sumFitness = 0;// 适应度总和  
  
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
	
    // 复制染色体，k表示新染色体在种群中的位置，kk表示旧的染色体在种群中的位置  
    private void copyGh(int k, int kk) {  
        int i;  
        for (i = 0; i < cityNum; i++) {  
            newPopulation[k][i] = oldPopulation[kk][i];  
        }  
    }  
    
    // 挑选某代种群中适应度最高的个体，直接复制到子代中  
    // 前提是已经计算出各个个体的适应度Fitness[max]  
    private void selectBestGh() {  
        int k, i, maxid;  
        int maxevaluation;  //越小适应度越高
  
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
            bestGen = t;// 最好的染色体出现的代数;  
            for (i = 0; i < cityNum; i++) {  
                bestTour[i] = oldPopulation[maxid][i];
            }
        }  
  
        // 复制染色体，k表示新染色体在种群中的位置，kk表示旧的染色体在种群中的位置  
        copyGh(0, maxid);// 将当代种群中适应度最高的染色体k复制到新种群中，排在第一位0  
    }  
    
    // 赌轮选择策略挑选  
    private void select() {  
        int k, i, selectId;  
        double ran1; 
        for (k = 1; k < population; k++) {  
            ran1 = (double) (random.nextInt(65535) % 1000 / 1000.0);  
            // 产生方式  
            for (i = 0; i < population; i++) {  
                if (ran1 <= p[i]) {  
                    break;  
                }  
            }  
            selectId = i; 
            copyGh(k, selectId);  
        }  
    }
    
    //进化函数，正常交叉变异  
    private void evolution() {  
        int k;  
        // 挑选某代种群中适应度最高的个体  
        selectBestGh();  
        // 赌轮选择策略挑选scale-1个下一代个体  
        select();  
   
        double r;  
        // 交叉方法  
        for (k = 0; k < population; k = k + 2) {  
            r = random.nextDouble();// /产生概率  
            if (r < pc) {   
                //OXCross(k, k + 1);// 进行交叉  
                OXCross1(k, k + 1);  
            } else {  
                r = random.nextDouble();// /产生概率  
                // 变异  
                if (r < pm) {    
                    OnCVariation(k);  
                }  
                r = random.nextDouble();// /产生概率    
                // 变异  
                if (r < pm) {    
                    OnCVariation(k + 1);  
                }  
            }  
  
        }  
    }  
  
    //进化函数，保留最好染色体不进行交叉变异  
    private void evolution1() {  
        int k;  
        // 挑选某代种群中适应度最高的个体  
        selectBestGh();  
        // 赌轮选择策略挑选scale-1个下一代个体  
        select();  
    
        double r;  
  
        //k=0（最优）情况直接留到子代
        for (k = 1; k + 1 < population / 2; k = k + 2) {  
            r = random.nextDouble();// /产生概率  
            if (r < pc) {  
                OXCross1(k, k + 1);// 进行交叉  
                //OXCross(k,k+1);//进行交叉  
            } else {  
                r = random.nextDouble();// /产生概率  
                // 变异  
                if (r < pm) {  
                    OnCVariation(k);  
                }  
                r = random.nextDouble();// /产生概率  
                // 变异  
                if (r < pm) {  
                    OnCVariation(k + 1);  
                }  
            }  
        }  
        if (k == population / 2 - 1)// 剩最后一个染色体没有交叉L-1  
        {  
            r = random.nextDouble();// /产生概率  
            if (r < pm) {  
                OnCVariation(k);  
            }  
        }  
  
    }  
  
    // 类OX交叉算子  
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
  
        if (ran1 > ran2)// 确保ran1<ran2  
        {  
            temp = ran1;  
            ran1 = ran2;  
            ran2 = temp;  
        }  
       
        flag = ran2 - ran1 + 1;// 删除重复基因前染色体长度  
        for (i = 0, j = ran1; i < flag; i++, j++) {  
            Gh1[i] = newPopulation[k2][j];  
            Gh2[i] = newPopulation[k1][j];  
        }  
        // 已经赋值i=ran2-ran1个基因  
  
        for (k = 0, j = flag; j < cityNum;)// 染色体长度  
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
  
        for (k = 0, j = flag; j < cityNum;)// 染色体长度  
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
            newPopulation[k1][i] = Gh1[i];// 交叉完毕放回种群  
            newPopulation[k2][i] = Gh2[i];// 交叉完毕放回种群  
        }  
  
    }  
  
    // 交叉算子,相同染色体交叉产生不同子代染色体  
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
  
        if (ran1 > ran2)// 确保ran1<ran2  
        {  
            temp = ran1;  
            ran1 = ran2;  
            ran2 = temp;  
        }  
  
        // 将染色体1中的第三部分移到染色体2的首部  
        for (i = 0, j = ran2; j < cityNum; i++, j++) {  
            Gh2[i] = newPopulation[k1][j];  
        }  
  
        flag = i;// 染色体2原基因开始位置  
  
        for (k = 0, j = flag; j < cityNum;)// 染色体长度  
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
        for (k = 0, j = 0; k < cityNum;)// 染色体长度  
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
            newPopulation[k1][i] = Gh1[i];// 交叉完毕放回种群  
            newPopulation[k2][i] = Gh2[i];// 交叉完毕放回种群  
        }  
    }  
  
    // 多次对换变异算子  
    private void OnCVariation(int k) {  
        int ran1, ran2, temp;  
        int count;// 对换次数  
  
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
  
        // 初始化种群  
        initPopulation();  
        
        // 计算初始化种群适应度，Fitness[max]  
        for (k = 0; k < population; k++) {  
            fitness[k] = calculateTourLength(oldPopulation[k]);   
        }  
        // 计算初始化种群中各个个体的累积概率  
        countRate();   
          
        for (t = 0; t < maxGen; t++) {  
            evolution1();  
            //evolution();  
            // 将新种群newGroup复制到旧种群oldGroup中，准备下一代进化  
            for (k = 0; k < population; k++) {  
                for (i = 0; i < cityNum; i++) {  
                    oldPopulation[k][i] = newPopulation[k][i];  
                }  
            }  
            // 计算种群适应度  
            for (k = 0; k < population; k++) {  
                fitness[k] = calculateTourLength(oldPopulation[k]);  
            }  
            // 计算种群中各个个体的累积概率  
            countRate();  
        }  
 
        printOptimal();
        
	}
}
