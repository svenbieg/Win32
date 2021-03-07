//==========
// Menu.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "PopupMenu.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {
		namespace Menus {


//========
// Common
//========

VOID Menu::CheckItem(WORD uid, BOOL b)
{
auto hitem=GetItem(uid);
if(hitem)
	hitem->Checked=b;
}

VOID Menu::Clear()
{
if(!hMenu)
	return;
UINT ucount=GetItemCount();
for(UINT u=0; u<ucount; u++)
	DeleteMenu(hMenu, 0, MF_BYPOSITION);
Items->Clear();
}

VOID Menu::Destroy()
{
if(!hMenu)
	return;
if(Parent)
	{
	SetMenu(Parent->GetHandle(), NULL);
	Parent->MessageReceived.Remove(this);
	Parent=nullptr;
	}
DestroyMenu(hMenu);
hMenu=NULL;
Items=nullptr;
}

VOID Menu::EnableItem(WORD uid, BOOL b)
{
auto hitem=GetItem(uid);
if(hitem)
	hitem->Enabled=b;
}

Handle<MenuItem> Menu::GetItem(WORD uid)
{
for(auto hit=Items->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hitem=hit->GetCurrent();
	if(hitem->GetId()==uid)
		return hitem;
	auto hsubmenu=hitem->SubMenu;
	if(hsubmenu)
		{
		hitem=hsubmenu->GetItem(uid);
		if(hitem)
			return hitem;
		}
	}
return nullptr;
}

UINT Menu::GetItemCount()
{
return GetMenuItemCount(hMenu);
}

Handle<MenuItem> Menu::InsertItem(INT ipos, WORD uid, Handle<String> htext, WORD uico)
{
Handle<MenuItem> hitem=new MenuItem(this, ipos, uid, htext, uico);
Items->InsertAt(ipos, hitem);
Changed(this);
return hitem;
}

VOID Menu::SetItemIcon(WORD uid, WORD uico)
{
auto hitem=GetItem(uid);
if(hitem)
	hitem->SetIcon(uico);
}

VOID Menu::SetItemText(WORD uid, Handle<String> htext)
{
auto hitem=GetItem(uid);
if(hitem)
	hitem->Text=htext;
}


//============================
// Con-/Destructors Protected
//============================

Menu::Menu(Handle<Window> hparent):
Parent(hparent),
hMenu(NULL)
{
Items=new ItemList();
Parent->MessageReceived.Add(this, &Menu::OnParentMessageReceived);
}


//================
// Common Private
//================

VOID Menu::OnParentMessageReceived(Handle<Window> hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, BOOL& bhandled, LRESULT& status)
{
switch(umsg)
	{
	case WM_DESTROY:
		{
		Destroy();
		break;
		}
	case WM_MENUSELECT:
		{
		auto hmenu=(HMENU)lparam;
		if(hmenu!=hMenu)
			return;
		WORD uid=LOWORD(wparam);
		auto hitem=GetItem(uid);
		SelectionChanged(this, hitem);
		return;
		}
	}
}

}}}
