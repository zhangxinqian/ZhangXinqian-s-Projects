#include "CipherObj.h"

class Vigenere:CipherObj
{
public:
	Vigenere(string ifn,string ofn,string kw="");
	~Vigenere();
	void execute(bool flag);
private:
	void encryption();
	void decryption();
private:
	string KeyWord;
};