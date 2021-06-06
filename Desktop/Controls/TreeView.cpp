//==============
// TreeView.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"
#include "TreeView.h"

using namespace Graphics::Gdi;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

TreeView::TreeView(Handle<Container> hparent, BOOL bborder):
Control(hparent)
{
SetLastError(0);
INITCOMMONCONTROLSEX icex;
icex.dwSize=sizeof(INITCOMMONCONTROLSEX);
icex.dwICC=ICC_TREEVIEW_CLASSES;
InitCommonControlsEx(&icex);
DWORD dwerr=GetLastError();
switch(dwerr)
	{
	case ERROR_SUCCESS:
		break;
	case ERROR_CLASS_ALREADY_EXISTS:
		break;
	default:
		{
		DebugPrint("InitCommonControlsEx() failed\n");
		return;
		}
	}
WindowInfo info;
info.BackgroundBrush=new GdiBrush(GetSysColor(COLOR_WINDOW));
info.Class=WC_TREEVIEW;
info.Style=TVS_HASBUTTONS|TVS_LINESATROOT|TVS_SHOWSELALWAYS|WS_VISIBLE;
if(bborder)
	info.StyleEx|=WS_EX_STATICEDGE;
Create(info);
//SendMessage(hWindow, TVM_SETEXTENDEDSTYLE, TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);
UINT icosize=GetSystemMetrics(SM_CXSMICON);
hImgListNormal=new ImageList(icosize, icosize, ILC_COLOR24, 0, 0);
hImgListNormal->SetBackgroundColor(GetSysColor(COLOR_WINDOW));
hImgListState=new ImageList(icosize, icosize, ILC_COLOR24, 0, 0);
hImgListState->SetBackgroundColor(GetSysColor(COLOR_WINDOW));
SendMessage(hWindow, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)hImgListNormal->GetHandle());
SendMessage(hWindow, TVM_SETIMAGELIST, TVSIL_STATE, (LPARAM)hImgListState->GetHandle());
}


//========
// Common
//========

UINT TreeView::AddIcon(UINT unormal, UINT ustate)
{
ASSERT(unormal);
if(!ustate)
	ustate=unormal;
UINT uid=hImgListNormal->AddIcon(unormal);
hImgListState->AddIcon(ustate);
return uid;
}

HTREEITEM TreeView::AddItem(HTREEITEM hparent, Handle<String> htext, UINT uico)
{
TVINSERTSTRUCT cinfo;
ZeroMemory(&cinfo, sizeof(TVINSERTSTRUCT));
cinfo.hParent=hparent;
cinfo.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
cinfo.item.iImage=uico;
cinfo.item.iSelectedImage=uico;
cinfo.item.pszText=(LPTSTR)htext->Begin();
return InsertItem(cinfo);
}

UINT TreeView::GetChildCount(HTREEITEM hitem)
{
HTREEITEM hchild=GetNextItem(hitem, TVGN_CHILD);
if(!hchild)
	return 0;
UINT ucount=0;
for(; hchild!=0; ucount++)
	hchild=GetNextItem(hchild, TVGN_NEXT);
return ucount;
}

Handle<TreeView::ItemList> TreeView::GetChildItems(HTREEITEM hitem)
{
Handle<ItemList> hitems=new ItemList();
UINT ucount=0;
HTREEITEM hchild=GetNextItem(hitem, TVGN_CHILD);
while(hchild)
	{
	hitems->Append(hchild);
	hchild=GetNextItem(hchild, TVGN_NEXT);
	}
return hitems;
}

Handle<String> TreeView::GetItemText(HTREEITEM hitem)
{
TVITEMEX tvi;
ZeroMemory(&tvi, sizeof(TVITEMEX));
tvi.mask=TVIF_TEXT;
tvi.hItem=hitem;
GetItem(tvi);
UINT ulen=tvi.cchTextMax;
Handle<String> htext=new String(ulen+1, nullptr);
LPTSTR ptext=(LPTSTR)htext->Begin();
tvi.pszText=ptext;
GetItem(tvi);
ptext[ulen]=0;
return htext;
}

UINT TreeView::GetLevel(HTREEITEM hitem)
{
UINT ulevel=0;
while(hitem!=0)
	{
	hitem=GetParentItem(hitem);
	if(!hitem)
		break;
	ulevel++;
	}
return ulevel;
}

HTREEITEM TreeView::GetParentItem(HTREEITEM hitem, UINT level)
{
if(!hitem)
	return 0;
for(UINT u=0; u<=level; u++)
	{
	hitem=GetNextItem(hitem, TVGN_PARENT);
	if(u==level)
		return hitem;
	}
return 0;
}

Handle<TreeView::ItemList> TreeView::GetSelectedItems()
{
Handle<ItemList> hitems=new ItemList();
HTREEITEM hsel=GetNextItem(0, TVGN_NEXTSELECTED);
while(hsel)
	{
	hitems->Append(hsel);
	hsel=GetNextItem(hsel, TVGN_NEXTSELECTED);
	}
return hitems;
}

HTREEITEM TreeView::InsertItem(TVINSERTSTRUCT const& cinfo)
{
HTREEITEM hitem=(HTREEITEM)SendMessage(hWindow, TVM_INSERTITEM, 0, (LPARAM)&cinfo);
if(cinfo.hParent)
	{
	TVITEMEX tvi;
	ZeroMemory(&tvi, sizeof(TVITEMEX));
	tvi.hItem=cinfo.hParent;
	tvi.mask=TVIF_CHILDREN;
	tvi.cChildren=1;
	SetItem(tvi);
	}
return hitem;
}

VOID TreeView::SetItemText(HTREEITEM hitem, Handle<String> htext)
{
TVITEMEX tvi;
ZeroMemory(&tvi, sizeof(TVITEMEX));
tvi.mask=TVIF_TEXT;
tvi.hItem=hitem;
tvi.pszText=(LPTSTR)htext->Begin();
SendMessage(hWindow, TVM_SETITEM, 0, (LPARAM)&tvi);
}

}}
