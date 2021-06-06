//===============
// Container.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Container::Container():
Container(Current, nullptr)
{}

Container::Container(Handle<Container> hparent, WindowInfo* pinfo):
Core::Controls::Container(hparent),
Control(hparent, pinfo)
{
MessageReceived.Add(this, &Container::OnMessageReceived);
}


//========
// Common
//========

Container* Container::Current=nullptr;

VOID Container::Repaint()
{
Control::Repaint();
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hchild=hit->GetCurrent().As<Control>();
	if(!hchild->Visible.GetInternal())
		continue;
	hchild->Repaint();
	}
}


//================
// Common Private
//================

VOID Container::OnMessageReceived(Handle<Window> hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, BOOL& bhandled, LRESULT& lr)
{
if(bhandled)
	return;
bhandled=true;
switch(umsg)
	{
	case WM_COMMAND:
		{
		HWND hctl=(HWND)lparam;
		Handle<Control> hcontrol=(Control*)GetProp(hctl, "WindowHandle");
		BOOL bcmd=false;
		Command(this, hcontrol, wparam, bcmd);
		if(!bcmd)
			bhandled=false;
		break;
		}
	case WM_CTLCOLORBTN:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORSTATIC:
		{
		auto hctl=(HWND)lparam;
		for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
			{
			Handle<Window> hchild=hit->GetCurrent().As<Window>();
			if(hchild->GetHandle()==hctl)
				{
				auto hbr=hchild->BackgroundBrush;
				HDC hdc=(HDC)wparam;
				SetBkColor(hdc, hbr->GetColor());
				lr=(LRESULT)hbr->GetHandle();
				break;
				}
			}
		break;
		}
	case WM_DRAWITEM:
		{
		auto pinfo=(const DRAWITEMSTRUCT*)lparam;
		ItemDrawed(this, pinfo);
		lr=true;
		break;
		}
	case WM_NOTIFY:
		{
		BOOL bnotified=false;
		LRESULT status=0;
		Notified(this, (NMHDR const*)lparam, bnotified, status);
		if(!bnotified)
			{
			bhandled=false;
			break;
			}
		lr=status;
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