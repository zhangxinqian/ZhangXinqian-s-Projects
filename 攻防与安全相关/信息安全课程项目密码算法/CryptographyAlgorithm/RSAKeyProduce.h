#include "decnum.h"

struct RSAKEY  //RSA公钥或私钥的结构体
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
	/*快速模幂运算求(a^b) mod c*/
	static decnum powMod(decnum a,decnum b,decnum c);
	/*
	用书上说的Miller-Robin的概率素数测试，
	判断是否是素数
	*/
	static bool MillerRabin(decnum &n/*n为大奇数*/);
	static bool MillerRabinTest(decnum &n,int k);//测试k次
	//生成十进制位数为n位的大奇数
	static void bigOdd(decnum &odd,int n);
	/*求两个数的最大公约数*/
	static decnum gcd(decnum x,decnum y);
	/*
	用扩展欧几里德算法求乘法逆元，
	f*result=1 (mod d)
	*/
	static bool ExtendedEuclid( decnum f,decnum d ,decnum &result);
private:
	static decnum p,q;//两个大素数
	/*
	(e,n)为公钥，为了加快使用公钥是的运算速度，选择e=65537即2^16+1
	*/
	const static decnum e;
	static decnum d;//(d,n)为私钥
	static decnum n;//n=p*q
	static decnum thn;//小于n且与n互素的正整数个数,thn=(p-1)*(q-1)
};