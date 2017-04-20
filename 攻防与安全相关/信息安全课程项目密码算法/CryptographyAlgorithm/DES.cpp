#include "stdafx.h"

#include "DES.h"
#include <cmath>
using namespace std;

const char DES::IP_table[64]={     //初始置换(IP)
	58, 50, 42, 34, 26, 18, 10,  2,
	60, 52, 44, 36, 28, 20, 12,  4,
	62, 54, 46, 38, 30, 22, 14,  6,
	64, 56, 48, 40, 32, 24, 16,  8,
	57, 49, 41, 33, 25, 17,  9,  1,
	59, 51, 43, 35, 27, 19, 11,  3,
	61, 53, 45, 37, 29, 21, 13,  5,
	63, 55, 47, 39, 31, 23, 15,  7
};

const char DES::InvIP_table[64]={  //逆初始值换(IP-1)
	40,  8, 48, 16, 56, 24, 64, 32,
	39,  7, 47, 15, 55, 23, 63, 31,
	38,  6, 46, 14, 54, 22, 62, 30,
	37,  5, 45, 13, 53, 21, 61, 29,
	36,  4, 44, 12, 52, 20, 60, 28,
	35,  3, 43, 11, 51, 19, 59, 27,
	34,  2, 42, 10, 50, 18, 58, 26,
	33,  1, 41,  9, 49, 17, 57, 25
};

const char DES::E_table[48]={    //扩展置换(E)
	32,  1,  2,  3,  4,  5,
	4,  5,  6,  7,  8,  9,
	8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32,  1
};

const char DES::P_table[32]={   //置换函数(P)
	16,  7, 20, 21, 29, 12, 28, 17,
	1, 15, 23, 26,   5, 18, 31, 10,
	2,  8, 24, 14, 32, 27,  3,  9,
	19, 13, 30,  6, 22, 11,  4, 25
};

const char DES::S_box[8][4][16]={  //DES的S盒定义
	//S1
	14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
	0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
	4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
	15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
	//S2
	15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
	3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
	0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
	13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
	//S3
	10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
	1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
	//S4
	7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
	13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
	10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
	3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
	//S5
	2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
	14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
	4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
	11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
	//S6
	12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
	10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
	9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
	4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
	//S7
	4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
	13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
	1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
	6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
	//S8
	13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
	1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
	7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
	2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};

const char DES::PC1_table[56]={  //置换选择1(PC-1)
	57, 49, 41, 33, 25, 17,  9,
	1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4
};

const char DES::PC2_table[48]={  //置换选择2(PC-2)
	14, 17, 11, 24,  1,  5,  3, 28,
	15,  6, 21, 10, 23, 19, 12,  4,
	26,  8, 16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55, 30, 40,
	51, 45, 33, 48, 44, 49, 39, 56,
	34, 53, 46, 42, 50, 36, 29, 32
};

const char DES::KeyLeftNum[16]={   //左移次数的确定16轮,下标从0开始
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

DES::DES(string ifn,string ofn,string ks):CipherObj(ifn,ofn)
{
	this->keyStr=ks;
	//getKey64();
	getKey64H();
	produceSubKey();
}

DES::~DES()
{

}

//字节到位的转换
void DES::byte2bit(bool *output, const char *input, int bitsLen)
{
	for(int i=0;i<bitsLen;i++)
		output[i]=(input[i/8]>>(i%8))&1;
}

//位到字节转换
void DES::bit2byte(char *output, const bool *input, int bitsLen)
{
	int i;
	for(i=0;i<bitsLen/8;i++)
		output[i]=0;
	for(i=0;i<bitsLen;i++)
		output[i/8]|=input[i]<<(i%8);			//"|="组合了位操作符和赋值操作符的功能
}

void DES::xor(const bool *x,const bool *y,bool *c,int bitsLen)
{
	for(int i=0;i<bitsLen;i++)
		c[i]=x[i]^y[i];
}

void DES::displace(bool *output,const bool *input,char *table,int len)
{
	int i;
	/*为了能让输入输出为同一个数组，引入一个临时数组*/
	bool temp[64];
	for (i=0;i<len;i++)
		temp[i]=input[table[i]-1];
	for (i=0;i<len;i++)
		output[i]=temp[i];
}

void DES::leftMove(bool *halfKey,char s)//左移,返回结果还是halfKey
{
	/*
	因为多次要调用该函数，
	避免多次创建该变量产生过大的系统开销，
	所以设置成静态变量
	*/
	static bool temp[64];

	int i;
	for (i=s;i<28;i++)
		temp[i-s]=halfKey[i];
	for (i=0;i<s;i++)
		temp[i]=halfKey[i];
	for (i=0;i<28;i++)
		halfKey[i]=temp[i];
}

/*将8字节密钥字符串转换成64位密钥*/
void DES::getKey64()
{
	char key[8];//8*8=64
	int i=0;
	for (i=0;i<keyStr.length() && i<8;i++)
		key[i]=keyStr[i];
	while (i<8)
		key[i++]='\0';//不足8个字符用0来补足
	byte2bit(key64,key,64);
}

void DES::getKey64H()
{
	char key[16];//4*16=64
	int i=0;
	for (i=0;i<keyStr.length() && i<16;i++)
		key[i]=keyStr[i];
	while (i<16)
		key[i++]='0';//不足16个16进制数用0来补足
	H2B(key64,key,64);
}

/*
将64位密钥压缩置换（置换选择1）为56位，
并用置换选择2和循环左移产生16个子密钥
*/
void DES::produceSubKey()
{
	bool key56[56];/*压缩后的56位密钥*/
	displace(key56,key64,(char *)PC1_table,56);//置换选择1
	bool *keyLeft=&key56[0],
		*keyRight=&key56[28];
	for (int i=0;i<16;i++)//产生16个子密钥
	{
		//循环左移
		leftMove(keyLeft,KeyLeftNum[i]);
		leftMove(keyRight,KeyLeftNum[i]);
		displace(subKey[i],key56,(char *)PC2_table,48);//置换选择2
	}
}

void DES::S(bool output[32],const bool input[48])
{
	int s,i,o;
	int row,col;
	int v,j;
	for (s=0,i=0,o=0;s<8;s++,i+=6,o+=4)
	{
		row=input[i]*2+input[i+5];
		col=input[i+1]*8+input[i+2]*4+input[i+3]*2+input[i+4];
		v=S_box[s][row][col];
		for (j=3;j>=0;j--)
		{
			output[o+j]=v%2;
			v=v/2;
		}
	}
}

void DES::FFunction(bool inOut[32],const bool key[48])
{
	/*设置成静态，开销小*/
	static bool key48[48];
	displace(key48,inOut,(char *)E_table,48);//扩展置换E
	/*和子密钥异或*/
	xor(key48,key,key48,48);
	S(inOut,key48);//代替选择S盒，48位变为32位
	displace(inOut,inOut,(char *)P_table,32);//置换P
}

void DES::des(bool target[64],bool flag)//一次加密解密64位
{
	bool *L=&target[0],
		*R=&target[32],
		temp[32];//用于临时存放R右32位
	int i,j;
	displace(target,target,(char *)IP_table,64);//初始置换

	if (flag)
	{
		for (i=0;i<16;i++) //16轮加密
		{
			for (j=0;j<32;j++)
				temp[j]=R[j];

			FFunction(R,subKey[i]);
			xor(L,R,R,32);

			for (j=0;j<32;j++)
				L[j]=temp[j];
		}
	}
	else
	{
		for (i=15;i>=0;i--) //16轮解密
		{
			for (j=0;j<32;j++)
				temp[j]=L[j];

			FFunction(L,subKey[i]);
			xor(L,R,L,32);

			for (j=0;j<32;j++)
				R[j]=temp[j];
		}
	}

	displace(target,target,(char *)InvIP_table,64);//逆初始置换
}


void DES::execute(bool flag)
{
	char p;
	//char targetByte[8];
	char targetByte[16];
	bool targetBit[64];
	int i=0;//记录字节数
	int j;
	
	while (inFile>>p/*inFile.get(p)*/)
	{
		//targetByte[i%8]=p;
		targetByte[i%16]=p;
		i++;
		//if (i%8==0)
		if (i%16==0)
		{
			H2B(targetBit,targetByte,64);
			//byte2bit(targetBit,targetByte,64);
			des(targetBit,flag);
			//bit2byte(targetByte,targetBit,64);
			B2H(targetByte,targetBit,64);
			for (j=0;j<16/*8*/;j++)
				outFile<<targetByte[j];
		}
	}
	if (/*i%8>0*/i%16>0) //说明明文或密文字节数不是8的倍数，则用'\0'补足
	{
		/*
		for (j=i%8;j<8;j++)
			targetByte[j]='\0';
		*/
		for (j=i%16;j<16;j++)
			targetByte[j]='0';
		H2B(targetBit,targetByte,64);
		//byte2bit(targetBit,targetByte,64);
		des(targetBit,flag);
		//bit2byte(targetByte,targetBit,64);
		B2H(targetByte,targetBit,64);
		for (j=0;j<16/*8*/;j++)
			outFile<<targetByte[j];
	}
}

void DES::H2B(bool *output,const char *input,int len)
{
	int i,j;
	int v;
	char p;
	for (i=0;i<len/4;i++)
	{
		if (input[i]>='0' && input[i]<='9')
			v=input[i]-'0';
		else
		{
			p=tolower(input[i]);
			if (p>='a' && p<='f')
				v=p-'a'+10;
			else v=0;
		}
		for (j=3;j>=0;j--)
		{
			output[4*i+j]=v%2;
			//output[4*i+j]=(bool)v%2;这样会出现2%2=1的情况？？
			v=v/2;
		}
	}
}

void DES::B2H(char *output,const bool *input,int len)
{
	int i,j;
	int v=0;
	for (i=0;i<len;i++)
	{
		v+=pow((double)2,3-i%4)*input[i];
		if (i%4==3)
		{
			if (v>=0 && v<=9) output[i/4]=char('0'+v);
			else
			{
				if (v>9 && v<16) output[i/4]=char('a'+v-10);
				else output[i/4]='0';
			}
			v=0;
		}
	}
}