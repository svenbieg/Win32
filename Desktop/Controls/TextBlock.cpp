//===============
// TextBlock.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"
#include "TextBlock.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

TextBlock::TextBlock(Handle<Container> hparent, Handle<String> htext):
CoreWindow(hparent),
CoreControl(hparent),
CoreTextBlock(hparent, htext),
Control(hparent)
{
Painted.Add(this, &TextBlock::OnPainted);
WindowInfo info;
info.Class="TextBlock";
info.Style=WS_VISIBLE;
Create(info);
}


//================
// Common Private
//================

VOID TextBlock::OnPainted(Handle<Window> hwnd, Handle<GdiContext> hdc)
{
Render(hdc);
}

}}
