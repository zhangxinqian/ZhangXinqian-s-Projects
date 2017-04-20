

#define V 2    /* �ڲ�����м�����С��Ŀ */

#define MAXNUM_KEY (V * 2)    /* �ڲ����������������Ϊ2v ,����*/
#define MAXNUM_POINTER (MAXNUM_KEY + 1)    /* �ڲ���������ָ��������ָ�������Ϊ2v */
#define MAXNUM_DATA (V * 2)    /* Ҷ�ӽ����������ݸ�����Ϊ2v */

/* ��ֵ������*/
typedef int KEY_TYPE;    /* Ϊ�����������Ϊint���ͣ�ʵ�ʵ�B+����ֵ����Ӧ���ǿ���� */
/*��ע�� Ϊ�������Ҷ�ӽ�������Ҳֻ�洢��ֵ*/

/* ������� */
enum NODE_TYPE
{
NODE_TYPE_ROOT     = 1,    // �����
NODE_TYPE_INTERNAL = 2,    // �ڲ����
NODE_TYPE_LEAF     = 3,    // Ҷ�ӽ��
};

#define NULL 0
#define INVALID 0

#define FLAG_LEFT 1
#define FLAG_RIGHT 2

/* ������ݽṹ��Ϊ�ڲ�����Ҷ�ӽ��ĸ��� */
class CNode
{
public:

    CNode();
    virtual ~CNode();
   
    
    NODE_TYPE GetType() { return Type; }//��ȡ�������
    void SetType(NODE_TYPE type) {Type = type;}//���ý������

   
    int GetCount() { return Count;} // ��ȡ��Ч���ݸ���
    void SetCount(int i) { Count = i; } // ������Ч���ݸ���

    
    virtual KEY_TYPE GetElement(int i) {return -1;}//��ȡĳ��Ԫ�أ����м�ڵ�ָ����Ҷ�ӽڵ�ָ����
    virtual void SetElement(int i, KEY_TYPE value) { }//����ĳ��Ԫ�أ����м�ڵ�ָ����Ҷ�ӽڵ�ָ����
   
  
    virtual CNode* GetPointer(int i) {return NULL;}//��ȡĳ��ָ�룬���м���ָָ�룬��Ҷ�ӽ��������
    virtual void SetPointer(int i, CNode* pointer) { }// ����ĳ��ָ�룬���м���ָָ�룬��Ҷ�ӽ��������

   
    CNode* GetFather() { return pFather;}//��ȡ���ڵ�
    void SetFather(CNode* father) { pFather = father; }//���ø��ڵ�

  
    CNode* GetBrother(int& flag);  // ��ȡһ��������ֵܽ��

    
    void DeleteChildren();// ɾ�����

protected:
   
    NODE_TYPE Type;    // ������ͣ�ȡֵΪNODE_TYPE����

    int Count;    // ��Ч���ݸ��������м���ָ����������Ҷ�ӽ��ָ���ݸ���

    CNode* pFather;     // ָ�򸸽���ָ�룬��׼B+���в�û�и�ָ�룬������Ϊ�˸����ʵ�ֽ����Ѻ���ת�Ȳ���

};

/* �ڲ�������ݽṹ */
class CInternalNode : public CNode
{
public:

    CInternalNode();
    virtual ~CInternalNode();

    // ��ȡ��ֵ�����û���˵�����ִ�1��ʼ
    KEY_TYPE GetElement(int i)
    {
        if ((i > 0 ) && (i <= MAXNUM_KEY))
        {
            return Keys[i - 1];
        }
        else
        {
            return INVALID;
        }
    }
	// ���ü�ֵ�����û���˵�����ִ�1��ʼ
    void SetElement(int i, KEY_TYPE key)
    {
        if ((i > 0 ) && (i <= MAXNUM_KEY))
        {
            Keys[i - 1] = key;
        }
    }

    // ��ȡָ�룬���û���˵�����ִ�1��ʼ
    CNode* GetPointer(int i)
    {
        if ((i > 0 ) && (i <= MAXNUM_POINTER))
        {
            return Pointers[i - 1];
        }
        else
        {
            return NULL;
        }
    }
	//����ָ�룬���û���˵�����ִ�1��ʼ
    void SetPointer(int i, CNode* pointer)
    {
        if ((i > 0 ) && (i <= MAXNUM_POINTER))
        {
            Pointers[i - 1] = pointer;
        }
    }

    // �����
    bool Insert(KEY_TYPE value, CNode* Node);
    // ɾ����
    bool Delete(KEY_TYPE value);

    // ���ѽ��
    KEY_TYPE Split(CInternalNode* Node, KEY_TYPE key);
    // ��Ͻ��
    bool Combine(CNode* Node);
    // ����һ�����һ��Ԫ�ص������
    bool MoveOneElement(CNode* Node);

protected:

    KEY_TYPE Keys[MAXNUM_KEY];           // ������
    CNode* Pointers[MAXNUM_POINTER];     // ָ������

};

/* Ҷ�ӽ�����ݽṹ */
class CLeafNode : public CNode
{
public:

    CLeafNode();
    virtual ~CLeafNode();

    // ��ȡ����������
    KEY_TYPE GetElement(int i)
    {
        if ((i > 0 ) && (i <= MAXNUM_DATA))
        {
            return m_Datas[i - 1];
        }
        else
        {
            return INVALID;
        }
    }

    void SetElement(int i, KEY_TYPE data)
    {
        if ((i > 0 ) && (i <= MAXNUM_DATA))
        {
            m_Datas[i - 1] = data;
        }
    }

    // ��ȡ������ָ�룬��Ҷ�ӽ�������壬ֻ��ʵ�и�����麯��
    CNode* GetPointer(int i)
    {
        return NULL;
    }
   
    // ��������
    bool Insert(KEY_TYPE value);
    // ɾ������
    bool Delete(KEY_TYPE value);

    // ���ѽ��
    KEY_TYPE Split(CNode* Node);
    // ��Ͻ��
    bool Combine(CNode* Node);

public:
    // ����������������ʵ��˫������
    CLeafNode* pPrevNode;                 // ǰһ�����
    CLeafNode* pNextNode;                 // ��һ�����
   
protected:

    KEY_TYPE m_Datas[MAXNUM_DATA];    // ��������

};

/* B+�����ݽṹ */
class BPlusTree
{
public:
  
    BPlusTree();
    virtual ~BPlusTree();

    // ����ָ��������
    bool Search(KEY_TYPE data, char* sPath);
    // ����ָ��������
    bool Insert(KEY_TYPE data);
    // ɾ��ָ��������
    bool Delete(KEY_TYPE data);

    // �����
    void ClearTree();

    // ��ӡ��
    void PrintTree();

    

    
	//��ӡ�ڵ�
    void PrintNode(CNode* Node);

    // �ݹ����㼰�������Ƿ�����B+���Ķ���
    bool CheckNode(CNode* Node);

    // ��ȡ�����
    CNode* GetRoot()
    {
        return Root;
    }
	//���ø����
    void SetRoot(CNode* root)
    {
        Root = root;
    }

    // ��ȡ���
    int GetDepth()
    {
        return Depth;
    }
	// �������
    void SetDepth(int depth)
    {
        Depth = depth;
    }
   
    // ��ȼ�һ
    void IncDepth()
    {
        Depth = Depth + 1;
    }

    // ��ȼ�һ
    void DecDepth()
    {
        if (Depth > 0)
        {
            Depth = Depth - 1;
        }
    }

public:
    // ����������������ʵ��˫������
    CLeafNode* pLeafHead;                 // ͷ���
    CLeafNode* pLeafTail;                   // β���

protected:

    // Ϊ���������Ҷ�ӽ��
    CLeafNode* SearchLeafNode(KEY_TYPE data);
    //��������м���
    bool InsertInternalNode(CInternalNode* Node, KEY_TYPE key, CNode* pRightSon);
    // ���м�����ɾ����
    bool DeleteInternalNode(CInternalNode* Node, KEY_TYPE key);
   
    CNode* Root;    // �����
    int Depth;    // �������
};




