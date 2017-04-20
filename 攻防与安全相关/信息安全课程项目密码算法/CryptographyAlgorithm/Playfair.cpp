#include "stdafx.h"

#include "Playfair.h"


Playfair::Playfair(string ifn,string ofn,string keyWord/* ="" */):CipherObj(ifn,ofn)
{
	this->KeyWord=keyWord;
	createKey();
}

Playfair::~Playfair()
{

}

void Playfair::createKey()
{
	int i,j,k;
	//������Կ�ʹ��ɵ�5*5����
	j=0;
	for (i=0;i<KeyWord.length();i++) 
	{
		char kw=toupper(KeyWord[i]);
		if (kw=='J') continue;//������ĸJ
		for (k=0;k<j;k++)
			if (KeyMatr[k/5][k%5]==kw) break;
		if (k>=j) {KeyMatr[j/5][j%5]=kw;j++;}
	}

	for (i=0;i<26;i++)
	{
		if (i==9) continue; //I��J����ͬһ����ĸI����,Ӣ����Jʹ��Ƶ�ʺ�С
		char kw=char('A'+i);
		for (k=0;k<j;k++)
			if (KeyMatr[k/5][k%5]==kw) break;
		if (k>=j) {KeyMatr[j/5][j%5]=kw;j++;}
	}
}

void Playfair::encryption(char p[2],char keyMatr[5][5])
{
	int i,j,p0x,p0y,p1x,p1y;
	for (i=0;i<5;i++)
		for (j=0;j<5;j++)
		{
			if (keyMatr[i][j]==p[0]) {p0x=i;p0y=j;}
			if (keyMatr[i][j]==p[1]) {p1x=i;p1y=j;}
		}
		if (p0x==p1x) {p[0]=keyMatr[p0x][(p0y+6)%5];p[1]=keyMatr[p0x][(p1y+6)%5];}
		else
			if (p0y==p1y) {p[0]=keyMatr[(p0x+6)%5][p0y];p[1]=keyMatr[(p1x+6)%5][p0y];}
			else 
			{
				p[0]=keyMatr[p0x][p1y];
				p[1]=keyMatr[p1x][p0y];
			}
}

void Playfair::decryption(char p[2],char keyMatr[5][5])
{
	int i,j,p0x,p0y,p1x,p1y;
	for (i=0;i<5;i++)
		for (j=0;j<5;j++)
		{
			if (keyMatr[i][j]==p[0]) {p0x=i;p0y=j;}
			if (keyMatr[i][j]==p[1]) {p1x=i;p1y=j;}
		}
		if (p0x==p1x) {p[0]=keyMatr[p0x][(p0y+4)%5];p[1]=keyMatr[p0x][(p1y+4)%5];}
		else
			if (p0y==p1y) {p[0]=keyMatr[(p0x+4)%5][p0y];p[1]=keyMatr[(p1x+4)%5][p0y];}
			else 
			{
				p[0]=keyMatr[p0x][p1y];
				p[1]=keyMatr[p1x][p0y];
			}
}

void Playfair::execute(bool flag)
{
	void(*fp)(char[2],char[5][5]);
	flag?fp=encryption:fp=decryption;
	char p[2],p1;
	int i=0;
	inFile.get(p1);     //һ��һ���ַ��ض�������һ�μ��������ַ���I/O�����л�Ƶ����������ЩЧ�ʵ���ʧ��
	while (inFile)
	{
		p1=toupper(p1);//����ľ�Ϊ��д��ĸ
		if (p1<'A' || p1>'Z') outFile<<p1; //�������Ӣ����ĸ���ı�ֱ�����
		else 
		{
			if (p1=='J') p1='I';//��ĸJ������ĸI
			if (i%2==0) p[0]=p1;
			else
			{
				if (p1==p[0]) 
				{
					p[1]='X';  //��ĸ����ͬ��X���
					i++;
				}
				else p[1]=p1;
				(*fp)(p,this->KeyMatr);
				outFile<<p[0];outFile<<p[1];
			}
			i++;
		}
		inFile.get(p1);
	}
}