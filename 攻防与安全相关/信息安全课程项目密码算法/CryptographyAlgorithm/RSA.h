#include<string>
#include<fstream>
#include "RSAKeyProduce.h"
using namespace std;

class RSA
{
public:
	RSA(string ifn/*�����ļ�·��*/,string ofn/*����ļ�·��*/,RSAKEY PU/*��Կ*/,RSAKEY PR/*˽Կ*/);
	~RSA();
	void encryption(RSAKEY PU);//�ñ��˵Ĺ�Կ���ܣ�������ͨ�Ż������֤
	void decryption();//���Լ���˽Կ���ܱ��˷�������,���������ǩ��
private:
	/*
	���(x^y)%z��ֵ������x��y�η�����z������
	����y=key.P; z=key.N
	*/
	//int calNum(int x,RSAKEY &key);
	decnum powMod(decnum a,decnum b,decnum c);
private:
	string inFileName;/*�����ļ�·��*/
	string outFileName;/*����ļ�·��*/
	ifstream inFile;
	ofstream outFile;
private:
	/*�Լ��Ĺ�Կ(e,n)��˽Կ(d,n)*/
	RSAKEY PU;
	RSAKEY PR;
};