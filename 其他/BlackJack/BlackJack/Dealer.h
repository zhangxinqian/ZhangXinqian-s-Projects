#ifndef _DEALER_H
#define _DEALER_H

#include "Gambler.h"

class Dealer : public Gambler
{
public:
	Dealer(int _burstValue=21);
	virtual ~Dealer();

	//ׯ���Ƿ�Ҫ�ƣ�����<= burstValue-5 ʱׯ��ѡ��Ҫ�ƣ�����Ҫ��ƽ�����Ƶ�������Ϊ burstValue-4
	virtual bool isHitting() const;

	void flipFirstCard();        //��ʼ�����Ƶĵ�һ���Ʒ�Ϊ�෴��һ��
};

#endif