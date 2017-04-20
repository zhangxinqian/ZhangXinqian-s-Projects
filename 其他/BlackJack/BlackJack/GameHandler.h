#ifndef GH_H
#define GH_H

#include "Card.h"
#include "Cards.h"
#include "Deck.h"
#include "Dealer.h"
#include "Player.h"

#include <iostream>
#include <string>
using namespace std;

class GameHandler
{
public:
	GameHandler();
	~GameHandler();

	void play();

private:
	void welcome();
	void initGame();
	void clearGame();

	void setPlayersName();

	//选择牌堆类型
	void chooseDeck();
	//第一次（为开局前）下注
	void makeFirstBet();
	//初始发两张牌
	void initHands();
	//删除所有人的手牌
	void clearAllHands();
	//玩家回合
	void playerTurn();
	//庄家回合
	void dealerTurn();

	//每个玩家只和庄家比大小
	void againstDealer();
	//每个玩家也要比较输赢
	void againstPlayers();

	void loopPlay();

	void showACard(Card *card);
	void showCards(Cards *cards);

private:
	Dealer *dealer;
	Deck *deck;//庄家的牌堆

	Deck **decks;//玩家的牌堆
	Player **players;
	double *betList;
	int numberOfPlayers;

	double initBalance;

	int burstValue;
};

#endif