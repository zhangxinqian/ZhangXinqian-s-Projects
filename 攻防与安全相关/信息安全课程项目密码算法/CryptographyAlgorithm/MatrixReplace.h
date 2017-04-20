#include "CipherObj.h"

class MatrixReplace:CipherObj
{
public:
	MatrixReplace(string ifn,string ofn,string key="");
	~MatrixReplace();
	void execute(bool flag);
private:
	void encryption(string &target,string &result,char **temp,int *order);
	void decryption(string &target,string &result,char **temp,int *order);
private:
	int* keyArr;//密钥数组
	int keyLen;//密钥数组长度,不能超过9
	int row,/*矩阵行数*/
		col;/*矩阵列数*/
};