#include "Card.h"

int Card::getFaceValue() const
{
	int value = face;    //����Ϊֽ������ʾ�ĵ���
	if (value > 10)
		value = 10;    //J��Q��K ��Ϊ10������(A�����ں�������п��Ե���11����)
	return value;
}