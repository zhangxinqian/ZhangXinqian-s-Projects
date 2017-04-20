#include "stdafx.h"

#include "Vigenere.h"

Vigenere::Vigenere(string ifn,string ofn,string kw/* ="" */):CipherObj(ifn,ofn)
{
	for (int i=0;i<kw.length();i++)   //密钥词一律转换成大写字母
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
	while (inFile.get(p))       //秘钥只对英文字母有效，忽略空格等其他字符,一律输出大写字母
	{
		p=toupper(p);
		if (p>='A' && p<='Z')  //如果不是字母则直接输出
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
	秘钥只对英文字母有效，忽略空格等其他字符,一律输出大写字母
	一个一个字符地读并处理，I/O操作切换频繁，多少有些效率的损失。
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