//===============
// HyperLink.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include <shellapi.h>
#include "Container.h"
#include "HyperLink.h"

using namespace Graphics::Gdi;
using namespace Graphics;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

HyperLink::HyperLink(Handle<Container> hparent, Handle<String> htext, Handle<String> hlink):
Interactive(hparent),
Color(this, Colors::Blue),
Link(this, hlink),
Text(this, htext)
{
if(!DefaultCursor)
	DefaultCursor=new GdiCursor(Cursors::Hand);
if(!DefaultFont)
	DefaultFont=new GdiFont(Fonts::HyperLinkFont);
Painted.Add(this, &HyperLink::OnPainted);
PointerClicked.Add(this, &HyperLink::OnPointerClicked);
Text.Changed.Add(this, &HyperLink::OnTextChanged);
WindowInfo info;
info.Cursor=DefaultCursor;
info.Font=DefaultFont;
info.Class="HyperLink";
info.Style=WS_VISIBLE;
Create(info);
}


//========
// Common
//========

Handle<Cursor> HyperLink::DefaultCursor;

Handle<Gdi::Font> HyperLink::DefaultFont;


//==================
// Common Protected
//==================

Graphics::SIZE HyperLink::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize=Interactive::GetMinSize(hdc);
SIZE textsize=hdc->MeasureText(Font, Text);
minsize.Width=textsize.Width;
minsize.Height=textsize.Height;
return minsize;
}


//================
// Common Private
//================

VOID HyperLink::OnPainted(Handle<Window> hsender, Handle<GdiContext> hdc)
{
if(!Text)
	return;
RECT const& rc=hdc->Clip;
RECT rcc(rc);
RECT const& padding=Padding.GetInternal();
rcc.Left+=padding.Left;
rcc.Top+=padding.Top;
rcc.Right-=padding.Right;
rcc.Bottom-=padding.Bottom;
hdc->DrawText(Text, Color, rcc);
}

VOID HyperLink::OnPointerClicked(POINT pt, UINT id)
{
if(id!=0)
	return;
Handle<String> hlink=Link;
if(hlink)
	ShellExecute(NULL, "open", hlink->Begin(), nullptr, nullptr, SW_SHOW);
}

VOID HyperLink::OnTextChanged(Handle<String> htext)
{
if(pParent)
	{
	pParent->InvalidateArrangement();
	return;
	}
Invalidate();
}

}}