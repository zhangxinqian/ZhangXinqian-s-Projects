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

// ��ȡһ��������ֵܽ��
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
		// �ҵ�������λ��
		if (pFather->GetPointer(i) == this)
		{
			if (i == (pFather->GetCount() + 1))
			{
				pBrother = pFather->GetPointer(i - 1);    // ���������һ��ָ�룬ֻ����ǰһ��ָ��
				flag = FLAG_LEFT;
			}
			else
			{
				pBrother = pFather->GetPointer(i + 1);    // �����Һ�һ��ָ��
				flag = FLAG_RIGHT;
			}
		}
	}

	return pBrother;
}

// ɾ���ӽ��
void CNode::DeleteChildren()
{
	for (int i = 1; i <= GetCount(); i++)
	{
		CNode * Node = GetPointer(i);
		if (NULL != Node)    // Ҷ�ӽ��û��ָ��
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

// ���м����в����
bool CInternalNode::Insert(KEY_TYPE value, CNode* Node)
{
	// ����м���������ֱ�ӷ���ʧ��
	if (GetCount() >= MAXNUM_KEY)
	{
		return false;
	}

	int j = 0;

	// �ҵ�Ҫ�������λ��
	int i;
	for (i = 0; (value > Keys[i]) && ( i < Count); i++)
	{
	}

	// ��ǰλ�ü������ļ����κ��ƣ��ճ���ǰλ��
	for (j = Count; j > i; j--)
	{
		Keys[j] = Keys[j - 1];
	}

	// ��ǰλ�ü�������ָ�����κ���
	for (j = Count + 1; j > i + 1; j--)
	{
		Pointers[j] = Pointers[j - 1];
	}

	// �Ѽ���ָ����뵱ǰλ��
	Keys[i] = value;
	Pointers[i + 1] = Node;    // ע���ǵ�i+1��ָ������ǵ�i��ָ��
	Node->SetFather(this);      // �ǳ���Ҫ

	Count++;

	// ���سɹ�
	return true;
}

// ���м�����ɾ�������Լ��ü����ָ��
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

/* �����м���
\
*/
KEY_TYPE CInternalNode::Split(CInternalNode* Node, KEY_TYPE key)
{
	int i = 0, j = 0;

	// ���Ҫ����ļ�ֵ�ڵ�V��V+1����ֵ�м䣬��Ҫ��תһ�£�����ȴ�������
	if ((key > this->GetElement(V)) && (key < this->GetElement(V + 1)))
	{
		// �ѵ�V+1 -- 2V�����Ƶ�ָ���Ľ����

		for (i = V + 1; i <= MAXNUM_KEY; i++)
		{
			j++;
			Node->SetElement(j, this->GetElement(i));
			this->SetElement(i, INVALID);
		}

		// �ѵ�V+2 -- 2V+1��ָ���Ƶ�ָ���Ľ����
		j = 0;
		for (i = V + 2; i <= MAXNUM_POINTER; i++)
		{
			j++;
			this->GetPointer(i)->SetFather(Node);    // ���������ӽ��ĸ���
			Node->SetPointer(j, this->GetPointer(i));
			this->SetPointer(i, INVALID);
		}

		// ���ú�Count����
		this->SetCount(V);
		Node->SetCount(V);

		// ��ԭ��ֵ����
		return key;
	}

	// ���´���keyС�ڵ�V����ֵ��key���ڵ�V+1����ֵ�����

	// �ж�����ȡ��V����V+1����
	int position = 0;
	if (key < this->GetElement(V))
	{
		position = V;
	}
	else
	{
		position = V + 1;
	}

	// �ѵ�position�������������Ϊ�µļ�ֵ����
	KEY_TYPE RetKey = this->GetElement(position);

	// �ѵ�position+1 -- 2V�����Ƶ�ָ���Ľ����
	j = 0;
	for (i = position + 1; i <= MAXNUM_KEY; i++)
	{
		j++;
		Node->SetElement(j, this->GetElement(i));
		this->SetElement(i, INVALID);
	}

	// �ѵ�position+1 -- 2V+1��ָ���Ƶ�ָ���Ľ����(ע��ָ��ȼ���һ��)
	j = 0;
	for (i = position + 1; i <= MAXNUM_POINTER; i++)
	{
		j++;
		this->GetPointer(i)->SetFather(Node);    // ���������ӽ��ĸ���
		Node->SetPointer(j, this->GetPointer(i));
		this->SetPointer(i, INVALID);
	}

	// �����ȡ����λ��
	this->SetElement(position, INVALID);

	// ���ú�Count����
	this->SetCount(position - 1);
	Node->SetCount(MAXNUM_KEY - position);


	return RetKey;
}

// ��Ͻ�㣬��ָ���м��������ȫ�����е����м���
bool CInternalNode::Combine(CNode* Node)
{
	// �������
	if (this->GetCount() + Node->GetCount() + 1> MAXNUM_DATA)    // Ԥ��һ���¼���λ��
	{
		return false;
	}

	// ȡ���ϲ����ĵ�һ�����ӵĵ�һ��Ԫ����Ϊ�¼�ֵ
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

// ����һ�����һ��Ԫ�ص������
bool CInternalNode::MoveOneElement(CNode* Node)
{
	// �������
	if (this->GetCount() >= MAXNUM_DATA)
	{
		return false;
	}

	int i,j;


	// �ֵܽ���ڱ�������
	if (Node->GetElement(1) < this->GetElement(1))
	{
		// ���ڳ�λ��
		for (i = Count; i > 0; i--)
		{
			Keys[i] = Keys[i -1];
		}
		for (j = Count + 1; j > 0; j--)
		{
			Pointers[j] = Pointers[j -1];
		}

		// ��ֵ
		// ��һ����ֵ�����ֵܽ������һ����ֵ�����Ǳ�����һ���ӽ��ĵ�һ��Ԫ�ص�ֵ
		Keys[0] = GetPointer(1)->GetElement(1);
		// ��һ���ӽ��Ϊ�ֵܽ������һ���ӽ��
		Pointers[0] = Node->GetPointer(Node->GetCount() + 1);

		// �޸��ֵܽ��
		Node->SetElement(Node->GetCount(), INVALID);
		Node->SetPointer(Node->GetCount() + 1, INVALID);
	}
	else    // �ֵܽ���ڱ�����ұ�
	{
		// ��ֵ
		// ���һ����ֵ�����ֵܽ��ĵ�һ����ֵ�������ֵܽ���һ���ӽ��ĵ�һ��Ԫ�ص�ֵ
		Keys[Count] = Node->GetPointer(1)->GetElement(1);
		// ���һ���ӽ��Ϊ�ֵܽ��ĵ�һ���ӽ��
		Pointers[Count + 1] = Node->GetPointer(1);

		// �޸��ֵܽ��
		for (i = 1; i < Node->GetCount() - 1; i++)
		{
			Node->SetElement(i, Node->GetElement(i + 1));
		}
		for (j = 1; j < Node->GetCount(); j++)
		{
			Node->SetPointer(j, Node->GetPointer(j + 1));
		}
	}

	// ������Ŀ
	this->SetCount(this->GetCount() + 1);
	Node->SetCount(Node->GetCount() - 1);

	return true;
}

// ���Ҷ�ӽ���е�����
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

// ��Ҷ�ӽ���в�������
bool CLeafNode::Insert(KEY_TYPE value)
{
	// ���Ҷ�ӽ��������ֱ�ӷ���ʧ��
	if (GetCount() >= MAXNUM_DATA)
	{
		return false;
	}

	// �ҵ�Ҫ�������ݵ�λ��
	int i;
	for (i = 0; (value > m_Datas[i]) && ( i < Count); i++)
	{
	}

	// ��ǰλ�ü��������������κ��ƣ��ճ���ǰλ��
	int j;
	for (j = Count; j > i; j--)
	{
		m_Datas[j] = m_Datas[j - 1];
	}

	// �����ݴ��뵱ǰλ��
	m_Datas[i] = value;

	Count++;

	// ���سɹ�
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
	// ���û���ҵ�������ʧ��
	if (false == found)
	{
		return false;
	}

	// �������������ǰ��
	int j;
	for (j = i; j < Count - 1; j++)
	{
		m_Datas[j] = m_Datas[j + 1];
	}

	m_Datas[j] = INVALID;
	Count--;

	// ���سɹ�
	return true;

}

// ����Ҷ�ӽ�㣬�ѱ�Ҷ�ӽ��ĺ�һ�����ݼ��е�ָ����Ҷ�ӽ����
KEY_TYPE CLeafNode::Split(CNode* Node)
{
	// �ѱ�Ҷ�ӽ��ĺ�һ�������Ƶ�ָ���Ľ����
	int j = 0;
	for (int i = V + 1; i <= MAXNUM_DATA; i++)
	{
		j++;
		Node->SetElement(j, this->GetElement(i));
		this->SetElement(i, INVALID);
	}
	// ���ú�Count����
	this->SetCount(this->GetCount() - j);
	Node->SetCount(Node->GetCount() + j);

	// �����½��ĵ�һ��Ԫ����Ϊ��
	return Node->GetElement(1);
}

// ��Ͻ�㣬��ָ��Ҷ�ӽ�������ȫ�����е���Ҷ�ӽ��
bool CLeafNode::Combine(CNode* Node)
{
	// �������
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
//���캯��
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

// ��B+���в�������

bool BPlusTree::Search(KEY_TYPE data, char* sPath)
{
	int i = 0;
	int offset = 0;
	if (NULL != sPath)
	{
		(void)sprintf(sPath+offset, "�����Ľڵ�·��Ϊ ");
		offset+=17;
	}

	CNode * Node = GetRoot();
	// ѭ�����Ҷ�Ӧ��Ҷ�ӽ��
	while (NULL != Node)
	{        
		// ���ΪҶ�ӽ�㣬ѭ������
		if (NODE_TYPE_LEAF == Node->GetType())
		{
			break;
		}

		// �ҵ���һ����ֵ���ڵ���key��λ��
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

	// û�ҵ�
	if (NULL == Node)
	{
		return false;
	}

	if (NULL != sPath)
	{
		(void)sprintf(sPath+offset, "%3d", Node->GetElement(1));
		offset+=3;
	}

	// ��Ҷ�ӽ���м�����
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
			//cout<<sPath+offset<<" -->"<<data<<" ��"<<endl;
		}
		else
		{
			(void)sprintf(sPath+offset, " û���ҵ� ");
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
	// ѭ�����Ҷ�Ӧ��Ҷ�ӽ��
	while (NULL != pNode)
	{         
		// ���ΪҶ�ӽ�㣬ѭ������
		if (NODE_TYPE_LEAF == pNode->GetType())
		{
			break;
		}

		// �ҵ���һ����ֵ���ڵ���key��λ��
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

	// û�ҵ�
	if (NULL == pNode)
	{
		return false;
	}

	if (NULL != sPath)
	{
		(void)sprintf(sPath+offset, "%3d", pNode->GetElement(1));
		offset+=3;
	}

	// ��Ҷ�ӽ���м�����
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
/* ��B+���в�������

*/
bool BPlusTree::Insert(KEY_TYPE data)
{
	// ����Ƿ��ظ�����
	bool found = Search(data, NULL);
	if (true == found)
	{//�Ѵ��ڷ���false
		return false;
	}


	// ���������Ҷ�ӽ��
	CLeafNode* pOldNode = SearchLeafNode(data);
	// ���û���ҵ���˵���������ǿյģ����ɸ����
	if (NULL == pOldNode)
	{
		pOldNode = new CLeafNode;
		pLeafHead = pOldNode;   
		pLeafTail = pOldNode;
		SetRoot(pOldNode);
	}

	// Ҷ�ӽ��δ������Ӧ���1��ֱ�Ӳ���
	if (pOldNode->GetCount() < MAXNUM_DATA)
	{
		return pOldNode->Insert(data);
	}

	// ԭҶ�ӽ���������½�Ҷ�ӽ�㣬����ԭ����һ�����ݼ��е��½��
	CLeafNode* pNewNode = new CLeafNode;
	KEY_TYPE key = INVALID;
	key = pOldNode->Split(pNewNode);

	// ��˫�������в�����
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


	// �ж��ǲ��뵽ԭ��㻹���½���У�ȷ���ǰ�����ֵ�����
	if (data < key)
	{
		pOldNode->Insert(data);    // ����ԭ���
	}
	else
	{
		pNewNode->Insert(data);    // �����½��
	}

	// �����
	CInternalNode* pFather = (CInternalNode*)(pOldNode->GetFather());

	// ���ԭ����Ǹ��ڵ㣬��Ӧ���2
	if (NULL == pFather)
	{
		CNode* Node1 = new CInternalNode;
		Node1->SetPointer(1, pOldNode);                           // ָ��1ָ��ԭ���
		Node1->SetElement(1, key);                                // ���ü�
		Node1->SetPointer(2, pNewNode);                           // ָ��2ָ���½��
		pOldNode->SetFather(Node1);                               // ָ�������
		pNewNode->SetFather(Node1);                               // ָ�������
		Node1->SetCount(1);

		SetRoot(Node1);                                           // ָ���µĸ����
		return true;
	}

	// ���3�����4������ʵ��
	bool ret = InsertInternalNode(pFather, key, pNewNode);
	return ret;
}

/* ɾ��ĳ����

*/
bool BPlusTree::Delete(KEY_TYPE data)
{
	// ���������Ҷ�ӽ��
	CLeafNode* pOldNode = SearchLeafNode(data);
	// ���û���ҵ�������ʧ��
	if (NULL == pOldNode)
	{
		return false;
	}

	// ɾ�����ݣ����ʧ��һ����û���ҵ���ֱ�ӷ���ʧ��
	bool success = pOldNode->Delete(data);
	if (false == success)
	{
		return false;
	}

	// ��ȡ�����
	CInternalNode* pFather = (CInternalNode*)(pOldNode->GetFather());
	if (NULL == pFather)
	{
		// ���һ�����ݶ�û���ˣ�ɾ�������(ֻ�и��ڵ���ܳ��ִ����)
		if (0 == pOldNode->GetCount())
		{
			delete pOldNode;
			pLeafHead = NULL;
			pLeafTail = NULL;
			SetRoot(NULL);
		}

		return true;
	}


	// ɾ����Ҷ�ӽ��������>=50%����Ӧ���1
	if (pOldNode->GetCount() >= V)
	{
		for (int i = 1; (data >= pFather->GetElement(i)) && (i <= pFather->GetCount()); i++)
		{
			// ���ɾ�����Ǹ����ļ�ֵ����Ҫ���ĸü�
			if (pFather->GetElement(i) == data)
			{
				pFather->SetElement(i, pOldNode->GetElement(1));    // ����ΪҶ�ӽ���µĵ�һ��Ԫ��
			}
		}

		return true;
	}

	// �ҵ�һ��������ֵܽ��(����B+���Ķ��壬����Ҷ�ӽ�㣬�������ҵ���)
	int flag = FLAG_LEFT;
	CLeafNode* pBrother = (CLeafNode*)(pOldNode->GetBrother(flag));

	// �ֵܽ������>50%����Ӧ���2A
	KEY_TYPE NewData = INVALID;
	if (pBrother->GetCount() > V)
	{
		if (FLAG_LEFT == flag)    // �ֵ�����ߣ������һ�����ݹ���
		{
			NewData = pBrother->GetElement(pBrother->GetCount());
		}
		else    // �ֵ����ұߣ��Ƶ�һ�����ݹ���
		{
			NewData = pBrother->GetElement(1);
		}

		pOldNode->Insert(NewData);
		pBrother->Delete(NewData);

		// �޸ĸ����ļ�ֵ
		if (FLAG_LEFT == flag)
		{
			for (int i = 1; i <= pFather->GetCount() + 1; i++)
			{
				if (pFather->GetPointer(i) == pOldNode && i > 1)
				{
					pFather->SetElement(i - 1 , pOldNode->GetElement(1));    // ���ı�����Ӧ�ļ�
				}
			}
		}
		else
		{
			for (int i = 1; i <= pFather->GetCount() + 1; i++)
			{
				if (pFather->GetPointer(i) == pOldNode && i > 1)
				{
					pFather->SetElement(i - 1, pOldNode->GetElement(1));    // ���ı�����Ӧ�ļ�
				}
				if (pFather->GetPointer(i) == pBrother && i > 1)
				{
					pFather->SetElement(i - 1 , pBrother->GetElement(1));    // �����ֵܽ���Ӧ�ļ�
				}
			}
		}


		return true;
	}

	// ���2B

	// �������Ҫɾ���ļ�
	KEY_TYPE NewKey = NULL;

	// �ѱ�������ֵܽ��ϲ���������κϲ������ݽ�С�Ľ�㣬���������������޸�ָ��

	if (FLAG_LEFT == flag)
	{
		(void)pBrother->Combine(pOldNode);
		NewKey = pOldNode->GetElement(1);

		CLeafNode* pOldNext = pOldNode->pNextNode;
		pBrother->pNextNode = pOldNext;
		// ��˫��������ɾ�����
		if (NULL == pOldNext)
		{
			pLeafTail = pBrother;
		}
		else
		{
			pOldNext->pPrevNode = pBrother;
		}
		// ɾ�������
		delete pOldNode;
	}
	else
	{
		(void)pOldNode->Combine(pBrother);
		NewKey = pBrother->GetElement(1);

		CLeafNode* pOldNext = pBrother->pNextNode;
		pOldNode->pNextNode = pOldNext;
		// ��˫��������ɾ�����
		if (NULL == pOldNext)
		{
			pLeafTail = pOldNode;
		}
		else
		{
			pOldNext->pPrevNode = pOldNode;
		}
		// ɾ�������
		delete pBrother;
	}

	return DeleteInternalNode(pFather, NewKey);
}

// �����������ɾ�����н��
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

	// ����Ƿ�����50%������
	if ((Node->GetCount() < V) && (Node != GetRoot()))
	{
		return false;
	}

	// �����������Ƿ񰴴�С����
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

	// ���м��㣬�ݹ�������
	for (i = 1; i <= Node->GetCount() + 1; i++)
	{
		ret = CheckNode(Node->GetPointer(i));
		// ֻҪ��һ�����Ϸ��ͷ��ز��Ϸ�
		if (false == ret)
		{
			return false;
		}
	}

	return true;

}

// ��ӡ������
void BPlusTree::PrintTree()
{
	CNode* pRoot = GetRoot();
	if (NULL == pRoot) {
		cout<<"��Ϊ��"<<endl;
		return;
	}
	CNode* p1, *p2, *p3;
	int i, j, k;
	int total = 0;

	cout<<"��һ��"<<endl;
	PrintNode(pRoot);
	total = 0;
	cout<<"�ڶ���"<<endl;
	for (i = 1; i <= MAXNUM_POINTER; i++)
	{
		p1 = pRoot->GetPointer(i);
		if (NULL == p1) return;
		PrintNode(p1);
		total++;
		//if (total%4 == 0) printf(" | ");
	}
	total = 0;
	cout<<"������"<<endl;
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
	cout<<"���Ĳ�"<<endl;
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

// ��ӡĳ���
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

// ���Ҷ�Ӧ��Ҷ�ӽ��
CLeafNode* BPlusTree::SearchLeafNode(KEY_TYPE data)
{
	int i = 0;

	CNode * Node = GetRoot();
	// ѭ�����Ҷ�Ӧ��Ҷ�ӽ��
	while (NULL != Node)
	{        
		// ���ΪҶ�ӽ�㣬ѭ������
		if (NODE_TYPE_LEAF == Node->GetType())
		{
			break;
		}

		// �ҵ���һ����ֵ���ڵ���key��λ��
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

//�ݹ麯������������м���
bool BPlusTree::InsertInternalNode(CInternalNode* Node, KEY_TYPE key, CNode* pRightSon)
{
	if (NULL == Node || NODE_TYPE_LEAF ==Node->GetType())
	{
		return false;
	}

	// ���δ����ֱ�Ӳ���
	if (Node->GetCount() < MAXNUM_KEY)
	{
		return Node->Insert(key, pRightSon);
	}

	CInternalNode* pBrother = new CInternalNode;
	KEY_TYPE NewKey = INVALID;
	// ���ѱ����
	NewKey = Node->Split(pBrother, key);

	if (Node->GetCount() < pBrother->GetCount())
	{
		Node->Insert(key, pRightSon);
	}
	else if (Node->GetCount() > pBrother->GetCount())
	{
		pBrother->Insert(key, pRightSon);
	}
	else    // ������ȣ�����ֵ�ڵ�V��V+1����ֵ�м����������ֽڵ�ҵ��½��ĵ�һ��ָ����
	{
		pBrother->SetPointer(1,pRightSon);
		pRightSon->SetFather(pBrother);
	}

	CInternalNode* pFather = (CInternalNode*)(Node->GetFather());
	// ֱ������㶼���ˣ������ɸ����
	if (NULL == pFather)
	{
		pFather = new CInternalNode;
		pFather->SetPointer(1, Node);                           // ָ��1ָ��ԭ���
		pFather->SetElement(1, NewKey);                          // ���ü�
		pFather->SetPointer(2, pBrother);                        // ָ��2ָ���½��
		Node->SetFather(pFather);                               // ָ�������
		pBrother->SetFather(pFather);                            // ָ�������
		pFather->SetCount(1);

		SetRoot(pFather);                                        // ָ���µĸ����
		return true;
	}

	// �ݹ�
	return InsertInternalNode(pFather, NewKey, pBrother);
}

// �ݹ麯�������м�����ɾ����
bool BPlusTree::DeleteInternalNode(CInternalNode* Node, KEY_TYPE key)
{
	// ɾ���������ʧ��һ����û���ҵ���ֱ�ӷ���ʧ��
	bool success = Node->Delete(key);
	if (false == success)
	{
		return false;
	}

	// ��ȡ�����
	CInternalNode* pFather = (CInternalNode*)(Node->GetFather());
	if (NULL == pFather)
	{
		// ���һ�����ݶ�û���ˣ��Ѹ����ĵ�һ�������Ϊ�����
		if (0 == Node->GetCount())
		{
			SetRoot(Node->GetPointer(1));
			delete Node;
		}

		return true;
	}

	// ɾ������������>=50%
	if (Node->GetCount() >= V)
	{
		for (int i = 1; (key >= pFather->GetElement(i)) && (i <= pFather->GetCount()); i++)
		{
			// ���ɾ�����Ǹ����ļ�ֵ����Ҫ���ĸü�
			if (pFather->GetElement(i) == key)
			{
				pFather->SetElement(i, Node->GetElement(1));    // ����ΪҶ�ӽ���µĵ�һ��Ԫ��
			}
		}

		return true;
	}

	// �ҵ�һ��������ֵܽ��(����B+���Ķ��壬���˸���㣬�������ҵ���)
	int flag = FLAG_LEFT;
	CInternalNode* pBrother = (CInternalNode*)(Node->GetBrother(flag));

	// �ֵܽ������>50%
	KEY_TYPE NewData = INVALID;
	if (pBrother->GetCount() > V)
	{
		Node->MoveOneElement(pBrother);

		// �޸ĸ����ļ�ֵ
		if (FLAG_LEFT == flag)
		{
			for (int i = 1; i <= pFather->GetCount() + 1; i++)
			{
				if (pFather->GetPointer(i) == Node && i > 1)
				{
					pFather->SetElement(i - 1 , Node->GetElement(1));    // ���ı�����Ӧ�ļ�
				}
			}
		}
		else
		{
			for (int i = 1; i <= pFather->GetCount() + 1; i++)
			{
				if (pFather->GetPointer(i) == Node && i > 1)
				{
					pFather->SetElement(i - 1, Node->GetElement(1));    // ���ı�����Ӧ�ļ�
				}
				if (pFather->GetPointer(i) == pBrother && i > 1)
				{
					pFather->SetElement(i - 1 , pBrother->GetElement(1));    // �����ֵܽ���Ӧ�ļ�
				}
			}
		}

		return true;
	}

	// �������Ҫɾ���ļ�
	KEY_TYPE NewKey = NULL;

	// �ѱ�������ֵܽ��ϲ���������κϲ������ݽ�С�Ľ�㣬���������������޸�ָ��
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

	// �ݹ�
	return DeleteInternalNode(pFather, NewKey);
}