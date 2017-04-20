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
	SPADE, //����
	CLUB, //÷��
	DIAMOND,//����
	HEART //����
};

class Card
{
public:

	Card(Face f = ACE, Suit s = SPADE, bool ifu = true/*Ĭ�����泯��*/)
		:face(f),suit(s),isFaceUp(ifu){};

	~Card(){};

	int getFaceValue() const;           // ����ֽ�Ƶĵ���

	Face getFace() const {return face;}
	void setFace(const Face face){this->face=face;}

	Suit getSuit() const {return suit;}
	void setSuit(const Suit suit){this->suit=suit;}

	bool getState() const {return isFaceUp;}

	void flipOver() {isFaceUp=!isFaceUp;}               //���棬��ֽ�������ϣ�������

protected:
	Face face;
	Suit suit;
	bool isFaceUp;	
};

#endif