//===============
// StatusBar.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"
#include "StatusBar.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//==================
// Con-/Destructors
//==================

StatusBar::StatusBar(Handle<Container> hparent):
Control(hparent)
{
Painted.Add(this, &StatusBar::OnPainted);
hPanels=new PanelList();
WindowInfo info;
info.Class="StatusBar";
info.Style=WS_VISIBLE;
Create(info);
AddPanel(0);
}


//========
// Common
//========

VOID StatusBar::AddPanel(UINT uwidth)
{
StatusPanel panel;
ZeroMemory(&panel, sizeof(StatusPanel));
panel.Visible=true;
panel.Width=uwidth;
hPanels->Append(panel);
Invalidate();
}

Handle<String> StatusBar::GetText(UINT uid)
{
auto panel=hPanels->GetAt(uid);
return panel.Text;
}

VOID StatusBar::SetText(UINT uid, Handle<String> htext)
{
auto panel=hPanels->GetAt(uid);
panel.Text=htext;
hPanels->SetAt(uid, panel);
Invalidate();
}

VOID StatusBar::ShowPanel(UINT uid, BOOL bvisible)
{
auto panel=hPanels->GetAt(uid);
panel.Visible=bvisible;
hPanels->SetAt(uid, panel);
Invalidate();
}


//==================
// Common Protected
//==================

Graphics::SIZE StatusBar::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize=Control::GetMinSize(hdc);
minsize.Height+=GetSystemMetrics(SM_CXSMICON)+8;
return minsize;
}


//================
// Common Private
//================

VOID StatusBar::OnPainted(Handle<Window> hsender, Handle<GdiContext> hdc)
{
RECT rc=hdc->Clip;
UINT ufree=rc.Right-rc.Left;
for(auto hit=hPanels->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto panel=hit->GetCurrent();
	if(!panel.Visible)
		continue;
	ufree-=panel.Width;
	}
RECT rcc({ rc.Left+2, 0, rc.Right, rc.Bottom });
for(auto hit=hPanels->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto panel=hit->GetCurrent();
	if(!panel.Visible)
		continue;
	UINT uwidth=panel.Width;
	if(!uwidth)
		uwidth=ufree;
	::RECT rcm({ rcc.Left, rcc.Top, rcc.Left+(INT)uwidth, rcc.Bottom });
	if(rcm.left!=2)
		{
		DrawEdge(hdc->GetHandle(), &rcm, EDGE_ETCHED, BF_LEFT);
		rcm.left+=4;
		}
	if(panel.Icon)
		{
		hdc->DrawIcon(panel.Icon, rcm.left, 4);
		rcm.left+=panel.Icon->GetWidth();
		}
	rcm.left=MIN(rcm.left, 8);
	if(panel.Text)
		{
		SIZE size=hdc->MeasureText(Font, panel.Text);
		rcm.top=(rcm.bottom-size.Height)/2;
		UINT ulen=panel.Text->GetLength();
		DrawText(hdc->GetHandle(), panel.Text->Begin(), panel.Text->GetLength(), &rcm, DT_LEFT);
		}
	rc.Left+=uwidth;
	}
}

}}