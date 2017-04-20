#include "Dealer.h"

Dealer::Dealer(int _burstValue):Gambler(_burstValue){}

Dealer::~Dealer(){}

bool Dealer::isHitting() const
{
	//最大牌为10（A不算，因为可以当做1来算），保证庄家有二分之一的概率不会爆牌，故减去5
	return (getTotalValues() <= burstValue-5);
}   

void Dealer::flipFirstCard()
{
	if (!cards.empty()) cards.front()->flipOver();
}