//==============
// ToolBand.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "Graphics/Gdi/Bitmap.h"
#include "ToolBar.h"

using namespace Graphics::Gdi;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {
		namespace Menus {


//==================
// Con-/Destructors
//==================

ToolBand::ToolBand(Handle<ToolBar> htb, INT ipos, WORD uid):
Core::Controls::Container(htb),
Container(htb),
pToolBar(htb),
uId(uid)
{
Items=new ItemList();
Visible.Changed.Add(this, &ToolBand::OnVisibleChanged);
Visible.Reading.Add(this, &ToolBand::OnVisibleReading);
WindowInfo info;
info.Class=TOOLBARCLASSNAME;
info.Style=BTNS_AUTOSIZE|CCS_NODIVIDER|CCS_NOPARENTALIGN|CCS_NORESIZE|TBSTYLE_FLAT|TBSTYLE_LIST|TBSTYLE_TOOLTIPS|WS_VISIBLE;
Create(info);
SendMessage(hWindow, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
SendMessage(hWindow, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS);
UINT uicosize=GetSystemMetrics(SM_CXSMICON);
hImgList=new ImageList(uicosize, uicosize);
hImgList->SetBackgroundColor(GetSysColor(COLOR_BTNFACE));
hImgListDisabled=new ImageList(uicosize, uicosize);
hImgListDisabled->SetBackgroundColor(GetSysColor(COLOR_BTNFACE));
SendMessage(hWindow, TB_SETIMAGELIST, 0, (WPARAM)hImgList->GetHandle());
SendMessage(hWindow, TB_SETDISABLEDIMAGELIST, 0, (WPARAM)hImgListDisabled->GetHandle());
REBARBANDINFO rbbi;
ZeroMemory(&rbbi, sizeof(REBARBANDINFO));
rbbi.cbSize=REBARBANDINFO_V6_SIZE;
rbbi.fMask=RBBIM_CHILD|RBBIM_ID|RBBIM_STYLE;
rbbi.fStyle=RBBS_CHILDEDGE|RBBS_NOGRIPPER;
rbbi.wID=uId;
rbbi.hwndChild=hWindow;
SetLastError(0);
SendMessage(Parent->GetHandle(), RB_INSERTBAND, ipos, (LPARAM)&rbbi);
pToolBar->InvalidateArrangement();
}


//========
// Common
//========

BOOL ToolBand::CheckButton(WORD uid, BOOL bchecked)
{
return (BOOL)SendMessage(hWindow, TB_CHECKBUTTON, uid, bchecked);
}

BOOL ToolBand::EnableButton(WORD uid, BOOL benabled)
{
return (BOOL)SendMessage(hWindow, TB_ENABLEBUTTON, uid, benabled);
}

Handle<ToolBandItem> ToolBand::GetItem(WORD uid)
{
for(auto hit=Items->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hitem=hit->GetCurrent();
	if(hitem->GetId()==uid)
		return hitem;
	}
return nullptr;
}

UINT ToolBand::GetItemCount()
{
if(!hWindow)
	return 0;
return (UINT)SendMessage(hWindow, TB_BUTTONCOUNT, 0, 0);
}

BOOL ToolBand::GetItemInfo(WORD uid, TBBUTTONINFO& tbbi)
{
return SendMessage(hWindow, TB_GETBUTTONINFO, uid, (LPARAM)&tbbi)>=0;
}

UINT ToolBand::GetItemPosition(WORD uid)
{
UINT upos=0;
for(auto hit=Items->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hitem=hit->GetCurrent();
	if(hitem->GetId()==uid)
		return upos;
	upos++;
	}
return -1;
}

UINT ToolBand::GetPosition()
{
return Parent->GetBandPosition(uId);
}

Handle<ToolBandItem> ToolBand::InsertButton(INT ipos, WORD uid, WORD uico, WORD uicod, Handle<String> htext)
{
if(ipos==-1)
	ipos=GetItemCount();
return new ToolBandItem(this, ipos, uid, uico, uicod, htext);
}

BOOL ToolBand::IsButtonChecked(WORD uid)
{
TBBUTTONINFO tbbi;
ZeroMemory(&tbbi, sizeof(TBBUTTONINFO));
tbbi.cbSize=sizeof(TBBUTTONINFO);
tbbi.dwMask=TBIF_STATE;
if(!GetItemInfo(uid, tbbi))
	return false;
return tbbi.fsState&TBSTATE_CHECKED;
}

BOOL ToolBand::IsButtonEnabled(WORD uid)
{
TBBUTTONINFO tbbi;
ZeroMemory(&tbbi, sizeof(TBBUTTONINFO));
tbbi.cbSize=sizeof(TBBUTTONINFO);
tbbi.dwMask=TBIF_STATE;
if(!GetItemInfo(uid, tbbi))
	return false;
return tbbi.fsState&TBSTATE_ENABLED;
}

VOID ToolBand::Rearrange(Handle<DeviceContext> hdc, RECT& rc)
{
Container::Rearrange(hdc, rc);
INT iminwidth=0;
INT iminheight=0;
for(auto hit=Items->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hitem=hit->GetCurrent();
	if(!hitem->Visible)
		continue;
	RECT rc=hitem->GetRect();
	iminwidth+=rc.Right-rc.Left;
	iminheight=MAX(iminheight, rc.Bottom-rc.Top);
	}
UINT uband=GetPosition();
REBARBANDINFO rbbi;
ZeroMemory(&rbbi, sizeof(REBARBANDINFO));
rbbi.cbSize=REBARBANDINFO_V6_SIZE;
rbbi.fMask=RBBIM_CHILDSIZE;
rbbi.cxMinChild=iminwidth;
rbbi.cyChild=iminheight;
rbbi.cyMaxChild=iminheight;
rbbi.cyMinChild=iminheight;
SendMessage(Parent->GetHandle(), RB_SETBANDINFO, uband, (LPARAM)&rbbi);
}

BOOL ToolBand::ShowButton(WORD uid, BOOL bvisible)
{
auto hitem=GetItem(uid);
if(!hitem)
	return false;
hitem->Visible=bvisible;
return true;
}


//================
// Common Private
//================

INT ToolBand::AddIcon(WORD unormal, WORD udisabled)
{
if(!unormal)
	return -1;
UINT uicosize=GetSystemMetrics(SM_CXSMICON);
Handle<Icon> hico=new Icon(unormal, uicosize);
hImgList->AddIcon(hico->GetHandle());
if(udisabled)
	{
	hico=new Icon(udisabled, uicosize);
	return hImgListDisabled->AddIcon(hico->GetHandle());
	}
Handle<Bitmap> hbmp=new Bitmap(hico->GetHandle());
for(UINT y=0; y<hbmp->GetHeight(); y++)
	{
	for(UINT x=0; x<hbmp->GetWidth(); x++)
		{
		COLORREF c=hbmp->GetPixel(x, y);
		auto pc=(BYTE*)&c;
		BYTE b=(UINT)(pc[0]+pc[1]+pc[2])/3;
		if(b>=0x80)
			{
			b=(BYTE)(128.f+(b-128.f)*0.5f);
			}
		else
			{
			b=(BYTE)(128.f-(128.f-b)*0.5f);
			}
		b=min(b+0x30, 0xFF);
		pc[0]=pc[1]=pc[2]=b;
		hbmp->SetPixel(x, y, c);
		}
	}
hico=new Icon(hbmp->GetHandle());
return hImgListDisabled->AddIcon(hico->GetHandle());
}

VOID ToolBand::OnVisibleChanged(BOOL bvisible)
{
UINT ucount=Parent->GetBandCount();
REBARBANDINFO rbbi;
ZeroMemory(&rbbi, sizeof(REBARBANDINFO));
rbbi.cbSize=REBARBANDINFO_V6_SIZE;
rbbi.fMask=RBBIM_ID|RBBIM_STYLE;
for(UINT u=0; u<ucount; u++)
	{
	SetLastError(0);
	if(!SendMessage(Parent->GetHandle(), RB_GETBANDINFO, u, (LPARAM)&rbbi))
		break;
	if(rbbi.wID!=uId)
		continue;
	rbbi.fMask=RBBIM_STYLE;
	SetFlag(rbbi.fStyle, RBBS_HIDDEN, !bvisible);
	if(SendMessage(Parent->GetHandle(), RB_SETBANDINFO, u, (LPARAM)&rbbi))
		break;
	Parent->InvalidateArrangement();
	break;
	}
}

VOID ToolBand::OnVisibleReading(BOOL& bvisible)
{
UINT ucount=Parent->GetBandCount();
REBARBANDINFO rbbi;
ZeroMemory(&rbbi, sizeof(REBARBANDINFO));
rbbi.cbSize=REBARBANDINFO_V6_SIZE;
rbbi.fMask=RBBIM_ID|RBBIM_STYLE;
for(UINT u=0; u<ucount; u++)
	{
	SetLastError(0);
	if(!SendMessage(Parent->GetHandle(), RB_GETBANDINFO, u, (LPARAM)&rbbi))
		break;
	if(rbbi.wID!=uId)
		continue;
	bvisible=!(rbbi.fStyle&RBBS_HIDDEN);
	break;
	}
}

}}}
