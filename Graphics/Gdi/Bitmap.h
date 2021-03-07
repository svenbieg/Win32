//==========
// Bitmap.h
//==========

#pragma once


//=======
// Using
//=======

#include "Graphics/Bitmap.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Gdi {


//========
// Bitmap
//========

class Bitmap: public Graphics::Bitmap
{
public:
	// Con-/Destructors
	Bitmap(HBITMAP Bitmap);
	Bitmap(HICON Icon);
	Bitmap(UINT Width, UINT Height, BitmapFormat Format);
	~Bitmap();

	// Common
	HBITMAP GetHandle()const { return hBitmap; }

protected:
	// Con-/Destructors
	Bitmap();

	// Common
	VOID Initialize();
	BITMAP cBitmap;
	HBITMAP hBitmap;
};

}}
