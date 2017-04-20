#include "stdafx.h"

#include "Hill.h"

Hill::Hill(string ifn,string ofn,int size,int **key):CipherObj(ifn,ofn)
{
	int i,j;
	this->Size=size;
	Key=new int *[Size];
	for (i=0;i<Size;i++)
	{
		Key[i]=new int[Size];
		for (j=0;j<Size;j++)
		{
			while (key[i][j]<0) key[i][j]+=26; //转换成0到25之间的值
			Key[i][j]=key[i][j]%26;
		}
	}
	hasInvKey=false;
	invKey=new int *[Size];
	for (i=0;i<Size;i++)
		invKey[i]=new int[Size];
}

Hill::~Hill()
{
	int i;
	for (i=0;i<Size;i++)
		delete[] Key[i];
	delete[] Key;

	for (i=0;i<Size;i++)
		delete[] invKey[i];
	delete[] invKey;
}

/*
前提是v和result指针都分配了Size大小的空间,key指向Key或invKey中的一个
*/
void Hill::VectorMultipleKeyMatrix(int *v,int **key,int *result)
{
	int i,j;
	for (j=0;j<Size;j++)
	{
		result[j]=0;
		for (i=0;i<Size;i++)
			result[j]+=v[i]*key[i][j];
		while (result[j]<0)
			result[j]+=26;
		result[j]%=26;//结果转换成在0~25范围内
	}
}

/*
对密钥矩阵Key求det即行列式，使用拉普拉斯定律
实现上使用递归函数，detLaplace是一个递归函数
*/
int Hill::detLaplace(int size,int **key)
{
	int result=0,i,j,k,l;
	if (size==1) return key[0][0];
	for (i=0;i<size;i++)
	{
		int **key0=new int*[size-1];//key0的行列式为Key的余子式
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
		result+=key[0][i]*int(pow(float(-1),i))*detLaplace(size-1,key0);//累加求得行列式
		for (j=0;j<size-1;j++)  //释放key0
			delete[] key0[j];
		delete[] key0;
	}
	/*防止数据溢出，多次取余
	由于和线性代数中的方法有些不同，且经过数学证明，做出如下处理求得的逆矩阵不变
	*/
	
	while (result<0) result+=26;
	result%=26;
	
	return result;
}

/*
得到密钥矩阵的逆矩阵，使用伴随矩阵法
求逆矩阵方法和线性代数中的方法有些不同，就是说并不是传统意义上的逆矩阵
*/
bool Hill::getInvKey()
{
	int i,j,k,l;
	int n=detLaplace(Size,Key);
	//if (n==0) return;
	while (n<0) n+=26;//如果行列式是负数则通过重复加上26转换成正数
	//i=1;
	//while ((n*i)%26!=1) i++; //按照书上的方法，行列式乘上一个正数除以26余数为1时满足要求,(即求乘法逆)
	//n=i;

	int x;
	if (!ExtendedEuclid(n,26,x)) return false;

	int **key0=new int*[Size-1];//key0用来存放中间结果
	for (j=0;j<Size-1;j++)
		key0[j]=new int[Size-1];
	for (i=0;i<Size;i++)
	{
		for (j=0;j<Size;j++)
		{
			int a=0,b=0;
			for (k=0;k<Size;k++)
			{
				if (k==j) continue;//跳过，因为是求余子式
				b=0;
				for (l=0;l<Size;l++)
				{
					if (l==i) continue;//跳过，因为是求余子式
					key0[a][b]=Key[k][l];
					b++;
				}
				a++;
			}
			//invKey[i][j]=int(pow(float(-1),i+j))*n*detLaplace(Size-1,key0);
			invKey[i][j]=int(pow(float(-1),i+j))*x*detLaplace(Size-1,key0);
			while (invKey[i][j]<0) invKey[i][j]+=26;
			invKey[i][j]%=26;
		}
	}
	for (j=0;j<Size-1;j++) //释放key0
		delete[] key0[j];
	delete[] key0;

	hasInvKey=true;

	return true;
}

/*用扩展欧几里德算法求乘法逆元*/
int Hill::ExtendedEuclid( int f,int d ,int &result)
{
	int x1,x2,x3,y1,y2,y3,t1,t2,t3,q;
	x1 = y2 = 1;
	x2 = y1 = 0;
    x3 = ( f>=d )?f:d;
    y3 = ( f>=d )?d:f;
    while(true)
	{
		if ( y3 == 0 )
		{
			result = x3; /* 两个数不互素则result为两个数的最大公约数，此时返回值为零 */
			return 0;
		}
		if ( y3 == 1 )
		{
			result = y2; /* 两个数互素则resutl为其乘法逆元，此时返回值为1 */
			return 1;
		}
		q = x3/y3;
		t1 = x1 - q*y1;
		t2 = x2 - q*y2;
		t3 = x3 - q*y3;
		x1 = y1;
		x2 = y2;
		x3 = y3;
		y1 = t1;
		y2 = t2;
		y3 = t3;
	}
}

void Hill::execute(bool flag)//flag为true为加密，使用Key；为false为解密，使用invKey。
{
	int **key;
	if (flag) key=Key;
	else
	{
		if (!hasInvKey) getInvKey();
		key=invKey;
	}
	/*一次读取Size大小个字符，并进行加密*/
	char p;
	int *v=new int[Size], //一次加密解密的向量
		*result=new int[Size];//一次加密解密返回的结果向量
	int i=0,//记录有效字符个数
		j=0;
	while (inFile.get(p))
	{
		p=toupper(p);
		if (p>='A' && p<='Z')
		{
			v[i%Size]=p-'A';
			i++;
			if (i%Size==0) 
			{
				VectorMultipleKeyMatrix(v,key,result);
				for (j=0;j<Size;j++)
					outFile<<char(result[j]+'A');//一律输出大写英文字母
			}
		}
		else outFile<<p;//如果不是英文字母则直接输出,略过加密解密过程
	}
	/*
	若有效加密解密的字符个数不是Size的整数倍，剩下的用X补足,X-A=23
	*/
	if (i%Size!=0)
	{
		while (i%Size!=0)
		{
			v[i%Size]=23;
			i++;
		}
		VectorMultipleKeyMatrix(v,key,result);
		for (j=0;j<Size;j++)
			outFile<<char(result[j]+'A');
	}

	delete[] v;
	delete[] result;
}