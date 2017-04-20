#include <iostream>
#include <fstream>
using namespace std;

void encryptionOfCaesar(int k,char &p);
void decryptionOfCaesar(int k,char &p);
void execute(void (*fp)(int,char&),int k);
int main()
{
	char option;
	int k;
	cout<<"press 1 to execute encryption..."<<endl;
	cout<<"press 2 to execute decryption..."<<endl;
	cout<<"others to exit..."<<endl;
	cin>>option;
	if (! cin) {cout<<"exit..."<<endl;return 1;}
	if (option=='1') 
	{
		cout<<"execute encryption..."<<endl;
		cout<<"please input the key:";
		cin>>k;
		while (!cin) cin>>k;
		execute(encryptionOfCaesar,k);
	}
	else if (option=='2') 
	{
		cout<<"execute decryption..."<<endl;
		cout<<"please input the key:";
		cin>>k;
		while (!cin) cin>>k;
		execute(decryptionOfCaesar,k);
	}
	else cout<<"exit..."<<endl;
	return 0;
}

void encryptionOfCaesar(int k,char &p)
{
	if (p>='A' && p<='Z')
	{
		p=char((p-'A'+k)%26+'A');
	}
	if (p>='a' && p<='z')
	{
		p=char((p-'a'+k)%26+'a');
	}
}

void decryptionOfCaesar(int k,char &p)
{
	if (p>='A' && p<='Z')
	{
		p=char((p-'A'-k+26)%26+'A');
	}
	if (p>='a' && p<='z')
	{
		p=char((p-'a'-k+26)%26+'a');
	}
}

void execute(void (*fp)(int,char&),int k)
{
	ifstream inFile;
	ofstream outFile;
	char p;
	if (fp==encryptionOfCaesar) inFile.open("plaintext.txt");
	if (fp==decryptionOfCaesar) inFile.open("ciphertext.txt");
	outFile.open("result.txt");
	inFile.get(p);
	while (inFile)
	{
		(*fp)(k,p);
		outFile<<p;
		inFile.get(p);
	}
	cout<<"has been done..."<<endl;
	inFile.close();
	outFile.close();
}