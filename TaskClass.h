//=============
// TaskClass.h
//=============

#pragma once


//=======
// Using
//=======

#include <functional>


//======
// Task
//======

class Task: public Object
{
public:
	// Common
	volatile BOOL Cancel;
	VOID Wait();

protected:
	// Con-/Destructors
	Task();
	~Task();

	// Common
	Handle<Task> hThis;
	HANDLE hThread;
};


//============
// Task Typed
//============

template <class _owner_t, class... _args_t>
class TaskTyped: public Task
{
public:
	// Using
	typedef VOID (_owner_t::*TaskProc)(_args_t...);

	// Con-/Destructors
	TaskTyped(_owner_t* Owner, TaskProc Procedure):
		hOwner(Owner),
		pProcedure(Procedure)
		{}

	VOID Run(_args_t... Arguments)
		{
		if(hThread)
			return;
		hThis=this;
		cProcedure=[this, Arguments...](){ (hOwner->*pProcedure)(Arguments...); };
		hThread=CreateThread(nullptr, 0, &DoTask, this, 0, nullptr);
		}

private:
	// Common
	static DWORD WINAPI DoTask(VOID* Param)
		{
		Handle<TaskTyped> task=(TaskTyped*)Param;
		task->cProcedure();
		task->hThis=nullptr;
		return 0;
		}
	std::function<VOID()> cProcedure;
	Handle<_owner_t> hOwner;
	TaskProc pProcedure;
};
