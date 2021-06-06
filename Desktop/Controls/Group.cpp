//===========
// Group.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Group.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Group::Group(Handle<Container> hparent, Handle<String> htitle):
Core::Controls::Container(hparent),
Container(hparent),
Title(this, htitle)
{
Padding={ 8, 8, 8, 8 };
Painted.Add(this, &Group::OnPainted);
Title.Changed.Add(this, &Group::OnTitleChanged);
WindowInfo info;
info.Class="Group";
info.Style=WS_CLIPCHILDREN|WS_VISIBLE;
info.StyleEx=WS_EX_CONTROLPARENT;
Create(info);
}


//========
// Common
//========

VOID Group::Rearrange(Handle<DeviceContext> hdc, RECT& rc)
{
auto hcontent=Children->GetAt(0);
if(!hcontent)
	return;
if(!hcontent->Visible.GetInternal())
	return;
Control::Rearrange(hdc, rc);
if(Title)
	rc.Top+=Font->GetHeight()+4;
RECT const& margin=hcontent->Margin.GetInternal();
rc.Left+=margin.Left;
rc.Top+=margin.Top;
rc.Right-=margin.Right;
rc.Bottom-=margin.Bottom;
hcontent->Move(rc);
}


//==================
// Common Protected
//==================

Graphics::SIZE Group::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize=Control::GetMinSize(hdc);
SIZE textsize=hdc->MeasureText(Font, Title);
minsize.Height+=textsize.Height;
auto hcontent=Children->GetAt(0);
if(hcontent)
	{
	if(hcontent->Visible.GetInternal())
		{
		SIZE msc=hcontent->GetMinSize(hdc);
		RECT const& margin=hcontent->Margin.GetInternal();
		minsize.Width+=MAX(textsize.Width, margin.Left+msc.Width+margin.Right);
		minsize.Height+=margin.Top+msc.Height+margin.Bottom;
		}
	}
return minsize;
}


//================
// Common Private
//================

VOID Group::OnPainted(Handle<Window> hsender, Handle<GdiContext> hdc)
{
RECT const& rc=hdc->Clip;
SIZE textsize(0, 0);
if(Title)
	textsize=hdc->MeasureText(Font, Title);
::RECT rcc({ rc.Left, rc.Top+(INT)textsize.Height/2, rc.Right, rc.Bottom });
DrawEdge(hdc->GetHandle(), &rcc, EDGE_ETCHED, BF_RECT);
if(!Title)
	return;
RECT rcdst;
rcdst.Left=rcc.right/2-textsize.Width/2;
rcdst.Top=0;
rcdst.Right=rcdst.Left+textsize.Width;
rcdst.Bottom=textsize.Height;
RECT rcfill(rcdst);
rcfill.Left-=2;
rcfill.Right+=2;
hdc->FillRect(rcfill, BackgroundBrush);
hdc->DrawText(Title, Colors::Black, rcdst);
}

VOID Group::OnTitleChanged(Handle<String> htitle)
{
InvalidateArrangement();
}

}}
