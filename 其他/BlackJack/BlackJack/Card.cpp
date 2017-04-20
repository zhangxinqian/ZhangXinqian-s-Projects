#include "Card.h"

int Card::getFaceValue() const
{
	int value = face;    //点数为纸牌上显示的点数
	if (value > 10)
		value = 10;    //J，Q，K 做为10来处理(A可以在后面计算中可以当做11来用)
	return value;
}