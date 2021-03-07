//=================
// ProcessBuffer.h
//=================

#pragma once


//===========
// Namespace
//===========

namespace Storage {


//================
// Process-Buffer
//================

class ProcessBuffer: public Object
{
public:
	// Con-/Destructors
	ProcessBuffer(HANDLE Process, UINT Size);
	~ProcessBuffer();

	// Common
	VOID* Begin() { return pBuffer; }

private:
	// Common
	HANDLE hProcess;
	VOID* pBuffer;
};

}
