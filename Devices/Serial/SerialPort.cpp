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

SerialPort::SerialPort(UINT id, BaudRate baud)
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

VOID SerialPort::Listen()
{
hListenTask=CreateTask(this, &SerialPort::DoListen);
}


//==============
// Input-Stream
//==============

SIZE_T SerialPort::Available()
{
return 0;
}

SIZE_T SerialPort::Read(VOID* buf, SIZE_T size)
{
return 0;
}


//===============
// Output-Stream
//===============

VOID SerialPort::Flush()
{
}

SIZE_T SerialPort::Write(VOID const* pbuf, SIZE_T size)
{
ScopedLock lock(cCriticalSection);
if(!pbuf||!size)
	return 0;
DWORD dwwritten=0;
HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
DWORD dwmode=0;
if(GetConsoleMode(hout, &dwmode))
	{
	WriteConsole(hout, pbuf, (UINT)size, &dwwritten, 0);
	}
else
	{
	WriteFile(hout, pbuf, (UINT)size, &dwwritten, nullptr);
	}
return dwwritten;
}


//================
// Common Private
//================

VOID SerialPort::DoListen()
{
while(!hListenTask->Cancel)
	{
	}
}

}}
