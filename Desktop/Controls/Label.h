//=========
// Label.h
//=========

#pragma once


//=======
// Using
//=======

#include "Core/Controls/Label.h"
#include "Graphics/Color.h"
#include "Control.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//=======
// Label
//=======

class Label: public Control, public virtual Core::Controls::Label
{
public:
	// Con-/Destructors
	Label(Handle<Container> Parent, Handle<Variable> Variable);

private:
	// Common
	VOID OnPainted(Handle<Window> Sender, Handle<GdiContext> DeviceContext);
};

}}
