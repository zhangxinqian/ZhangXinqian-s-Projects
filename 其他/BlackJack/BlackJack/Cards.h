#ifndef _CARDS_H
#define _CARDS_H

#include "Card.h"
#include <list>
using namespace std;

//表示几张牌的组合（牌组）的一个类
class Cards
{
public:
	Cards(){};
	virtual ~Cards();

	void getACard(Card* pCard);  //添加一张牌

	void clear();           //清除牌组中的牌

	int getCardNumber() const;

	bool isEmpty() const;

	virtual int getTotalValues() const;   //获得牌组中所有牌点数的总和

	list<Card*>* getCards() const;

protected:
	list<Card*> cards;   //包含Card对象指针的链表
};

#endif