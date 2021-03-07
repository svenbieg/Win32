//=================
// ProgressBar.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include <CommCtrl.h>
#include "Container.h"
#include "ProgressBar.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

ProgressBar::ProgressBar(Handle<Container> hparent):
CoreWindow(hparent),
CoreControl(hparent),
Control(hparent),
Progress(this, 0.f),
uMax(0)
{
SetLastError(0);
INITCOMMONCONTROLSEX icex;
icex.dwSize=sizeof(INITCOMMONCONTROLSEX);
icex.dwICC=ICC_BAR_CLASSES|ICC_COOL_CLASSES;
InitCommonControlsEx(&icex);
UINT dwerr=GetLastError();
switch(dwerr)
	{
	case ERROR_SUCCESS:
		break;
	case ERROR_CLASS_ALREADY_EXISTS:
		break;
	default:
		{
		DebugPrint("InitCommonControls failed (%xh)\n", dwerr);
		return;
		}
	}
Moved.Add(this, &ProgressBar::OnMoved);
Progress.Changed.Add(this, &ProgressBar::OnProgressChanged);
WindowInfo info;
info.Class=PROGRESS_CLASS;
info.Style=WS_VISIBLE;
Window::Create(info);
}


//==================
// Common Protected
//==================

Graphics::SIZE ProgressBar::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize=Control::GetMinSize(hdc);
minsize.Height+=GetSystemMetrics(SM_CYHSCROLL);
return minsize;
}


//================
// Common Private
//================

VOID ProgressBar::OnMoved(Handle<CoreWindow> hwnd, RECT const& rc)
{
uMax=(UINT)(rc.Right-rc.Left);
UINT urange=uMax<<16;
SendMessage(hWindow, PBM_SETRANGE, 0, urange);
SendMessage(hWindow, PBM_SETPOS, (UINT)(Progress.GetInternal()*uMax), 0);
}

VOID ProgressBar::OnProgressChanged(FLOAT f)
{
if(!hWindow)
	return;
SendMessage(hWindow, PBM_SETPOS, (UINT)(f*uMax), 0);
}

}}
