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
Desktop::Window(hparent)
{
if(pinfo)
	Create(*pinfo);
}

}}
