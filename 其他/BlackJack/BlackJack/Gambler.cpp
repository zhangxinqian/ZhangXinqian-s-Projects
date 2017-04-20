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
		int aceNum = 0;     //ȷ��A�ĸ���
		for (iter = cards.begin(); iter != cards.end(); ++iter)
			if ((*iter)->getFaceValue() == ACE)
				aceNum++;
		//ʹ����A������£�totalÿ�α��ֲ��ᱬ��������������ֵ
		while (aceNum>0 && total <= burstValue -10)
		{
			//������A���ҵ����㹻�ͣ���ÿ��A����11����ÿ��ֻҪ����10
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