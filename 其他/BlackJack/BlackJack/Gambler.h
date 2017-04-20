#ifndef _GAMBLER_H
#define _GAMBLER_H

#include "Cards.h"

class Gambler : public Cards //������
{
public:
	Gambler(int _burstValue=21);
	virtual ~Gambler();

	//�Ƿ����Ҫ��
	virtual bool isHitting() const = 0;//����Ϊ�麯��

	bool isBusted() const;    //�Ƿ񱬵��ˣ�

	virtual int getTotalValues() const;

	bool hasBlackJack() const;

	void setBurstValue(const int _burstValue){burstValue=_burstValue;}

protected:
	int burstValue;//���Ƶ�����Ĭ��Ϊ21��ע�����Ƶ���������Ӯ����
};

#endif