//============
// Button.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Button.h"
#include "Container.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Button::Button(Handle<Container> hparent, Handle<String> htext, UINT uid, UINT ustyle):
CoreWindow(hparent),
CoreControl(hparent),
CoreInteractive(hparent),
CoreButton(hparent),
Interactive(hparent)
{
WindowInfo info;
info.Class="BUTTON";
info.Id=uid;
info.Style=ustyle|WS_TABSTOP|WS_VISIBLE;
info.Text=htext;
Create(info);
Text.Changed.Add(this, &Button::OnTextChanged);
}


//================
// Common Private
//================

VOID Button::OnTextChanged(Handle<String> htext)
{
if(hWindow)
	SendMessage(hWindow, WM_SETTEXT, 0, (LPARAM)htext->Begin());
}

}}