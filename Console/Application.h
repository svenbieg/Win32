//===============
// Application.h
//===============

#pragma once


//=======
// Using
//=======

#include "Runtime/Application.h"


//===========
// Namespace
//===========

namespace Console {


//=============
// Application
//=============

class Application: public Runtime::Application
{
public:
	// Con-/Destructors
	Application(Handle<String> Name);
};

}