//=============
// Picture.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#pragma comment(lib, "shlwapi.lib")

#include <OleCtl.h>
#include <Shlwapi.h>
#include "Picture.h"

using namespace Storage;


//===========
// Namespace
//===========

namespace Graphics {
	namespace Gdi {


//==================
// Con-/Destructors
//==================

Picture::Picture(Handle<String> hpath)
{
if(!hpath)
	return;
Microsoft::WRL::ComPtr<IStream> pstream;
if(SHCreateStreamOnFile(hpath->Begin(), STGM_READ, &pstream)!=S_OK)
	return;
if(OleLoadPicture(pstream.Get(), 0, false, IID_IPicture, &pPicture)!=S_OK)
	return;
if(pPicture->get_Handle((OLE_HANDLE*)&hBitmap)!=S_OK)
	{
	pPicture=nullptr;
	return;
	}
GetObject(hBitmap, sizeof(BITMAP), &cBitmap);
Initialize();
}

Picture::~Picture()
{
hBitmap=NULL;
}

}}