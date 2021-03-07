//===========
// Picture.h
//===========

#pragma once


//=======
// Using
//=======

#include "Graphics/Gdi/Bitmap.h"
#include "Control.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//=========
// Picture
//=========

class Picture: public Control
{
private:
	// Using
	using Bitmap=Graphics::Gdi::Bitmap;

public:
	// Con-/Destructors
	Picture(Handle<Container> Parent, Handle<Bitmap> Bitmap=nullptr);

private:
	// Common
	VOID OnPainted(Handle<Window> Sender, Handle<GdiContext> DeviceContext);
	FLOAT fZoom;
	Handle<Bitmap> hBitmap;
	INT iOffsetX;
	INT iOffsetY;
};

}}