#include "stdafx.h"
#include "StockStrategy.h"

StockStrategy::StockStrategy()
{

}

StockStrategy::~StockStrategy()
{

}

int StockStrategy::dbMax(double* dbArr, int length)
{
	int index = 0;
	for (int i = 1; i < length; i++)
	{
		if (dbArr[index] < dbArr[i])
		{
			index = i;
		}
	}
	return index;
}

int StockStrategy::dbMin(double* dbArr, int length)
{
	int index = 0;
	for (int i = 1; i < length; i++)
	{
		if (dbArr[index] > dbArr[i])
		{
			index = i;
		}
	}
	return index;
}