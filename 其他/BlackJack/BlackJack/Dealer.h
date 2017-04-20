#ifndef _DEALER_H
#define _DEALER_H

#include "Gambler.h"

class Dealer : public Gambler
{
public:
	Dealer(int _burstValue=21);
	virtual ~Dealer();

	//庄家是否要牌，点数<= burstValue-5 时庄家选择要牌，否则不要，平局手牌点数至少为 burstValue-4
	virtual bool isHitting() const;

	void flipFirstCard();        //初始两张牌的第一张牌翻为相反的一面
};

#endif