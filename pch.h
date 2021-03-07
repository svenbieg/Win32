//=======
// pch.h
//=======

#pragma once


//======================
// Forward-Declarations
//======================

#define _CRT_SECURE_NO_WARNINGS 1
#define _WIN32_WINNT 0x602
#define WIN32_LEAN_AND_MEAN

inline void setenv(char const*, char const*, bool) {}

#include <Windows.h>

#undef ARRAYSIZE
#undef GetForm

#include "TimeHelper.h"


//=======
// Using
//=======

#include "Win/Platform.h"


//========
// Common
//========

#include "Runtime/Application.h"
#include "TaskHelper.h"
