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



/*�÷��������ü�������
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
		//} while (gcd(e,thn)!=1);//�����ɵ�p,q������e=65537����������p,q
		n=p*q;
	} while (!ExtendedEuclid(e,thn,d));
	PU.P=e;
	PU.N=n;
	PR.P=d;
	PR.N=n;
}

void RSAKeyProduce::bigOdd(decnum &odd,int n)
{
	//�������ʮ���Ƶ�nλ�Ĵ�����
	int i,j;
	decnum temp=decnum(10);
	srand(time(NULL));  //���������Ϊ��ǰϵͳʱ��
	j=rand()%10;
	while(j==0)//���λ��Ϊ0,��֤��100λ
		j=rand()%10;
	odd=decnum(j);

	for (i=0;i<n-2;i++)
	{
		j=rand()%10;
		odd=odd*temp+decnum(j);
	}
	/*���һλΪ����*/
	j=rand()%10;
	while(j%2==0)
		j=rand()%10;
	odd=odd*temp+decnum(j);
}

bool RSAKeyProduce::MillerRabin(decnum &n)
{
	//���������㷨
	decnum a;
	decnum s=0,t=n-1,i,x=1;

	do 
	{
		srand(time(NULL));
		a=int(rand());//�������һ��������a<n
	} while (a>=n);//��֤aС��n

	do//����n-1=2^s*t,sΪ�Ǹ�������t��������
	{
		s+=1;
		t=t/2;
	}while((t%2)==0); //��֤tΪ����

	/*�ٶ�̫��
	i=1;
	while(i<=t)
	{
		x=(x*a)%n;
		i+=1;
	}
	*/

	x=powMod(a,t,n);

	if((x==1)||(x==n-1))return true; //ͨ������

	for(i=1;i<=s-1;i+=1)//��һ�����
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
	/*������շת�����*/
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

/*����չŷ������㷨��˷���Ԫ*/
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
			result = x3; /* ��������������resultΪ�����������Լ������ʱ����ֵΪ�� */
			return false;
		}
		if ( y3 == 1 )
		{
			result = y2; /* ������������resutlΪ��˷���Ԫ����ʱ����ֵΪ�� */
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

/*ʹ�ÿα���Ч�ʸߵĿ���ģ���㷨*/
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