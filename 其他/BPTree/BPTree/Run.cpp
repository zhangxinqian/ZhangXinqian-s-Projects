#include <stdlib.h>
#include<iostream>
#include <time.h>
#include<fstream>
#include "BPlusTree.h"
using namespace std;



// 在B+树中查找某数据
void Search(BPlusTree* root, int data)
{
	char sPath[255] = {0, };

	(void)root->Search(data, sPath);
	printf("\n%s", sPath);
}

// 在B+树中插入某数据
void Insert(BPlusTree* root, int data)
{
	bool success = root->Insert(data);
	if (true == success)
	{
		cout<<"插入数据"<<data<<"成功！"<<endl;

	}
	else
	{
		cout<<"插入数据"<<data<<"失败！"<<endl;
	}
}


// 在B+树中删除某数据
void Delete(BPlusTree* root, int data)
{
	bool success = root->Delete(data);
	if (true == success)
	{
		cout<<"删除"<<data<<"成功！"<<endl;
	}
	else
	{
		cout<<"删除"<<data<<"失败！"<<endl;
	}
}


// 输出整个B+树
void Print(BPlusTree* root)
{
	root->PrintTree();
}


int main()
{
	BPlusTree* root = new BPlusTree;

	ifstream inData;
	inData.open("input1.txt");

	char command=' ';

	inData.get(command);
	while(inData){

		switch(command)
		{

		case 'P':
			cout<<"操作为 P ：显示树的全貌："<<endl;
			Print(root);
			cout<<endl;
			break;
		case '+':
			cout<<"操作位 + ： 插入数据："<<endl;
			inData.get(command);
			while(command!='\n')
			{
				inData.get(command);
				int number=0;
				while(int(command)>=48&&int(command)<=57)
				{

					number=int(command)-48+number*10;
					inData.get(command);
				}

				Insert(root, number);
			}
			cout<<endl;
			break;
		case '-':
			cout<<"操作为 - ：删除数据："<<endl;
			inData.get(command);
			while(command!='\n')
			{
				inData.get(command);
				int number=0;
				while(int(command)>=48&&int(command)<=57){

					number=int(command)-48+number*10;
					inData.get(command);
				}

				Delete(root, number);

			}
			cout<<endl;
			break;
		case '~':
			cout<<"操作为 ~ ：查询数据："<<endl;
			inData.get(command);
			while(command!='\n')
			{
				inData.get(command);
				int number=0;
				while(int(command)>=48&&int(command)<=57){

					number=int(command)-48+number*10;
					inData.get(command);
				}
				Search(root, number);
				
			}
			cout<<endl;
			break;
			getchar();

		}
		inData.get(command);

	}
	getchar();
	delete root;
	return 0;
}