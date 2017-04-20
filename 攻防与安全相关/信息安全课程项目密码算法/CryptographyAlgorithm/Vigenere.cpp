#include "stdafx.h"

#include "Vigenere.h"

Vigenere::Vigenere(string ifn,string ofn,string kw/* ="" */):CipherObj(ifn,ofn)
{
	for (int i=0;i<kw.length();i++)   //��Կ��һ��ת���ɴ�д��ĸ
		kw[i]=toupper(kw[i]);
	this->KeyWord=kw;
}

Vigenere::~Vigenere()
{

}

void Vigenere::encryption()
{
	char p;
	int i=0,j=KeyWord.length();
	while (inFile.get(p))       //��Կֻ��Ӣ����ĸ��Ч�����Կո�������ַ�,һ�������д��ĸ
	{
		p=toupper(p);
		if (p>='A' && p<='Z')  //���������ĸ��ֱ�����
		{
			p=char('A'+(p-'A'+KeyWord[i%j]-'A')%26);
			i++;
		}
		outFile<<p;
	}
}

void Vigenere::decryption()
{
	char p;
	int i=0,j=KeyWord.length();
	/*
	��Կֻ��Ӣ����ĸ��Ч�����Կո�������ַ�,һ�������д��ĸ
	һ��һ���ַ��ض�������I/O�����л�Ƶ����������ЩЧ�ʵ���ʧ��
	*/
	while (inFile.get(p))
	{
		p=toupper(p);
		if (p>='A' && p<='Z') 
		{
			p=char('A'+(p-KeyWord[i%j]+26)%26);
			i++;
		}
		outFile<<p;
	}
}

void Vigenere::execute(bool flag)
{
	flag?encryption():decryption();
}