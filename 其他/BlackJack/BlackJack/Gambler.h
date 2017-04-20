#ifndef _GAMBLER_H
#define _GAMBLER_H

#include "Cards.h"

class Gambler : public Cards //抽象类
{
public:
	Gambler(int _burstValue=21);
	virtual ~Gambler();

	//是否继续要牌
	virtual bool isHitting() const = 0;//声明为虚函数

	bool isBusted() const;    //是否爆掉了）

	virtual int getTotalValues() const;

	bool hasBlackJack() const;

	void setBurstValue(const int _burstValue){burstValue=_burstValue;}

protected:
	int burstValue;//爆牌点数，默认为21；注：爆牌点数等于输赢点数
};

#endif