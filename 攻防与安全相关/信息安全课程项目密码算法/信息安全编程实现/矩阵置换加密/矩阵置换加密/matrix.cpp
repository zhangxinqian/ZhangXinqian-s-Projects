#include <iostream>
#include <string>
#include <fstream>
using namespace std;

void encripting(int key);
void decripting(int key);
void read(string &str,int &count);
void write(string str);

int main()
{
	int key;
	int i;
	cout<<"输入秘钥：";
	cin>>key;
	cout<<"输入1加密2解密";
	cin>>i;
	if (i==1) encripting(key);
	if (i==2) decripting(key);
	return 0;
}

void read(string &str,int &count)
{
	ifstream inFile;
	char p;
	str="";
	count=0;
	inFile.open("target.txt");
	while(inFile>>p) //只读取英文字母
	{
		count++;
		str+=p;
	}
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

void encripting(int key)
{
	string str1="",str2="";
	int count=0,row=key,col=1;
	read(str1,count);
	while (row/=10) col++;
	count%col==0?row=count/col:row=count/col+1;
	int i=0,j=0,k=0;
	char** p=new char*[row];
	int *a=new int[col];
	for (i=0;i<row;i++)
		p[i]=new char[col];
	for (i=0;i<row-1;i++)
		for (j=0;j<col;j++)
			p[i][j]=str1[k++];
	for (j=0;j<col;j++)
		k<count?p[i][j]=str1[k++]:p[i][j]='?';
	i=key;j=col-1;
	while (i)
	{
		a[i%10-1]=j;
		i=i/10;
		j--;
	}
	for (j=0;j<col;j++)
		for (i=0;i<row;i++)
			str2+=p[i][a[j]];
	write(str2);
	cout<<"完成"<<endl;
	delete[] a;
	for (i=0;i<row;i++)
		delete[] p[i];
	delete[] p;
}

void decripting(int key)
{
	string str1="",str2="";
	int count=0,row=key,col=1;
	read(str1,count);
	while (row/=10) col++;
	count%col==0?row=count/col:row=count/col+1;
	int i=0,j=0,k=0;
	char** p=new char*[row];
	int *a=new int[col];
	for (i=0;i<row;i++)
		p[i]=new char[col];
	i=key;j=col-1;
	while (i)
	{
		a[i%10-1]=j;
		i=i/10;
		j--;
	}
	for (j=0;j<col;j++)
		for (i=0;i<row;i++)
			p[i][a[j]]=str1[k++];
	for (i=0;i<row;i++)
		for (j=0;j<col;j++)
			str2+=p[i][j];
	write(str2);
	cout<<"完成"<<endl;
	delete[] a;
	for (i=0;i<row;i++)
		delete[] p[i];
	delete[] p;
}