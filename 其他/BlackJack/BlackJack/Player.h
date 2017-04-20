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

	virtual bool isHitting() const;   //�Ƿ�����Ҫһ����

	//�����Ƿ�Ҫ�ƣ���rΪtrue���ӦҪ�ƣ�����Ҫ��
	void request(bool r);

	bool makeBet(double bet);//��ע
	void getBet(double bet);//��öĽ�

private:
	string name;
	int ID;
	bool response;
	double balance;//�Ľ����
};

#endif