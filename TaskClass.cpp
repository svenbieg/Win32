//===============
// TaskClass.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "TaskClass.h"


//============================
// Con-/Destructors Protected
//============================

Task::Task():
Cancel(false),
hThread(INVALID_HANDLE_VALUE)
{}

Task::~Task()
{
if(hThread!=INVALID_HANDLE_VALUE)
	CloseHandle(hThread);
}


//========
// Common
//========

VOID Task::Wait()
{
while(hThis)
	Sleep(10);
}
