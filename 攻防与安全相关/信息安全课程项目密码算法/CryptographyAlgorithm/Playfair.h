#include "CipherObj.h"

class Playfair:CipherObj
{
public:
	~Playfair();
	Playfair(string ifn/*�����ļ�·��*/,string ofn/*����ļ�·��*/,string keyWord=""/*��Կ*/);
	void execute(bool flag);//ִ�к�����fpΪ����ָ�룬ָ��ִ�е��Ǽ��ܻ��ǽ����㷨
private:
	static void encryption(char p[2],char[5][5]);//�����㷨��ÿ�μ���������ĸ����playfair�Ƕ���ĸ�������룩
	static void decryption(char p[2],char[5][5]);//�����㷨
	void createKey();//����Կ��ת����5*5�Ķ�ά�������
private:
	string KeyWord;
	char KeyMatr[5][5];
};