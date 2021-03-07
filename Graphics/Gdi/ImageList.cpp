//===============
// ImageList.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "ImageList.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace Graphics {
	namespace Gdi {


//==================
// Con-/Destructors
//==================

ImageList::ImageList(UINT uwidth, UINT uheight, UINT utype, UINT usize, UINT ugrow):
hImgList(NULL),
uBackgroundColor(0)
{
SetLastError(0);
hImgList=ImageList_Create(uwidth, uheight, utype, usize, ugrow);
if(hImgList==INVALID_HANDLE_VALUE)
	hImgList=NULL;
ASSERT(hImgList);
}

ImageList::~ImageList()
{
if(hImgList)
	ImageList_Destroy(hImgList);
}


//========
// Common
//========

UINT ImageList::AddIcon(UINT uid)
{
INT iwidth=0;
INT iheight=0;
ImageList_GetIconSize(hImgList, &iwidth, &iheight);
Handle<Icon> hico=new Icon(uid, iwidth);
return AddIcon(hico->GetHandle());
}

UINT ImageList::AddIcon(HICON hico)
{
if(!hico)
	return -1;
return ImageList_ReplaceIcon(hImgList, -1, hico);
}

VOID ImageList::Clear()
{
ImageList_Remove(hImgList, -1);
}

VOID ImageList::SetBackgroundColor(Color c)
{
ImageList_SetBkColor(hImgList, c);
}

HICON ImageList::GetIcon(UINT uid)
{
return ImageList_GetIcon(hImgList, uid, 0);
}

}}