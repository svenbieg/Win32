//================
// TimeHelper.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "TimeHelper.h"


//========
// Common
//========

UINT64 GetTickCountUs()
{
LARGE_INTEGER ticks;
QueryPerformanceFrequency(&ticks);
ticks.QuadPart/=1000;
LARGE_INTEGER time;
QueryPerformanceCounter(&time);
return time.QuadPart/ticks.QuadPart;
}

VOID SleepMicroseconds(UINT uus)
{
UINT64 ustart=GetTickCountUs();
while(GetTickCountUs()<ustart+uus);
}
