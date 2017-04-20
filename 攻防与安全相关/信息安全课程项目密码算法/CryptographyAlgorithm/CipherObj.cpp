#include "stdafx.h"
#include "CipherObj.h"

CipherObj::CipherObj(string ifn,string ofn)
{
	this->inFileName=ifn;
	this->outFileName=ofn;
	inFile.open(inFileName.c_str());
	outFile.open(outFileName.c_str());
}

CipherObj::~CipherObj()
{
	inFile.close();
	outFile.close();
}