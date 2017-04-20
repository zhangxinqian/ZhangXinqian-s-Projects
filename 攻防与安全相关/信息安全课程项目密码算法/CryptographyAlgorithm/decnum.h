#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;

class decnum
{
	friend decnum pow(const decnum& x, int n);
	friend decnum root(const decnum& x, int n);
	friend decnum div(const decnum& x, const decnum& y, decnum& r);
	friend decnum abs(const decnum& x);
	friend bool operator==(const decnum& x, const decnum&y);
	friend bool operator!=(const decnum& x, const decnum&y);
	friend bool operator>(const decnum& x, const decnum&y);
	friend bool operator<(const decnum& x, const decnum&y);
	friend bool operator>=(const decnum& x, const decnum&y);
	friend bool operator<=(const decnum& x, const decnum&y);
	friend ostream& operator<<(ostream&os, const decnum& x);

public:
	decnum() : cap(0), num(0), sign(0), val(NULL) { }
	decnum(const char *v, int n)
	{
		int i, j,k;
		val = new char[n];
		if(v[0] == '-') 
		{
			sign = 1;
			v++;
			n--;
		}
		else
		{
			sign = 0;
			if(v[0] == '+')
				v++;
		}
		for(i = 0; i < n && v[i] == '0'; i++);
		for (j=0,k=n-1;k>=i;--k,++j)
		{
			if (v[k]>='0' && v[k]<='9')
			{
				val[j] = v[k]-'0';
			}
			else
				break;
		}
		num = j;
		cap = n;
	}
	decnum(const decnum& x) 
	{
		val = new char[x.num];
		memcpy(val, x.val, x.num);
		sign = x.sign;
		num = x.num;
		cap = x.num;
	}
	decnum(int x)
	{
		if(x==0)
		{
			cap = num = 0;
			val = NULL;
			return;
		}
		if(x < 0) 
		{
			sign = 1;
			x = -x;
		}
		else
			sign = 0;
		char temp[20];
		sprintf(temp, "%d", x);
		int n = strlen(temp);
		num = cap = n;
		val = new char[n];
		for(int i=0; i<n; i++)
			val[i] = temp[n-1-i] - '0';
	}
	decnum(long long x)
	{
		if(x==0)
		{
			cap = num = 0;
			val = NULL;
			return;
		}
		if(x < 0) 
		{
			sign = 1;
			x = -x;
		}
		else
			sign = 0;
		char temp[20];
		sprintf(temp, "%lld", x);
		int n = strlen(temp);
		num = cap = n;
		val = new char[n];
		for(int i=0; i<n; i++)
			val[i] = temp[n-1-i] - '0';
	}
	~decnum() { delete[] val; }

	int size() const { return num; }

	decnum& operator=(const decnum& x)
	{
		if (this != &x) 
		{
			if(cap < x.num)
			{
				delete[] val;
				val = new char[x.num];
				cap = x.num;
			}
			memcpy(val, x.val, x.num);
			num = x.num;
			sign = x.sign;
		}
		return *this;
	}
	decnum& operator=(int x)
	{
		*this = decnum(x);
		return *this;
	}
	decnum& operator=(long long x)
	{
		*this = decnum(x);
		return *this;
	}
	decnum& abs()
	{
		sign = 0;
		return *this;
	}
	decnum& operator+=(const decnum& x);
	decnum& operator-=(const decnum& x);
	decnum& operator*=(const decnum& x);
	decnum& operator/=(const decnum& x);
	decnum& operator%=(const decnum& x);
	decnum  operator+(const decnum& x) const;
	decnum  operator-(const decnum& x) const;
	decnum  operator*(const decnum& x) const;
	decnum  operator/(const decnum& x) const;
	decnum  operator%(const decnum& x) const;

	bool ispow(int n, decnum& r) const;
	bool ispow() const;
private:
	int  cap;
	int  num;
	int  sign;
	char *val;
private:
	char root_1(int n);
	decnum& absadd(const decnum& x);
	decnum& abssub(const decnum& x);
	bool absge(const decnum& x);
};