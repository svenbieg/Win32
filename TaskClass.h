//=============
// TaskClass.h
//=============

#pragma once


//=======
// Using
//=======

#include "Procedure.h"


//=============
// Task-Status
//=============

enum class TaskStatus
{
Initializing,
Running,
Abort,
Done
};


//======
// Task
//======

class Task: public Object
{
public:
	// Con-/Destructors
	Task(Procedure Procedure, UINT Stack=2048);
	~Task();

	// Common
	VOID Abort();
	TaskStatus GetStatus()const { return uStatus; }
	VOID Wait();

private:
	// Common
	static DWORD WINAPI DoTask(VOID* Param);
	Handle<Task> hThis;
	HANDLE hThread;
	Procedure* pProcedure;
	TaskStatus uStatus;
};
