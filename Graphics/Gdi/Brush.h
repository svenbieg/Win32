//=========
// Brush.h
//=========

#pragma once


//=======
// Using
//=======

#include "Graphics/Color.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Gdi {


//=======
// Brush
//=======

class Brush: public Object
{
private:
	// Using
	using Color=COLOR;

public:
	// Con-/Destructors
	Brush(Color Color);
	~Brush();

	// Common
	Color GetColor()const { return uColor; }
	HBRUSH GetHandle()const { return hBrush; }

private:
	// Common
	HBRUSH hBrush;
	Color uColor;
};

}}