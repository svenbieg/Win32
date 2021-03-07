//================
// SerialPort.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "SerialPort.h"


//===========
// Namespace
//===========

namespace Devices {
	namespace Serial {


//==================
// Con-/Destructors
//==================

SerialPort::SerialPort()
{
hInput=GetStdHandle(STD_INPUT_HANDLE);
hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
#ifdef _UNICODE
DWORD dwmode=0;
if(!GetConsoleMode(hOutput, &dwmode))
	{
	WCHAR uc=0xFEFF;
	DWORD dwwritten=0;
	WriteFile(hOutput, &uc, 2, &dwwritten, nullptr);
	}
#endif
}


//========
// Common
//========

Handle<SerialPort> SerialPort::Current;

VOID SerialPort::Print(LPCSTR pvalue)
{
ScopedLock lock(cCriticalSection);
DoPrint(0, pvalue);
}

VOID SerialPort::Print(LPCWSTR pvalue)
{
ScopedLock lock(cCriticalSection);
DoPrint(0, pvalue);
}

VOID SerialPort::Print(Handle<String> hvalue)
{
if(hvalue==nullptr)
	return;
ScopedLock lock(cCriticalSection);
DoPrint(0, hvalue->Begin());
}

VOID SerialPort::Print(UINT ulen, LPCSTR pvalue)
{
ScopedLock lock(cCriticalSection);
DoPrint(ulen, pvalue);
}

VOID SerialPort::Print(UINT ulen, LPCWSTR pvalue)
{
ScopedLock lock(cCriticalSection);
DoPrint(ulen, pvalue);
}

Handle<String> SerialPort::Read()
{
TCHAR pbuf[128];
DWORD dwread=0;
ReadConsole(hInput, pbuf, 128, &dwread, nullptr);
if(dwread==0)
	return nullptr;
for(; dwread>0; dwread--)
	{
	if(pbuf[dwread-1]>0x20)
		break;
	}
if(dwread==0)
	return nullptr;
pbuf[dwread]=0;
return new String(pbuf);
}

SIZE_T SerialPort::Write(VOID const* pbuf, SIZE_T usize)
{
ScopedLock lock(cCriticalSection);
return DoWrite((BYTE const*)pbuf, usize);
}


//================
// Common Private
//================

VOID SerialPort::DoPrint(UINT ulen, LPCSTR pstr)
{
ulen=StringLength(pstr, ulen);
if(!ulen)
	return;
DWORD dwwritten=0;
HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
DWORD dwmode=0;
#ifdef _UNICODE
WCHAR pbuf[STRING_PRINTF_MAX];
StringCopy(pbuf, STRING_PRINTF_MAX, pstr);
if(GetConsoleMode(hout, &dwmode))
	{
	WriteConsoleW(hout, pbuf, ulen, &dwwritten, 0);
	}
else
	{
	WriteFile(hout, pbuf, ulen*sizeof(WCHAR), &dwwritten, nullptr);
	}
#else
if(GetConsoleMode(hout, &dwmode))
	{
	WriteConsoleA(hout, pstr, ulen, &dwwritten, 0);
	}
else
	{
	WriteFile(hout, pstr, ulen, &dwwritten, nullptr);
	}
#endif
}

VOID SerialPort::DoPrint(UINT ulen, LPCWSTR pstr)
{
ulen=StringLength(pstr, ulen);
if(!ulen)
	return;
DWORD dwwritten=0;
HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
DWORD dwmode=0;
#ifndef _UNICODE
CHAR pbuf[256];
StringCopy(pbuf, 256, pstr);
if(GetConsoleMode(hout, &dwmode))
	{
	WriteConsoleA(hout, pbuf, ulen, &dwwritten, 0);
	}
else
	{
	WriteFile(hout, pbuf, ulen, &dwwritten, nullptr);
	}
#else
if(GetConsoleMode(hout, &dwmode))
	{
	WriteConsoleW(hout, pstr, ulen, &dwwritten, 0);
	}
else
	{
	WriteFile(hout, pstr, ulen*sizeof(WCHAR), &dwwritten, nullptr);
	}
#endif
}

SIZE_T SerialPort::DoWrite(BYTE const* pbuf, SIZE_T usize)
{
if(!pbuf||!usize)
	return 0;
DWORD dwwritten=0;
HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
DWORD dwmode=0;
if(GetConsoleMode(hout, &dwmode))
	{
	WriteConsole(hout, pbuf, (UINT)usize, &dwwritten, 0);
	}
else
	{
	WriteFile(hout, pbuf, (UINT)usize, &dwwritten, nullptr);
	}
return dwwritten;
}

}}
