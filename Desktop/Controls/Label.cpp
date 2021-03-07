//===========
// Label.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"
#include "Label.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Label::Label(Handle<Container> hparent, Handle<Variable> hvar):
CoreWindow(hparent),
CoreControl(hparent),
CoreLabel(hparent, hvar),
Control(hparent)
{
Painted.Add(this, &Label::OnPainted);
WindowInfo info;
info.Class="Label";
info.Style=WS_VISIBLE;
Create(info);
}


//================
// Common Private
//================

VOID Label::OnPainted(Handle<Window> hwnd, Handle<GdiContext> hdc)
{
Render(hdc);
}

}}
