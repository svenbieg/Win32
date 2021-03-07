//==========
// Icon.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "User/Storage/Buffer.h"
#include "Icon.h"

using namespace User::Storage;


//===========
// Namespace
//===========

namespace Graphics {
	namespace Gdi {


//==================
// Con-/Destructors
//==================

Icon::Icon(WORD uid):
hDC(NULL),
hIcon(NULL)
{
ZeroMemory(&cInfo, sizeof(ICONINFO));
if(!uid)
	return;
HINSTANCE hinst=GetModuleHandle(nullptr);
hIcon=LoadIcon(hinst, MAKEINTRESOURCE(uid));
GetIconInfo(hIcon, &cInfo);
}

Icon::Icon(WORD uid, UINT usize):
hDC(NULL),
hIcon(NULL)
{
ZeroMemory(&cInfo, sizeof(ICONINFO));
if(!uid)
	return;
HINSTANCE hinst=GetModuleHandle(NULL);
hIcon=(HICON)LoadImage(hinst, MAKEINTRESOURCE(uid), IMAGE_ICON, usize, usize, 0);
GetIconInfo(hIcon, &cInfo);
}

Icon::Icon(HICON hico):
hDC(NULL),
hIcon(NULL)
{
ZeroMemory(&cInfo, sizeof(ICONINFO));
if(!hico)
	return;
ICONINFO cinfo;
GetIconInfo(hico, &cinfo);
hIcon=CreateIconIndirect(&cinfo);
GetIconInfo(hIcon, &cInfo);
}

Icon::Icon(HBITMAP hbmp):
hDC(NULL),
hIcon(NULL)
{
ZeroMemory(&cInfo, sizeof(ICONINFO));
if(!hbmp)
	return;
BITMAP bmp;
GetObject(hbmp, sizeof(BITMAP), &bmp);
UINT umasksize=bmp.bmWidth*bmp.bmHeight/8;
Buffer cmask(umasksize);
memset(cmask.Begin(), 0xFFFFFFFF, umasksize);
HBITMAP hbmpmask=CreateBitmap(bmp.bmWidth, bmp.bmHeight, 1, 1, cmask.Begin());
if(hbmpmask==INVALID_HANDLE_VALUE)
	hbmpmask=NULL;
if(!hbmpmask)
	return;
ICONINFO cinfo;
ZeroMemory(&cinfo, sizeof(ICONINFO));
cinfo.fIcon=true;
cinfo.hbmColor=hbmp;
cinfo.hbmMask=hbmpmask;
cinfo.xHotspot=bmp.bmWidth/2;
cinfo.yHotspot=bmp.bmHeight/2;
hIcon=CreateIconIndirect(&cinfo);
if(hIcon==INVALID_HANDLE_VALUE)
	hIcon=NULL;
GetIconInfo(hIcon, &cInfo);
}

Icon::~Icon()
{
if(hIcon)
	DestroyIcon(hIcon);
}

}}