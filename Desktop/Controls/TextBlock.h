//=============
// TextBlock.h
//=============

#pragma once


//=======
// Using
//=======

#include "Core/Controls/TextBlock.h"
#include "Graphics/Color.h"
#include "Control.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//============
// Text-Block
//============

class TextBlock: public Control, public virtual Core::Controls::TextBlock
{
public:
	// Con-/Destructors
	TextBlock(Handle<Container> Parent, Handle<String> Text=nullptr);

private:
	// Common
	VOID OnPainted(Handle<Window> Sender, Handle<GdiContext> DeviceContext);
};

}}
