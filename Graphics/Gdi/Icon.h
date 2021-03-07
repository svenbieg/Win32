//========
// Icon.h
//========

#pragma once


//===========
// Namespace
//===========

namespace Graphics {
	namespace Gdi {


//======
// Icon
//======

class Icon: public Object
{
public:
	// Con-/Destructors
	Icon(WORD Id);
	Icon(WORD Id, UINT Size);
	Icon(HICON Handle);
	Icon(HBITMAP Bitmap);
	~Icon();

	// Common
	HBITMAP GetColorMap()const { return cInfo.hbmColor; }
	HICON GetHandle()const { return hIcon; }
	UINT GetHeight()const { return cInfo.yHotspot*2; }
	HBITMAP GetMask()const { return cInfo.hbmMask; }
	UINT GetPixelCount()const { return cInfo.xHotspot*cInfo.yHotspot*4; }
	UINT GetWidth()const { return cInfo.xHotspot*2; }

private:
	// Common
	ICONINFO cInfo;
	HDC hDC;
	HICON hIcon;
};

}}