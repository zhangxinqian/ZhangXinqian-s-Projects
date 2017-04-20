#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int KmpSearch(char* s, char* p)  
{  
	int i = 0;  
	int j = 0;  
	int sLen = strlen(s);  
	int pLen = strlen(p);  
	while (i < sLen && j < pLen)  
	{  
		//①如果j = -1，或者当前字符匹配成功（即S[i] == P[j]），都令i++，j++      
		if (j == -1 || s[i] == p[j])  
		{  
			i++;  
			j++;  
		}  
		else  
		{  
			//②如果j != -1，且当前字符匹配失败（即S[i] != P[j]），则令 i 不变，j = next[j]      
			//next[j]即为j所对应的next值        
			j = next[j];  
		}  
	}  
	if (j == pLen)  
		return i - j;  
	else  
		return -1;  
}  

void GetNext(char* p, int next[])  
{  
	int pLen = strlen(p);  
	next[0] = -1;
	int k = -1;  
	int j = 0;  
	while (j < pLen - 1)  
	{  
		//p[k]表示前缀，p[j]表示后缀  
		if (k == -1 || p[j] == p[k])   
		{  
			++k;  
			++j;  
			next[j] = k;  
		}  
		else   
		{  
			k = next[k];  
		}  
	}  
}

//优化过后的next 数组求法  
void GetNextval(char* p, int next[])  
{  
	int pLen = strlen(p);  
	next[0] = -1;  
	int k = -1;  
	int j = 0;  
	while (j < pLen - 1)  
	{  
		//p[k]表示前缀，p[j]表示后缀    
		if (k == -1 || p[j] == p[k])  
		{  
			++j;  
			++k;  
			//较之前next数组求法，改动在下面4行  
			if (p[j] != p[k])  
				next[j] = k;   //之前只有这一行  
			else  
				//因为不能出现p[j] = p[ next[j ]]，所以当出现时需要继续递归，k = next[k] = next[next[k]]  
				next[j] = next[k];  
		}  
		else  
		{  
			k = next[k];  
		}  
	}  
}  

inline void NEXT(const string &T, vector<int> &next)
{
	next[0] = -1;
	for(int i = 1; i < T.size(); i++)
	{
		int j = next[i-1];
		while(T[i] != T[j+1] && j >= 0)
		{
			j = next[j];//递推计算
		}
		if(T[i] == T[j+1])
		{
			next[i] = j + 1;
		}
		else 
		{ 
			next[i] = 0;
		}
	}
}

inline int COUNT_KMP(const string &S, const string &T)
{
	//利用模式串T的next函数求T在主串S中的个数count的KMP算法
	//其中T非空，
	vector<int> next(T.size());
	NEXT(T, next);
	int index = 0, count = 0;
	for(index = 0; index < S.size(); ++index)
	{
		int pos = 0;
		int iter=index;
		while(pos < T.size() && iter < S.size())
		{
			if(S[iter] == T[pos])
			{
				++iter;
				++pos;
			}
			else
			{
				if(pos == 0)
				{
					++iter;
				}
				else
				{	
					pos = next[pos-1] + 1;
				}
			}
		}
		if((pos == T.size()) && ((iter - index) == T.size()))
		{
			++count;
		}
	}

	return count;
}

int main(int argc, char* argv[])
{
	string S = "abaabcacabaabcacabaabcacabaabcacabaabcac";
	string T = "ab";
	int count = COUNT_KMP(S, T);
	cout << count << endl;

	system("PAUSE");

	return 0;
}