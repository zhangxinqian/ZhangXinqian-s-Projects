#include "Cards.h"

Cards::~Cards()
{
	clear();
}

void Cards::getACard(Card* pCard)
{
	cards.push_back(pCard);  //在牌组尾部添加一张牌
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
	//将总点数加起来，将A当作1
	for (iter = cards.begin(); iter != cards.end(); ++iter)
	{
		//如果有一张牌面朝下，则返回0
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