//===========
// Frame.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"
#include "Frame.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Frame::Frame(Handle<Container> hparent, Handle<CoreControl> hcontent, Handle<DirectXContext> hdc):
CoreWindow(hparent),
CoreControl(hparent),
SwapChainPanel(hparent, hdc),
Content(hcontent)
{
Clicked.Add(this, &Frame::OnClicked);
ContextMenu.Add(this, &Frame::OnContextMenu);
DoubleClicked.Add(this, &Frame::OnDoubleClicked);
KeyDown.Add(this, &Frame::OnKeyDown);
KeyUp.Add(this, &Frame::OnKeyUp);
LButtonDown.Add(this, &Frame::OnLButtonDown);
LButtonUp.Add(this, &Frame::OnLButtonUp);
PointerMoved.Add(this, &Frame::OnPointerMoved);
PointerWheel.Add(this, &Frame::OnPointerWheel);
Rendered.Add(this, &Frame::OnRendered);
WindowInfo info;
info.Class="Frame";
info.ClassStyle=CS_OWNDC;
info.Style=WS_VISIBLE;
Create(info);
}


//==================
// Common Protected
//==================

Graphics::SIZE Frame::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize=Interactive::GetMinSizeInternal(hdc);
if(Content)
	{
	SIZE ctrlsize=Content->GetMinSize(hdc);
	minsize.Width+=ctrlsize.Width;
	minsize.Height+=ctrlsize.Height;
	}
return minsize;
}


//================
// Common Private
//================

VOID Frame::OnClicked(Handle<CoreInteractive> hsender, POINT const& pt)
{
if(Content)
	Content->DoClick(pt);
}

VOID Frame::OnContextMenu(Handle<CoreInteractive> hsender, POINT const& pt)
{
if(Content)
	Content->DoContextMenu(pt);
}

VOID Frame::OnDoubleClicked(Handle<CoreInteractive> hsender, POINT const& pt)
{
if(Content)
	Content->DoDoubleClick(pt);
}

VOID Frame::OnKeyDown(Handle<CoreInteractive> hsender, VirtualKey key)
{
if(Content)
	Content->DoKey(key, true);
}

VOID Frame::OnKeyUp(Handle<CoreInteractive> hsender, VirtualKey key)
{
if(Content)
	Content->DoKey(key, false);
}

VOID Frame::OnLButtonDown(Handle<CoreInteractive> hsender, POINT const& pt)
{
if(Content)
	Content->DoLButton(pt, true);
}

VOID Frame::OnLButtonUp(Handle<CoreInteractive> hsender, POINT const& pt)
{
if(Content)
	Content->DoLButton(pt, false);
}

VOID Frame::OnPointerMoved(Handle<CoreInteractive> hsender, POINT const& pt)
{
if(Content)
	Content->DoMovePointer(pt);
}

VOID Frame::OnPointerWheel(Handle<CoreInteractive> hsender, POINT const& pt, INT idelta)
{
if(Content)
	Content->DoPointerWheel(pt, idelta);
}

VOID Frame::OnRendered(Handle<SwapChainPanel> hpanel)
{
if(Content)
	{
	RECT rc(0, 0, rcRect.Right-rcRect.Left, rcRect.Bottom-rcRect.Top);
	Content->Move(rc);
	//Content->Render(hDeviceContext);
	}
}

}}