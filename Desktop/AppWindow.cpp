//===============
// AppWindow.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "AppWindow.h"

using namespace Graphics::Gdi;


//===========
// Namespace
//===========

namespace Desktop {


//==================
// Con-/Destructors
//==================

AppWindow::AppWindow(Handle<String> htitle, INT iico):
CoreWindow(nullptr),
CoreControl(nullptr),
CoreContainer(nullptr),
Title(this, htitle)
{
Destroyed.Add(this, &AppWindow::OnDestroyed);
Title.Changed.Add(this, &AppWindow::OnTitleChanged);
WindowInfo info;
info.BackgroundBrush=new GdiBrush(GetSysColor(COLOR_BTNFACE));
info.Class="AppWindow";
info.ClassStyle=CS_HREDRAW|CS_OWNDC|CS_VREDRAW;
info.Cursor=new GdiCursor(Cursors::Arrow);
info.Font=new GdiFont(Fonts::MenuFont);
info.Height=CW_USEDEFAULT;
info.Icon=iico;
info.Icon=iico;
info.Left=CW_USEDEFAULT;
info.Style=WS_CLIPCHILDREN|WS_OVERLAPPEDWINDOW;
info.Text=Title;
info.Top=CW_USEDEFAULT;
info.Width=CW_USEDEFAULT;
Create(info);
}


//========
// Common
//========

VOID AppWindow::Show(INT icmd)
{
ShowWindow(hWindow, icmd);
}


//================
// Common Private
//================

VOID AppWindow::OnDestroyed(Handle<Window> hwnd)
{
PostQuitMessage(0);
}

VOID AppWindow::OnTitleChanged(Handle<String> htitle)
{
SetWindowText(hWindow, htitle? htitle->Begin(): "");
}

}