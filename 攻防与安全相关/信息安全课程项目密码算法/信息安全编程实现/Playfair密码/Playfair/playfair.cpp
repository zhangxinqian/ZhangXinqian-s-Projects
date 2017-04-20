#include<iostream>
#include <fstream>
#include<string>
using namespace std;

void createKey(string word,char key[5][5]);//key��һ��5*5�Ķ�ά����
void execute(string word,void(*fp)(char[2],char[5][5]));
void encrypting(char p[2],char key[5][5]);
void decrypting(char p[2],char key[5][5]);

int main()
{
	string word; //����ȫ����Ӣ����ĸ
	cout<<"������Կ�ʣ�����ȫ��Ӣ����ĸ���ɣ���";
	cin>>word;
	for (int i=0;i<word.length();i++)
		if (toupper(word[i])<'A' || toupper(word[i])>'Z') 
		{
			cout<<"����Ƿ�"<<endl;
			return 1;
		}
	execute(word,encrypting);
	cout<<endl;
	execute(word,decrypting);
	return 0;
}

void createKey(string word,char key[5][5])
{
	int i,j,k;
	//������Կ�ʹ��ɵ�5*5����
	j=0;
	for (i=0;i<word.length();i++) 
	{
		char kw=toupper(word[i]);
		if (kw=='J') continue;//������ĸJ
		for (k=0;k<j;k++)
			if (key[k/5][k%5]==kw) break;
		if (k>=j) {key[j/5][j%5]=kw;j++;}
	}
	for (i=0;i<26;i++)
	{
		if (i==9) continue; //I��J����ͬһ����ĸ,Ӣ����Jʹ��Ƶ�ʺ�С
		char kw=char('A'+i);
		for (k=0;k<j;k++)
			if (key[k/5][k%5]==kw) break;
		if (k>=j) {key[j/5][j%5]=kw;j++;}
	}
	cout<<"��Կ����"<<endl;
	for (i=0;i<5;i++)
	{
		for (j=0;j<5;j++)
			cout<<key[i][j]<<' ';
		cout<<endl;
	}
}

void execute(string word,void(*fp)(char[2],char[5][5]))
{
	char key[5][5];
	ifstream inFile;
	ofstream outFile;
	createKey(word,key);
	if (fp==encrypting) 
	{
		inFile.open("plaintext.txt");
		outFile.open("resultOfencryption.txt");
		cout<<"���м���..."<<endl;
	}
	if (fp==decrypting) 
	{
		inFile.open("ciphertext.txt");
		outFile.open("resultOfdecryption.txt");
		cout<<"���н���..."<<endl;
	}
	char p[2],p1;
	int i=0;
	inFile.get(p1);
	while (inFile)
	{
		p1=toupper(p1);
		if (p1<'A' || p1>'Z') outFile<<p1;
		else 
		{
			if (p1=='J') p1='I';//��ĸJ������ĸI
			if (i%2==0) p[0]=p1;
			else
			{
				if (p1==p[0]) 
				{
					p[1]='X';//��ĸ����ͬ��X���
					i++;
				}
				else p[1]=p1;
				(*fp)(p,key);
				outFile<<p[0];outFile<<p[1];
			}
			i++;
		}
		inFile.get(p1);
	}
	cout<<"���..."<<endl;
	inFile.close();
	outFile.close();
}

void encrypting(char p[2],char key[5][5])
{
	int i,j,p0x,p0y,p1x,p1y;
	for (i=0;i<5;i++)
		for (j=0;j<5;j++)
		{
			if (key[i][j]==p[0]) {p0x=i;p0y=j;}
			if (key[i][j]==p[1]) {p1x=i;p1y=j;}
		}
	if (p0x==p1x) {p[0]=key[p0x][(p0y+6)%5];p[1]=key[p0x][(p1y+6)%5];}
	else
		if (p0y==p1y) {p[0]=key[(p0x+6)%5][p0y];p[1]=key[(p1x+6)%5][p0y];}
		else 
		{
			p[0]=key[p0x][p1y];
			p[1]=key[p1x][p0y];
		}
}
void decrypting(char p[2],char key[5][5])
{
	int i,j,p0x,p0y,p1x,p1y;
	for (i=0;i<5;i++)
		for (j=0;j<5;j++)
		{
			if (key[i][j]==p[0]) {p0x=i;p0y=j;}
			if (key[i][j]==p[1]) {p1x=i;p1y=j;}
		}
	if (p0x==p1x) {p[0]=key[p0x][(p0y+4)%5];p[1]=key[p0x][(p1y+4)%5];}
	else
		if (p0y==p1y) {p[0]=key[(p0x+4)%5][p0y];p[1]=key[(p1x+4)%5][p0y];}
		else 
		{
			p[0]=key[p0x][p1y];
			p[1]=key[p1x][p0y];
		}
}