#include "BPlusTree.h"
#include<iostream>
#include "stdlib.h"
using namespace std;

CNode::CNode()
{
	Type = NODE_TYPE_LEAF;
	Count = 0;
	pFather = NULL;
}
CNode::~CNode()
{
	DeleteChildren();
}

// 获取一个最近的兄弟结点
CNode* CNode::GetBrother(int& flag)
{
	CNode* pFather = GetFather();
	if (NULL == pFather)
	{
		return NULL;
	}

	CNode* pBrother = NULL;

	for (int i = 1; i <= pFather->GetCount() + 1; i++)
	{
		// 找到本结点的位置
		if (pFather->GetPointer(i) == this)
		{
			if (i == (pFather->GetCount() + 1))
			{
				pBrother = pFather->GetPointer(i - 1);    // 本身是最后一个指针，只能找前一个指针
				flag = FLAG_LEFT;
			}
			else
			{
				pBrother = pFather->GetPointer(i + 1);    // 优先找后一个指针
				flag = FLAG_RIGHT;
			}
		}
	}

	return pBrother;
}

// 删除子结点
void CNode::DeleteChildren()
{
	for (int i = 1; i <= GetCount(); i++)
	{
		CNode * Node = GetPointer(i);
		if (NULL != Node)    // 叶子结点没有指针
		{
			Node->DeleteChildren();
		}

		delete Node;
	}
}

CInternalNode::CInternalNode()
{
	Type = NODE_TYPE_INTERNAL;

	int i = 0;
	for (i = 0; i < MAXNUM_KEY; i++)
	{
		Keys[i] = INVALID;
	}

	for (i = 0; i < MAXNUM_POINTER; i++)
	{
		Pointers[i] = NULL;
	}
}
CInternalNode::~CInternalNode()
{
	for (int i = 0; i < MAXNUM_POINTER; i++)
	{
		Pointers[i] = NULL;
	}
}

// 在中间结点中插入键
bool CInternalNode::Insert(KEY_TYPE value, CNode* Node)
{
	// 如果中间结点已满，直接返回失败
	if (GetCount() >= MAXNUM_KEY)
	{
		return false;
	}

	int j = 0;

	// 找到要插入键的位置
	int i;
	for (i = 0; (value > Keys[i]) && ( i < Count); i++)
	{
	}

	// 当前位置及其后面的键依次后移，空出当前位置
	for (j = Count; j > i; j--)
	{
		Keys[j] = Keys[j - 1];
	}

	// 当前位置及其后面的指针依次后移
	for (j = Count + 1; j > i + 1; j--)
	{
		Pointers[j] = Pointers[j - 1];
	}

	// 把键和指针存入当前位置
	Keys[i] = value;
	Pointers[i + 1] = Node;    // 注意是第i+1个指针而不是第i个指针
	Node->SetFather(this);      // 非常重要

	Count++;

	// 返回成功
	return true;
}

// 在中间结点中删除键，以及该键后的指针
bool CInternalNode::Delete(KEY_TYPE key)
{
	int i;
	for (i = 0; (key >= Keys[i]) && (i < Count); i++)
	{
	}

	int j;
	for (j = i - 1; j < Count - 1; j++)
	{
		Keys[j] = Keys[j + 1];
	}
	Keys[j] = INVALID;

	int k;
	for (k = i; k < Count; k++)
	{
		Pointers[k] = Pointers[k + 1];
	}
	Pointers[k] = NULL;

	Count--;

	return true;
}

/* 分裂中间结点
\
*/
KEY_TYPE CInternalNode::Split(CInternalNode* Node, KEY_TYPE key)
{
	int i = 0, j = 0;

	// 如果要插入的键值在第V和V+1个键值中间，需要翻转一下，因此先处理此情况
	if ((key > this->GetElement(V)) && (key < this->GetElement(V + 1)))
	{
		// 把第V+1 -- 2V个键移到指定的结点中

		for (i = V + 1; i <= MAXNUM_KEY; i++)
		{
			j++;
			Node->SetElement(j, this->GetElement(i));
			this->SetElement(i, INVALID);
		}

		// 把第V+2 -- 2V+1个指针移到指定的结点中
		j = 0;
		for (i = V + 2; i <= MAXNUM_POINTER; i++)
		{
			j++;
			this->GetPointer(i)->SetFather(Node);    // 重新设置子结点的父亲
			Node->SetPointer(j, this->GetPointer(i));
			this->SetPointer(i, INVALID);
		}

		// 设置好Count个数
		this->SetCount(V);
		Node->SetCount(V);

		// 把原键值返回
		return key;
	}

	// 以下处理key小于第V个键值或key大于第V+1个键值的情况

	// 判断是提取第V还是V+1个键
	int position = 0;
	if (key < this->GetElement(V))
	{
		position = V;
	}
	else
	{
		position = V + 1;
	}

	// 把第position个键提出来，作为新的键值返回
	KEY_TYPE RetKey = this->GetElement(position);

	// 把第position+1 -- 2V个键移到指定的结点中
	j = 0;
	for (i = position + 1; i <= MAXNUM_KEY; i++)
	{
		j++;
		Node->SetElement(j, this->GetElement(i));
		this->SetElement(i, INVALID);
	}

	// 把第position+1 -- 2V+1个指针移到指定的结点中(注意指针比键多一个)
	j = 0;
	for (i = position + 1; i <= MAXNUM_POINTER; i++)
	{
		j++;
		this->GetPointer(i)->SetFather(Node);    // 重新设置子结点的父亲
		Node->SetPointer(j, this->GetPointer(i));
		this->SetPointer(i, INVALID);
	}

	// 清除提取出的位置
	this->SetElement(position, INVALID);

	// 设置好Count个数
	this->SetCount(position - 1);
	Node->SetCount(MAXNUM_KEY - position);


	return RetKey;
}

// 结合结点，把指定中间结点的数据全部剪切到本中间结点
bool CInternalNode::Combine(CNode* Node)
{
	// 参数检查
	if (this->GetCount() + Node->GetCount() + 1> MAXNUM_DATA)    // 预留一个新键的位置
	{
		return false;
	}

	// 取待合并结点的第一个孩子的第一个元素作为新键值
	KEY_TYPE NewKey = Node->GetPointer(1)->GetElement(1);

	Keys[Count] = NewKey;
	Count++;
	Pointers[Count] = Node->GetPointer(1);

	for (int i = 1; i <= Node->GetCount(); i++)
	{
		Keys[Count] = Node->GetElement(i);
		Count++;
		Pointers[Count] = Node->GetPointer(i+1);
	}

	return true;
}

// 从另一结点移一个元素到本结点
bool CInternalNode::MoveOneElement(CNode* Node)
{
	// 参数检查
	if (this->GetCount() >= MAXNUM_DATA)
	{
		return false;
	}

	int i,j;


	// 兄弟结点在本结点左边
	if (Node->GetElement(1) < this->GetElement(1))
	{
		// 先腾出位置
		for (i = Count; i > 0; i--)
		{
			Keys[i] = Keys[i -1];
		}
		for (j = Count + 1; j > 0; j--)
		{
			Pointers[j] = Pointers[j -1];
		}

		// 赋值
		// 第一个键值不是兄弟结点的最后一个键值，而是本结点第一个子结点的第一个元素的值
		Keys[0] = GetPointer(1)->GetElement(1);
		// 第一个子结点为兄弟结点的最后一个子结点
		Pointers[0] = Node->GetPointer(Node->GetCount() + 1);

		// 修改兄弟结点
		Node->SetElement(Node->GetCount(), INVALID);
		Node->SetPointer(Node->GetCount() + 1, INVALID);
	}
	else    // 兄弟结点在本结点右边
	{
		// 赋值
		// 最后一个键值不是兄弟结点的第一个键值，而是兄弟结点第一个子结点的第一个元素的值
		Keys[Count] = Node->GetPointer(1)->GetElement(1);
		// 最后一个子结点为兄弟结点的第一个子结点
		Pointers[Count + 1] = Node->GetPointer(1);

		// 修改兄弟结点
		for (i = 1; i < Node->GetCount() - 1; i++)
		{
			Node->SetElement(i, Node->GetElement(i + 1));
		}
		for (j = 1; j < Node->GetCount(); j++)
		{
			Node->SetPointer(j, Node->GetPointer(j + 1));
		}
	}

	// 设置数目
	this->SetCount(this->GetCount() + 1);
	Node->SetCount(Node->GetCount() - 1);

	return true;
}

// 清除叶子结点中的数据
CLeafNode::CLeafNode()
{
	Type = NODE_TYPE_LEAF;

	for (int i = 0; i < MAXNUM_DATA; i++)
	{
		m_Datas[i] = INVALID;
	}

	pPrevNode = NULL;
	pNextNode = NULL;
}
CLeafNode::~CLeafNode()
{

}

// 在叶子结点中插入数据
bool CLeafNode::Insert(KEY_TYPE value)
{
	// 如果叶子结点已满，直接返回失败
	if (GetCount() >= MAXNUM_DATA)
	{
		return false;
	}

	// 找到要插入数据的位置
	int i;
	for (i = 0; (value > m_Datas[i]) && ( i < Count); i++)
	{
	}

	// 当前位置及其后面的数据依次后移，空出当前位置
	int j;
	for (j = Count; j > i; j--)
	{
		m_Datas[j] = m_Datas[j - 1];
	}

	// 把数据存入当前位置
	m_Datas[i] = value;

	Count++;

	// 返回成功
	return true;
}

bool CLeafNode::Delete(KEY_TYPE value)
{
	bool found = false;
	int i;
	for (i = 0; i < Count; i++)
	{
		if (value == m_Datas[i])
		{
			found = true;
			break;
		}
	}
	// 如果没有找到，返回失败
	if (false == found)
	{
		return false;
	}

	// 后面的数据依次前移
	int j;
	for (j = i; j < Count - 1; j++)
	{
		m_Datas[j] = m_Datas[j + 1];
	}

	m_Datas[j] = INVALID;
	Count--;

	// 返回成功
	return true;

}

// 分裂叶子结点，把本叶子结点的后一半数据剪切到指定的叶子结点中
KEY_TYPE CLeafNode::Split(CNode* Node)
{
	// 把本叶子结点的后一半数据移到指定的结点中
	int j = 0;
	for (int i = V + 1; i <= MAXNUM_DATA; i++)
	{
		j++;
		Node->SetElement(j, this->GetElement(i));
		this->SetElement(i, INVALID);
	}
	// 设置好Count个数
	this->SetCount(this->GetCount() - j);
	Node->SetCount(Node->GetCount() + j);

	// 返回新结点的第一个元素作为键
	return Node->GetElement(1);
}

// 结合结点，把指定叶子结点的数据全部剪切到本叶子结点
bool CLeafNode::Combine(CNode* Node)
{
	// 参数检查
	if (this->GetCount() + Node->GetCount() > MAXNUM_DATA)
	{
		return false;
	}

	for (int i = 1; i <= Node->GetCount(); i++)
	{
		this->Insert(Node->GetElement(i));
	}

	return true;
}
//构造函数
BPlusTree::BPlusTree()
{
	Depth = 0;
	Root = NULL;
	pLeafHead = NULL;
	pLeafTail = NULL;
}
BPlusTree::~BPlusTree()
{
	ClearTree();
}

// 在B+树中查找数据

bool BPlusTree::Search(KEY_TYPE data, char* sPath)
{
	int i = 0;
	int offset = 0;
	if (NULL != sPath)
	{
		(void)sprintf(sPath+offset, "搜索的节点路径为 ");
		offset+=17;
	}

	CNode * Node = GetRoot();
	// 循环查找对应的叶子结点
	while (NULL != Node)
	{        
		// 结点为叶子结点，循环结束
		if (NODE_TYPE_LEAF == Node->GetType())
		{
			break;
		}

		// 找到第一个键值大于等于key的位置
		for (i = 1; (data >= Node->GetElement(i) )&& (i <= Node->GetCount()); i++)
		{
		}

		if (NULL != sPath)
		{
			(void)sprintf(sPath+offset, " %3d -->", Node->GetElement(1));
			offset+=8;
		}

		Node = Node->GetPointer(i);
	}

	// 没找到
	if (NULL == Node)
	{
		return false;
	}

	if (NULL != sPath)
	{
		(void)sprintf(sPath+offset, "%3d", Node->GetElement(1));
		offset+=3;
	}

	// 在叶子结点中继续找
	bool found = false;
	for (i = 1; (i <= Node->GetCount()); i++)
	{
		if (data == Node->GetElement(i))
		{
			found = true;
		}
	}


	if (NULL != sPath)
	{
		if (true == found)
		{

			(void)sprintf(sPath+offset," --> %3d",data);
			//cout<<sPath+offset<<" -->"<<data<<" 。"<<endl;
		}
		else
		{
			(void)sprintf(sPath+offset, " 没有找到 ");
		}
	}

	return found;
}

/*
bool BPlusTree::Search(KEY_TYPE data, char* sPath)
{
	int i = 0;
	int offset = 0;
	if (NULL != sPath)
	{
		(void)sprintf(sPath+offset, "The search path is:");
		//cout<<sPath+offset<<"The search path is:"<<endl;
		offset+=19;
	}

	CNode * pNode = GetRoot();
	// 循环查找对应的叶子结点
	while (NULL != pNode)
	{         
		// 结点为叶子结点，循环结束
		if (NODE_TYPE_LEAF == pNode->GetType())
		{
			break;
		}

		// 找到第一个键值大于等于key的位置
		for (i = 1; (data >= pNode->GetElement(i) )&& (i <= pNode->GetCount()); i++)
		{
		}

		if (NULL != sPath)
		{
			(void)sprintf(sPath+offset, " %3d -->", pNode->GetElement(1));
			//cout<<sPath+offset<<"  --> "<<pNode->GetElement(1)<<endl;
			offset+=8;
		}

		pNode = pNode->GetPointer(i);
	}

	// 没找到
	if (NULL == pNode)
	{
		return false;
	}

	if (NULL != sPath)
	{
		(void)sprintf(sPath+offset, "%3d", pNode->GetElement(1));
		offset+=3;
	}

	// 在叶子结点中继续找
	bool found = false;
	for (i = 1; (i <= pNode->GetCount()); i++)
	{
		if (data == pNode->GetElement(i))
		{
			found = true;
		}
	}


	if (NULL != sPath)
	{
		if (true == found)
		{

			(void)sprintf(sPath+offset, " ,successed.\n");
		}
		else
		{
			(void)sprintf(sPath+offset, " ,failed.");
		}
	}

	return found;
}
*/
/* 在B+树中插入数据

*/
bool BPlusTree::Insert(KEY_TYPE data)
{
	// 检查是否重复插入
	bool found = Search(data, NULL);
	if (true == found)
	{//已存在返回false
		return false;
	}


	// 查找理想的叶子结点
	CLeafNode* pOldNode = SearchLeafNode(data);
	// 如果没有找到，说明整个树是空的，生成根结点
	if (NULL == pOldNode)
	{
		pOldNode = new CLeafNode;
		pLeafHead = pOldNode;   
		pLeafTail = pOldNode;
		SetRoot(pOldNode);
	}

	// 叶子结点未满，对应情况1，直接插入
	if (pOldNode->GetCount() < MAXNUM_DATA)
	{
		return pOldNode->Insert(data);
	}

	// 原叶子结点已满，新建叶子结点，并把原结点后一半数据剪切到新结点
	CLeafNode* pNewNode = new CLeafNode;
	KEY_TYPE key = INVALID;
	key = pOldNode->Split(pNewNode);

	// 在双向链表中插入结点
	CLeafNode* pOldNext = pOldNode->pNextNode;
	pOldNode->pNextNode = pNewNode;
	pNewNode->pNextNode = pOldNext;
	pNewNode->pPrevNode = pOldNode;
	if (NULL == pOldNext)
	{
		pLeafTail = pNewNode;
	}
	else
	{
		pOldNext->pPrevNode = pNewNode;
	}


	// 判断是插入到原结点还是新结点中，确保是按数据值排序的
	if (data < key)
	{
		pOldNode->Insert(data);    // 插入原结点
	}
	else
	{
		pNewNode->Insert(data);    // 插入新结点
	}

	// 父结点
	CInternalNode* pFather = (CInternalNode*)(pOldNode->GetFather());

	// 如果原结点是根节点，对应情况2
	if (NULL == pFather)
	{
		CNode* Node1 = new CInternalNode;
		Node1->SetPointer(1, pOldNode);                           // 指针1指向原结点
		Node1->SetElement(1, key);                                // 设置键
		Node1->SetPointer(2, pNewNode);                           // 指针2指向新结点
		pOldNode->SetFather(Node1);                               // 指定父结点
		pNewNode->SetFather(Node1);                               // 指定父结点
		Node1->SetCount(1);

		SetRoot(Node1);                                           // 指定新的根结点
		return true;
	}

	// 情况3和情况4在这里实现
	bool ret = InsertInternalNode(pFather, key, pNewNode);
	return ret;
}

/* 删除某数据

*/
bool BPlusTree::Delete(KEY_TYPE data)
{
	// 查找理想的叶子结点
	CLeafNode* pOldNode = SearchLeafNode(data);
	// 如果没有找到，返回失败
	if (NULL == pOldNode)
	{
		return false;
	}

	// 删除数据，如果失败一定是没有找到，直接返回失败
	bool success = pOldNode->Delete(data);
	if (false == success)
	{
		return false;
	}

	// 获取父结点
	CInternalNode* pFather = (CInternalNode*)(pOldNode->GetFather());
	if (NULL == pFather)
	{
		// 如果一个数据都没有了，删除根结点(只有根节点可能出现此情况)
		if (0 == pOldNode->GetCount())
		{
			delete pOldNode;
			pLeafHead = NULL;
			pLeafTail = NULL;
			SetRoot(NULL);
		}

		return true;
	}


	// 删除后叶子结点填充度仍>=50%，对应情况1
	if (pOldNode->GetCount() >= V)
	{
		for (int i = 1; (data >= pFather->GetElement(i)) && (i <= pFather->GetCount()); i++)
		{
			// 如果删除的是父结点的键值，需要更改该键
			if (pFather->GetElement(i) == data)
			{
				pFather->SetElement(i, pOldNode->GetElement(1));    // 更改为叶子结点新的第一个元素
			}
		}

		return true;
	}

	// 找到一个最近的兄弟结点(根据B+树的定义，除了叶子结点，总是能找到的)
	int flag = FLAG_LEFT;
	CLeafNode* pBrother = (CLeafNode*)(pOldNode->GetBrother(flag));

	// 兄弟结点填充度>50%，对应情况2A
	KEY_TYPE NewData = INVALID;
	if (pBrother->GetCount() > V)
	{
		if (FLAG_LEFT == flag)    // 兄弟在左边，移最后一个数据过来
		{
			NewData = pBrother->GetElement(pBrother->GetCount());
		}
		else    // 兄弟在右边，移第一个数据过来
		{
			NewData = pBrother->GetElement(1);
		}

		pOldNode->Insert(NewData);
		pBrother->Delete(NewData);

		// 修改父结点的键值
		if (FLAG_LEFT == flag)
		{
			for (int i = 1; i <= pFather->GetCount() + 1; i++)
			{
				if (pFather->GetPointer(i) == pOldNode && i > 1)
				{
					pFather->SetElement(i - 1 , pOldNode->GetElement(1));    // 更改本结点对应的键
				}
			}
		}
		else
		{
			for (int i = 1; i <= pFather->GetCount() + 1; i++)
			{
				if (pFather->GetPointer(i) == pOldNode && i > 1)
				{
					pFather->SetElement(i - 1, pOldNode->GetElement(1));    // 更改本结点对应的键
				}
				if (pFather->GetPointer(i) == pBrother && i > 1)
				{
					pFather->SetElement(i - 1 , pBrother->GetElement(1));    // 更改兄弟结点对应的键
				}
			}
		}


		return true;
	}

	// 情况2B

	// 父结点中要删除的键
	KEY_TYPE NewKey = NULL;

	// 把本结点与兄弟结点合并，无论如何合并到数据较小的结点，这样父结点就无需修改指针

	if (FLAG_LEFT == flag)
	{
		(void)pBrother->Combine(pOldNode);
		NewKey = pOldNode->GetElement(1);

		CLeafNode* pOldNext = pOldNode->pNextNode;
		pBrother->pNextNode = pOldNext;
		// 在双向链表中删除结点
		if (NULL == pOldNext)
		{
			pLeafTail = pBrother;
		}
		else
		{
			pOldNext->pPrevNode = pBrother;
		}
		// 删除本结点
		delete pOldNode;
	}
	else
	{
		(void)pOldNode->Combine(pBrother);
		NewKey = pBrother->GetElement(1);

		CLeafNode* pOldNext = pBrother->pNextNode;
		pOldNode->pNextNode = pOldNext;
		// 在双向链表中删除结点
		if (NULL == pOldNext)
		{
			pLeafTail = pOldNode;
		}
		else
		{
			pOldNext->pPrevNode = pOldNode;
		}
		// 删除本结点
		delete pBrother;
	}

	return DeleteInternalNode(pFather, NewKey);
}

// 清除整个树，删除所有结点
void BPlusTree::ClearTree()
{
	CNode* Node = GetRoot();
	if (NULL != Node)
	{
		Node->DeleteChildren();

		delete Node;
	}

	pLeafHead = NULL;
	pLeafTail = NULL;
	SetRoot(NULL);
}



bool BPlusTree::CheckNode(CNode* Node)
{
	if (NULL == Node)
	{
		return true;
	}

	int i = 0;
	bool ret = false;

	// 检查是否满足50%的填充度
	if ((Node->GetCount() < V) && (Node != GetRoot()))
	{
		return false;
	}

	// 检查键或数据是否按大小排序
	for (i = 1; i < Node->GetCount(); i++)
	{
		if (Node->GetElement(i) > Node->GetElement(i + 1))
		{
			return false;
		}
	}

	if (NODE_TYPE_LEAF == Node->GetType())
	{
		return true;
	}

	// 对中间结点，递归检查子树
	for (i = 1; i <= Node->GetCount() + 1; i++)
	{
		ret = CheckNode(Node->GetPointer(i));
		// 只要有一个不合法就返回不合法
		if (false == ret)
		{
			return false;
		}
	}

	return true;

}

// 打印整个树
void BPlusTree::PrintTree()
{
	CNode* pRoot = GetRoot();
	if (NULL == pRoot) {
		cout<<"树为空"<<endl;
		return;
	}
	CNode* p1, *p2, *p3;
	int i, j, k;
	int total = 0;

	cout<<"第一层"<<endl;
	PrintNode(pRoot);
	total = 0;
	cout<<"第二层"<<endl;
	for (i = 1; i <= MAXNUM_POINTER; i++)
	{
		p1 = pRoot->GetPointer(i);
		if (NULL == p1) return;
		PrintNode(p1);
		total++;
		//if (total%4 == 0) printf(" | ");
	}
	total = 0;
	cout<<"第三层"<<endl;
	for (i = 1; i <= MAXNUM_POINTER; i++)
	{
		p1 = pRoot->GetPointer(i);
		if (NULL == p1) continue;
		for (j = 1; j <= MAXNUM_POINTER; j++)
		{
			p2 = p1->GetPointer(j);
			if (NULL == p2) return;
			PrintNode(p2);
			total++;
			//if (total%4 == 0) printf(" | ");
		}
	}
	total = 0;
	cout<<"第四层"<<endl;
	for (i = 1; i <= MAXNUM_POINTER; i++)
	{
		p1 = pRoot->GetPointer(i);
		if (NULL == p1) continue;
		for (j = 1; j <= MAXNUM_POINTER; j++)
		{
			p2 = p1->GetPointer(j);
			if (NULL == p2) continue;
			for (k = 1; k <= MAXNUM_POINTER; k++)
			{
				p3 = p2->GetPointer(k);
				if (NULL == p3) return;
				PrintNode(p3);
				total++;
				//if (total%4 == 0) printf(" | ");
			}
		}
	}
}

// 打印某结点
void BPlusTree::PrintNode(CNode* Node)
{
	if (NULL == Node)
	{
		return;
	}

	for (int i = 1; i <= MAXNUM_KEY; i++)
	{
		if(Node->GetElement(i)!=0)
			cout<<" "<<Node->GetElement(i)<<" ";

		if (i >= MAXNUM_KEY)
		{
			//cout<<" | "<<endl;
			cout<<endl;
		}

	}
}

// 查找对应的叶子结点
CLeafNode* BPlusTree::SearchLeafNode(KEY_TYPE data)
{
	int i = 0;

	CNode * Node = GetRoot();
	// 循环查找对应的叶子结点
	while (NULL != Node)
	{        
		// 结点为叶子结点，循环结束
		if (NODE_TYPE_LEAF == Node->GetType())
		{
			break;
		}

		// 找到第一个键值大于等于key的位置
		for (i = 1; i <= Node->GetCount(); i++)
		{
			if (data < Node->GetElement(i))
			{
				break;
			}
		}

		Node = Node->GetPointer(i);
	}

	return (CLeafNode*)Node;
}

//递归函数：插入键到中间结点
bool BPlusTree::InsertInternalNode(CInternalNode* Node, KEY_TYPE key, CNode* pRightSon)
{
	if (NULL == Node || NODE_TYPE_LEAF ==Node->GetType())
	{
		return false;
	}

	// 结点未满，直接插入
	if (Node->GetCount() < MAXNUM_KEY)
	{
		return Node->Insert(key, pRightSon);
	}

	CInternalNode* pBrother = new CInternalNode;
	KEY_TYPE NewKey = INVALID;
	// 分裂本结点
	NewKey = Node->Split(pBrother, key);

	if (Node->GetCount() < pBrother->GetCount())
	{
		Node->Insert(key, pRightSon);
	}
	else if (Node->GetCount() > pBrother->GetCount())
	{
		pBrother->Insert(key, pRightSon);
	}
	else    // 两者相等，即键值在第V和V+1个键值中间的情况，把字节点挂到新结点的第一个指针上
	{
		pBrother->SetPointer(1,pRightSon);
		pRightSon->SetFather(pBrother);
	}

	CInternalNode* pFather = (CInternalNode*)(Node->GetFather());
	// 直到根结点都满了，新生成根结点
	if (NULL == pFather)
	{
		pFather = new CInternalNode;
		pFather->SetPointer(1, Node);                           // 指针1指向原结点
		pFather->SetElement(1, NewKey);                          // 设置键
		pFather->SetPointer(2, pBrother);                        // 指针2指向新结点
		Node->SetFather(pFather);                               // 指定父结点
		pBrother->SetFather(pFather);                            // 指定父结点
		pFather->SetCount(1);

		SetRoot(pFather);                                        // 指定新的根结点
		return true;
	}

	// 递归
	return InsertInternalNode(pFather, NewKey, pBrother);
}

// 递归函数：在中间结点中删除键
bool BPlusTree::DeleteInternalNode(CInternalNode* Node, KEY_TYPE key)
{
	// 删除键，如果失败一定是没有找到，直接返回失败
	bool success = Node->Delete(key);
	if (false == success)
	{
		return false;
	}

	// 获取父结点
	CInternalNode* pFather = (CInternalNode*)(Node->GetFather());
	if (NULL == pFather)
	{
		// 如果一个数据都没有了，把根结点的第一个结点作为根结点
		if (0 == Node->GetCount())
		{
			SetRoot(Node->GetPointer(1));
			delete Node;
		}

		return true;
	}

	// 删除后结点填充度仍>=50%
	if (Node->GetCount() >= V)
	{
		for (int i = 1; (key >= pFather->GetElement(i)) && (i <= pFather->GetCount()); i++)
		{
			// 如果删除的是父结点的键值，需要更改该键
			if (pFather->GetElement(i) == key)
			{
				pFather->SetElement(i, Node->GetElement(1));    // 更改为叶子结点新的第一个元素
			}
		}

		return true;
	}

	// 找到一个最近的兄弟结点(根据B+树的定义，除了根结点，总是能找到的)
	int flag = FLAG_LEFT;
	CInternalNode* pBrother = (CInternalNode*)(Node->GetBrother(flag));

	// 兄弟结点填充度>50%
	KEY_TYPE NewData = INVALID;
	if (pBrother->GetCount() > V)
	{
		Node->MoveOneElement(pBrother);

		// 修改父结点的键值
		if (FLAG_LEFT == flag)
		{
			for (int i = 1; i <= pFather->GetCount() + 1; i++)
			{
				if (pFather->GetPointer(i) == Node && i > 1)
				{
					pFather->SetElement(i - 1 , Node->GetElement(1));    // 更改本结点对应的键
				}
			}
		}
		else
		{
			for (int i = 1; i <= pFather->GetCount() + 1; i++)
			{
				if (pFather->GetPointer(i) == Node && i > 1)
				{
					pFather->SetElement(i - 1, Node->GetElement(1));    // 更改本结点对应的键
				}
				if (pFather->GetPointer(i) == pBrother && i > 1)
				{
					pFather->SetElement(i - 1 , pBrother->GetElement(1));    // 更改兄弟结点对应的键
				}
			}
		}

		return true;
	}

	// 父结点中要删除的键
	KEY_TYPE NewKey = NULL;

	// 把本结点与兄弟结点合并，无论如何合并到数据较小的结点，这样父结点就无需修改指针
	if (FLAG_LEFT == flag)
	{
		(void)pBrother->Combine(Node);
		NewKey = Node->GetElement(1);
		delete Node;
	}
	else
	{
		(void)Node->Combine(pBrother);
		NewKey = pBrother->GetElement(1);
		delete pBrother;
	}

	// 递归
	return DeleteInternalNode(pFather, NewKey);
}