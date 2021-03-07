//=========
//Grid.cpp
//=========

#include "pch.h"


//=======
// Using
//=======

#include "Grid.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Grid::Grid(Handle<DesktopContainer> hparent):
CoreWindow(hparent),
CoreControl(hparent),
CoreContainer(hparent),
CoreGrid(hparent),
DesktopContainer(hparent)
{
WindowInfo info;
info.Class="Grid";
info.ClassStyle=CS_HREDRAW|CS_VREDRAW;
info.Style=WS_CLIPCHILDREN|WS_VISIBLE;
info.StyleEx=WS_EX_CONTROLPARENT;
Create(info);
}

}}
