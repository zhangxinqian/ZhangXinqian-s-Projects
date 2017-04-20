#include "stdafx.h"

#include "Hill.h"

Hill::Hill(string ifn,string ofn,int size,int **key):CipherObj(ifn,ofn)
{
	int i,j;
	this->Size=size;
	Key=new int *[Size];
	for (i=0;i<Size;i++)
	{
		Key[i]=new int[Size];
		for (j=0;j<Size;j++)
		{
			while (key[i][j]<0) key[i][j]+=26; //ת����0��25֮���ֵ
			Key[i][j]=key[i][j]%26;
		}
	}
	hasInvKey=false;
	invKey=new int *[Size];
	for (i=0;i<Size;i++)
		invKey[i]=new int[Size];
}

Hill::~Hill()
{
	int i;
	for (i=0;i<Size;i++)
		delete[] Key[i];
	delete[] Key;

	for (i=0;i<Size;i++)
		delete[] invKey[i];
	delete[] invKey;
}

/*
ǰ����v��resultָ�붼������Size��С�Ŀռ�,keyָ��Key��invKey�е�һ��
*/
void Hill::VectorMultipleKeyMatrix(int *v,int **key,int *result)
{
	int i,j;
	for (j=0;j<Size;j++)
	{
		result[j]=0;
		for (i=0;i<Size;i++)
			result[j]+=v[i]*key[i][j];
		while (result[j]<0)
			result[j]+=26;
		result[j]%=26;//���ת������0~25��Χ��
	}
}

/*
����Կ����Key��det������ʽ��ʹ��������˹����
ʵ����ʹ�õݹ麯����detLaplace��һ���ݹ麯��
*/
int Hill::detLaplace(int size,int **key)
{
	int result=0,i,j,k,l;
	if (size==1) return key[0][0];
	for (i=0;i<size;i++)
	{
		int **key0=new int*[size-1];//key0������ʽΪKey������ʽ
		for (j=0;j<size-1;j++)
			key0[j]=new int[size-1];
		for (j=1;j<size;j++)
		{
			l=0;
			for (k=0;k<size;k++)
			{
				if (k==i) continue;
				key0[j-1][l]=key[j][k];
				l++;
			}
		}
		result+=key[0][i]*int(pow(float(-1),i))*detLaplace(size-1,key0);//�ۼ��������ʽ
		for (j=0;j<size-1;j++)  //�ͷ�key0
			delete[] key0[j];
		delete[] key0;
	}
	/*��ֹ������������ȡ��
	���ں����Դ����еķ�����Щ��ͬ���Ҿ�����ѧ֤�����������´�����õ�����󲻱�
	*/
	
	while (result<0) result+=26;
	result%=26;
	
	return result;
}

/*
�õ���Կ����������ʹ�ð������
������󷽷������Դ����еķ�����Щ��ͬ������˵�����Ǵ�ͳ�����ϵ������
*/
bool Hill::getInvKey()
{
	int i,j,k,l;
	int n=detLaplace(Size,Key);
	//if (n==0) return;
	while (n<0) n+=26;//�������ʽ�Ǹ�����ͨ���ظ�����26ת��������
	//i=1;
	//while ((n*i)%26!=1) i++; //�������ϵķ���������ʽ����һ����������26����Ϊ1ʱ����Ҫ��,(����˷���)
	//n=i;

	int x;
	if (!ExtendedEuclid(n,26,x)) return false;

	int **key0=new int*[Size-1];//key0��������м���
	for (j=0;j<Size-1;j++)
		key0[j]=new int[Size-1];
	for (i=0;i<Size;i++)
	{
		for (j=0;j<Size;j++)
		{
			int a=0,b=0;
			for (k=0;k<Size;k++)
			{
				if (k==j) continue;//��������Ϊ��������ʽ
				b=0;
				for (l=0;l<Size;l++)
				{
					if (l==i) continue;//��������Ϊ��������ʽ
					key0[a][b]=Key[k][l];
					b++;
				}
				a++;
			}
			//invKey[i][j]=int(pow(float(-1),i+j))*n*detLaplace(Size-1,key0);
			invKey[i][j]=int(pow(float(-1),i+j))*x*detLaplace(Size-1,key0);
			while (invKey[i][j]<0) invKey[i][j]+=26;
			invKey[i][j]%=26;
		}
	}
	for (j=0;j<Size-1;j++) //�ͷ�key0
		delete[] key0[j];
	delete[] key0;

	hasInvKey=true;

	return true;
}

/*����չŷ������㷨��˷���Ԫ*/
int Hill::ExtendedEuclid( int f,int d ,int &result)
{
	int x1,x2,x3,y1,y2,y3,t1,t2,t3,q;
	x1 = y2 = 1;
	x2 = y1 = 0;
    x3 = ( f>=d )?f:d;
    y3 = ( f>=d )?d:f;
    while(true)
	{
		if ( y3 == 0 )
		{
			result = x3; /* ��������������resultΪ�����������Լ������ʱ����ֵΪ�� */
			return 0;
		}
		if ( y3 == 1 )
		{
			result = y2; /* ������������resutlΪ��˷���Ԫ����ʱ����ֵΪ1 */
			return 1;
		}
		q = x3/y3;
		t1 = x1 - q*y1;
		t2 = x2 - q*y2;
		t3 = x3 - q*y3;
		x1 = y1;
		x2 = y2;
		x3 = y3;
		y1 = t1;
		y2 = t2;
		y3 = t3;
	}
}

void Hill::execute(bool flag)//flagΪtrueΪ���ܣ�ʹ��Key��ΪfalseΪ���ܣ�ʹ��invKey��
{
	int **key;
	if (flag) key=Key;
	else
	{
		if (!hasInvKey) getInvKey();
		key=invKey;
	}
	/*һ�ζ�ȡSize��С���ַ��������м���*/
	char p;
	int *v=new int[Size], //һ�μ��ܽ��ܵ�����
		*result=new int[Size];//һ�μ��ܽ��ܷ��صĽ������
	int i=0,//��¼��Ч�ַ�����
		j=0;
	while (inFile.get(p))
	{
		p=toupper(p);
		if (p>='A' && p<='Z')
		{
			v[i%Size]=p-'A';
			i++;
			if (i%Size==0) 
			{
				VectorMultipleKeyMatrix(v,key,result);
				for (j=0;j<Size;j++)
					outFile<<char(result[j]+'A');//һ�������дӢ����ĸ
			}
		}
		else outFile<<p;//�������Ӣ����ĸ��ֱ�����,�Թ����ܽ��ܹ���
	}
	/*
	����Ч���ܽ��ܵ��ַ���������Size����������ʣ�µ���X����,X-A=23
	*/
	if (i%Size!=0)
	{
		while (i%Size!=0)
		{
			v[i%Size]=23;
			i++;
		}
		VectorMultipleKeyMatrix(v,key,result);
		for (j=0;j<Size;j++)
			outFile<<char(result[j]+'A');
	}

	delete[] v;
	delete[] result;
}