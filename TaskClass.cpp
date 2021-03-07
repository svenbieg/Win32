//===============
// TaskClass.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "TaskClass.h"


//==================
// Con-/Destructors
//==================

Task::Task(Procedure proc, UINT ustack):
hThread(INVALID_HANDLE_VALUE),
pProcedure(new Procedure(proc)),
uStatus(TaskStatus::Initializing)
{
hThis=this;
hThread=CreateThread(nullptr, ustack, &DoTask, this, 0, nullptr);
}

Task::~Task()
{
delete pProcedure;
}


//========
// Common
//========

VOID Task::Abort()
{
if(uStatus!=TaskStatus::Done)
	{
	uStatus=TaskStatus::Abort;
	Wait();
	}
hThis=nullptr;
}

VOID Task::Wait()
{
while(uStatus!=TaskStatus::Done)
	Sleep(1);
}


//================
// Common Private
//================

DWORD WINAPI Task::DoTask(VOID* pparam)
{
Handle<Task> htask=(Task*)pparam;
htask->uStatus=TaskStatus::Running;
htask->pProcedure->Run();
htask->uStatus=TaskStatus::Done;
htask->Abort();
return 0;
}
