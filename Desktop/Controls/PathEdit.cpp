//==============
// PathEdit.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include <ShlObj.h>
#include "Core/Application.h"
#include "Desktop/Dialogs/SaveFileDialog.h"
#include "Desktop/Dialogs/SelectFolderDialog.h"
#include "Container.h"
#include "PathEdit.h"

using namespace Core;
using namespace Desktop::Dialogs;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

PathEdit::PathEdit(Handle<Container> hparent, Handle<String> hpath):
Core::Controls::Container(hparent),
Container(hparent),
Button(this, true),
Directory(false),
uPathEditFlags(PathEditFlags::None)
{
Button.Changed.Add(this, &PathEdit::OnButtonChanged);
Command.Add(this, &PathEdit::OnCommand);
WindowInfo info;
info.Class="PathEdit";
info.Style=WS_CLIPCHILDREN|WS_VISIBLE;
info.StyleEx=WS_EX_CONTROLPARENT;
Create(info);
hEdit=new Edit(this, hpath);
hButton=new Controls::Button(this, "..");
hButton->Margin={ 2, 1, 0, 1 };
hButton->MinSize.Width=0;
hButton->Padding={ 6, 0, 6, 0 };
hButton->TabStop=false;
}


//========
// Common
//========

Handle<String> PathEdit::GetPath()
{
return hEdit->GetText();
}

VOID PathEdit::Rearrange(Handle<DeviceContext> hdc, RECT& rc)
{
Container::Rearrange(hdc, rc);
BOOL bbtn=Button.GetInternal();
UINT ubtnwidth=0;
if(bbtn)
	{
	SIZE size=hButton->GetMinSize(hdc);
	RECT const& margin=hButton->Margin.GetInternal();
	ubtnwidth=margin.Left+size.Width+margin.Right;
	}
RECT rce(rc);
rce.Right-=ubtnwidth;
hEdit->Move(rce);
hButton->Visible=bbtn;
if(bbtn)
	{
	RECT rcb(rc);
	rcb.Left=rcb.Right-ubtnwidth;
	hButton->Move(rcb);
	}
}

VOID PathEdit::SetPath(Handle<String> hpath)
{
hEdit->SetText(hpath);
}


//==================
// Common Protected
//==================

Graphics::SIZE PathEdit::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize=Container::GetMinSize(hdc);
SIZE editsize=hEdit->GetMinSize(hdc);
minsize.Width+=300;
minsize.Height+=editsize.Height;
if(Button.GetInternal())
	{
	SIZE btnsize=hButton->GetMinSize(hdc);
	minsize.Width+=btnsize.Width;
	}
return minsize;
}


//================
// Common Private
//================

VOID PathEdit::OnButtonChanged(BOOL b)
{
InvalidateArrangement();
}

VOID PathEdit::OnCommand(Handle<Window> hsender, Handle<Control> hcontrol, UINT ucmd, BOOL& bhandled)
{
auto hpath=hEdit->GetText();
if(Directory)
	{
	hpath=SelectFolderDialog::Show(this, hpath);
	}
else
	{
	hpath=SaveFileDialog::Show(this, Application::Current->Name, hpath);
	}
if(hpath)
	hEdit->SetText(hpath);
}

}}
