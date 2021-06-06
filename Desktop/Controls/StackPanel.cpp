//================
// StackPanel.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "StackPanel.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

StackPanel::StackPanel(Handle<Container> hparent, StackOrientation or):
Core::Controls::Container(hparent),
Container(hparent),
uOrientation(or)
{
WindowInfo info;
info.Class="StackPanel";
info.Style=WS_CLIPCHILDREN|WS_VISIBLE;
info.StyleEx=WS_EX_CONTROLPARENT;
Create(info);
}


//========
// Common
//========

VOID StackPanel::Rearrange(Handle<DeviceContext> hdc, RECT& rc)
{
Container::Rearrange(hdc, rc);
RECT rcc(rc);
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hchild=hit->GetCurrent();
	if(!hchild->Visible.GetInternal())
		continue;
	SIZE msc=hchild->GetMinSize(hdc);
	rcc.Right=rcc.Left+msc.Width;
	rcc.Bottom=rcc.Top+msc.Height;
	hchild->Move(rcc);
	if(uOrientation==StackOrientation::Horizontal)
		{
		rcc.Left=rcc.Right;
		}
	else
		{
		rcc.Top=rcc.Bottom;
		}
	}
}


//==================
// Common Protected
//==================

Graphics::SIZE StackPanel::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize=Container::GetMinSizeInternal(hdc);
if(uOrientation==StackOrientation::Horizontal)
	{
	UINT uheight=0;
	for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
		{
		auto hchild=hit->GetCurrent();
		if(!hchild->Visible.GetInternal())
			continue;
		RECT const& margin=hchild->Margin.GetInternal();
		SIZE msc=hchild->GetMinSize(hdc);
		minsize.Width+=margin.Left+msc.Width+margin.Right;
		uheight=MAX(uheight, margin.Top+msc.Height+margin.Bottom);
		}
	minsize.Height+=uheight;
	}
else
	{
	UINT uwidth=0;
	for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
		{
		auto hchild=hit->GetCurrent();
		if(!hchild->Visible.GetInternal())
			continue;
		RECT const& margin=hchild->Margin.GetInternal();
		SIZE msc=hchild->GetMinSize(hdc);
		uwidth=MAX(uwidth, margin.Left+msc.Width+margin.Right);
		minsize.Height+=margin.Top+msc.Height+margin.Bottom;
		}
	minsize.Width+=uwidth;
	}
return minsize;
}

}}