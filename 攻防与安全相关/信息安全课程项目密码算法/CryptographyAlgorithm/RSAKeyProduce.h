#include "decnum.h"

struct RSAKEY  //RSA��Կ��˽Կ�Ľṹ��
{
	RSAKEY(decnum p=0,decnum n=0)
	{
		P=p;
		N=n;
	}
	decnum P;
	decnum N;
};

class RSAKeyProduce
{
public:
	RSAKeyProduce();
	~RSAKeyProduce();
public:
	static void produceKey(RSAKEY &PU,RSAKEY &PR);
	static RSAKEY getPublicKey();
	static RSAKEY getPrivateKey();
private:
	/*����ģ��������(a^b) mod c*/
	static decnum powMod(decnum a,decnum b,decnum c);
	/*
	������˵��Miller-Robin�ĸ����������ԣ�
	�ж��Ƿ�������
	*/
	static bool MillerRabin(decnum &n/*nΪ������*/);
	static bool MillerRabinTest(decnum &n,int k);//����k��
	//����ʮ����λ��Ϊnλ�Ĵ�����
	static void bigOdd(decnum &odd,int n);
	/*�������������Լ��*/
	static decnum gcd(decnum x,decnum y);
	/*
	����չŷ������㷨��˷���Ԫ��
	f*result=1 (mod d)
	*/
	static bool ExtendedEuclid( decnum f,decnum d ,decnum &result);
private:
	static decnum p,q;//����������
	/*
	(e,n)Ϊ��Կ��Ϊ�˼ӿ�ʹ�ù�Կ�ǵ������ٶȣ�ѡ��e=65537��2^16+1
	*/
	const static decnum e;
	static decnum d;//(d,n)Ϊ˽Կ
	static decnum n;//n=p*q
	static decnum thn;//С��n����n���ص�����������,thn=(p-1)*(q-1)
};