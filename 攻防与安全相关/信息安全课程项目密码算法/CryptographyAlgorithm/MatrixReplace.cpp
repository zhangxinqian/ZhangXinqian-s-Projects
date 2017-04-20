#include "stdafx.h"

#include "MatrixReplace.h"

MatrixReplace::MatrixReplace(string ifn,string ofn,string key):CipherObj(ifn,ofn)
{
	keyLen=key.length();//规定密钥长度不能超过9
	keyArr=new int[keyLen];
	for (int i=0;i<keyLen;i++)
	{
		if (isdigit(key[i]) && key[i]!='0')//密钥只能为数字且不为0
			keyArr[i]=key[i]-'1';//密钥取值1~9，但存在数组中是0~8
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
	string target="",/*目标文本字符串*/
		result="";/*结果字符串*/
	char p;
	while(inFile>>p) //忽略空格和回车
	{
		p=toupper(p);//若为英文字母一律转换成大写字母
		target+=p;
	}
	/*确定矩阵的行数*/
	target.length()%col==0?row=target.length()/col:row=target.length()/col+1;

	int i=0,j=0;
	int *order=new int[col];//用于存放根据keyArr得到的加密列的顺序
	char **temp=new char*[row];//用于存放字符矩阵
	for (i=0;i<row;i++)
		temp[i]=new char[col];

	for (i=0;i<col;i++)
		order[keyArr[i]]=i;

	flag?encryption(target,result,temp,order):decryption(target,result,temp,order);

	delete order;
	for (i=0;i<row;i++)
		delete[] temp[i];
	delete[] temp;

	if (result!="")//成功
		for (int i=0;i<result.length();i++)//输出到文本
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
		k<target.length()?temp[i][j]=target[k++]:temp[i][j]='X';//不足的用字母X填充

	for (j=0;j<col;j++)     //加密过程核心算法
		for (i=0;i<row;i++)
			result+=temp[i][order[j]];
}

void MatrixReplace::decryption(string &target,string &result,char **temp,int *order)
{
	if (target.length()!=col*row) return ;//密文字符不能填满整个矩阵，出错 
	int i=0,j=0,k=0;

	for (j=0;j<col;j++)    //解密过程核心算法
		for (i=0;i<row;i++)
			temp[i][order[j]]=target[k++];

	for (i=0;i<row;i++)
		for (j=0;j<col;j++)
			result+=temp[i][j];
}