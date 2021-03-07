//===========
// Brush.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Brush.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace Graphics {
	namespace Gdi {


//==================
// Con-/Destructors
//==================

Brush::Brush(Color c):
hBrush(NULL),
uColor(c)
{
hBrush=CreateSolidBrush(uColor&0xFFFFFF);
if(hBrush==INVALID_HANDLE_VALUE)
	hBrush=NULL;
}

Brush::~Brush()
{
if(hBrush)
	DeleteObject(hBrush);
}

}}