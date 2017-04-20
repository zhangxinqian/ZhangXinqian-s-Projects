#include "stdafx.h"
#include "Caesar.h"

Caesar::Caesar(string ifn,string ofn,int key/* =0 */):CipherObj(ifn,ofn)
{
	while (key<0) //把key的大小转换成0~25之间
	{
		key+=26;
	}
	key%=26;
	this->Key=key;
}

Caesar::~Caesar()
{

}

void Caesar::encryption(char &p,int key) //加密单个字符，如果是非英文字母则不改变直接输出
{
	p=toupper(p);
	if (p>='A' && p<='Z')
	{
		p=char((p-'A'+key)%26+'A');
	}
}

void Caesar::decryption(char &p,int key) //解密单个字符，如果是非英文字母则不改变直接输出
{
	p=toupper(p);
	if (p>='A' && p<='Z')
	{
		p=char((p-'A'-key+26)%26+'A');
	}
}

void Caesar::execute(bool flag)  //执行函数，读入文件内容并输出到目的文件
{
	void (*fp)(char&,int);//定义一个函数指针
	flag?fp=encryption:fp=decryption;
	char p;
	inFile.get(p); //一个字符一个字符地读，一次加密一个字符,I/O操作切换频繁，多少有些效率的损失。
	while (inFile)
	{
		(*fp)(p,this->Key);
		outFile<<p;
		inFile.get(p);
	}
}