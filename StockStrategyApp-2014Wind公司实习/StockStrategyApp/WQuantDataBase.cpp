#include "stdafx.h"
#include "WQuantDataBase.h"

//////////////////////////////////////////////////////////////////////////

bool WQuantDataBase::authorized = false;

bool WQuantDataBase::mainCallbackReturn = false;

bool WQuantDataBase::tdaysCallbackReturn = false;
list<wstring> WQuantDataBase::tdaysListBuffer;

bool WQuantDataBase::tdaysOffsetCallbackReturn = false;
wstring WQuantDataBase::tdaysOffsetDataBuffer;

bool WQuantDataBase::tdaysCountCallbackReturn = false;
int WQuantDataBase::countBuffer = 0;
//////////////////////////////////////////////////////////////////////////

WQuantDataBase::WQuantDataBase()
{

}

WQuantDataBase::~WQuantDataBase()
{

}
//////////////////////////////////////////////////////////////////////////

int WINAPI WQuantDataBase::mainCallback(WQEvent* pEvent, LPVOID pParam)
{
	if (!pEvent) 
	{
		return 0;
	}

	if (pEvent->EventType ==  eWQLogin )
	{
		mainCallbackReturn = true;

		if(pEvent->ErrCode == WQERR_OK)
		{
			authorized = true;
		}
	}

	return 0;
}

int WINAPI WQuantDataBase::tdaysCallback(WQEvent* pEvent, LPVOID pParam)
{
	if (!pEvent) 
	{
		return 0;
	}

	CString msg;

	if (pEvent->pQuantData != NULL) 
	{ 
		int codenum = pEvent->pQuantData->ArrWindCode.arrLen;
		int indnum = pEvent->pQuantData->ArrWindFields.arrLen;
		int timenum = pEvent->pQuantData->ArrDateTime.arrLen;

		VARIANT& data = pEvent->pQuantData->MatrixData; 
		if ((data.vt & VT_ARRAY) && (data.parray != NULL))
		{
			tdaysListBuffer.clear();

			for (int i = 0; i < timenum; i++) 
			{ 
				for (int j = 0; j < codenum; j++)
				{
					for (int k = 0; k < indnum; k++) 
					{ 
						int pos = i * codenum * indnum + j * indnum + k; 
						msg = toString(data.parray->pvData, pos, data.vt);
						tdaysListBuffer.push_back(wstring(msg.GetBuffer()));
						msg.ReleaseBuffer();
					}
				}
			}
		} 
		else 
		{ 
			msg = toString(&data);
			tdaysListBuffer.clear();
			tdaysListBuffer.push_back(wstring(msg.GetBuffer()));
			msg.ReleaseBuffer();
		} 
	}

	tdaysCallbackReturn = true;

	return 0;
}

int WINAPI WQuantDataBase::tdaysOffsetCallback(WQEvent* pEvent, LPVOID pParam)
{
	if (!pEvent) 
	{
		return 0;
	}

	CString msg;

	if (pEvent->pQuantData != NULL) 
	{ 
		int codenum = pEvent->pQuantData->ArrWindCode.arrLen;
		int indnum = pEvent->pQuantData->ArrWindFields.arrLen;
		int timenum = pEvent->pQuantData->ArrDateTime.arrLen;

		VARIANT& data = pEvent->pQuantData->MatrixData; 
		if ((data.vt & VT_ARRAY) && (data.parray != NULL))
		{ 
			for (int i = 0; i < timenum; i++) 
			{ 
				for (int j = 0; j < codenum; j++)
				{
					for (int k = 0; k < indnum; k++) 
					{ 
						int pos = i * codenum * indnum + j * indnum + k; 
						msg = toString(data.parray->pvData, pos, data.vt); 
					}
				}
			}
		} 
		else 
		{ 
			msg = toString(&data); 
		} 
	}

	tdaysOffsetDataBuffer = msg.GetBuffer();
	msg.ReleaseBuffer();

	tdaysOffsetCallbackReturn = true;

	return 0;
}

int WINAPI WQuantDataBase::tdaysCountCallback(WQEvent* pEvent, LPVOID pParam)
{
	if (!pEvent) 
	{
		return 0;
	}

	double count = 0;

	if (pEvent->pQuantData != NULL) 
	{ 
		int codenum = pEvent->pQuantData->ArrWindCode.arrLen;
		int indnum = pEvent->pQuantData->ArrWindFields.arrLen;
		int timenum = pEvent->pQuantData->ArrDateTime.arrLen;

		VARIANT& data = pEvent->pQuantData->MatrixData; 
		if ((data.vt & VT_ARRAY) && (data.parray != NULL))
		{ 
			for (int i = 0; i < timenum; i++) 
			{ 
				for (int j = 0; j < codenum; j++)
				{
					for (int k = 0; k < indnum; k++) 
					{ 
						int pos = i * codenum * indnum + j * indnum + k; 
						count = toDouble(data.parray->pvData, pos, data.vt); 
					}
				}
			}
		} 
		else 
		{ 
			count = toDouble(&data); 
		} 
	}

	countBuffer = (int) count;
	tdaysCountCallbackReturn = true;

	return 0;
}

//////////////////////////////////////////////////////////////////////////

bool WQuantDataBase::authorize()
{
	if (!authorized)
	{
		//设置主回调函数
		SetEventHandler(mainCallback);

		mainCallbackReturn = false;
		WDataAuthorize(NULL);
 		while (!mainCallbackReturn)
		{
			Sleep(100);
		}
		mainCallbackReturn = false;
	}

	return authorized;
}

bool WQuantDataBase::authorize(const wstring &windAccountId, const wstring &windPassword)
{
	if (!authorized)
	{
		//设置主回调函数
		SetEventHandler(mainCallback);

		WQAUTH_INFO *pAuthInfo = new WQAUTH_INFO();
		pAuthInfo->bSilentLogin = 1;
		wcscpy_s(pAuthInfo->strUserName, windAccountId.length() + 1, windAccountId.c_str());
		wcscpy_s(pAuthInfo->strPassword, windPassword.length() + 1, windPassword.c_str());

		mainCallbackReturn = false;
		WQID err = WDataAuthorize(pAuthInfo);
		while (!mainCallbackReturn)
		{
			Sleep(100);
		}
		mainCallbackReturn = false;

		delete pAuthInfo;
	}

	return authorized;
}

bool WQuantDataBase::authorizeQuit()
{
	if (authorized)
	{
		if (0 == WDataAuthQuit())
		{
			authorized = false;
		}
		else
		{
			return false;
		}
	}
	
	return true;
}

bool WQuantDataBase::isAuthorized()
{
	return authorized;
}

//////////////////////////////////////////////////////////////////////////

bool WQuantDataBase::cancelAllRequest()
{
	bool res = false;
	if (0 == CancelAllRequest())
	{
		res = true;
	}
	return res;
}

//////////////////////////////////////////////////////////////////////////

wstring WQuantDataBase::tdaysOffset(const wstring &beginTime, int offset)
{
	wstringstream wss;
	wss << offset;
	wstring offsetStr = wss.str();
	WQID req = TDaysOffset(beginTime.c_str(), offsetStr.c_str(), NULL, tdaysOffsetCallback, NULL);

	if (req == 0)
	{	
		return L"NULL";
	}

	tdaysOffsetCallbackReturn = false;
	while (!tdaysOffsetCallbackReturn)
	{
		Sleep(100);
	}
	tdaysOffsetCallbackReturn = false;

	return tdaysOffsetDataBuffer;
}

list<wstring> WQuantDataBase::tdays(const wstring &beginTime, const wstring &endTime)
{
	WQID req = TDays(beginTime.c_str(), endTime.c_str(), NULL, tdaysCallback, NULL);

	tdaysCallbackReturn = false;
	while (!tdaysCallbackReturn)
	{
		Sleep(100);
	}
	tdaysCallbackReturn = false;

	return tdaysListBuffer;
}

int WQuantDataBase::tdaysCount(const wstring &beginTime, const wstring &endTime)
{
	WQID req = TDaysCount(beginTime.c_str(), endTime.c_str(), NULL, tdaysCountCallback, NULL);

	tdaysCountCallbackReturn = false;
	while (!tdaysCountCallbackReturn)
	{
		Sleep(100);
	}
	tdaysCountCallbackReturn = false;

	return countBuffer;
}

//////////////////////////////////////////////////////////////////////////

double WQuantDataBase::toDouble(const LPVARIANT data)
{
	//DBL_MAX 表示数据缺失或其他异常
	double result = DBL_MAX; 

	switch(data->vt & VT_BSTR_BLOB) 
	{ 
	case VT_R8:
		result = data->dblVal; 
		break; 
	case VT_I4: 
		result = data->intVal;
		break; 
	default: 
		break; 
	}

	return result;
}

double WQuantDataBase::toDouble(PVOID pData, int pos, VARTYPE vt)
{
	//DBL_MAX 表示数据缺失或其他异常
	double result = DBL_MAX; 

	switch (vt & VT_BSTR_BLOB) 
	{ 
	case VT_VARIANT: 
		{ 
			LPVARIANT data = (LPVARIANT)pData + pos; 
			result = toDouble(data); 
		} 
		break; 
	case VT_R8: 
		{ 
			double * data = (double *)pData + pos;
			result = *data;
			break; 
		} 
	case VT_I4: 
		{ 
			int* data = (int*)pData + pos;
			result = *data;
			break;
		}
	default:   
		break;  
	}  

	return result;
}

CString WQuantDataBase::toString(const LPVARIANT data)
{
	CString msg;

	switch(data->vt & VT_BSTR_BLOB) 
	{ 
	case VT_R8: 
		msg.Format(L"%lf", data->dblVal); 
		break; 
	case VT_EMPTY: 
		msg = L"NULL"; 
		break; 
	case VT_BSTR: 
		msg.Format(L"%s", data->bstrVal); 
		break; 
	case VT_DATE: 
		msg = COleDateTime(data->date).Format(); 
		break; 
	default: 
		break; 
	}

	return msg;  
}

CString WQuantDataBase::toString(PVOID pData, int pos, VARTYPE vt)
{
	CString msg;

	switch (vt & VT_BSTR_BLOB) 
	{ 
	case VT_VARIANT: 
		{ 
			LPVARIANT data = (LPVARIANT)pData + pos; 
			msg = toString(data); 
		} 
		break; 
	case VT_BSTR: 
		{ 
			LPBSTR data = (LPBSTR)pData + pos; 
			msg = L""; 
			msg += *data;
			break; 
		} 
	case VT_R8: 
		{ 
			DOUBLE * data = (DOUBLE *)pData + pos; 
			msg.Format(L"%lf", *data); 
			break; 
		} 
	case VT_DATE: 
		{ 
			DATE * data = (DATE *)pData + pos; 
			msg = COleDateTime(*data).Format(); 
			break; 
		} 
	case VT_I4: 
		{ 
			INT* data = (INT*)pData + pos; 
			msg.Format(L"%d", *data); 
			break; 
		} 
	case VT_I8: 
		{ 
			LONGLONG* data = (LONGLONG*)pData + pos; 
			msg.Format(L"%I64d", *data); 
			break; 
		} 
	default:   
		break;
	}

	return msg;
}

//////////////////////////////////////////////////////////////////////////