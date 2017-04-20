#include "CipherObj.h"

class Playfair:CipherObj
{
public:
	~Playfair();
	Playfair(string ifn/*输入文件路径*/,string ofn/*输出文件路径*/,string keyWord=""/*秘钥*/);
	void execute(bool flag);//执行函数，fp为函数指针，指明执行的是加密还是解密算法
private:
	static void encryption(char p[2],char[5][5]);//加密算法，每次加密两个字母，（playfair是多字母代替密码）
	static void decryption(char p[2],char[5][5]);//解密算法
	void createKey();//把密钥词转换成5*5的二维数组矩阵
private:
	string KeyWord;
	char KeyMatr[5][5];
};