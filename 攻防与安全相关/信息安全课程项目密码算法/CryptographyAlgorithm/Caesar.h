#include "CipherObj.h"

class Caesar:public CipherObj
{
public:
	Caesar(string ifn/*�����ļ�·��*/,string ofn/*����ļ�·��*/,int key=0/*��Կ*/);
	~Caesar();
public:
	void execute(bool flag);
private:
	static void encryption(char &p,int key);//���ܵ����ַ�
	static void decryption(char &p,int key);//���ܵ����ַ�
private:
	int Key;
};