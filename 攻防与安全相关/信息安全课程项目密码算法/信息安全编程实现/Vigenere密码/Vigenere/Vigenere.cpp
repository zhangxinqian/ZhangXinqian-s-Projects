#include<iostream>
#include <fstream>
#include <string>
using namespace std;

void encripting(string key);
void decripting(string key);
int main()
{
	int i;
	string key;
	cout<<"����1���ܣ�����2����:";
	cin>>i;
	cout<<"������Կ:";
	cin>>key;
	if (i==1) encripting(key);
	if (i==2) decripting(key);
	return 0;
}

void encripting(string key)
{
	ifstream inFile;
	ofstream outFile;
	char p;
	int i=0,j=key.length();
	inFile.open("target.txt");
	outFile.open("result.txt");
	while (inFile.get(p))//��Կֻ��Ӣ����ĸ��Ч�����Կո�������ַ�
	{
		if (p>='A' && p<='Z') 
		{
			p=char('A'+(p-'A'+key[i%j]-'A')%26);
			i++;
		}
		if (p>='a' && p<='z')
		{
			p=char('a'+(p-'a'+key[i%j]-'a')%26);
			i++;
		}
		outFile<<p;
	}
	cout<<"���"<<endl;
	inFile.close();
	outFile.close();
}

void decripting(string key)
{
	ifstream inFile;
	ofstream outFile;
	char p;
	int i=0,j=key.length();
	inFile.open("target.txt");
	outFile.open("result.txt");
	while (inFile.get(p))//��Կֻ��Ӣ����ĸ��Ч�����Կո�������ַ�
	{
		if (p>='A' && p<='Z') 
		{
			p=char('A'+(p-key[i%j]+26)%26);
			i++;
		}
		if (p>='a' && p<='z')
		{
			p=char('a'+(p-key[i%j]+26)%26);
			i++;
		}
		outFile<<p;
	}
	cout<<"���"<<endl;
	inFile.close();
	outFile.close();
}