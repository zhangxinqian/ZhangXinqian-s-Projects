

#define V 2    /* 内部结点中键的最小数目 */

#define MAXNUM_KEY (V * 2)    /* 内部结点中最多键个数，为2v ,阶数*/
#define MAXNUM_POINTER (MAXNUM_KEY + 1)    /* 内部结点中最多指向子树的指针个数，为2v */
#define MAXNUM_DATA (V * 2)    /* 叶子结点中最多数据个数，为2v */

/* 键值的类型*/
typedef int KEY_TYPE;    /* 为简单起见，定义为int类型，实际的B+树键值类型应该是可配的 */
/*备注： 为简单起见，叶子结点的数据也只存储键值*/

/* 结点类型 */
enum NODE_TYPE
{
NODE_TYPE_ROOT     = 1,    // 根结点
NODE_TYPE_INTERNAL = 2,    // 内部结点
NODE_TYPE_LEAF     = 3,    // 叶子结点
};

#define NULL 0
#define INVALID 0

#define FLAG_LEFT 1
#define FLAG_RIGHT 2

/* 结点数据结构，为内部结点和叶子结点的父类 */
class CNode
{
public:

    CNode();
    virtual ~CNode();
   
    
    NODE_TYPE GetType() { return Type; }//获取结点类型
    void SetType(NODE_TYPE type) {Type = type;}//设置结点类型

   
    int GetCount() { return Count;} // 获取有效数据个数
    void SetCount(int i) { Count = i; } // 设置有效数据个数

    
    virtual KEY_TYPE GetElement(int i) {return -1;}//获取某个元素，对中间节点指键，叶子节点指数据
    virtual void SetElement(int i, KEY_TYPE value) { }//设置某个元素，对中间节点指键，叶子节点指数据
   
  
    virtual CNode* GetPointer(int i) {return NULL;}//获取某个指针，对中间结点指指针，对叶子结点无意义
    virtual void SetPointer(int i, CNode* pointer) { }// 设置某个指针，对中间结点指指针，对叶子结点无意义

   
    CNode* GetFather() { return pFather;}//获取父节点
    void SetFather(CNode* father) { pFather = father; }//设置父节点

  
    CNode* GetBrother(int& flag);  // 获取一个最近的兄弟结点

    
    void DeleteChildren();// 删除结点

protected:
   
    NODE_TYPE Type;    // 结点类型，取值为NODE_TYPE类型

    int Count;    // 有效数据个数，对中间结点指键个数，对叶子结点指数据个数

    CNode* pFather;     // 指向父结点的指针，标准B+树中并没有该指针，加上是为了更快地实现结点分裂和旋转等操作

};

/* 内部结点数据结构 */
class CInternalNode : public CNode
{
public:

    CInternalNode();
    virtual ~CInternalNode();

    // 获取键值，对用户来说，数字从1开始
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
	// 设置键值，对用户来说，数字从1开始
    void SetElement(int i, KEY_TYPE key)
    {
        if ((i > 0 ) && (i <= MAXNUM_KEY))
        {
            Keys[i - 1] = key;
        }
    }

    // 获取指针，对用户来说，数字从1开始
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
	//设置指针，对用户来说，数字从1开始
    void SetPointer(int i, CNode* pointer)
    {
        if ((i > 0 ) && (i <= MAXNUM_POINTER))
        {
            Pointers[i - 1] = pointer;
        }
    }

    // 插入键
    bool Insert(KEY_TYPE value, CNode* Node);
    // 删除键
    bool Delete(KEY_TYPE value);

    // 分裂结点
    KEY_TYPE Split(CInternalNode* Node, KEY_TYPE key);
    // 结合结点
    bool Combine(CNode* Node);
    // 从另一结点移一个元素到本结点
    bool MoveOneElement(CNode* Node);

protected:

    KEY_TYPE Keys[MAXNUM_KEY];           // 键数组
    CNode* Pointers[MAXNUM_POINTER];     // 指针数组

};

/* 叶子结点数据结构 */
class CLeafNode : public CNode
{
public:

    CLeafNode();
    virtual ~CLeafNode();

    // 获取和设置数据
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

    // 获取和设置指针，对叶子结点无意义，只是实行父类的虚函数
    CNode* GetPointer(int i)
    {
        return NULL;
    }
   
    // 插入数据
    bool Insert(KEY_TYPE value);
    // 删除数据
    bool Delete(KEY_TYPE value);

    // 分裂结点
    KEY_TYPE Split(CNode* Node);
    // 结合结点
    bool Combine(CNode* Node);

public:
    // 以下两个变量用于实现双向链表
    CLeafNode* pPrevNode;                 // 前一个结点
    CLeafNode* pNextNode;                 // 后一个结点
   
protected:

    KEY_TYPE m_Datas[MAXNUM_DATA];    // 数据数组

};

/* B+树数据结构 */
class BPlusTree
{
public:
  
    BPlusTree();
    virtual ~BPlusTree();

    // 查找指定的数据
    bool Search(KEY_TYPE data, char* sPath);
    // 插入指定的数据
    bool Insert(KEY_TYPE data);
    // 删除指定的数据
    bool Delete(KEY_TYPE data);

    // 清除树
    void ClearTree();

    // 打印树
    void PrintTree();

    

    
	//打印节点
    void PrintNode(CNode* Node);

    // 递归检查结点及其子树是否满足B+树的定义
    bool CheckNode(CNode* Node);

    // 获取根结点
    CNode* GetRoot()
    {
        return Root;
    }
	//设置根结点
    void SetRoot(CNode* root)
    {
        Root = root;
    }

    // 获取深度
    int GetDepth()
    {
        return Depth;
    }
	// 设置深度
    void SetDepth(int depth)
    {
        Depth = depth;
    }
   
    // 深度加一
    void IncDepth()
    {
        Depth = Depth + 1;
    }

    // 深度减一
    void DecDepth()
    {
        if (Depth > 0)
        {
            Depth = Depth - 1;
        }
    }

public:
    // 以下两个变量用于实现双向链表
    CLeafNode* pLeafHead;                 // 头结点
    CLeafNode* pLeafTail;                   // 尾结点

protected:

    // 为插入而查找叶子结点
    CLeafNode* SearchLeafNode(KEY_TYPE data);
    //插入键到中间结点
    bool InsertInternalNode(CInternalNode* Node, KEY_TYPE key, CNode* pRightSon);
    // 在中间结点中删除键
    bool DeleteInternalNode(CInternalNode* Node, KEY_TYPE key);
   
    CNode* Root;    // 根结点
    int Depth;    // 树的深度
};




