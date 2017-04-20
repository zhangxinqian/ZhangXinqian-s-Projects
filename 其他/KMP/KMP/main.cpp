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
		//�����j = -1�����ߵ�ǰ�ַ�ƥ��ɹ�����S[i] == P[j]��������i++��j++      
		if (j == -1 || s[i] == p[j])  
		{  
			i++;  
			j++;  
		}  
		else  
		{  
			//�����j != -1���ҵ�ǰ�ַ�ƥ��ʧ�ܣ���S[i] != P[j]�������� i ���䣬j = next[j]      
			//next[j]��Ϊj����Ӧ��nextֵ        
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
		//p[k]��ʾǰ׺��p[j]��ʾ��׺  
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

//�Ż������next ������  
void GetNextval(char* p, int next[])  
{  
	int pLen = strlen(p);  
	next[0] = -1;  
	int k = -1;  
	int j = 0;  
	while (j < pLen - 1)  
	{  
		//p[k]��ʾǰ׺��p[j]��ʾ��׺    
		if (k == -1 || p[j] == p[k])  
		{  
			++j;  
			++k;  
			//��֮ǰnext�����󷨣��Ķ�������4��  
			if (p[j] != p[k])  
				next[j] = k;   //֮ǰֻ����һ��  
			else  
				//��Ϊ���ܳ���p[j] = p[ next[j ]]�����Ե�����ʱ��Ҫ�����ݹ飬k = next[k] = next[next[k]]  
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
			j = next[j];//���Ƽ���
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
	//����ģʽ��T��next������T������S�еĸ���count��KMP�㷨
	//����T�ǿգ�
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