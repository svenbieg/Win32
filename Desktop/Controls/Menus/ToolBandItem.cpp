//==================
// ToolBandItem.cpp
//==================

#include "pch.h"


//=======
// Using
//=======

#include "ToolBar.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {
		namespace Menus {


//==================
// Con-/Destructors
//==================

ToolBandItem::ToolBandItem(Handle<ToolBand> hparent, INT ipos, WORD uid, WORD uico, WORD uicod, Handle<String> htext):
Parent(hparent),
Text(this, htext),
Visible(this, true),
uId(uid)
{
Text.Changed.Add(this, &ToolBandItem::OnTextChanged);
Visible.Changed.Add(this, &ToolBandItem::OnVisibleChanged);
Visible.Reading.Add(this, &ToolBandItem::OnVisibleReading);
TBBUTTON tbb;
ZeroMemory(&tbb, sizeof(TBBUTTON));
tbb.fsStyle=BTNS_AUTOSIZE;
tbb.idCommand=uid;
BOOL binserted=false;
if(!uico)
	{
	tbb.fsStyle|=BTNS_SEP;
	SendMessage(Parent->GetHandle(), TB_INSERTBUTTON, ipos, (LPARAM)&tbb);
	}
else
	{
	tbb.fsState=TBSTATE_ENABLED;
	tbb.fsStyle|=BTNS_BUTTON;
	tbb.iBitmap=Parent->AddIcon(uico, uicod);
	tbb.iString=(INT_PTR)(Text? Text->Begin(): nullptr);
	SendMessage(Parent->GetHandle(), TB_INSERTBUTTON, ipos, (LPARAM)&tbb);
	}
Parent->Items->InsertAt(ipos, this);
Parent->InvalidateArrangement();
}


//========
// Common
//========

UINT ToolBandItem::GetHeight()
{
UINT upos=GetPosition();
::RECT rc;
if(!SendMessage(Parent->GetHandle(), TB_GETITEMRECT, upos, (LPARAM)&rc))
	return 0;
return rc.bottom-rc.top;
}

UINT ToolBandItem::GetPosition()
{
return Parent->GetItemPosition(uId);
}

Graphics::RECT ToolBandItem::GetRect()
{
UINT upos=GetPosition();
::RECT rc;
if(!SendMessage(Parent->GetHandle(), TB_GETITEMRECT, upos, (LPARAM)&rc))
	return RECT({ 0, 0, 0, 0 });
return RECT({ rc.left, rc.top, rc.right, rc.bottom });
}

UINT ToolBandItem::GetWidth()
{
UINT upos=GetPosition();
::RECT rc;
if(!SendMessage(Parent->GetHandle(), TB_GETITEMRECT, upos, (LPARAM)&rc))
	return 0;
return rc.right-rc.left;
}


//================
// Common Private
//================

VOID ToolBandItem::OnTextChanged(Handle<String> htext)
{
TBBUTTONINFO tbbi;
ZeroMemory(&tbbi, sizeof(TBBUTTONINFO));
tbbi.cbSize=sizeof(TBBUTTONINFO);
tbbi.dwMask=TBIF_TEXT;
tbbi.pszText=htext? (LPSTR)htext->Begin(): nullptr;
SendMessage(Parent->GetHandle(), TB_SETBUTTONINFO, uId, (LPARAM)&tbbi);
}

VOID ToolBandItem::OnVisibleChanged(BOOL bvisible)
{
SendMessage(Parent->GetHandle(), TB_HIDEBUTTON, uId, !bvisible);
Parent->InvalidateArrangement();
}

VOID ToolBandItem::OnVisibleReading(BOOL& bvisible)
{
SetLastError(0);
TBBUTTONINFO tbbi;
ZeroMemory(&tbbi, sizeof(TBBUTTONINFO));
tbbi.cbSize=sizeof(TBBUTTONINFO);
tbbi.dwMask=TBIF_STATE;
if(SendMessage(Parent->GetHandle(), TB_GETBUTTONINFO, uId, (LPARAM)&tbbi)<0)
	return;
bvisible=!(tbbi.fsState&TBSTATE_HIDDEN);
}

}}}
