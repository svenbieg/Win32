//===========
// Picture.h
//===========

#pragma once


//=======
// Using
//=======

#include <OCIdl.h>
#include <wrl.h>
#include "Bitmap.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Gdi {


//=========
// Picture
//=========

class Picture: public Bitmap
{
public:
	// Con-/Destructors
	Picture(Handle<String> Path);
	~Picture();

private:
	// Common
	Microsoft::WRL::ComPtr<IPicture> pPicture;
};

}}