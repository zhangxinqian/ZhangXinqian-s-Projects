#include "stdafx.h"

#include "RSAKeyProduce.h"
using namespace std;

decnum RSAKeyProduce::p=0;
decnum RSAKeyProduce::q=0;
const decnum RSAKeyProduce::e=65537;
decnum RSAKeyProduce::d=0;
decnum RSAKeyProduce::n=0;
decnum RSAKeyProduce::thn=0;

RSAKeyProduce::RSAKeyProduce()
{

}

RSAKeyProduce::~RSAKeyProduce()
{

}


RSAKEY RSAKeyProduce::getPublicKey()
{
	return RSAKEY(e,n);
}

RSAKEY RSAKeyProduce::getPrivateKey()
{
	return RSAKEY(d,n);
}



/*该方法不适用检测大素数
bool RSAKeyProduce::isPrime(int prime)
{
	int i,
		len=sqrt((double)prime);
	for (i=2;i<=len;i++)
		if (0==prime%i) return false;
	return true;
}
*/

void RSAKeyProduce::produceKey(RSAKEY &PU,RSAKEY &PR)
{
	decnum temp;
	do 
	{
		//do 
		//{
			do 
			{
				bigOdd(temp,100);
			} while (!MillerRabinTest(temp,10));
			p=temp;
			do 
			{
				bigOdd(temp,100);
			} while (!MillerRabinTest(temp,10));
			q=temp;
			thn=(p-1)*(q-1);
		//} while (gcd(e,thn)!=1);//若生成的p,q不满足e=65537则重新生成p,q
		n=p*q;
	} while (!ExtendedEuclid(e,thn,d));
	PU.P=e;
	PU.N=n;
	PR.P=d;
	PR.N=n;
}

void RSAKeyProduce::bigOdd(decnum &odd,int n)
{
	//随机生成十进制的n位的大奇数
	int i,j;
	decnum temp=decnum(10);
	srand(time(NULL));  //随机数种子为当前系统时间
	j=rand()%10;
	while(j==0)//最高位不为0,保证有100位
		j=rand()%10;
	odd=decnum(j);

	for (i=0;i<n-2;i++)
	{
		j=rand()%10;
		odd=odd*temp+decnum(j);
	}
	/*最后一位为奇数*/
	j=rand()%10;
	while(j%2==0)
		j=rand()%10;
	odd=odd*temp+decnum(j);
}

bool RSAKeyProduce::MillerRabin(decnum &n)
{
	//素数检验算法
	decnum a;
	decnum s=0,t=n-1,i,x=1;

	do 
	{
		srand(time(NULL));
		a=int(rand());//随机生成一个正整数a<n
	} while (a>=n);//保证a小于n

	do//构造n-1=2^s*t,s为非负整数，t是正奇数
	{
		s+=1;
		t=t/2;
	}while((t%2)==0); //保证t为奇数

	/*速度太慢
	i=1;
	while(i<=t)
	{
		x=(x*a)%n;
		i+=1;
	}
	*/

	x=powMod(a,t,n);

	if((x==1)||(x==n-1))return true; //通过测试

	for(i=1;i<=s-1;i+=1)//进一步检测
	{
		x=pow((decnum)x%n,2)%n;
		if (x==1) return false;
		if (x==n-1) return true;
	}
	return false;
}

bool RSAKeyProduce::MillerRabinTest(decnum &n,int k)
{
	for (int i=0;i<k;i++)
		if (!MillerRabin(n)) return false;
	return true;
}

decnum RSAKeyProduce::gcd(decnum x,decnum y)
{
	/*大数的辗转相除法*/
	decnum temp;
	decnum zero=0;
	if (x<y)
	{
		temp=x;
		x=y;
		y=temp;
	}

	temp=x%y;
	while (temp!=zero)
	{
		x=y;
		y=temp;
		temp=x%y;
	}
	return y;
}

/*用扩展欧几里德算法求乘法逆元*/
bool RSAKeyProduce::ExtendedEuclid( decnum f,decnum d /*mod d*/,decnum &result)
{
	decnum x1,x2,x3,y1,y2,y3,t1,t2,t3,q;
	x1 = y2 = 1;
	x2 = y1 = 0;
	x3 = ( f>=d )?f:d;
	y3 = ( f>=d )?d:f;
	while(true)
	{
		if ( y3 == 0 )
		{
			result = x3; /* 两个数不互素则result为两个数的最大公约数，此时返回值为假 */
			return false;
		}
		if ( y3 == 1 )
		{
			result = y2; /* 两个数互素则resutl为其乘法逆元，此时返回值为真 */
			return true;
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

/*使用课本上效率高的快速模幂算法*/
decnum RSAKeyProduce::powMod(decnum a,decnum b,decnum c)
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