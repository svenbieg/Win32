//==============
// MenuItem.cpp
//==============

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


//==================
// Con-/Destructors
//==================

MenuItem::MenuItem(Handle<Menu> hparent, INT ipos, WORD uid, Handle<String> htext, WORD uico):
Checked(this, false),
Enabled(this, true),
Icon(this),
Parent(hparent),
SubMenu(this),
Text(this, htext),
uId(uid)
{
MENUITEMINFO mii;
ZeroMemory(&mii, sizeof(MENUITEMINFO));
mii.cbSize=sizeof(MENUITEMINFO);
mii.fMask=MIIM_FTYPE|MIIM_ID;
mii.wID=uid;
if(!Text)
	{
	mii.fType=MFT_SEPARATOR;
	InsertMenuItem(Parent->GetHandle(), ipos, true, &mii);
	}
else
	{
	mii.fMask|=MIIM_STRING;
	mii.dwTypeData=(LPSTR)Text->Begin();
	if(uico)
		{
		UINT uicosize=GetSystemMetrics(SM_CXSMICON);
		Icon=new Graphics::Gdi::Icon(uico, uicosize);
		hBitmap=new Bitmap(Icon->GetHandle());
		mii.fMask|=MIIM_BITMAP;
		mii.hbmpItem=hBitmap->GetHandle();
		}
	InsertMenuItem(Parent->GetHandle(), ipos, true, &mii);
	}
Checked.Changed.Add(this, &MenuItem::OnCheckedChanged);
Checked.Reading.Add(this, &MenuItem::OnCheckedReading);
Enabled.Changed.Add(this, &MenuItem::OnEnabledChanged);
Enabled.Reading.Add(this, &MenuItem::OnEnabledReading);
Icon.Changed.Add(this, &MenuItem::OnIconChanged);
SubMenu.Changed.Add(this, &MenuItem::OnSubMenuChanged);
Text.Changed.Add(this, &MenuItem::OnTextChanged);
}


//========
// Common
//========

Handle<MenuItem> MenuItem::InsertItem(INT ipos, WORD uid, Handle<String> htext, WORD uico)
{
if(!SubMenu)
	SubMenu=new PopupMenu(Parent->Parent);
return SubMenu->InsertItem(ipos, uid, htext, uico);
}

VOID MenuItem::SetIcon(WORD uico)
{
UINT uicosize=GetSystemMetrics(SM_CXSMICON);
Icon=new Graphics::Gdi::Icon(uico, uicosize);
}


//================
// Common Private
//================

VOID MenuItem::OnCheckedChanged(BOOL bchecked)
{
CheckMenuItem(Parent->GetHandle(), uId, MF_BYCOMMAND|(bchecked? MF_CHECKED: MF_UNCHECKED));
Changed(this);
Parent->Notify();
}

VOID MenuItem::OnCheckedReading(BOOL& bchecked)
{
MENUITEMINFO mii;
ZeroMemory(&mii, sizeof(MENUITEMINFO));
mii.cbSize=sizeof(MENUITEMINFO);
mii.fMask=MIIM_STATE;
GetMenuItemInfo(Parent->GetHandle(), uId, false, &mii);
bchecked=mii.fState&MFS_CHECKED;
}

VOID MenuItem::OnEnabledChanged(BOOL benabled)
{
EnableMenuItem(Parent->GetHandle(), uId, MF_BYCOMMAND|(benabled? MFS_ENABLED: MFS_GRAYED));
Changed(this);
Parent->Notify();
}

VOID MenuItem::OnEnabledReading(BOOL& benabled)
{
MENUITEMINFO mii;
ZeroMemory(&mii, sizeof(MENUITEMINFO));
mii.cbSize=sizeof(MENUITEMINFO);
mii.fMask=MIIM_STATE;
GetMenuItemInfo(Parent->GetHandle(), uId, false, &mii);
benabled=!(mii.fState&MFS_DISABLED);
}

VOID MenuItem::OnIconChanged(Handle<Graphics::Gdi::Icon> hicon)
{
MENUITEMINFO mii;
ZeroMemory(&mii, sizeof(MENUITEMINFO));
mii.cbSize=sizeof(MENUITEMINFO);
mii.fMask=MIIM_BITMAP;
if(hicon)
	{
	hBitmap=new Bitmap(hicon->GetHandle());
	mii.hbmpItem=hBitmap->GetHandle();
	}
else
	{
	hBitmap=nullptr;
	}
SetMenuItemInfo(Parent->GetHandle(), uId, false, &mii);
Changed(this);
Parent->Notify();
}

VOID MenuItem::OnSubMenuChanged(Handle<PopupMenu> hsubmenu)
{
MENUITEMINFO mii;
ZeroMemory(&mii, sizeof(MENUITEMINFO));
mii.cbSize=sizeof(MENUITEMINFO);
mii.fMask=MIIM_SUBMENU;
mii.hSubMenu=hsubmenu? hsubmenu->GetHandle(): NULL;
SetMenuItemInfo(Parent->GetHandle(), uId, false, &mii);
Changed(this);
Parent->Notify();
}

VOID MenuItem::OnTextChanged(Handle<String> htext)
{
MENUITEMINFO mii;
ZeroMemory(&mii, sizeof(MENUITEMINFO));
mii.cbSize=sizeof(MENUITEMINFO);
mii.fMask=MIIM_FTYPE|MIIM_STRING;
if(!htext)
	{
	mii.fType=MFT_SEPARATOR;
	SetMenuItemInfo(Parent->GetHandle(), uId, false, &mii);
	}
else
	{
	mii.dwTypeData=(LPSTR)htext->Begin();
	SetMenuItemInfo(Parent->GetHandle(), uId, false, &mii);
	}
Changed(this);
Parent->Notify();
}

}}}
