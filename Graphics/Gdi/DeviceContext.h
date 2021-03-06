//=================
// DeviceContext.h
//=================

#pragma once


//=======
// Using
//=======

#include "Graphics/DeviceContext.h"
#include "Bitmap.h"
#include "Brush.h"
#include "Font.h"
#include "Icon.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Gdi {


//================
// Device-Context
//================

class DeviceContext: public Graphics::DeviceContext
{
public:
	// Using
	using GdiBitmap=Gdi::Bitmap;
	using GdiBrush=Gdi::Brush;
	using GdiFont=Gdi::Font;

	// Con-/Destructors
	DeviceContext();
	DeviceContext(HDC DeviceContext);
	~DeviceContext();

	// Common
	VOID Clear(COLOR Color=0)override;
	VOID Clear(Handle<GdiBrush> Brush);
	VOID DrawBitmap(Handle<GdiBitmap> Bitmap, RECT const& Rect);
	VOID DrawIcon(Handle<Icon> Icon, INT Left, INT Top);
	VOID DrawText(Handle<String> Text, COLOR Color=0)override;
	VOID DrawText(Handle<String> Text, COLOR Color, RECT const& Rect, UINT Format=DT_CENTER|DT_VCENTER);
	VOID FillRect(RECT const& Rect, COLOR Color=0)override;
	VOID FillRect(RECT const& Rect, Handle<GdiBrush> Brush);
	VOID Flush()override;
	HDC GetHandle()const { return hDeviceContext; }
	SIZE MeasureText(Handle<Graphics::Font> Font, Handle<String> Text)override;
	VOID SetPixel(INT Left, INT Top, COLOR c=0)override;
	VOID SetTarget(Handle<GdiBitmap> Target);

private:
	// Flags
	enum class DeviceContextFlags
		{
		None=0,
		Delete=1
		};

	// Common
	HDC hDeviceContext;
	HGDIOBJ hOldTarget;
	Handle<GdiBitmap> hGdiTarget;
	DeviceContextFlags uFlags;
};

}}