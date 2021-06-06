//==============
// CheckBox.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "CheckBox.h"
#include "Container.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

CheckBox::CheckBox(Handle<Container> hparent, Handle<String> htext):
Interactive(hparent),
Checked(this, false),
Text(this, htext),
bChanging(false)
{
if(pParent)
	pParent->Command.Add(this, &CheckBox::OnParentCommand);
Checked.Changed.Add(this, &CheckBox::OnCheckedChanged);
Text.Changed.Add(this, &CheckBox::OnTextChanged);
WindowInfo info;
info.Class="BUTTON";
info.Style=BS_AUTOCHECKBOX|WS_TABSTOP|WS_VISIBLE;
info.Text=htext;
Create(info);
}

CheckBox::~CheckBox()
{
if(pParent)
	pParent->Command.Remove(this);
}


//==================
// Common Protected
//==================

Graphics::SIZE CheckBox::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize=Interactive::GetMinSizeInternal(hdc);
SIZE textsize=hdc->MeasureText(Font, Text);
minsize.Width+=GetSystemMetrics(SM_CXVSCROLL)+textsize.Width;
minsize.Height+=MAX(textsize.Height, GetSystemMetrics(SM_CXVSCROLL));
return minsize;
}


//================
// Common Private
//================

VOID CheckBox::OnCheckedChanged(BOOL bchecked)
{
if(!bChanging)
	SendMessage(hWindow, BM_SETCHECK, bchecked, 0);
}

VOID CheckBox::OnParentCommand(Handle<Window> hsender, Handle<Control> hcontrol, UINT ucmd, BOOL& bhandled)
{
if(bhandled)
	return;
if(hcontrol!=this)
	return;
bhandled=true;
switch(HIWORD(ucmd))
	{
	case BN_CLICKED:
		{
		bChanging=true;
		BOOL bchecked=(BOOL)SendMessage(hWindow, BM_GETCHECK, 0, 0);
		Checked=bchecked;
		bChanging=false;
		break;
		}
	default:
		{
		bhandled=false;
		break;
		}
	}
}

VOID CheckBox::OnTextChanged(Handle<String> htext)
{
if(!hWindow)
	return;
SendMessage(hWindow, WM_SETTEXT, 0, (LPARAM)htext->Begin());
}

}}
