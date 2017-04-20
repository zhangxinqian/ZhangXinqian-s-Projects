#ifndef _PLAYER_H
#define _PLAYER_H

#include "Gambler.h"
#include <string>
using namespace std;

class Player : public Gambler
{
public:
	Player(double _balance=100,int _burstValue=21);
	virtual ~Player();

	void setId(const int id){ID=id;}
	int getId() const {return ID;}

	void setName(const string _name){name=_name;}
	string getName() const {return name;}

	double getBalance() const {return balance;}

	virtual bool isHitting() const;   //是否想再要一张牌

	//请求是否要牌，若r为true则回应要牌，否则不要牌
	void request(bool r);

	bool makeBet(double bet);//下注
	void getBet(double bet);//获得赌金

private:
	string name;
	int ID;
	bool response;
	double balance;//赌金余额
};

#endif