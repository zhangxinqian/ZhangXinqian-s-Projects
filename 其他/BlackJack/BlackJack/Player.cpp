#include "Player.h"

Player::Player(double _balance,int _burstValue):Gambler(_burstValue)
{
	ID=0;
	response=false;
	balance=_balance;
}

Player::~Player(){}

inline bool Player::isHitting() const
{
	return response;
}

//inline void Player::request(bool r)//加上inline报错，不知道为什么？
void Player::request(bool r)
{
	response=r;
}

//inline void Player::getBet(double bet)
void Player::getBet(double bet)
{
	balance+=bet;
}

bool Player::makeBet(double bet)
{
	bool result=false;
	if (balance>=bet)
	{
		balance-=bet;
		result=true;
	}
	return result;
}