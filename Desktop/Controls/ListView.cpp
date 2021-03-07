//==============
// ListView.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"
#include "ListView.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

ListView::ListView(Handle<Container> hparent):
CoreWindow(hparent),
CoreControl(hparent),
Control(hparent),
uColumnCount(0),
uSortColumn(0)
{
SetLastError(0);
INITCOMMONCONTROLSEX icex;
icex.dwSize=sizeof(INITCOMMONCONTROLSEX);
icex.dwICC=ICC_LISTVIEW_CLASSES;
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
		DebugPrint("InitCommonControlsEx failed (%xh)\n", dwerr);
		return;
		}
	}
pParent->Notified.Add(this, &ListView::OnParentNotified);
WindowInfo info;
info.BackgroundBrush=new GdiBrush(GetSysColor(COLOR_WINDOW));
info.Class=WC_LISTVIEW;
info.Style=LVS_REPORT|WS_CLIPCHILDREN|WS_VISIBLE;
Create(info);
UINT icosize=GetSystemMetrics(SM_CXSMICON);
hImgListNormal=new ImageList(icosize, icosize, ILC_COLOR24, 0, 0);
hImgListNormal->SetBackgroundColor(GetSysColor(COLOR_WINDOW));
hImgListState=new ImageList(icosize, icosize, ILC_COLOR24, 0, 0);
hImgListState->SetBackgroundColor(GetSysColor(COLOR_WINDOW));
SendMessage(hWindow, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)hImgListNormal->GetHandle());
SendMessage(hWindow, LVM_SETIMAGELIST, LVSIL_STATE, (LPARAM)hImgListState->GetHandle());
}

ListView::~ListView()
{
pParent->Notified.Remove(this);
}


//========
// Common
//========

UINT ListView::AddColumn(Handle<String> htext, UINT uwidth)
{
if(!htext)
	return -1;
LVCOLUMN cinfo;
ZeroMemory(&cinfo, sizeof(LVCOLUMN));
cinfo.mask=LVCF_MINWIDTH|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
cinfo.cx=uwidth;
cinfo.cxMin=20;
cinfo.iSubItem=uColumnCount;
cinfo.pszText=(LPSTR)htext->Begin();
InsertColumn(uColumnCount, cinfo);
return cinfo.iSubItem;
}

UINT ListView::AddIcon(UINT unormal, UINT ustate)
{
ASSERT(unormal);
if(!ustate)
	ustate=unormal;
UINT uid=hImgListNormal->AddIcon(unormal);
hImgListState->AddIcon(ustate);
return uid;
}

VOID ListView::Clear()
{
ClearItems();
ClearIcons();
}

VOID ListView::ClearIcons()
{
hImgListNormal->Clear();
hImgListState->Clear();
}

VOID ListView::ClearItems()
{
SendMessage(hWindow, LVM_DELETEALLITEMS, 0, 0);
}

VOID ListView::ClearSelection()
{
SendMessage(hWindow, LVM_SETSELECTIONMARK, 0, -1);
}

UINT ListView::GetColumnWidth(UINT uid)
{
LVCOLUMN cinfo;
ZeroMemory(&cinfo, sizeof(LVCOLUMN));
cinfo.mask=LVCF_SUBITEM|LVCF_WIDTH;
for(UINT u=0; u<uColumnCount; u++)
	{
	SendMessage(hWindow, LVM_GETCOLUMN, u, (LPARAM)&cinfo);
	if(cinfo.iSubItem==uid)
		return cinfo.cx;
	}
return 0;
}

UINT ListView::GetItemCount()
{
return (UINT)SendMessage(hWindow, LVM_GETITEMCOUNT, 0, 0);
}

Handle<String> ListView::GetItemText(UINT uitem, UINT ucol)
{
LVITEM cinfo;
ZeroMemory(&cinfo, sizeof(LVITEM));
cinfo.mask=LVIF_TEXT;
cinfo.iItem=uitem;
cinfo.iSubItem=ucol;
SendMessage(hWindow, LVM_GETITEM, 0, (LPARAM)&cinfo);
UINT ulen=cinfo.cchTextMax;
Handle<String> hstr=new String(ulen+1, nullptr);
LPSTR pstr=(LPSTR)hstr->Begin();
cinfo.pszText=pstr;
SendMessage(hWindow, LVM_GETITEM, 0, (LPARAM)&cinfo);
pstr[ulen]=0;
return hstr;
}

ListViewMode ListView::GetMode()
{
return (ListViewMode)SendMessage(hWindow, LVM_GETVIEW, 0, 0);
}

UINT ListView::GetSelectedCount()
{
return (UINT)SendMessage(hWindow, LVM_GETSELECTEDCOUNT, 0, 0);
}

Handle<ListView::SelectionList> ListView::GetSelection()
{
UINT ucount=(UINT)SendMessage(hWindow, LVM_GETSELECTEDCOUNT, 0, 0);
Handle<SelectionList> hsel=new SelectionList(ucount);
INT id=0;
for(UINT u=0; u<ucount; u++)
	{
	id=(INT)SendMessage(hWindow, LVM_GETNEXTITEM, id, LVNI_SELECTED);
	hsel->SetAt(u, id);
	}
return hsel;
}

UINT ListView::InsertColumn(INT ipos, LVCOLUMN const& cinfo)
{
if(ipos<0)
	ipos=uColumnCount;
ipos=(INT)SendMessage(hWindow, LVM_INSERTCOLUMN, ipos, (LPARAM)&cinfo);
ASSERT(ipos>=0);
uColumnCount++;
return ipos;
}

UINT ListView::InsertItem(INT ipos, Handle<String> htext)
{
if(!htext)
	return -1;
LVITEM cinfo;
ZeroMemory(&cinfo, sizeof(LVITEM));
cinfo.mask=LVIF_TEXT;
cinfo.iItem=ipos;
cinfo.pszText=(LPSTR)htext->Begin();
return (UINT)SendMessage(hWindow, LVM_INSERTITEM, 0, (LPARAM)&cinfo);
}

UINT ListView::InsertItem(INT ipos, Handle<String> htext, UINT uico)
{
if(!htext)
	return -1;
LVITEM cinfo;
ZeroMemory(&cinfo, sizeof(LVITEM));
cinfo.mask=LVIF_IMAGE|LVIF_TEXT;
cinfo.iImage=uico;
cinfo.iItem=ipos;
cinfo.pszText=(LPSTR)htext->Begin();
return (UINT)SendMessage(hWindow, LVM_INSERTITEM, 0, (LPARAM)&cinfo);
}

VOID ListView::RemoveItem(UINT uid)
{
SendMessage(hWindow, LVM_DELETEITEM, uid, 0);
}

VOID ListView::SetItemIcon(UINT uitem, UINT uico)
{
LVITEM cinfo;
ZeroMemory(&cinfo, sizeof(LVITEM));
cinfo.mask=LVIF_IMAGE;
cinfo.iImage=uico;
cinfo.iItem=uitem;
SendMessage(hWindow, LVM_SETITEM, 0, (LPARAM)&cinfo);
}

VOID ListView::SetItemText(UINT uitem, UINT ucol, Handle<String> htext)
{
if(!htext)
	return;
LVITEM cinfo;
ZeroMemory(&cinfo, sizeof(LVITEM));
cinfo.mask=LVIF_TEXT;
cinfo.iItem=uitem;
cinfo.iSubItem=ucol;
cinfo.pszText=(LPSTR)htext->Begin();
SendMessage(hWindow, LVM_SETITEM, 0, (LPARAM)&cinfo);
}

VOID ListView::SetMode(ListViewMode mode)
{
SendMessage(hWindow, LVM_SETVIEW, (UINT)mode, true);
}

VOID ListView::SortItems(UINT ucol)
{
if(ucol>=uColumnCount)
	ucol=0;
uSortColumn=ucol;
SendMessage(hWindow, LVM_SORTITEMSEX, (WPARAM)this, (LPARAM)CompareItems);
}


//================
// Common Private
//================

INT CALLBACK ListView::CompareItems(LPARAM lparam1, LPARAM lparam2, LPARAM llv)
{
Handle<ListView> hlv=(ListView*)llv;
auto hstr1=hlv->GetItemText((UINT)lparam1, hlv->uSortColumn);
auto hstr2=hlv->GetItemText((UINT)lparam2, hlv->uSortColumn);
return StringCompare(hstr1->Begin(), hstr2->Begin());
}

VOID ListView::OnEditLabelsChanged(Handle<Variable> hvar)
{
SetStyle(LVS_EDITLABELS, EditLabels);
}

VOID ListView::OnEditLabelsReading(Handle<Variable> hvar, BOOL& bvalue)
{
bvalue=GetStyle(LVS_EDITLABELS);
}

VOID ListView::OnFocusItemChanged(Handle<Variable> hvar)
{
SendMessage(hWindow, LVM_SETSELECTIONMARK, 0, FocusItem->Get());
}

VOID ListView::OnFocusItemReading(Handle<Variable> hvar, INT& ivalue)
{
ivalue=(INT)SendMessage(hWindow, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
}

VOID ListView::OnParentNotified(Handle<Window> hsender, NMHDR const* phdr, BOOL& bhandled, LRESULT& status)
{
auto pinfo=(NMLVDISPINFO*)phdr;
if(pinfo->hdr.hwndFrom!=hWindow)
	return;
switch(pinfo->hdr.code)
	{
	case LVN_BEGINLABELEDIT:
		{
		hOldLabel=pinfo->item.pszText;
		BeginLabelEdit(this, pinfo->item.iItem, hOldLabel);
		break;
		}
	case LVN_DELETEITEM:
		{
		ItemDeleted(this, pinfo->item.iItem);
		break;
		}
	case LVN_ENDLABELEDIT:
		{
		if(!pinfo->item.pszText)
			break;
		Handle<String> hnewlabel=new String(pinfo->item.pszText);
		EndLabelEdit(this, pinfo->item.iItem, hOldLabel, hnewlabel);
		break;
		}
	case LVN_ITEMACTIVATE:
		{
		ItemActivated(this, pinfo->item.iItem);
		break;
		}
	case LVN_ITEMCHANGED:
		{
		SelectionChanged(this, pinfo->item.iItem);
		break;
		}
	}
}

}}
