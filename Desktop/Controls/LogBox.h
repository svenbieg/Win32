//==========
// LogBox.h
//==========

#pragma once


//=======
// Using
//=======

#include "Storage/Filesystem/TextFile.h"
#include "Control.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Controls {


//=========
// Log-Box
//=========

class LogBox: public Control
{
private:
	// Using
	using TextFile=Storage::Filesystem::TextFile;

public:
	// Con-/Destructors
	LogBox(Handle<Container> Parent, Handle<String> FilePath=nullptr);

	// Common
	VOID Clear();
	VOID SetText(Handle<String> Text);
	VOID Write(Handle<String> Text);
	template<typename... _args_t> VOID Write(Handle<String> Format, _args_t... Arguments)
		{
		Handle<String> hstr=new String(Format, Arguments...);
		Write(hstr);
		}

private:
	// Common
	Handle<TextFile> hFile;
};

}}
