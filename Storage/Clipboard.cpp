//===============
// Clipboard.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Clipboard.h"


//===========
// Namespace
//===========

namespace Storage {


//==================
// Con-/Destructors
//==================

Clipboard::Clipboard():
hWindow(NULL)
{
HINSTANCE hinst=GetModuleHandleW(NULL);
WNDCLASSEXW wc;
ZeroMemory(&wc, sizeof(WNDCLASSEXW));
wc.cbSize=sizeof(WNDCLASSEXW);
wc.hInstance=hinst;
wc.lpfnWndProc=WndProc;
wc.lpszClassName=L"Clipboard";
SetLastError(0);
aClass=RegisterClassExW(&wc);
hWindow=CreateWindowExW(0, L"Clipboard", 0, 0, 0, 0, 0, 0, 0, 0, hinst, (VOID*)this);
if(hWindow==INVALID_HANDLE_VALUE)
	hWindow=NULL;
if(!hWindow)
	{
	DebugPrint("Clipboard - CreateWindow() failed\n");
	return;
	}
SetLastError(0);
if(!AddClipboardFormatListener(hWindow))
	{
	DebugPrint("Clipboard - AddClipboardFormatListener() failed\n");
	}
}

Clipboard::~Clipboard()
{
if(hWindow)
	{
	RemoveClipboardFormatListener(hWindow);
	DestroyWindow(hWindow);
	}
if(aClass)
	{
	HINSTANCE hinst=GetModuleHandleW(NULL);
	UnregisterClassW((LPCWSTR)aClass, hinst);
	}
}


//========
// Common
//========

Handle<Buffer> Clipboard::GetData(UINT uformat)
{
if(!OpenClipboard(hWindow))
	return nullptr;
Handle<Buffer> hbuf;
BYTE* pbuf=nullptr;
try
	{
	HGLOBAL hdata=GetClipboardData(uformat);
	if(!hdata)
		return nullptr;
	pbuf=(BYTE*)GlobalLock(hdata);
	UINT usize=0;
	CopyMemory(&usize, pbuf, sizeof(UINT));
	hbuf=new Buffer(usize);
	CopyMemory(hbuf->Begin(), &pbuf[4], usize);
	}
catch(HRESULT)
	{}
if(pbuf)
	GlobalUnlock(pbuf);
CloseClipboard();
return hbuf;
}

Handle<Buffer> Clipboard::GetData(LPCWSTR pformat)
{
UINT uformat=GetFormat(pformat);
return GetData(uformat);
}

UINT Clipboard::GetFormat(LPCWSTR pformat)
{
if(!OpenClipboard(hWindow))
	throw E_ACCESSDENIED;
HRESULT status=E_INVALIDARG;
UINT uformat=0;
try
	{
	WCHAR ptmp[64];
	while(uformat=EnumClipboardFormats(uformat))
		{
		GetClipboardFormatNameW(uformat, ptmp, 64);
		if(StringCompare(pformat, ptmp, 0, false)==0)
			{
			status=S_OK;
			break;
			}
		}
	}
catch(HRESULT)
	{}
CloseClipboard();
ThrowIfFailed(status);
return uformat;
}

Handle<String> Clipboard::GetText()
{
if(!OpenClipboard(hWindow))
	return nullptr;
Handle<String> hstr;
LPWSTR pstr=nullptr;
UINT ulen=0;
try
	{
	HGLOBAL hdata=GetClipboardData(CF_UNICODETEXT);
	if(!hdata)
		return nullptr;
	pstr=(LPWSTR)GlobalLock(hdata);
	hstr=new String(pstr);
	}
catch(HRESULT)
	{}
if(pstr)
	GlobalUnlock(pstr);
CloseClipboard();
return hstr;
}

BOOL Clipboard::IsFormatAvailable(UINT uformat)
{
return IsClipboardFormatAvailable(uformat);
}

BOOL Clipboard::IsFormatAvailable(LPCWSTR pformat)
{
UINT uformat=0;
try
	{
	uformat=GetFormat(pformat);
	}
catch(HRESULT hr)
	{
	if(hr==E_INVALIDARG)
		return false;
	throw hr;
	}
return IsClipboardFormatAvailable(uformat);
}

VOID Clipboard::SetData(UINT uformat, VOID const* pdata, UINT usize)
{
if(!OpenClipboard(hWindow))
	throw E_ACCESSDENIED;
HGLOBAL hbuf=GlobalAlloc(GMEM_MOVEABLE, usize+4);
if(!hbuf)
	{
	CloseClipboard();
	throw E_OUTOFMEMORY;
	}
BYTE* pbuf=(BYTE*)GlobalLock(hbuf);
CopyMemory(pbuf, &usize, 4);
CopyMemory(&pbuf[4], pdata, usize);
GlobalUnlock(hbuf);
SetClipboardData(uformat, hbuf);
CloseClipboard();
}

VOID Clipboard::SetData(LPCWSTR pformat, VOID const* pdata, UINT usize)
{
UINT uformat=GetFormat(pformat);
SetData(uformat, pdata, usize);
}

VOID Clipboard::SetText(LPCWSTR pstr)
{
if(!OpenClipboard(hWindow))
	throw E_ACCESSDENIED;
UINT ulen=StringLength(pstr);
HGLOBAL hbuf=GlobalAlloc(GMEM_MOVEABLE, (ulen+1)*sizeof(WCHAR));
if(!hbuf)
	{
	CloseClipboard();
	throw E_OUTOFMEMORY;
	}
LPWSTR pbuf=(LPWSTR)GlobalLock(hbuf);
StringCopy(pbuf, ulen+1, pstr);
GlobalUnlock(hbuf);
SetClipboardData(CF_UNICODETEXT, hbuf);
CloseClipboard();
}

UINT Clipboard::RegisterFormat(LPCWSTR pformat)
{
return RegisterClipboardFormatW(pformat);
}


//================
// Common Private
//================

LRESULT CALLBACK Clipboard::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
Handle<Clipboard> hcb=(Clipboard*)GetProp(hwnd, "ClipboardHandle");
if(umsg==WM_CREATE)
	{
	CREATESTRUCT* pcs=(CREATESTRUCT*)lparam;
	SetProp(hwnd, "ClipboardHandle", pcs->lpCreateParams);
	return 0;
	}
if(!hcb)
	return DefWindowProc(hwnd, umsg, wparam, lparam);
switch(umsg)
	{
	case WM_CLIPBOARDUPDATE:
		{
		hcb->Changed(hcb);
		return 0;
		}
	}
hcb=nullptr;
return DefWindowProc(hwnd, umsg, wparam, lparam);
}

}
