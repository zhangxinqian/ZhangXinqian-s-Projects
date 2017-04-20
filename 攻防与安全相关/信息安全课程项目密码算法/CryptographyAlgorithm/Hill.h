#include "CipherObj.h"
#include <cmath>
using namespace std;

class Hill:CipherObj
{
public:
	Hill(string ifn/*输入文件路径*/,string ofn/*输出文件路径*/,int size,int **key);
	~Hill();
	void execute(bool flag);//flag为true为加密，使用Key；为false为解密，使用invKey。
	bool getInvKey();  //得到密钥的逆矩阵，使用的是伴随矩阵法
private:
	void VectorMultipleKeyMatrix(int *v,int **key,int *result);//对个字母和密钥矩阵相乘，得到新的向量
	int ExtendedEuclid( int f,int d ,int &result);//扩展欧几里德算法求乘法逆
	int detLaplace(int size,int **key);  //对密钥矩阵Key求det即行列式，使用拉普拉斯定律。实现上使用递归函数，detLaplace是一个递归函数
private:
	bool hasInvKey;//判断是否得到了逆矩阵
	int Size;//密钥矩阵大小
	int **Key;//密钥矩阵
	int **invKey;//密钥矩阵的逆矩阵
};