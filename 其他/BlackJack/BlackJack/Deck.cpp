#include "Deck.h"

Deck::Deck(){}

Deck::~Deck(){}

void Deck::createDeck()   //����һ��52�ŵ���
{
	//������ԭ������
	clear();
	for (int s = SPADE; s <= HEART; ++s)     //����suit��face�����п��ܵ����ʹ��
		for (int f = ACE; f <= KING; ++f)
			 //static_cast��int����ǿ��ת��ΪCard�ж����ö������
			getACard(new Card(static_cast<Face>(f), static_cast<Suit>(s)));
	//shuffle();
}

void Deck::removeCards(int number)
{
	if (!cards.empty()) 
	{
		Card *tmpCard;
		while (number--)
		{
			tmpCard=cards.back();
			delete tmpCard;
			cards.pop_back();
		}
	}
}

void Deck::removeCards(Face face)
{
	list<Card*>::iterator iter;
	Card* tmpCard;
	for (iter=cards.begin();iter!=cards.end();)
	{
		if ((*iter)->getFace()==face)
		{
			tmpCard=*iter;
			delete tmpCard;
			iter=cards.erase(iter);
		}
		else ++iter;
	}
}

void Deck::removeCards(Suit suit)
{
	list<Card*>::iterator iter;
	Card* tmpCard;
	for (iter=cards.begin();iter!=cards.end();)
	{
		if ((*iter)->getSuit()==suit)
		{
			tmpCard=*iter;
			delete tmpCard;
			iter=cards.erase(iter);
		}
		else ++iter;
	}
}

void Deck::shuffle(int seed)
{
	vector<Card*> tmpVec;
	vector<Card*>::iterator iter1;
	list<Card*>::iterator iter2;

	for (iter2=cards.begin();iter2!=cards.end();++iter2)
		tmpVec.push_back((*iter2));

	srand((unsigned int)(time(0)+seed));  // ���ݵ�ǰʱ�䣬���������������������
	random_shuffle(tmpVec.begin(), tmpVec.end());

	cards.clear();

	for (iter1=tmpVec.begin();iter1!=tmpVec.end();++iter1)
		cards.push_back((*iter1));
}

bool Deck::dealACard(Gambler& gamber)
{
	if (!cards.empty())
	{
		gamber.getACard(cards.back());
		cards.pop_back();   //ɾ��ʸ�������һ��Ԫ��
		return true;
	}
	else return false;
}