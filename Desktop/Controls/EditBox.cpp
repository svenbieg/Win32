//=============
// EditBox.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"
#include "EditBox.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

EditBox::EditBox(Handle<Container> hparent):
Interactive(hparent),
ReadOnly(this, false)
{
pParent->Command.Add(this, &EditBox::OnParentCommand);
ReadOnly.Changed.Add(this, &EditBox::OnReadOnlyChanged);
WindowInfo info;
info.Class="EDIT";
info.Style=ES_MULTILINE|WS_VISIBLE;
Create(info);
}

EditBox::~EditBox()
{
pParent->Command.Remove(this);
}


//========
// Common
//========

VOID EditBox::Clear()
{
if(!hWindow)
	return;
SendMessage(hWindow, WM_SETTEXT, 0, (LPARAM)nullptr);
}

Handle<String> EditBox::GetText()
{
if(!hWindow)
	return nullptr;
UINT ulen=(UINT)SendMessage(hWindow, WM_GETTEXTLENGTH, 0, 0);
Handle<String> hstr=new String(ulen+1, nullptr);
SendMessage(hWindow, WM_GETTEXT, ulen+1, (LPARAM)hstr->Begin());
return hstr;
}

VOID EditBox::Write(Handle<String> hstr)
{
if(!hWindow||!hstr)
	return;
UINT ustart=0;
UINT uend=0;
SendMessage(hWindow, EM_GETSEL, (WPARAM)&ustart, (LPARAM)&uend);
UINT ulen=(UINT)SendMessage(hWindow, WM_GETTEXTLENGTH, 0, 0);
SendMessage(hWindow, EM_SETSEL, ulen, ulen);
SendMessage(hWindow, EM_REPLACESEL, true, (LPARAM)hstr->Begin());
SendMessage(hWindow, EM_SETSEL, ustart, uend);
}

VOID EditBox::WriteLine(Handle<String> hstr)
{
if(!hWindow)
	return;
UINT ustart=0;
UINT uend=0;
SendMessage(hWindow, EM_GETSEL, (WPARAM)&ustart, (LPARAM)&uend);
UINT ulen=(UINT)SendMessage(hWindow, WM_GETTEXTLENGTH, 0, 0);
SendMessage(hWindow, EM_SETSEL, ulen, ulen);
if(hstr)
	SendMessage(hWindow, EM_REPLACESEL, true, (LPARAM)hstr->Begin());
SendMessage(hWindow, EM_REPLACESEL, true, (LPARAM)"\n");
SendMessage(hWindow, EM_SETSEL, ustart, uend);
}


//================
// Common Private
//================

VOID EditBox::OnParentCommand(Handle<Window> hsender, Handle<Control> hcontrol, UINT ucmd, BOOL& bhandled)
{
if(hcontrol!=this)
	return;
switch(HIWORD(ucmd))
	{
	case EN_CHANGE:
		{
		TextChanged(this);
		break;
		}
	}
}

VOID EditBox::OnReadOnlyChanged(BOOL breadonly)
{
if(!hWindow)
	return;
SendMessage(hWindow, EM_SETREADONLY, breadonly, 0);
}

}}
