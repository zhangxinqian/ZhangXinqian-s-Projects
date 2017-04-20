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

	//ѡ���ƶ�����
	void chooseDeck();
	//��һ�Σ�Ϊ����ǰ����ע
	void makeFirstBet();
	//��ʼ��������
	void initHands();
	//ɾ�������˵�����
	void clearAllHands();
	//��һغ�
	void playerTurn();
	//ׯ�һغ�
	void dealerTurn();

	//ÿ�����ֻ��ׯ�ұȴ�С
	void againstDealer();
	//ÿ�����ҲҪ�Ƚ���Ӯ
	void againstPlayers();

	void loopPlay();

	void showACard(Card *card);
	void showCards(Cards *cards);

private:
	Dealer *dealer;
	Deck *deck;//ׯ�ҵ��ƶ�

	Deck **decks;//��ҵ��ƶ�
	Player **players;
	double *betList;
	int numberOfPlayers;

	double initBalance;

	int burstValue;
};

#endif