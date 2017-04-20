#include "CipherObj.h"
#include <cmath>
using namespace std;

class Hill:CipherObj
{
public:
	Hill(string ifn/*�����ļ�·��*/,string ofn/*����ļ�·��*/,int size,int **key);
	~Hill();
	void execute(bool flag);//flagΪtrueΪ���ܣ�ʹ��Key��ΪfalseΪ���ܣ�ʹ��invKey��
	bool getInvKey();  //�õ���Կ�������ʹ�õ��ǰ������
private:
	void VectorMultipleKeyMatrix(int *v,int **key,int *result);//�Ը���ĸ����Կ������ˣ��õ��µ�����
	int ExtendedEuclid( int f,int d ,int &result);//��չŷ������㷨��˷���
	int detLaplace(int size,int **key);  //����Կ����Key��det������ʽ��ʹ��������˹���ɡ�ʵ����ʹ�õݹ麯����detLaplace��һ���ݹ麯��
private:
	bool hasInvKey;//�ж��Ƿ�õ��������
	int Size;//��Կ�����С
	int **Key;//��Կ����
	int **invKey;//��Կ����������
};