//=============
// Process.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Process.h"


//==================
// Con-/Destructors
//==================

Process::Process(UINT uid, UINT uacc):
hProcess(0)
{
ASSERT(uid&&uacc);
SetLastError(0);
hProcess=OpenProcess(uacc, 0, uid);
if(hProcess==INVALID_HANDLE_VALUE)
	hProcess=NULL;
ASSERT(hProcess);
}

Process::~Process()
{
if(hProcess)
	CloseHandle(hProcess);
}


//========
// Common
//========

SIZE_T Process::Read(VOID* ppos, SIZE_T usize, VOID* pbuf)
{
SetLastError(0);
if(!ReadProcessMemory(hProcess, ppos, pbuf, usize, 0))
	return 0;
return usize;
}
