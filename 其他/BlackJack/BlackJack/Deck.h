#ifndef _DECK_H
#define _DECK_H

#include "Card.h"
#include "Cards.h"
#include "Gambler.h"
#include <vector>
#include <algorithm>//�õ�����random_shuffle()����
#include <ctime>
using namespace std;

class Deck : public Cards
{
public:
	Deck();
	virtual ~Deck();

	void createDeck();         //����һ��52�ŵ���
	void shuffle(int seed);    //ϴ��

	void removeCards(int number);//���ƶѶ���ȥnumber����
	void removeCards(Face face);//���ƶѳ�ȥ��������Ϊface����
	void removeCards(Suit suit);//���ƶѳ�ȥ��������Ϊsuit����

	bool dealACard(Gambler& gamber);   //��һ���Ʒ���һ���������
};

#endif