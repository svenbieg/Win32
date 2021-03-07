//==========
// Button.h
//==========

#pragma once


//=======
// Using
//=======

#include "Core/Controls/Button.h"
#include "Interactive.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//========
// Button
//========

class Button: public Interactive, public Core::Controls::Button
{
public:
	// Con-/Destructors
	Button(Handle<Container> Parent, Handle<String> Text, UINT Id=0, UINT Style=BS_PUSHBUTTON);

private:
	// Common
	VOID OnTextChanged(Handle<String> Text);
};

}}