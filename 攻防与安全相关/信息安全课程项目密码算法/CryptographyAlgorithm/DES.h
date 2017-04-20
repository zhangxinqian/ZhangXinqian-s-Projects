#include "CipherObj.h"

class DES:CipherObj
{
public:
	DES(string ifn/*�����ļ�·��*/,string ofn/*����ļ�·��*/,string ks/*8�ֽ���Կ�ַ���*/);
	~DES();
	void execute(bool flag);
private:
	/*��8�ֽ���Կ�ַ���ת����64λ��Կ*/
	void getKey64();
	void getKey64H();
	/*
	��64λ��Կѹ���û����û�ѡ��1��Ϊ56λ��
	�����û�ѡ��2��ѭ�����Ʋ���16������Կ
	*/
	void produceSubKey();
	/*S�д���ѡ����*/
	void S(bool output[32],const bool input[48]);
	/*F����*/
	void FFunction(bool inOut[32]/*32λbits��key���ã�ͬʱҲ�Ƿ���ֵ*/,const bool key[48]);

	/*
	�����㷨��һ�μ��ܽ���64λ����8�ֽ�
	flagΪtureΪ���ܣ�falseΪ����
	*/
	void des(bool target[64],bool flag);

	/*���ֽ�����ת���ɱ�������*/
	void byte2bit(bool *output/*���bool���飬�������*/,
		const char *input/*�����ַ����飬charռ1�ֽ�*/,
		int bitsLen/*��������ı��ظ���*/);
	/*����������ת�����ַ�����*/
	void bit2byte(char *output, const bool *input, int bitsLen);
	/*��λ�����,���������c������*/
	void xor(const bool *x,const bool *y,bool *c,int bitsLen);
	/*����ָ�����û���������û�ѡ��,�������������ͬһ������*/
	void displace(bool *output,const bool *input,char *table,int len/*�û�����*/);
	void leftMove(bool halfKey[28],char s);/*��Կ28λ���ƺ���*/

	void H2B(bool *output,const char *input,int len);
	void B2H(char *output,const bool *input,int len);
private:
	string keyStr;/*8�ֽ���Կ�ַ���*/
	bool key64[64];/*�����64λ��Կ*/
	bool subKey[16][48];/*16�ֵ�����Կ*/
private:
	/*�������ֽڽ��м��ܣ�ÿ���ֽ�ת����8λ������λ���м���*/

	/*ʹ��char��1�ֽڣ����洢������int��4�ֽڣ�����ʡ�ռ�*/
	static const char IP_table[64];     //��ʼ�û�(IP)
	static const char InvIP_table[64];  //���ʼֵ��(IP-1)
	static const char E_table[48];   //��չ�û�(E)
	static const char P_table[32];   //�û�����(P)
	static const char S_box[8][4][16];  //DES��S�ж���
	static const char PC1_table[56];  //�û�ѡ��1(PC-1)
	static const char PC2_table[48];  //�û�ѡ��2(PC-2)
	static const char KeyLeftNum[16];   //���ƴ�����ȷ��16��,�±��0��ʼ
};