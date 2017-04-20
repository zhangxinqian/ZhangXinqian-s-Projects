#include "Gambler.h"

Gambler::Gambler(int _burstValue):Cards()
{
	burstValue=_burstValue;
}

Gambler::~Gambler(){}

int Gambler::getTotalValues() const
{
	int total=Cards::getTotalValues();

	if (total>0)
	{
		list<Card*>::const_iterator iter;
		int aceNum = 0;     //确定A的个数
		for (iter = cards.begin(); iter != cards.end(); ++iter)
			if ((*iter)->getFaceValue() == ACE)
				aceNum++;
		//使得有A的情况下，total每次保持不会爆掉的情况下有最大值
		while (aceNum>0 && total <= burstValue -10)
		{
			//若包含A，且点数足够低，则将每个A当作11，即每次只要加上10
			total += 10;
			aceNum--;
		}
	}

	return total;
}

bool Gambler::isBusted() const
{
	return (getTotalValues() > burstValue);
}

bool Gambler::hasBlackJack() const
{
	if (cards.size()==2 && getTotalValues()==21) return true;
	return false;
}