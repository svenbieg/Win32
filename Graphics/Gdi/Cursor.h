//==========
// Cursor.h
//==========

#pragma once


//===========
// Namespace
//===========

namespace Graphics {
	namespace Gdi {


//=========
// Cursors
//=========

enum class Cursors: UINT
{
Arrow,
Hand,
Move,
SizeNESW,
SizeNS,
SizeNWSE,
SizeWE,
Wait
};


//========
// Cursor
//========

class Cursor: public Object
{
public:
	// Con-/Destructors
	Cursor(Cursors Cursor=Cursors::Arrow);
	~Cursor();

	// Common
	HCURSOR GetHandle()const { return hCursor; }

private:
	// Common
	HCURSOR hCursor;
};

}}