#ifndef _DECK_H
#define _DECK_H

#include "Card.h"
#include "Cards.h"
#include "Gambler.h"
#include <vector>
#include <algorithm>//用到里面random_shuffle()函数
#include <ctime>
using namespace std;

class Deck : public Cards
{
public:
	Deck();
	virtual ~Deck();

	void createDeck();         //创建一幅52张的牌
	void shuffle(int seed);    //洗牌

	void removeCards(int number);//从牌堆顶抽去number张牌
	void removeCards(Face face);//从牌堆抽去所有牌面为face的牌
	void removeCards(Suit suit);//从牌堆抽去所有牌面为suit的牌

	bool dealACard(Gambler& gamber);   //将一张牌发到一个玩家手中
};

#endif