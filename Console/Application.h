//===============
// Application.h
//===============

#pragma once


//=======
// Using
//=======

#include "Core/Application.h"


//===========
// Namespace
//===========

namespace Console {


//=============
// Application
//=============

class Application: public Core::Application
{
public:
	// Con-/Destructors
	Application(Handle<String> Name);
};

}