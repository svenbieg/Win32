//==============
// ComboBox.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "ComboBox.h"
#include "Container.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

ComboBox::ComboBox(Handle<Container> hparent, BOOL bstatic, UINT ulines):
Interactive(hparent),
uLines(ulines)
{
if(pParent)
	pParent->Command.Add(this, &ComboBox::OnParentCommand);
WindowInfo info;
info.Class="COMBOBOX";
info.Style=CBS_DROPDOWN|WS_VISIBLE|WS_VSCROLL;
if(bstatic)
	info.Style|=CBS_DROPDOWNLIST;
Create(info);
}

ComboBox::~ComboBox()
{
if(pParent)
	pParent->Command.Remove(this);
}


//========
// Common
//========

INT ComboBox::Add(Handle<String> hline)
{
if(!hline)
	return -1;
return (INT)SendMessage(hWindow, CB_ADDSTRING, 0, (LPARAM)hline->Begin());
}

VOID ComboBox::Clear()
{
SendMessage(hWindow, CB_RESETCONTENT, 0, 0);
}

UINT ComboBox::GetCount()
{
return (UINT)SendMessage(hWindow, CB_GETCOUNT, 0, 0);
}

Handle<String> ComboBox::GetLine(UINT uline)
{
INT ilen=(INT)SendMessage(hWindow, CB_GETLBTEXTLEN, uline, 0);
if(ilen==0)
	return "";
Handle<String> hstr=new String(ilen+1, nullptr);
SendMessage(hWindow, CB_GETLBTEXT, uline, (LPARAM)hstr->Begin());
return hstr;
}

INT ComboBox::GetSelection()
{
return (INT)SendMessage(hWindow, CB_GETCURSEL, 0, 0);
}

VOID ComboBox::Set(Handle<String> hlines)
{
Clear();
if(!hlines)
	return;
LPCTSTR plines=hlines->Begin();
UINT ulen=StringLength(plines);
BOOL badded=false;
for(UINT upos=0; upos<ulen; )
	{
	UINT ucopy=0;
	if(!StringFind(&plines[upos], L"\1", &ucopy))
		ucopy=ulen-upos;
	if(!ucopy)
		{
		upos++;
		continue;
		}
	Handle<String> hline=new String(ucopy, &plines[upos]);
	Add(hline);
	upos+=ucopy+1;
	badded=true;
	}
if(badded)
	SetSelection(0);
}

VOID ComboBox::SetLines(UINT ulines)
{
uLines=ulines;
if(pParent)
	pParent->InvalidateArrangement();
}

VOID ComboBox::SetSelection(INT isel)
{
INT icur=(INT)SendMessage(hWindow, CB_GETCURSEL, 0, 0);
if(isel==icur)
	return;
SendMessage(hWindow, CB_SETCURSEL, isel, 0);
SelectionChanged(this);
}


//==================
// Common Protected
//==================

Graphics::SIZE ComboBox::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize=Interactive::GetMinSizeInternal(hdc);
UINT ubox=GetSystemMetrics(SM_CXVSCROLL);
minsize.Height+=MAX(Font->GetHeight(), ubox);
minsize.Width+=ubox;
return minsize;
}


//================
// Common Private
//================

VOID ComboBox::OnMoving(RECT& rc)
{
UINT uheight=rc.Bottom-rc.Top;
UINT ucount=GetCount();
UINT ulinecount=min(ucount, uLines);
UINT ulineheight=Font->GetHeight()+2;
rc.Bottom=rc.Top+uheight+ulinecount*ulineheight;
}

VOID ComboBox::OnParentCommand(Handle<Window> hsender, Handle<Control> hcontrol, UINT ucmd, BOOL& bhandled)
{
if(bhandled)
	return;
if(hcontrol!=this)
	return;
bhandled=true;
switch(HIWORD(ucmd))
	{
	case CBN_CLOSEUP:
		{
		ClosedUp(this);
		break;
		}
	case CBN_DROPDOWN:
		{
		DroppedDown(this);
		break;
		}
	case CBN_EDITCHANGE:
		{
		EditChanged(this);
		break;
		}
	case CBN_SELCHANGE:
		{
		SelectionChanged(this);
		break;
		}
	default:
		{
		bhandled=false;
		break;
		}
	}
}

}}
