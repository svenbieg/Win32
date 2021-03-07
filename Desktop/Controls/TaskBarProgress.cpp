//=====================
// TaskBarProgress.cpp
//=====================

#include "pch.h"


//=======
// Using
//=======

#include "TaskBarProgress.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

TaskBarProgress::TaskBarProgress(Handle<Window> hparent):
hParent(hparent)
{
ThrowIfFailed(CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_ALL, IID_ITaskbarList3, &pTaskbarList));
Stop();
}

TaskBarProgress::~TaskBarProgress()
{
Stop();
}


//========
// Common
//========

VOID TaskBarProgress::SetProgress(UINT uprg, UINT utotal)
{
uProgress=uprg;
pTaskbarList->SetProgressValue(hParent->GetHandle(), uProgress, utotal);
}

VOID TaskBarProgress::SetStatus(TaskBarProgressStatus status)
{
uStatus=status;
HWND hwnd=hParent->GetHandle();
switch(uStatus)
	{
	case TaskBarProgressStatus::Error:
		{
		pTaskbarList->SetProgressState(hwnd, TBPF_ERROR);
		break;
		}
	case TaskBarProgressStatus::Hidden:
		{
		pTaskbarList->SetProgressState(hwnd, TBPF_NOPROGRESS);
		break;
		}
	case TaskBarProgressStatus::Indeterminate:
		{
		pTaskbarList->SetProgressState(hwnd, TBPF_INDETERMINATE);
		break;
		}
	case TaskBarProgressStatus::Normal:
		{
		pTaskbarList->SetProgressState(hwnd, TBPF_NORMAL);
		break;
		}
	case TaskBarProgressStatus::Paused:
		{
		pTaskbarList->SetProgressState(hwnd, TBPF_PAUSED);
		break;
		}
	}
}

VOID TaskBarProgress::Stop()
{
pTaskbarList->SetProgressState(hParent->GetHandle(), TBPF_NOPROGRESS);
}

}}