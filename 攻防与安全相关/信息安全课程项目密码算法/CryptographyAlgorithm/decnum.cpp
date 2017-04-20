#include "stdafx.h"

#include "decnum.h"

decnum& decnum::absadd(const decnum& x)
{
	int i, max;
	char carry = 0, ch;

	if(x.num == 0) return *this;
	if(num < x.num)
		max = x.num + 1;
	else
		max = num + 1;
	if(max > cap)
	{
		cap = max;
		char *newval = new char[cap];
		memcpy(newval, val, num);
		memset(newval+num, 0, cap - num);
		delete[] val;
		val = newval;
	}
	else
	{
		memset(val+num, 0, max - num);
	}
	num = max-1;
	for(i=0; i<x.num; i++) 
	{
		ch = val[i] + x.val[i] + carry;
		if(ch > 9) 
		{
			carry = 1;
			val[i] = ch - 10;
		}
		else 
		{
			carry = 0;
			val[i] = ch;
		}
	}
	for(; i<=num && carry == 1; i++) 
	{
		ch = val[i] + 1;
		if(ch > 9) 
		{
			carry = 1;
			val[i] = ch - 10;
		}
		else 
		{
			carry = 0;
			val[i] = ch;
		}
	}
	if(i>num) num = i;
	return *this;
}

decnum& decnum::abssub(const decnum& x)
{
	if(x.num == 0) return *this;
	int i;
	char carry = 0, ch;
	for(i=0; i<x.num; i++) 
	{
		ch = val[i] - x.val[i] - carry;
		if(ch < 0)
		{
			carry = 1;
			val[i] = ch + 10;
		}
		else 
		{
			carry = 0;
			val[i] = ch;
		}
	}
	for(; i<num && carry == 1; i++)
	{
		ch = val[i] - 1;
		if(ch < 0) 
		{
			carry = 1;
			val[i] = ch + 10;
		}
		else 
		{
			carry = 0;
			val[i] = ch;
		}
	}
	for(i=num; i>0 && val[i-1] == 0; i--);
	num = i;
	return *this;
}

bool decnum::absge(const decnum& x)
{
	if(num > x.num) return true;
	if(num < x.num) return false;
	for(int i=num-1; i >= 0; i--)
	{
		if(val[i] > x.val[i])
			return true;
		else if(val[i] < x.val[i])
			return false;
	}
	return true;
}

decnum& decnum::operator+=(const decnum& x)
{
	if(x.sign == sign)
		return absadd(x);
	else if(absge(x))
		return abssub(x);
	else 
	{
		decnum tmp(*this);
		*this = x;
		return abssub(tmp);
	}
}

decnum& decnum::operator-=(const decnum& x)
{
	if(x.sign != sign)
		return absadd(x);
	else if(absge(x))
		return abssub(x);
	else 
	{
		decnum tmp(*this);
		*this = x;
		return abssub(tmp);
	}
}

decnum& decnum::operator*=(const decnum& x)
{
	if(num == 0) return *this;
	if(x.num == 0)
	{
		num = 0;
		return *this;
	}
	if(sign == x.sign)
		sign = 0;
	else
		sign = 1;
	int mul, i, n, newcap, max;
	char ch, carry;
	char *newval = new char[num + x.num];
	newcap = num + x.num;
	memset(newval, 0, num + x.num);

	decnum a, b;
	char *ptr;
	for(i=0; i<num && val[i] == 0; i++);
	int na = i;
	a.val = val + i;
	a.num = num - i;
	for(i=0; i<num && x.val[i] == 0; i++);
	int nb = i;
	b.val = x.val + i;
	b.num = x.num - i;
	ptr = newval + na + nb;

	for(n=0; n <= a.num + b.num - 2; n++) 
	{
		mul = 0;
		if(n > b.num - 1)
			i = n - b.num + 1;
		else
			i=0;
		max = n < a.num-1 ? n : a.num-1;
		for(; i<=max; i++)
			mul += a.val[i]*b.val[n-i];
		carry = 0;
		for(i=n; mul > 0 || carry > 0; mul /= 10, i++) 
		{
			ch = ptr[i] + mul % 10 + carry;
			if(ch > 9) 
			{
				carry = 1;
				ptr[i] = ch - 10;
			}
			else
			{
				carry = 0;
				ptr[i] = ch;
			}
		}
	}
	for(i=a.num + b.num; i>0 && ptr[i-1] == 0; i--);
	num = i + na + nb;
	if(cap >= num) 
	{
		memcpy(val, newval, num);
		delete[] newval;
	}
	else 
	{
		cap = newcap;
		delete[] val;
		val = newval;
	}
	a.val = b.val = NULL;
	return *this;
}

decnum& decnum::operator/=(const decnum& x)
{
	char ch, carry, fac;
	decnum tmp;
	int i;
	if(x.num == 0) return *this;
	if(num < x.num) 
	{
		num = 0;
		return *this;
	}
	if(sign == x.sign)
		sign = 0;
	else
		sign = 1;
	char *newval = new char[num - x.num + 1];
	memset(newval, 0, num - x.num + 1);
	carry = 0;
	fac = x.val[x.num-1] + 1;
	tmp.val = val + num - x.num + 1;
	tmp.cap = tmp.num = x.num -1;
	for(i=num-1; i>= x.num-1; i--) 
	{
		tmp.val--;
		tmp.num++;
		ch = (carry * 10 + val[i]) / fac;
		tmp  -= x * ch;
		while(tmp >= x) 
		{
			tmp -= x;
			ch++;
		}
		newval[i-x.num+1] = ch;
		carry = val[i];
	}
	tmp.val = NULL;
	for(i=num-x.num+1; i>0 && newval[i-1] == 0; i--);
	num = i;
	delete[] val;
	val = newval;
	return *this;
}

decnum& decnum::operator%=(const decnum& x)
{
	char ch, carry, fac;
	decnum tmp;
	int i;
	if(x.num == 0) return *this;
	if(num < x.num) return *this;

	carry = 0;
	fac = x.val[x.num-1] + 1;
	tmp.val = val + num - x.num + 1;
	tmp.num = x.num - 1;
	for(i=num-1; i>= x.num-1; i--) 
	{
		tmp.val--;
		tmp.num++;
		ch = (carry * 10 + val[i]) / fac;
		tmp  -= x * ch;
		while(tmp >= x) 
		{
			tmp -= x;
			ch++;
		}
		carry = val[i];
	}
	tmp.val = NULL;
	num = tmp.num;
	return *this;
}

decnum decnum::operator+(const decnum& x) const
{
	decnum tmp = *this;
	return tmp += x;
}

decnum decnum::operator-(const decnum& x) const
{
	decnum tmp = *this;
	return tmp -= x;
}

decnum decnum::operator*(const decnum& x) const
{
	decnum tmp = *this;
	return tmp *= x;
}

decnum decnum::operator/(const decnum& x) const
{
	decnum tmp = *this;
	return tmp /= x;
}

decnum decnum::operator%(const decnum& x) const
{
	decnum tmp = *this;
	return tmp %= x;
}

decnum abs(const decnum& x)
{
	decnum tmp(x);
	tmp.sign = 0;
	return tmp;
}

decnum pow(const decnum& x, int n)
{
	decnum tmp(1), fac(x);
	for(; n>0; n>>=1) 
	{
		if(n&0x01)
			tmp *= fac;
		fac *= fac;
	}
	return tmp;
}

char decnum::root_1(int n)
{
	char r = (int)(pow(1+val[num-1], 1.0/n) * pow(10,(num-1.0)/n));
	for(; r>0 && pow(decnum(r), n) > *this; r--);
	return r;
}

bool decnum::ispow(int n, decnum& r) const
{
	if(num == 0) 
	{
		r.num = 0;
		return true;
	}
	if(sign == 1 && (n&1 == 0))
	{
		r.num = 0;
		return false;
	}
	decnum tmp, p;
	r.cap = r.num = (num+n-1) / n;
	r.val = new char[r.num];
	r.sign = sign;
	memset(r.val, 0, r.num);
	tmp.val = val + (r.num-1)*n;
	tmp.num = num - (r.num-1)*n;
	r.val[r.num-1] = tmp.root_1(n);

	tmp.val = new char[r.num+1];
	tmp.cap = r.num+1;
	int v;
	p = pow(r, n);
	if(p == *this) return true;
	for(int i=r.num-2; i>=0; i--) 
	{
		memset(tmp.val, 0, i+1);
		tmp.val[i] = 1;
		tmp.num = i+1;
		tmp += r;
		p = (*this - p) / (pow(tmp, n) - p);
		if(p.num > 1)
			v = 9;
		else if(p.num > 0)
			v = p.val[0];
		else
			v = 0;
		for(; v>=0; v--) 
		{
			r.val[i] = v;
			p = pow(r, n);
			if(p == *this)
				return true;
			if(p < *this)
				break;
		}
	}
	return false;
}

bool decnum::ispow() const
{
	decnum r, dec2("2", 1);
	if(ispow(2, r)) return true;
	for(int n=3; r > dec2; n+=2) 
	{
		if(ispow(n, r)) return true;
	}
	return false;
}

decnum root(const decnum& x, int n)
{
	decnum r;
	x.ispow(n, r);
	return r;
}

decnum div(const decnum& x, const decnum& y, decnum& r)
{
	char ch, carry, fac;
	decnum d = x, tmp;
	int i;
	if(y.num == 0) return x;
	if(d.num < y.num)
	{
		r = x;
		d = 0;
		return d;
	}
	char *newval = new char[d.num - y.num + 1];
	memset(newval, 0, d.num - y.num + 1);
	carry = 0;
	fac = y.val[y.num-1] + 1;
	tmp.val = d.val + d.num - y.num + 1;
	tmp.num = y.num - 1;
	for(i=d.num-1; i>= y.num-1; i--) 
	{
		tmp.val--;
		tmp.num++;
		ch = (carry * 10 + d.val[i]) / fac;
		tmp  -= y * ch;
		while(tmp >= y) 
		{
			tmp -= y;
			ch++;
		}
		newval[i-y.num+1] = ch;
		carry = d.val[i];
	}
	r = tmp;
	tmp.val = NULL;
	for(i=d.num-y.num+1; i>0 && newval[i-1] == 0; i--);
	d.num = i;
	delete[] d.val;
	d.val = newval;
	return d;
}

bool operator==(const decnum& x, const decnum&y)
{
	if(x.sign != y.sign) return false;
	if(x.num != y.num) return false;
	for(int i=0; i < x.num; i++)
	{
		if(x.val[i] != y.val[i])
			return false;
	}
	return true;
}

bool operator!=(const decnum& x, const decnum&y)
{
	return !(x==y);
}

bool operator>(const decnum& x, const decnum&y)
{
	if(x.sign > y.sign) return false;
	if(x.sign < y.sign) return true;
	bool retval = (x.sign == 0);
	if(x.num > y.num) return retval;
	if(x.num < y.num) return !retval;
	for(int i=x.num-1; i >= 0; i--)
	{
		if(x.val[i] > y.val[i])
			return retval;
		else if(x.val[i] < y.val[i])
			return !retval;
	}
	return false;
}

bool operator<(const decnum& x, const decnum&y)
{
	return y > x;
}

bool operator>=(const decnum& x, const decnum&y)
{
	if(x.sign > y.sign) return false;
	if(x.sign < y.sign) return true;
	bool retval = (x.sign == 0);
	if(x.num > y.num) return retval;
	if(x.num < y.num) return !retval;
	for(int i=x.num-1; i >= 0; i--)
	{
		if(x.val[i] > y.val[i])
			return retval;
		else if(x.val[i] < y.val[i])
			return !retval;
	}
	return true;
}

bool operator<=(const decnum& x, const decnum&y)
{
	return y >= x;
}

ostream& operator<<(ostream&os, const decnum& x)
{
	if(x.size() == 0)
		os << 0;
	else
	{
		if(x.sign == 1)
			os << "-";
		for(int i = x.size()-1; i>=0; i--)
			os << (int)x.val[i];
	}
	return os;
}