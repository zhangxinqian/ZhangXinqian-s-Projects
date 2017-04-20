#ifndef _CARDS_H
#define _CARDS_H

#include "Card.h"
#include <list>
using namespace std;

//��ʾ�����Ƶ���ϣ����飩��һ����
class Cards
{
public:
	Cards(){};
	virtual ~Cards();

	void getACard(Card* pCard);  //���һ����

	void clear();           //��������е���

	int getCardNumber() const;

	bool isEmpty() const;

	virtual int getTotalValues() const;   //��������������Ƶ������ܺ�

	list<Card*>* getCards() const;

protected:
	list<Card*> cards;   //����Card����ָ�������
};

#endif