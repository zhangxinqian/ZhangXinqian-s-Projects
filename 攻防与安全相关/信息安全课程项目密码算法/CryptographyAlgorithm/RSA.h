#include<string>
#include<fstream>
#include "RSAKeyProduce.h"
using namespace std;

class RSA
{
public:
	RSA(string ifn/*输入文件路径*/,string ofn/*输出文件路径*/,RSAKEY PU/*公钥*/,RSAKEY PR/*私钥*/);
	~RSA();
	void encryption(RSAKEY PU);//用别人的公钥加密，和他人通信或进行认证
	void decryption();//用自己的私钥解密别人发的密文,或进行数字签名
private:
	/*
	求出(x^y)%z的值，即求x的y次方除以z的余数
	其中y=key.P; z=key.N
	*/
	//int calNum(int x,RSAKEY &key);
	decnum powMod(decnum a,decnum b,decnum c);
private:
	string inFileName;/*输入文件路径*/
	string outFileName;/*输出文件路径*/
	ifstream inFile;
	ofstream outFile;
private:
	/*自己的公钥(e,n)和私钥(d,n)*/
	RSAKEY PU;
	RSAKEY PR;
};