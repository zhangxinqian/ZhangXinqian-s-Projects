#include "MapHack.h"
#include <iostream>
using namespace std;

int main()
{
	SetConsoleTitle("My MapHack for Warcraft III 1.24E");
	if (MapHack::run()) cout<<"Success"<<endl;
	else cout<<"Failed"<<endl;
	cin.get();
	return 0;
}