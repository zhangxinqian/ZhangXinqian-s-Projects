#include "stdafx.h"
#include "Caesar.h"

Caesar::Caesar(string ifn,string ofn,int key/* =0 */):CipherObj(ifn,ofn)
{
	while (key<0) //��key�Ĵ�Сת����0~25֮��
	{
		key+=26;
	}
	key%=26;
	this->Key=key;
}

Caesar::~Caesar()
{

}

void Caesar::encryption(char &p,int key) //���ܵ����ַ�������Ƿ�Ӣ����ĸ�򲻸ı�ֱ�����
{
	p=toupper(p);
	if (p>='A' && p<='Z')
	{
		p=char((p-'A'+key)%26+'A');
	}
}

void Caesar::decryption(char &p,int key) //���ܵ����ַ�������Ƿ�Ӣ����ĸ�򲻸ı�ֱ�����
{
	p=toupper(p);
	if (p>='A' && p<='Z')
	{
		p=char((p-'A'-key+26)%26+'A');
	}
}

void Caesar::execute(bool flag)  //ִ�к����������ļ����ݲ������Ŀ���ļ�
{
	void (*fp)(char&,int);//����һ������ָ��
	flag?fp=encryption:fp=decryption;
	char p;
	inFile.get(p); //һ���ַ�һ���ַ��ض���һ�μ���һ���ַ�,I/O�����л�Ƶ����������ЩЧ�ʵ���ʧ��
	while (inFile)
	{
		(*fp)(p,this->Key);
		outFile<<p;
		inFile.get(p);
	}
}