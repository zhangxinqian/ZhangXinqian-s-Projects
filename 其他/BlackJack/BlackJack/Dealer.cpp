#include "Dealer.h"

Dealer::Dealer(int _burstValue):Gambler(_burstValue){}

Dealer::~Dealer(){}

bool Dealer::isHitting() const
{
	//�����Ϊ10��A���㣬��Ϊ���Ե���1���㣩����֤ׯ���ж���֮һ�ĸ��ʲ��ᱬ�ƣ��ʼ�ȥ5
	return (getTotalValues() <= burstValue-5);
}   

void Dealer::flipFirstCard()
{
	if (!cards.empty()) cards.front()->flipOver();
}