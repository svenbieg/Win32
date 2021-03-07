//=============
// Control.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Control::Control(Handle<Container> hparent, WindowInfo* pinfo):
CoreWindow(hparent),
CoreControl(hparent),
DesktopWindow(hparent)
{
if(pinfo)
	Create(*pinfo);
}

}}
