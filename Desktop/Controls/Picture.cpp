//=============
// Picture.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"
#include "Picture.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Picture::Picture(Handle<Container> hparent, Handle<Bitmap> hbmp):
Control(hparent),
fZoom(0.f),
hBitmap(hbmp),
iOffsetX(0),
iOffsetY(0)
{
Painted.Add(this, &Picture::OnPainted);
WindowInfo info;
info.BackgroundBrush=new GdiBrush(Colors::Transparent);
info.Class="Picture";
info.Style=WS_VISIBLE;
Create(info);
}


//================
// Common Private
//================

VOID Picture::OnPainted(Handle<Window> hsender, Handle<GdiContext> hdc)
{
if(!hBitmap)
	return;
FLOAT fbmpwidth=hBitmap->GetWidth();
FLOAT fbmpheight=hBitmap->GetHeight();
FLOAT faspect=fbmpwidth/fbmpheight;
RECT const& rc(hdc->Clip);
RECT rcc(hdc->Clip);
RECT const& padding=Padding.GetInternal();
rcc.Left+=padding.Left;
rcc.Top+=padding.Top;
rcc.Right-=padding.Right;
rcc.Bottom-=padding.Bottom;
FLOAT fwidth=rc.Right-rc.Left;
FLOAT fheight=rc.Bottom-rc.Top;
FLOAT fzoom=fZoom;
if(fzoom==0.f)
	fzoom=MIN(fwidth/fbmpwidth, fheight/fbmpheight);
UINT udstwidth=(UINT)(fbmpwidth*fzoom);
UINT udstheight=(UINT)(fbmpheight*fzoom);
RECT rcp(rcc);
rcp.Left=(rcp.Right-udstwidth)/2+(INT)(iOffsetX*fzoom);
rcp.Right=rcp.Left+udstwidth;
rcp.Top=(rcp.Bottom-udstheight)/2+(INT)(iOffsetX*fzoom);
rcp.Bottom=rcp.Top+udstheight;
if(rcp.Top>rc.Top)
	{
	RECT rcf(rc);
	rcf.Bottom=rcp.Top;
	hdc->FillRect(rcf, pParent->BackgroundBrush);
	}
if(rcp.Bottom<rc.Bottom)
	{
	RECT rcf(rc);
	rcf.Top=rcp.Bottom;
	hdc->FillRect(rcf, pParent->BackgroundBrush);
	}
if(rcp.Left>rc.Left)
	{
	RECT rcf(rc);
	rcf.Right=rcp.Left;
	hdc->FillRect(rcf, pParent->BackgroundBrush);
	}
if(rcp.Right<rc.Right)
	{
	RECT rcf(rc);
	rcf.Left=rcp.Right;
	hdc->FillRect(rcf, pParent->BackgroundBrush);
	}
hdc->DrawBitmap(hBitmap, rcp);
}

}}