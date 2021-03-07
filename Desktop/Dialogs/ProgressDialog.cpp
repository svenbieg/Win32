//====================
// ProgressDialog.cpp
//====================

#include "pch.h"


//=======
// Using
//=======

#include "Desktop/Controls/Container.h"
#include "ProgressDialog.h"

using namespace Desktop::Controls;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//==================
// Con-/Destructors
//==================

ProgressDialog::ProgressDialog(Handle<Container> hparent, Handle<String> htitle, UINT uico):
CoreWindow(hparent),
CoreControl(hparent),
CoreContainer(hparent),
Dialog(hparent, WS_CAPTION|WS_POPUP|WS_SYSMENU, htitle, uico),
Cancel(this),
Element(this),
ElementFooter(this),
ElementHeader(this),
ElementProgress(this, 0),
Total(this),
TotalFooter(this),
TotalHeader(this),
TotalProgress(this, 0)
{
Initialized.Add(this, &ProgressDialog::OnInitialized);
}


//========
// Common
//========

VOID ProgressDialog::OnCancelChanged(Handle<String> hstr)
{
if(!hstr)
	{
	hCancelButton=nullptr;
	hCancelGrid=nullptr;
	return;
	}
if(hCancelGrid)
	{
	hCancelButton->Text=hstr;
	return;
	}
if(!hGrid)
	return;
hCancelGrid=new Grid(hGrid);
hCancelGrid->Columns=3;
hCancelGrid->Lines=1;
hCancelGrid->Line=2;
hCancelGrid->Margin={ 0, 8, 0, 8 };
hCancelButton=new Button(hCancelGrid, hstr, IDCANCEL, BS_DEFPUSHBUTTON);
hCancelButton->Column=1;
}

VOID ProgressDialog::OnElementChanged(Handle<String> hstr)
{
if(!hstr)
	{
	hElementBar=nullptr;
	hElementFooter=nullptr;
	hElementGrid=nullptr;
	hElementGroup=nullptr;
	hElementHeader=nullptr;
	return;
	}
if(hElementGroup)
	{
	hElementGroup->Title=hstr;
	return;
	}
if(!hGrid)
	return;
hElementGroup=new Group(hGrid, hstr);
hElementGroup->Line=1;
hElementGroup->Margin={ 0, 0, 0, 8 };
hElementGrid=new Grid(hElementGroup);
hElementGrid->Margin={ 4, 4, 4, 4 };
hElementBar=new ProgressBar(hElementGrid);
hElementBar->Line=2;
hElementBar->Margin={ 0, 4, 0, 4 };
hElementBar->MinSize.Width=400;
hElementBar->Progress=ElementProgress;
ElementHeader.Changed(this, ElementHeader);
ElementFooter.Changed(this, ElementFooter);
}

VOID ProgressDialog::OnElementFooterChanged(Handle<String> hstr)
{
if(!hElementGrid)
	return;
if(hstr)
	{
	if(!hElementFooter)
		{
		hElementFooter=new TextBlock(hElementGrid, hstr);
		hElementFooter->Line=3;
		}
	else
		{
		hElementFooter->Text=hstr;
		}
	}
else
	{
	hElementFooter=nullptr;
	}
}

VOID ProgressDialog::OnElementHeaderChanged(Handle<String> hstr)
{
if(!hElementGrid)
	return;
if(hstr)
	{
	if(!hElementHeader)
		{
		hElementHeader=new TextBlock(hElementGrid, hstr);
		hElementHeader->Line=1;
		}
	else
		{
		hElementHeader->Text=hstr;
		}
	}
else
	{
	hElementHeader=nullptr;
	}
}

VOID ProgressDialog::OnElementProgressChanged(FLOAT f)
{
if(hElementBar)
	hElementBar->Progress=f;
}

VOID ProgressDialog::OnInitialized(Handle<Dialog> hdialog)
{
HMENU hmenu=GetSystemMenu(hWindow, false);
DeleteMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
DrawMenuBar(hWindow);
hGrid=new Grid(this);
hGrid->Margin={ 8, 8, 8, 8 };
Total.Changed(this, Total);
Element.Changed(this, Element);
Cancel.Changed(this, Cancel);
}

VOID ProgressDialog::OnTotalChanged(Handle<String> hstr)
{
if(!hstr)
	{
	hTotalBar=nullptr;
	hTotalFooter=nullptr;
	hTotalGrid=nullptr;
	hTotalGroup=nullptr;
	hTotalHeader=nullptr;
	return;
	}
if(hTotalGroup)
	{
	hTotalGroup->Title=hstr;
	return;
	}
if(!hGrid)
	return;
hTotalGroup=new Group(hGrid, hstr);
hTotalGroup->Margin={ 0, 0, 0, 8 };
hTotalGrid=new Grid(hTotalGroup);
hTotalGrid->Margin={ 4, 4, 4, 4 };
hTotalBar=new ProgressBar(hTotalGrid);
hTotalBar->Line=2;
hTotalBar->Margin={ 0, 4, 0, 4 };
hTotalBar->MinSize.Width=400;
hTotalBar->Progress=TotalProgress;
TotalHeader.Changed(this, TotalHeader);
TotalFooter.Changed(this, TotalFooter);
}

VOID ProgressDialog::OnTotalFooterChanged(Handle<String> hstr)
{
if(!hTotalGrid)
	return;
if(hstr)
	{
	if(!hTotalFooter)
		{
		hTotalFooter=new TextBlock(hTotalGrid, hstr);
		hTotalFooter->Line=3;
		}
	else
		{
		hTotalFooter->Text=hstr;
		}
	}
else
	{
	hTotalFooter=nullptr;
	}
}

VOID ProgressDialog::OnTotalHeaderChanged(Handle<String> hstr)
{
if(!hTotalGrid)
	return;
if(hstr)
	{
	if(!hTotalHeader)
		{
		hTotalHeader=new TextBlock(hTotalGrid, hstr);
		hTotalHeader->Line=1;
		}
	else
		{
		hTotalHeader->Text=hstr;
		}
	}
else
	{
	hTotalHeader=nullptr;
	}
}

VOID ProgressDialog::OnTotalProgressChanged(FLOAT f)
{
if(hTotalBar)
	hTotalBar->Progress=f;
}

}}
