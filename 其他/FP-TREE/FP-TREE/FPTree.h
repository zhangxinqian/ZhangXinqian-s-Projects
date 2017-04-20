#ifndef FPTREE_H
#define FPTREE_H

#include "treeNode.h"
#include <list>
#include <hash_map>
#include <iostream>
using namespace std;
using namespace stdext;

template<typename T>
class FPTree
{
public:
	//FPTree():minSupport(0),root(0),headerTable(0){};
	FPTree(int _minSupport = 0):minSupport(_minSupport){};
	/*
	FPTree(int minSupport=0,list<TreeNode<T>*>* headerTable=0,TreeNode<T>* root=0):
	minSupport(minSupport),headerTable(headerTable),root(root){};
	*/
	//FPTree(int minSupport=0,list<TreeNode<T>*>* headerTable=0,TreeNode<T>* root=0);

	virtual ~FPTree(){};

	int getMinSupport() const{
		return minSupport;
	}

	void setMinSupport(const int minSupport){
		this->minSupport=minSupport;
	}

	void FPGrowth(list<list<T>*>* transactionRecords,list<list<T>*>* &result,list<T>* postPattern);

private:
	list<TreeNode<T>*>* buildHeaderTable(list<list<T>*>* transactionRecords);

	TreeNode<T>* buildFPTree(list<list<T>*>* transactionRecords,list<TreeNode<T>*>* headerTable);

	// 把交易记录按项的频繁程序降序排列
	list<T>* sortAsHeaderTable(list<T>* records,list<TreeNode<T>*>* headerTable);
	//利用包括transTemp[k]在内及其以后的值为root节点创建子树
	void addNodes(TreeNode<T>* ancestor,list<T>* record,list<TreeNode<T>*>* headerTable);

	void destroyFPTree(TreeNode<T>* root,list<TreeNode<T>*>* headerTable);

	void destroyTransactionRecords(list<list<T>*>* transactionRecords);

	static bool descending(const TreeNode<T> *t1,const TreeNode<T> *t2)
	{
		return (t1->getCount()>t2->getCount());
	}
private:
	int minSupport;
	//list<TreeNode<T>*>* headerTable;
    //TreeNode<T>* root;
};

/*
template<typename T>
FPTree<T>::~FPTree()
{
	if (headerTable!=0)
	{
		//list<TreeNode<T>*>::iterator itor;
		TreeNode<T>* tnode,*tmpnode;
		while (headerTable->size()>0)
		{
			//tnode=reinterpret_cast<TreeNode<T>*>(itor);
			//tnode=(TreeNode<T>*)itor;
			tnode=headerTable->front();
			headerTable->pop_front();
			while (tnode!=0)
			{
				tmpnode=tnode;
				tnode=tnode->getNextHomonym();
				delete tmpnode;
			}
		}
		delete headerTable;
	}
	if (root!=0) delete root;
}
*/

template<typename T>
void FPTree<T>::destroyFPTree(TreeNode<T>* root,list<TreeNode<T>*>* headerTable)
{
	if (headerTable!=0)
	{
		TreeNode<T>* tnode,*tmpnode;
		while (headerTable->size()>0)
		{
			tnode=headerTable->front();
			headerTable->pop_front();
			while (tnode!=0)
			{
				tmpnode=tnode;
				tnode=tnode->getNextHomonym();
				delete tmpnode;
			}
		}
		delete headerTable;
	}
	if (root!=0) delete root;
}

template<typename T>
void FPTree<T>::destroyTransactionRecords(list<list<T>*>* transactionRecords)
{
	list<list<T>*>::iterator trans;
	list<T>* tmpList=0;

	for (trans=transactionRecords->begin();trans!=transactionRecords->end();++trans)
	{
		tmpList=*trans;
		delete tmpList;
	}
	delete transactionRecords;
	transactionRecords=0;
}

/*
template<typename T>
FPTree<T>::FPTree(int minSupport,list<TreeNode<T>*>* headerTable,TreeNode<T>* root)
{
	this->minSupport=minSupport;
	this->headerTable=headerTable;
	this->root=root;
}
*/

template<typename T>
list<TreeNode<T>*>* FPTree<T>::buildHeaderTable(list<list<T>*>* transactionRecords)
{
	if (transactionRecords->size()>0)
	{
		list<TreeNode<T>*>* f1=0;
		f1=new list<TreeNode<T>*>();
		//使用哈希表，使用链表效率太低了，每次输入一个值都要遍历一遍链表
		hash_map<T,TreeNode<T>*> mymap;
		hash_map<T,TreeNode<T>*>::iterator itor;
		list<list<T>*>::iterator itor1;
		list<T>::iterator itor2;
		for (itor1=transactionRecords->begin();itor1!=transactionRecords->end();++itor1)
			for (itor2=(*itor1)->begin();itor2!=(*itor1)->end();++itor2)
			{
				itor=mymap.find((*itor2));
				if (itor==mymap.end())
				{
					TreeNode<T>* node=new TreeNode<T>((*itor2),1);
					mymap.insert(make_pair((*itor2),node));
				}
				else itor->second->increaseCount(1);
			}
		TreeNode<T>* node;
		for (itor=mymap.begin();itor!=mymap.end();++itor)
		{
			node=itor->second;
			if (node->getCount()>=minSupport) f1->push_back(node);
		}
		f1->sort(descending);//从大到小排序
		//f1->sort();
		//headerTable=f1;

		return f1;
	}
	else return 0;
}

template<typename T>
TreeNode<T>* FPTree<T>::buildFPTree(list<list<T>*>* transactionRecords,list<TreeNode<T>*>* headerTable)
{
	TreeNode<T>* root = new TreeNode<T>(); // 创建树的根节点
	list<list<T>*>::iterator itor;
	for (itor=transactionRecords->begin();itor!=transactionRecords->end();++itor)
	{
		list<T> *record=sortAsHeaderTable((*itor),headerTable);
		TreeNode<T> *subTreeRoot=root;
		TreeNode<T> *tmpRoot=0;
		if (root->getChildren()!=0)
		{
			while (!record->empty() && (tmpRoot=subTreeRoot->searchChild(record->front()))!=0)
			{
				tmpRoot->increaseCount(1);
				subTreeRoot=tmpRoot;
				record->pop_front();
			}
		}
		addNodes(subTreeRoot,record,headerTable);
		//运行完record链表必为空，但是record指针指向的链表对象仍然存在，需要释放内存
		delete record;//释放在sortAsHeaderTable中申请的list<T>空间
	}
	return root;
}
template<typename T>
list<T>* FPTree<T>::sortAsHeaderTable(list<T>* records,list<TreeNode<T>*>* headerTable)
{
	list<TreeNode<T>*>::iterator itor1;
	list<T>::iterator itor2;
	list<T>* result=new list<T>();
	for (itor1=headerTable->begin();itor1!=headerTable->end();++itor1)
	{
		// headerTable已经是按降序排列的
		for (itor2=records->begin();itor2!=records->end();++itor2)
		{
			if ((*itor1)->getItem()==(*itor2)) result->push_back((*itor2));
		}
	}
	return result;
}

// 把record作为ancestor的后代插入树中
template<typename T>
void FPTree<T>::addNodes(TreeNode<T>* ancestor,list<T>* record,list<TreeNode<T>*>* headerTable)
{
	while (record->size() > 0) 
	{
		T item=record->front();
		record->pop_front();
		TreeNode<T> *leafnode = new TreeNode<T>(item);
		leafnode->setCount(1);
		leafnode->setParent(ancestor);
		ancestor->addAChild(leafnode);

		//树中相同名称的节点链接起来，链表的第一个元素就是表头项里的元素
		list<TreeNode<T>*>::iterator itor;
		for (itor=headerTable->begin();itor!=headerTable->end();++itor)
		{
			if ((*itor)->getItem()==item)
			{
				TreeNode<T>* tn=(*itor);
				while (tn->getNextHomonym()!=0)
				{
					tn=tn->getNextHomonym();
				}
				tn->setNextHomonym(leafnode);
				break;
			}
		}
		addNodes(leafnode, record, headerTable);
	}
}

template<typename T>
void FPTree<T>::FPGrowth(list<list<T>*>* transactionRecords,list<list<T>*>* &result,list<T>* postPattern)
{
	// 构建项头表，同时也是频繁1项集
	list<TreeNode<T>*>* headerTable = buildHeaderTable(transactionRecords);
	//递归结束
	if (headerTable==0) 
	{
		destroyTransactionRecords(transactionRecords);
		return;
	}

	// 构建FP-Tree
	TreeNode<T>* root = buildFPTree(transactionRecords, headerTable);
	// 如果FP-Tree为空则返回

	//这里开始就不需要原来的事务数据了，可以释放掉内存
	destroyTransactionRecords(transactionRecords);
	
	if (root->getChildren()==0 || root->getChildren()->size() == 0)
		return;

	list<TreeNode<T>*>::iterator tnIt;
	list<T>::iterator lIt;
	//输出项头表的每一项+postPattern
	if(postPattern!=0)
	{
		for (tnIt=headerTable->begin();tnIt!=headerTable->end();++tnIt)
		{
			cout<<(*tnIt)->getCount()<<"  "<<(*tnIt)->getItem()<<' ';

			list<T>* tmp=new list<T>();
			tmp->push_back((*tnIt)->getCount());
			tmp->push_back((*tnIt)->getItem());

			for (lIt=postPattern->begin();lIt!=postPattern->end();++lIt)
			{
				cout<<(*lIt)<<' ';

				tmp->push_back((*lIt));

			}

			result->push_back(tmp);

			cout<<endl;
		}
	}
	
	// 找到项头表的每一项的条件模式基(conditional pattern base)，进行递归迭代

	for (tnIt=headerTable->begin();tnIt!=headerTable->end();++tnIt) 
	{
		// 后缀模式增加一项
		list<T>* newPostPattern=new list<T>();
		newPostPattern->push_back((*tnIt)->getItem());
		if (postPattern != 0)
		{
			for (lIt=postPattern->begin();lIt!=postPattern->end();++lIt)
				newPostPattern->push_front(*lIt);
		}
		// 寻找header的条件模式基CPB，放入newTransRecords中
		list<list<T>*>* newTransRecords=new list<list<T>*>();
		TreeNode<T>* backnode=(*tnIt)->getNextHomonym();
		while (backnode != 0) 
		{
			int counter = backnode->getCount();
			list<T> *prenodes=new list<T>();
			TreeNode<T> *parent = backnode;
			// 遍历backnode的祖先节点，放到prenodes中
			while ((parent = parent->getParent())!= root) 
			{
				prenodes->push_front(parent->getItem());
			}
			while (counter-- > 0)
			{
				list<T> *temp=new list<T>();
				list<T>::iterator cp;
				for (cp=prenodes->begin();cp!=prenodes->end();++cp)
				{
					temp->push_back((*cp));
				}
				newTransRecords->push_back(temp);
				//newTransRecords->push_back(prenodes);
			}
			delete prenodes;
			backnode = backnode->getNextHomonym();
		}
		// 递归迭代
		FPGrowth(newTransRecords,result,newPostPattern);
	}
	delete postPattern;
	destroyFPTree(root,headerTable);
}

#endif
