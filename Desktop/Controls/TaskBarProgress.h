//===================
// TaskBarProgress.h
//===================

#pragma once


//=======
// Using
//=======

#include <Shobjidl.h>
#include <wrl.h>
#include "Control.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//========
// Status
//========

enum class TaskBarProgressStatus
{
Error,
Hidden,
Indeterminate,
Normal,
Paused
};


//===================
// Task-Bar-Progress
//===================

class TaskBarProgress: public Object
{
private:
	// Using
	template <class T> using ComPtr=Microsoft::WRL::ComPtr<T>;

public:
	// Con-/Destructors
	TaskBarProgress(Handle<Window> Parent);
	~TaskBarProgress();

	// Common
	UINT GetProgress()const { return uProgress; }
	TaskBarProgressStatus GetStatus()const { return uStatus; }
	VOID SetProgress(UINT Progress, UINT Total=100);
	VOID SetStatus(TaskBarProgressStatus Status);
	VOID Stop();

private:
	// Common
	Handle<Window> hParent;
	ComPtr<ITaskbarList3> pTaskbarList;
	UINT uProgress;
	TaskBarProgressStatus uStatus;
};

}}