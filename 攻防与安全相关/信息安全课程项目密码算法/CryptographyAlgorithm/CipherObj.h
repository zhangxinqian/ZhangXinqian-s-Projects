#ifndef _CIPHEROBJ
#define _CIPHEROBJ

#include<fstream>
#include <string>
using namespace std;

/*定义一个抽象类*/
class CipherObj
{
public:
	CipherObj(string ifn/*输入文件路径*/,string ofn/*输出文件路径*/);
	virtual ~CipherObj();
public:
	virtual void execute(bool flag)=0;/*flag为ture为加密，flag为false为解密*/
protected:
	string inFileName;/*输入文件路径*/
	string outFileName;/*输出文件路径*/
	ifstream inFile;
	ofstream outFile;
};

#endif