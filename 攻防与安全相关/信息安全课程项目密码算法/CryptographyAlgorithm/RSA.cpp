#include "stdafx.h"

#include "RSA.h"
using namespace std;

RSA::RSA(string ifn,string ofn,RSAKEY PU,RSAKEY PR)
{
	this->inFileName=ifn;
	this->outFileName=ofn;
	inFile.open(inFileName.c_str());
	outFile.open(outFileName.c_str());
	this->PU=PU;
	this->PR=PR;
}

RSA::~RSA()
{
	inFile.close();
	outFile.close();
}

void RSA::encryption(RSAKEY PU)
{
	/*
	char p;
	int t;
	while (inFile.get(p)) //只对英文字母有效,一律输出大写字母
	{
		p=toupper(p);
		if (p>='A' && p<='Z')
		{
			t=p-'A';
			t=calNum(t,PU);
			p=char(t+'A');
		}
		outFile<<p;
	}
	*/
	int p;
	decnum d;
	inFile>>p;
	d=p;
	d=powMod(d,PU.P,PU.N);
	outFile<<d;
}

void RSA::decryption()
{
	/*
	char p;
	int t;
	while (inFile.get(p)) //只对英文字母有效,一律输出大写字母
	{
		p=toupper(p);
		if (p>='A' && p<='Z')
		{
			t=p-'A';
			t=calNum(t,PR);
			p=char(t+'A');
		}
		outFile<<p;
	}
	*/
	int p;
	decnum d;
	inFile>>p;
	d=p;
	d=powMod(d,PR.P,PR.N);
	outFile<<d;
}

/*
int RSA::calNum(int x,RSAKEY &key)
{

	return 0;
}
*/
decnum RSA::powMod(decnum a,decnum b,decnum c)
{
	decnum w=1,z=0,t=2;
	if (a>=c) a%=c;
	for (;b>z;)
	{
		w=((b%t)*a*w)%c;
		a=(a*a)%c;
		b=b/t;
	}
	return w;
}