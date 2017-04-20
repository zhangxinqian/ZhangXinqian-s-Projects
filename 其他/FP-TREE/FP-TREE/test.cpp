#include <iostream>
#include <fstream>
#include <string>
#include "treeNode.h"
#include "FPTree.h"
using namespace std;

ifstream infile;
ofstream outfile;

void readData(string fileName,list<list<int>*>* &transRecords);
void writeData(string fileName,list<list<int>*>* result);
void destroyTransactionRecords(list<list<int>*>* transactionRecords);

int main()
{
	int x;
	string path1,path2;

	cout<<"输入文件路径：";
	cin>>path1;
	cout<<"输入输出文件路径：";
	cin>>path2;
	cout<<"输入最小支持度：";
	cin>>x;

    list<list<int>*>* transRecords=0,
		*result=0;

	//char *fileName="retail.dat";
	cout<<"开始"<<endl;

	readData(path1,transRecords);

	cout<<"数据读取成功"<<endl;

	FPTree<int> *fptree=new FPTree<int>(x);
	
	result=new list<list<int>*>();

	fptree->FPGrowth(transRecords,result,0);

	delete fptree;

	writeData(path2,result);

	destroyTransactionRecords(result);

	cout<<"结束"<<endl;

	cin.get();
	cin.get();

	return 0;
}

void readData(string fileName,list<list<int>*>* &transRecords)
{
	infile.open(fileName.c_str());

	string line;
	int data=0;
	int i;
	list<int>* tmpList=0;
	transRecords=new list<list<int>*>();
	while (getline(infile,line) && !infile.eof())
	{
		tmpList=new list<int>();
		for(i=0;i<line.length();i++)
		{
			if (line[i]!=' ') data=10*data+(line[i]-'0');
			else
			{
				tmpList->push_back(data);
				data=0;
			}
		}
		tmpList->push_back(data);
		data=0;
		transRecords->push_back(tmpList);
		tmpList=0;
	}

	infile.close();
}

//第一个整形存的是频数
void writeData(string fileName,list<list<int>*>* result)
{
	outfile.open(fileName.c_str());

	list<list<int>*>::iterator trans;
	list<int>::iterator reds;

	for (trans=result->begin();trans!=result->end();++trans)
	{
		outfile<<(*trans)->front()<<':'<<' ';
		reds=(*trans)->begin();
		++reds;
		for (;reds!=(*trans)->end();++reds)
		{
			outfile<<(*reds)<<' ';
		}
		outfile<<endl;
	}

	outfile.close();
}

void destroyTransactionRecords(list<list<int>*>* transactionRecords)
{
	list<list<int>*>::iterator trans;
	list<int>* tmpList=0;

	for (trans=transactionRecords->begin();trans!=transactionRecords->end();++trans)
	{
		tmpList=*trans;
		delete tmpList;
	}
	delete transactionRecords;
	transactionRecords=0;
}