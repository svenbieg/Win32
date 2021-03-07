//===========
// Process.h
//===========

#pragma once


//=========
// Process
//=========

class Process: public Object
{
public:
	// Con-/Destructors
	Process(UINT Id, UINT Access);

	// Common
	HANDLE GetHandle()const { return hProcess; }
	SIZE_T Read(VOID* Position, SIZE_T Size, VOID* Buffer);

private:
	// Con-/Destructors
	~Process();

	// Common
	HANDLE hProcess;
};

