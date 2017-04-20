#include "CipherObj.h"

class Caesar:public CipherObj
{
public:
	Caesar(string ifn/*输入文件路径*/,string ofn/*输出文件路径*/,int key=0/*秘钥*/);
	~Caesar();
public:
	void execute(bool flag);
private:
	static void encryption(char &p,int key);//加密单个字符
	static void decryption(char &p,int key);//解密单个字符
private:
	int Key;
};