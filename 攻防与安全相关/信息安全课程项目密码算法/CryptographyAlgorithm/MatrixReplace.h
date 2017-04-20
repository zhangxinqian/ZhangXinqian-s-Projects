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
	int* keyArr;//��Կ����
	int keyLen;//��Կ���鳤��,���ܳ���9
	int row,/*��������*/
		col;/*��������*/
};