#include <stdlib.h>
#include<iostream>
#include <time.h>
#include<fstream>
#include "BPlusTree.h"
using namespace std;



// ��B+���в���ĳ����
void Search(BPlusTree* root, int data)
{
	char sPath[255] = {0, };

	(void)root->Search(data, sPath);
	printf("\n%s", sPath);
}

// ��B+���в���ĳ����
void Insert(BPlusTree* root, int data)
{
	bool success = root->Insert(data);
	if (true == success)
	{
		cout<<"��������"<<data<<"�ɹ���"<<endl;

	}
	else
	{
		cout<<"��������"<<data<<"ʧ�ܣ�"<<endl;
	}
}


// ��B+����ɾ��ĳ����
void Delete(BPlusTree* root, int data)
{
	bool success = root->Delete(data);
	if (true == success)
	{
		cout<<"ɾ��"<<data<<"�ɹ���"<<endl;
	}
	else
	{
		cout<<"ɾ��"<<data<<"ʧ�ܣ�"<<endl;
	}
}


// �������B+��
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
			cout<<"����Ϊ P ����ʾ����ȫò��"<<endl;
			Print(root);
			cout<<endl;
			break;
		case '+':
			cout<<"����λ + �� �������ݣ�"<<endl;
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
			cout<<"����Ϊ - ��ɾ�����ݣ�"<<endl;
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
			cout<<"����Ϊ ~ ����ѯ���ݣ�"<<endl;
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