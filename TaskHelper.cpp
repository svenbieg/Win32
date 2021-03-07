//================
// TaskHelper.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "TaskHelper.h"


//========
// Common
//========

Handle<Task> CreateTask(Procedure proc, UINT ustack)
{
return new Task(proc, ustack);
}
