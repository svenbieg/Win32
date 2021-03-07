//=============
// ToolBar.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "ToolBar.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {
		namespace Menus {


//==================
// Con-/Destructors
//==================

ToolBar::ToolBar(Handle<Container> hparent):
CoreWindow(hparent),
CoreControl(hparent),
CoreContainer(hparent),
Container(hparent)
{
Moved.Add(this, &ToolBar::OnMoved);
Moving.Add(this, &ToolBar::OnMoving);
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
WindowInfo info;
info.Class=REBARCLASSNAME;
info.Style=CCS_NODIVIDER|RBS_AUTOSIZE|RBS_BANDBORDERS|RBS_VARHEIGHT|WS_VISIBLE;
Create(info);
}


//========
// Common
//========

VOID ToolBar::CheckButton(WORD uid, BOOL bchecked)
{
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hband=hit->GetCurrent().As<ToolBand>();
	if(hband->CheckButton(uid, bchecked))
		return;
	}
}

VOID ToolBar::EnableButton(WORD uid, BOOL benabled)
{
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hband=hit->GetCurrent().As<ToolBand>();
	if(hband->EnableButton(uid, benabled))
		return;
	}
}

Handle<ToolBand> ToolBar::GetBand(WORD uid)
{
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hband=hit->GetCurrent().As<ToolBand>();
	if(hband->GetId()==uid)
		return hband;
	}
return nullptr;
}

UINT ToolBar::GetBandCount()
{
return (UINT)SendMessage(hWindow, RB_GETBANDCOUNT, 0, 0);
}

BOOL ToolBar::GetBandInfo(WORD uid, REBARBANDINFO& rbbi)
{
UINT ucount=GetBandCount();
for(UINT u=0; u<ucount; u++)
	{
	if(!SendMessage(hWindow, RB_GETBANDINFO, u, (LPARAM)&rbbi));
		return false;
	if(rbbi.wID!=uid)
		return true;
	}
return false;
}

UINT ToolBar::GetBandPosition(WORD uid)
{
UINT ucount=GetBandCount();
REBARBANDINFO rbbi;
ZeroMemory(&rbbi, sizeof(REBARBANDINFO));
rbbi.cbSize=REBARBANDINFO_V6_SIZE;
rbbi.fMask=RBBIM_ID;
for(UINT u=0; u<ucount; u++)
	{
	SetLastError(0);
	if(!SendMessage(pParent->GetHandle(), RB_GETBANDINFO, u, (LPARAM)&rbbi))
		return -1;
	if(rbbi.wID==uid)
		return u;
	}
return -1;
}

Handle<ToolBand> ToolBar::InsertBand(INT ipos, WORD uid)
{
if(ipos<0)
	ipos=GetBandCount();
return new ToolBand(this, ipos, uid);
}

BOOL ToolBar::IsBandVisible(WORD uid)
{
REBARBANDINFO rbbi;
ZeroMemory(&rbbi, sizeof(REBARBANDINFO));
rbbi.cbSize=REBARBANDINFO_V6_SIZE;
rbbi.fMask=RBBIM_ID|RBBIM_STYLE;
if(!GetBandInfo(uid, rbbi))
	return false;
return !(rbbi.fStyle&RBBS_HIDDEN);
}

BOOL ToolBar::IsButtonChecked(WORD uid)
{
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hband=hit->GetCurrent().As<ToolBand>();
	TBBUTTONINFO tbbi;
	ZeroMemory(&tbbi, sizeof(TBBUTTONINFO));
	tbbi.cbSize=sizeof(TBBUTTONINFO);
	tbbi.dwMask=TBIF_STATE;
	if(hband->GetItemInfo(uid, tbbi))
		return tbbi.fsState&TBSTATE_CHECKED;
	}
return false;
}

BOOL ToolBar::IsButtonEnabled(WORD uid)
{
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hband=hit->GetCurrent().As<ToolBand>();
	TBBUTTONINFO tbbi;
	ZeroMemory(&tbbi, sizeof(TBBUTTONINFO));
	tbbi.cbSize=sizeof(TBBUTTONINFO);
	tbbi.dwMask=TBIF_STATE;
	if(hband->GetItemInfo(uid, tbbi))
		return tbbi.fsState&TBSTATE_ENABLED;
	}
return false;
}

VOID ToolBar::ShowBand(WORD uid, BOOL bvisible)
{
UINT upos=GetBandPosition(uid);
SendMessage(hWindow, RB_SHOWBAND, upos, bvisible);
pParent->InvalidateArrangement();
}

VOID ToolBar::ShowButton(WORD uid, BOOL bvisible)
{
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hband=hit->GetCurrent().As<ToolBand>();
	if(hband->ShowButton(uid, bvisible))
		return;
	}
}


//================
// Common Private
//================

/*HWND ToolBar::GetBandHandle(UINT upos)
{
REBARBANDINFO rbbi;
ZeroMemory(&rbbi, sizeof(REBARBANDINFO));
rbbi.cbSize=REBARBANDINFO_V6_SIZE;
rbbi.fMask=RBBIM_CHILD;
GetBandInfo(upos, &rbbi);
return rbbi.hwndChild;
}*/

/*UINT ToolBar::GetBandWidth(HWND hwnd)
{
UINT uitemcount=(UINT16)SendMessage(hwnd, TB_BUTTONCOUNT, 0, 0);
if(!uitemcount)
	return 0;
UINT upos=uitemcount-1;
RECT rc;
SendMessage(hwnd, TB_GETITEMRECT, upos, (LPARAM)&rc);
return rc.right;
}*/

VOID ToolBar::OnMoved(Handle<CoreWindow> hsender, RECT const& rc)
{
RearrangeBands();
}

VOID ToolBar::OnMoving(Handle<CoreWindow> hsender, RECT& rc)
{
SendMessage(hWindow, WM_SIZE, SIZE_RESTORED, rc.Right-rc.Left);
::RECT rcc;
GetClientRect(hWindow, &rcc);
rc.Right=rc.Left+rcc.right;
rc.Bottom=rc.Top+rcc.bottom;
}

VOID ToolBar::RearrangeBands()
{
UINT ucount=Children->GetCount();
for(UINT u=0; u<ucount; u++)
	{
	REBARBANDINFO rbbi;
	ZeroMemory(&rbbi, sizeof(REBARBANDINFO));
	rbbi.cbSize=REBARBANDINFO_V6_SIZE;
	rbbi.fMask=RBBIM_STYLE;
	if(!SendMessage(hWindow, RB_GETBANDINFO, u, (LPARAM)&rbbi))
		return;
	if(rbbi.fStyle&RBBS_HIDDEN)
		continue;
	::RECT rcband;
	SendMessage(hWindow, RB_GETRECT, u, (LPARAM)&rcband);
	INT itop=rcband.top;
	BOOL bfound=false;
	for(UINT u1=u+1; u1<ucount; u1++)
		{
		if(!SendMessage(hWindow, RB_GETBANDINFO, u1, (LPARAM)&rbbi))
			return;
		if(rbbi.fStyle&RBBS_HIDDEN)
			continue;
		bfound=true;
		SendMessage(hWindow, RB_GETRECT, u, (LPARAM)&rcband);
		if(rcband.top>itop)
			{
			SendMessage(hWindow, RB_MAXIMIZEBAND, u, 0);
			break;
			}
		else
			{
			SendMessage(hWindow, RB_MINIMIZEBAND, u, 0);
			break;
			}
		}
	if(!bfound)
		SendMessage(hWindow, RB_MAXIMIZEBAND, u, 0);
	}
}

}}}
