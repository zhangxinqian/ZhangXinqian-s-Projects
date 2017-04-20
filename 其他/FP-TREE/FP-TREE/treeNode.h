#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <list>
using namespace std;

template<typename T>
class TreeNode
{
public:
	TreeNode();
	virtual ~TreeNode();
	TreeNode(T item,int count = 0);

	T getItem() const;
	void setItem(const T item);

	int getCount() const;
	void setCount(const int count);

	TreeNode<T>* getParent() const;
	void setParent(TreeNode<T>* parent);

	list<TreeNode<T>*>* getChildren() const;
	void setChildren(list<TreeNode<T>*>* children);
	void addAChild(TreeNode<T>* child);
	TreeNode<T>* searchChild(const T item) const;

	TreeNode<T>* getNextHomonym() const;
	void setNextHomonym(TreeNode<T>* homonym);

	void increaseCount(int n);

	bool operator==(const TreeNode<T>&tn)
	{
		return ((this->item==tn.item)&&(this->count==tn.count));
	}

	bool operator<(const TreeNode<T>&tn)
	{
		return (this->count < tn.count);
	}
private:
	T item;
	int count;//频数，出现次数的计数
	TreeNode<T>* parent;
	list<TreeNode<T>*>* children;
	TreeNode<T>* nextHomonym;

};


template<typename T>
TreeNode<T>::TreeNode():count(0),parent(0),children(0),nextHomonym(0){};

template<typename T>
TreeNode<T>::TreeNode(T item,int count)
{
	this->item=item;
	this->count=count;
	this->parent=0;
	this->children=0;
	this->nextHomonym=0;
}

template<typename T>
TreeNode<T>::~TreeNode()
{
	if (children!=0) delete children;
};

template<typename T>
inline T TreeNode<T>::getItem() const
{
	return item;
}

template<typename T>
inline void TreeNode<T>::setItem(const T item)
{
	this->item=item;
}

template<typename T>
inline int TreeNode<T>::getCount() const
{
	return count;
}

template<typename T>
inline void TreeNode<T>::setCount(const int count)
{
	this->count=count;
}

template<typename T>
inline TreeNode<T>* TreeNode<T>::getParent() const
{
	return parent;
}

template<typename T>
inline void TreeNode<T>::setParent(TreeNode<T>* parent)
{
	this->parent=parent;
}

template<typename T>
inline list<TreeNode<T>*>* TreeNode<T>::getChildren() const
{
	return children;
}

template<typename T>
inline void TreeNode<T>::setChildren(list<TreeNode<T>*>* children)
{
	this->children=children;
}

template<typename T>
inline TreeNode<T>* TreeNode<T>::getNextHomonym() const
{
	return nextHomonym;
}

template<typename T>
inline void TreeNode<T>::setNextHomonym(TreeNode<T>* homonym)
{
	this->nextHomonym=homonym;
}

template<typename T>
inline void TreeNode<T>::increaseCount(int n)
{
	count+=n;
}

template<typename T>
void TreeNode<T>::addAChild(TreeNode<T>* child)
{
	if (children==0)
		children=new list<TreeNode<T>*>();
	children->push_back(child);
}

template<typename T>
TreeNode<T>* TreeNode<T>::searchChild(const T item) const
{
	list<TreeNode<T>*>* temp=children;
	if (temp != 0) 
	{
		list<TreeNode<T>*>::iterator itor;
		for (itor=temp->begin();itor!=temp->end();++itor)
			if ((*itor)->getItem()==item) return (*itor);
	}
	return 0;
}

#endif