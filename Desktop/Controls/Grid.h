//========
// Grid.h
//========

#pragma once


//=======
// Using
//=======

#include "Core/Controls/Grid.h"
#include "Container.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//======
// Grid
//======

class Grid: public Container, public virtual Core::Controls::Grid
{
public:
	// Con-/Destructors
	Grid(Handle<Container> Parent);
};

}}
