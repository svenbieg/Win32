//=============
// TabView.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include <CommCtrl.h>
#include "TabView.h"

using namespace Graphics::Gdi;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

TabView::TabView(Handle<Container> hparent):
Core::Controls::Container(hparent),
Container(hparent)
{
SetLastError(0);
INITCOMMONCONTROLSEX icex;
icex.dwSize=sizeof(INITCOMMONCONTROLSEX);
icex.dwICC=ICC_TAB_CLASSES;
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
pParent->ItemDrawed.Add(this, &TabView::OnParentItemDrawed);
pParent->Notified.Add(this, &TabView::OnParentNotified);
WindowInfo info;
info.Class=WC_TABCONTROL;
info.Style=TCS_MULTILINE|TCS_OWNERDRAWFIXED|WS_CLIPCHILDREN|WS_VISIBLE;
Create(info);
UINT icosize=GetSystemMetrics(SM_CXSMICON);
hImgList=new ImageList(icosize, icosize, ILC_COLOR24, 0, 0);
SendMessage(hWindow, TCM_SETIMAGELIST, 0, (LPARAM)hImgList->GetHandle());
}

TabView::~TabView()
{
pParent->ItemDrawed.Remove(this);
pParent->Notified.Remove(this);
}


//========
// Common
//========

UINT TabView::AddTab(Handle<Control> hctl, Handle<String> htext, UINT uicon)
{
hctl->SetStyle(WS_CLIPSIBLINGS, true);
TCITEM item;
ZeroMemory(&item, sizeof(TCITEM));
item.mask=TCIF_PARAM|TCIF_TEXT;
item.lParam=(LPARAM)(Control*)hctl;
item.pszText=(LPSTR)htext->Begin();
if(uicon)
	{
	item.iImage=hImgList->AddIcon(uicon);
	item.mask|=TCIF_IMAGE;
	}
UINT ucount=GetTabCount();
UINT uid=(UINT)SendMessage(hWindow, TCM_INSERTITEM, ucount, (LPARAM)&item);
if(!hCurrent)
	{
	hCurrent=hctl;
	InvalidateArrangement();
	}
return uid;
}

VOID TabView::Clear()
{
if(!hWindow)
	return;
SendMessage(hWindow, TCM_DELETEALLITEMS, 0, 0);
hCurrent=nullptr;
hImgList=nullptr;
}

UINT TabView::GetCurrentId()
{
if(!hWindow)
	return 0;
return (UINT)SendMessage(hWindow, TCM_GETCURSEL, 0, 0);
}

UINT TabView::GetTabCount()
{
if(!hWindow)
	return 0;
return (UINT)SendMessage(hWindow, TCM_GETITEMCOUNT, 0, 0);
}

Handle<String> TabView::GetTabText(UINT uid)
{
CHAR pstr[64];
TCITEM citem;
ZeroMemory(&citem, sizeof(TCITEM));
citem.cchTextMax=64;
citem.mask=TCIF_TEXT;
citem.pszText=pstr;
SendMessage(hWindow, TCM_GETITEM, uid, (LPARAM)&citem);
return pstr;
}

VOID TabView::Rearrange(Handle<DeviceContext> hdc, RECT& rc)
{
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hchild=hit->GetCurrent();
	hchild->Visible=(hchild==hCurrent);
	}
if(!hCurrent)
	return;
RECT rcc(rc);
SendMessage(hWindow, TCM_ADJUSTRECT, 0, (LPARAM)&rcc);
RECT const& padding=Padding.GetInternal();
rcc.Left+=padding.Left;
rcc.Top+=padding.Top;
rcc.Right-=padding.Right;
rcc.Bottom-=padding.Bottom;
hCurrent->Move(rcc);
}

VOID TabView::RemoveTab(UINT uid)
{
if(!hWindow)
	return;
SendMessage(hWindow, TCM_DELETEITEM, uid, 0);
}

VOID TabView::SetCurrent(UINT uid)
{
if(!hWindow)
	return;
SendMessage(hWindow, TCM_SETCURSEL, uid, 0);
}


//================
// Common Private
//================

VOID TabView::OnParentItemDrawed(Handle<Window> hsender, DRAWITEMSTRUCT const* pinfo)
{
if(pinfo->hwndItem!=hWindow)
	return;
HBRUSH hbr=GetSysColorBrush(COLOR_BTNFACE);
FillRect(pinfo->hDC, &pinfo->rcItem, hbr);
RECT const& rc=GetRect();
::RECT rcc({ 0, 0, rc.Right-rc.Left, rc.Bottom-rc.Top });
rcc.top+=pinfo->rcItem.bottom;
FillRect(pinfo->hDC, &rcc, hbr);
Handle<GdiFont> hfont=Font.As<GdiFont>();
SelectObject(pinfo->hDC, hfont->GetHandle());
auto htext=GetTabText(pinfo->itemID);
Handle<GdiContext> hdc=new GdiContext(pinfo->hDC);
SIZE textsize=hdc->MeasureText(Font, htext);
INT x=pinfo->rcItem.left+(pinfo->rcItem.right-pinfo->rcItem.left-textsize.Width)/2;
INT y=pinfo->rcItem.top+1+(pinfo->rcItem.bottom-pinfo->rcItem.top-textsize.Height-1)/2;
TextOut(pinfo->hDC, x, y, htext->Begin(), htext->GetLength());
}

VOID TabView::OnParentNotified(Handle<Window> hsender, NMHDR const* pinfo, BOOL& bhandled, LRESULT& status)
{
if(pinfo->hwndFrom!=hWindow)
	return;
switch(pinfo->code)
	{
	case TCN_SELCHANGE:
		{
		UINT uid=(UINT)SendMessage(hWindow, TCM_GETCURSEL, 0, 0);
		TCITEM item;
		ZeroMemory(&item, sizeof(TCITEM));
		item.mask=TCIF_PARAM;
		SendMessage(hWindow, TCM_GETITEM, uid, (LPARAM)&item);
		Handle<Control> hnew=(Control*)item.lParam;
		if(hnew==hCurrent)
			break;
		if(hCurrent)
			hCurrent->Visible=false;
		hCurrent=hnew;
		hCurrent->Visible=true;
		SendMessage(hCurrent->GetHandle(), WM_SETFOCUS, 0, 0);
		SelectionChanged(this);
		break;
		}
	}
}

}}