#include "CipherObj.h"

class DES:CipherObj
{
public:
	DES(string ifn/*输入文件路径*/,string ofn/*输出文件路径*/,string ks/*8字节密钥字符串*/);
	~DES();
	void execute(bool flag);
private:
	/*将8字节密钥字符串转换成64位密钥*/
	void getKey64();
	void getKey64H();
	/*
	将64位密钥压缩置换（置换选择1）为56位，
	并用置换选择2和循环左移产生16个子密钥
	*/
	void produceSubKey();
	/*S盒代替选择函数*/
	void S(bool output[32],const bool input[48]);
	/*F函数*/
	void FFunction(bool inOut[32]/*32位bits与key作用，同时也是返回值*/,const bool key[48]);

	/*
	核心算法，一次加密解密64位，即8字节
	flag为ture为加密，false为解密
	*/
	void des(bool target[64],bool flag);

	/*将字节数组转换成比特数组*/
	void byte2bit(bool *output/*输出bool数组，代表比特*/,
		const char *input/*输入字符数组，char占1字节*/,
		int bitsLen/*进行运算的比特个数*/);
	/*将比特数组转换成字符数组*/
	void bit2byte(char *output, const bool *input, int bitsLen);
	/*按位异或函数,结果保存在c数组中*/
	void xor(const bool *x,const bool *y,bool *c,int bitsLen);
	/*根据指定的置换矩阵进行置换选择,输入输出可以是同一个数组*/
	void displace(bool *output,const bool *input,char *table,int len/*置换表长度*/);
	void leftMove(bool halfKey[28],char s);/*密钥28位左移函数*/

	void H2B(bool *output,const char *input,int len);
	void B2H(char *output,const bool *input,int len);
private:
	string keyStr;/*8字节密钥字符串*/
	bool key64[64];/*输入的64位密钥*/
	bool subKey[16][48];/*16轮的子密钥*/
private:
	/*按输入字节进行加密，每个字节转换成8位二进制位进行加密*/

	/*使用char（1字节）来存储而不用int（4字节），节省空间*/
	static const char IP_table[64];     //初始置换(IP)
	static const char InvIP_table[64];  //逆初始值换(IP-1)
	static const char E_table[48];   //扩展置换(E)
	static const char P_table[32];   //置换函数(P)
	static const char S_box[8][4][16];  //DES的S盒定义
	static const char PC1_table[56];  //置换选择1(PC-1)
	static const char PC2_table[48];  //置换选择2(PC-2)
	static const char KeyLeftNum[16];   //左移次数的确定16轮,下标从0开始
};