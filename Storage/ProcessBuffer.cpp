//===================
// ProcessBuffer.cpp
//===================

#include "pch.h"


//=======
// Using
//=======

#include "ProcessBuffer.h"


//===========
// Namespace
//===========

namespace Storage {


//==================
// Con-/Destructors
//==================

ProcessBuffer::ProcessBuffer(HANDLE hproc, UINT usize):
hProcess(hproc),
pBuffer(nullptr)
{
ASSERT(hproc&&usize);
SetLastError(0);
pBuffer=VirtualAllocEx(hproc, 0, usize, MEM_COMMIT, PAGE_READWRITE);
ASSERT(pBuffer);
}

ProcessBuffer::~ProcessBuffer()
{
if(pBuffer)
	VirtualFreeEx(hProcess, pBuffer, 0, MEM_RELEASE);
}

}
