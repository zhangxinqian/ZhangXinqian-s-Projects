#ifndef _CIPHEROBJ
#define _CIPHEROBJ

#include<fstream>
#include <string>
using namespace std;

/*����һ��������*/
class CipherObj
{
public:
	CipherObj(string ifn/*�����ļ�·��*/,string ofn/*����ļ�·��*/);
	virtual ~CipherObj();
public:
	virtual void execute(bool flag)=0;/*flagΪtureΪ���ܣ�flagΪfalseΪ����*/
protected:
	string inFileName;/*�����ļ�·��*/
	string outFileName;/*����ļ�·��*/
	ifstream inFile;
	ofstream outFile;
};

#endif