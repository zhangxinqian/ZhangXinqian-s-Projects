#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;


void encripting();//加密
void decripting();//解密
void read(string &str);//从文本读入
void write(string str);//输出到文本
void getKey(int &size,int **key);//得到矩阵秘钥
void getInvKey(int size,int **key1,int **key2);// 求Hill密码的逆矩阵，与数学角度的不同，不含分数和负数
void core(string str1,string &str2,int size,int **key);//核心算法
void mutiply(int size,int* p,int **key,int* pp);//向量和矩阵相乘
int detLaplace(int size,int **key);//用拉普拉斯法则求行列式

int main()
{
	int i;
	cout<<"输入1加密2解密:";
	cin>>i;
	if (i==1) encripting();
	if (i==2) decripting();
	return 0;
}

void getKey(int &size,int **key)
{
	ifstream inFile;
	int i,j;
	int **key1;
	inFile.open("key.txt");
	inFile>>size;
	key1=new int*[size];
	for (i=0;i<size;i++)
		key1[i]=new int[size];
	for (i=0;i<size;i++)
		for (j=0;j<size;j++)
			inFile>>key1[i][j];
	inFile.close();
	key=key1;
}

void read(string &str)
{
	ifstream inFile;
	char p;
	str="";
	inFile.open("target.txt");
	while (inFile>>p) str+=tolower(p); //只能加密英文,自动转换成小写
	inFile.close();
}

void write(string str)
{
	ofstream outFile;
	outFile.open("result.txt");
	for (int i=0;i<str.length();i++)
		outFile<<str[i];
	outFile.close();
}

void core(string str1,string &str2,int size,int **key)
{
	int *p=new int[size],*pp=new int[size];
	int i,j;
	str2="";
	for (i=0;i<str1.length();i++)
	{
		p[i%size]=str1[i]-'a';
		if (i%size==size-1) 
		{
			mutiply(size,p,key,pp);
			for (j=0;j<size;j++)
				str2+=char(pp[j]+'a');
		}
	}
	if (str1.length()%size!=0) //不足的用X补足
	{
		for (i=str1.length()%size-1;i<size;i++)
			p[i]=23; 
		mutiply(size,p,key,pp);
		for (j=0;j<size;j++)
			str2+=char(pp[j]+'a');
	}
	delete[] p;
	delete[] pp;
}

void mutiply(int size,int* p,int **key,int* pp)
{
	int i,j;
	for (j=0;j<size;j++)
	{
		pp[j]=0;
		for (i=0;i<size;i++)
			pp[j]+=p[i]*key[i][j];
		while (pp[j]<0)
			pp[j]+=26;
		pp[j]%=26;
	}
}

int detLaplace(int size,int **key)
{
	int result=0,i,j,k,l;
	if (size==1) return key[0][0];
	for (int i=0;i<size;i++)
	{
		int **key0=new int*[size-1];
		for (j=0;j<size-1;j++)
			key0[j]=new int[size-1];
		for (j=1;j<size;j++)
		{
			l=0;
			for (k=0;k<size;k++)
			{
				if (k==i) continue;
				key0[j-1][l]=key[j][k];
				l++;
			}
		}
		result+=key[0][i]*int(pow(float(-1),i))*detLaplace(size-1,key0);
		for (j=0;j<size-1;j++)
			delete[] key0[j];
		delete[] key0;
	}
	return result;
}

void getInvKey(int size,int **key1,int **key2)
{
	int i,j,k,l;
	int **key0=new int*[size-1];
	int n=detLaplace(size,key1);
	while (n<0) n+=26;
	i=1;
	while ((n*i)%26!=1) i++;
	n=i;
	for (j=0;j<size-1;j++)
		key0[j]=new int[size-1];
	for (i=0;i<size;i++)
	{
		for (j=0;j<size;j++)
		{
			int a=0,b=0;
			for (k=0;k<size;k++)
			{
				if (k==j) continue;
				b=0;
				for (l=0;l<size;l++)
				{
					if (l==i) continue;
					key0[a][b]=key1[k][l];
					b++;
				}
				a++;
			}
			key2[i][j]=int(pow(float(-1),i+j))*n*detLaplace(size-1,key0);
			while (key2[i][j]<0) key2[i][j]+=26;
			key2[i][j]%=26;
		}
	}
	for (j=0;j<size-1;j++)
		delete[] key0[j];
	delete[] key0;
}

void encripting()
{
	string str1,str2;
	int **key=NULL;
	//int size,i;
	int size;

	ifstream inFile;
	int i,j;
	inFile.open("key.txt");
	inFile>>size;
	key=new int*[size];
	for (i=0;i<size;i++)
		key[i]=new int[size];
	for (i=0;i<size;i++)
		for (j=0;j<size;j++)
			inFile>>key[i][j];
	inFile.close();

	//getKey(size,key);
	read(str1);
	core(str1,str2,size,key);
	write(str2);
	for (i=0;i<size;i++)
		delete[] key[i];
	delete[] key;
}

void decripting()
{
	string str1,str2;
	int **key1=NULL,**key2=NULL;
	int size;

	ifstream inFile;
	int i,j;
	inFile.open("key.txt");
	inFile>>size;
	key1=new int*[size];
	for (i=0;i<size;i++)
		key1[i]=new int[size];
	key2=new int*[size];
	for (i=0;i<size;i++)
		key2[i]=new int[size];

	for (i=0;i<size;i++)
		for (j=0;j<size;j++)
			inFile>>key1[i][j];
	inFile.close();

	getInvKey(size,key1,key2);

	read(str1);
	core(str1,str2,size,key2);
	write(str2);
	for (i=0;i<size;i++)
		delete[] key1[i];
	delete[] key1;
	for (i=0;i<size;i++)
		delete[] key2[i];
	delete[] key2;
}