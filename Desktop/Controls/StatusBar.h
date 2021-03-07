//=============
// StatusBar.h
//=============

#pragma once


//=======
// Using
//=======

#include "Collections/Cluster.h"
#include "Graphics/Gdi/Icon.h"
#include "Control.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//============
// Status-Bar
//============

class StatusBar: public Control
{
private:
	// Panel
	typedef struct
		{
		Handle<GdiIcon> Icon;
		Handle<String> Text;
		BOOL Visible;
		UINT Width;
		}StatusPanel;

	// Using
	using PanelList=Collections::Cluster<StatusPanel>;

public:
	// Con-/Destructors
	StatusBar(Handle<Container> Parent);

	// Common
	VOID AddPanel(UINT Width);
	Handle<String> GetText(UINT Id);
	VOID SetText(UINT Id, Handle<String> Text);
	VOID ShowPanel(UINT Id, BOOL Visible);

protected:
	// Common
	SIZE GetMinSizeInternal(Handle<DeviceContext> DeviceContext)override;

private:
	// Common
	VOID OnPainted(Handle<Window> Sender, Handle<GdiContext> DeviceContext);
	Handle<PanelList> hPanels;
};

}}