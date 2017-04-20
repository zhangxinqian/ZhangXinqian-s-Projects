#include "Cards.h"

Cards::~Cards()
{
	clear();
}

void Cards::getACard(Card* pCard)
{
	cards.push_back(pCard);  //������β�����һ����
}

bool Cards::isEmpty() const
{
	return (cards.size()==0);
}

int Cards::getCardNumber() const
{
	return cards.size();
}

void Cards::clear()
{
	if (!cards.empty())
	{
		list<Card*>::const_iterator iter;
		Card *temp;
		for (iter = cards.begin(); iter != cards.end(); ++iter)
		{
			temp=*iter;
			delete temp;
		}
		cards.clear();
	}
}

int Cards::getTotalValues() const
{
	int total = 0;
	list<Card*>::const_iterator iter;
	//���ܵ�������������A����1
	for (iter = cards.begin(); iter != cards.end(); ++iter)
	{
		//�����һ�����泯�£��򷵻�0
		if (!(*iter)->getState()) return 0;
		total += (*iter)->getFaceValue();
	}

	return total;
}

list<Card*>* Cards::getCards() const
{
	list<Card*> *tmpCards=new list<Card*>();
	*tmpCards=cards;
	return tmpCards;
}