//=============
// ImageList.h
//=============

#pragma once


//=======
// Using
//=======

#pragma comment(lib, "comctl32.lib")

#include <CommCtrl.h>
#include "Graphics/Color.h"
#include "Icon.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Gdi {


//============
// Image-List
//============

class ImageList: public Object
{
private:
	// Using
	using Color=COLOR;

public:
	// Con-/Destructors
	ImageList(UINT Width, UINT Height, UINT Type=ILC_COLOR32, UINT Size=0, UINT Grow=0);
	~ImageList();

	// Common
	UINT AddIcon(UINT Id);
	UINT AddIcon(HICON Icon);
	Color GetBackgroundColor() { return uBackgroundColor; }
	VOID Clear();
	HICON GetIcon(UINT Id);
	HIMAGELIST GetHandle() { return hImgList; }
	VOID SetBackgroundColor(Color Color);

private:
	// Common
	HIMAGELIST hImgList;
	Color uBackgroundColor;
};

}}