//============
// Cursor.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Cursor.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Gdi {


//==================
// Con-/Destructors
//==================

Cursor::Cursor(Cursors c):
hCursor(NULL)
{
LPCTSTR pc=nullptr;
switch(c)
	{
	case Cursors::Arrow:
		{
		pc=IDC_ARROW;
		break;
		}
	case Cursors::Hand:
		{
		pc=IDC_HAND;
		break;
		}
	case Cursors::Move:
		{
		pc=IDC_SIZEALL;
		break;
		}
	case Cursors::SizeNESW:
		{
		pc=IDC_SIZENESW;
		break;
		}
	case Cursors::SizeNS:
		{
		pc=IDC_SIZENS;
		break;
		}
	case Cursors::SizeNWSE:
		{
		pc=IDC_SIZENWSE;
		break;
		}
	case Cursors::SizeWE:
		{
		pc=IDC_SIZEWE;
		break;
		}
	default:
		{
		pc=IDC_ARROW;
		break;
		}
	}
hCursor=LoadCursor(NULL, pc);
if(hCursor==INVALID_HANDLE_VALUE)
	hCursor=NULL;
}

Cursor::~Cursor()
{
if(hCursor)
	DeleteObject(hCursor);
}

}}