#include "stdafx.h"

#include "MatrixReplace.h"

MatrixReplace::MatrixReplace(string ifn,string ofn,string key):CipherObj(ifn,ofn)
{
	keyLen=key.length();//�涨��Կ���Ȳ��ܳ���9
	keyArr=new int[keyLen];
	for (int i=0;i<keyLen;i++)
	{
		if (isdigit(key[i]) && key[i]!='0')//��Կֻ��Ϊ�����Ҳ�Ϊ0
			keyArr[i]=key[i]-'1';//��Կȡֵ1~9����������������0~8
	}
	row=0;
	col=keyLen;
}

MatrixReplace::~MatrixReplace()
{
	delete[] keyArr;
}

void MatrixReplace::execute(bool flag)
{
	string target="",/*Ŀ���ı��ַ���*/
		result="";/*����ַ���*/
	char p;
	while(inFile>>p) //���Կո�ͻس�
	{
		p=toupper(p);//��ΪӢ����ĸһ��ת���ɴ�д��ĸ
		target+=p;
	}
	/*ȷ�����������*/
	target.length()%col==0?row=target.length()/col:row=target.length()/col+1;

	int i=0,j=0;
	int *order=new int[col];//���ڴ�Ÿ���keyArr�õ��ļ����е�˳��
	char **temp=new char*[row];//���ڴ���ַ�����
	for (i=0;i<row;i++)
		temp[i]=new char[col];

	for (i=0;i<col;i++)
		order[keyArr[i]]=i;

	flag?encryption(target,result,temp,order):decryption(target,result,temp,order);

	delete order;
	for (i=0;i<row;i++)
		delete[] temp[i];
	delete[] temp;

	if (result!="")//�ɹ�
		for (int i=0;i<result.length();i++)//������ı�
			outFile<<result[i];
	else outFile<<"ERROR";
}

void MatrixReplace::encryption(string &target,string &result,char **temp,int *order)
{
	int i=0,j=0,k=0;

	for (i=0;i<row-1;i++)
		for (j=0;j<col;j++)
			temp[i][j]=target[k++];
	for (j=0;j<col;j++)
		k<target.length()?temp[i][j]=target[k++]:temp[i][j]='X';//���������ĸX���

	for (j=0;j<col;j++)     //���ܹ��̺����㷨
		for (i=0;i<row;i++)
			result+=temp[i][order[j]];
}

void MatrixReplace::decryption(string &target,string &result,char **temp,int *order)
{
	if (target.length()!=col*row) return ;//�����ַ����������������󣬳��� 
	int i=0,j=0,k=0;

	for (j=0;j<col;j++)    //���ܹ��̺����㷨
		for (i=0;i<row;i++)
			temp[i][order[j]]=target[k++];

	for (i=0;i<row;i++)
		for (j=0;j<col;j++)
			result+=temp[i][j];
}