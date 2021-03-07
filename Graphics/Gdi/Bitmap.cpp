//============
// Bitmap.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Bitmap.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Gdi {


//==================
// Con-/Destructors
//==================

Bitmap::Bitmap(HBITMAP hbmp):
cBitmap({ 0 }),
hBitmap(NULL)
{
if(!hbmp)
	return;
BITMAP bmp;
GetObject(hbmp, sizeof(BITMAP), &bmp);
hBitmap=(HBITMAP)CopyImage(hbmp, IMAGE_BITMAP, bmp.bmWidth, bmp.bmHeight, 0);
GetObject(hBitmap, sizeof(BITMAP), &cBitmap);
Initialize();
}

Bitmap::Bitmap(HICON hico):
cBitmap({ 0 }),
hBitmap(NULL)
{
if(!hico)
	return;
ICONINFO ii;
if(!GetIconInfo(hico, &ii))
	return;
INT iicosize=2*ii.xHotspot;
BITMAPINFO bmi;
ZeroMemory(&bmi, sizeof(BITMAPINFO));
bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
bmi.bmiHeader.biPlanes=1;
bmi.bmiHeader.biCompression=BI_RGB;
bmi.bmiHeader.biWidth=iicosize;
bmi.bmiHeader.biHeight=iicosize;
bmi.bmiHeader.biBitCount=32;
HDC hdc=CreateCompatibleDC(NULL);
BYTE* pbuf=0;
SetLastError(0);
hBitmap=CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (VOID**)&pbuf, NULL, 0);
if(hBitmap==INVALID_HANDLE_VALUE)
	hBitmap=NULL;
if(!hBitmap)
	{
	DeleteDC(hdc);
	return;
	}
SelectObject(hdc, hBitmap);
SetLastError(0);
if(!DrawIconEx(hdc, 0, 0, hico, iicosize, iicosize, 0, NULL, DI_IMAGE|DI_MASK))
	{
	DeleteDC(hdc);
	DeleteObject(hBitmap);
	hBitmap=NULL;
	return;
	}
DeleteDC(hdc);
GetObject(hBitmap, sizeof(BITMAP), &cBitmap);
Initialize();
}

Bitmap::Bitmap(UINT uwidth, UINT uheight, BitmapFormat uformat):
cBitmap({ 0 }),
hBitmap(NULL)
{
BITMAPINFO bmi;
ZeroMemory(&bmi, sizeof(BITMAPINFO));
bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
bmi.bmiHeader.biPlanes=1;
bmi.bmiHeader.biCompression=BI_RGB;
bmi.bmiHeader.biWidth=uwidth;
bmi.bmiHeader.biHeight=-(INT)uheight;
bmi.bmiHeader.biBitCount=BitmapFormatToBits(uformat);
HDC hdc=CreateCompatibleDC(NULL);
BYTE* pbuf=0;
SetLastError(0);
hBitmap=CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (VOID**)&pbuf, NULL, 0);
DeleteDC(hdc);
if(hBitmap==INVALID_HANDLE_VALUE)
	hBitmap=NULL;
GetObject(hBitmap, sizeof(BITMAP), &cBitmap);
Initialize();
}

Bitmap::~Bitmap()
{
if(hBitmap)
	DeleteObject(hBitmap);
}


//============================
// Con-/Destructors Protected
//============================

Bitmap::Bitmap():
cBitmap({ 0 }),
hBitmap(NULL)
{}


//==================
// Common Protected
//==================

VOID Bitmap::Initialize()
{
pBuffer=(BYTE*)cBitmap.bmBits;
uFormat=BitmapFormatFromBits(cBitmap.bmBitsPixel);
uHeight=cBitmap.bmHeight;
uPitch=cBitmap.bmWidthBytes;
uSize=uHeight*uPitch;
uWidth=cBitmap.bmWidth;
}

}}
