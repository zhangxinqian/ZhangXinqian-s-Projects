#ifndef _CARD_H
#define _CARD_H

enum Face 
{
	ACE = 1, 
	TWO, 
	THREE, 
	FOUR, 
	FIVE, 
	SIX, 
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING,
};

enum Suit 
{
	SPADE, //黑桃
	CLUB, //梅花
	DIAMOND,//方块
	HEART //红桃
};

class Card
{
public:

	Card(Face f = ACE, Suit s = SPADE, bool ifu = true/*默认牌面朝上*/)
		:face(f),suit(s),isFaceUp(ifu){};

	~Card(){};

	int getFaceValue() const;           // 返回纸牌的点数

	Face getFace() const {return face;}
	void setFace(const Face face){this->face=face;}

	Suit getSuit() const {return suit;}
	void setSuit(const Suit suit){this->suit=suit;}

	bool getState() const {return isFaceUp;}

	void flipOver() {isFaceUp=!isFaceUp;}               //翻面，若纸牌面向上，则翻向上

protected:
	Face face;
	Suit suit;
	bool isFaceUp;	
};

#endif