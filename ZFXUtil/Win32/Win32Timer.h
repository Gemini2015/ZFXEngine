#pragma once

#include "..\ITimer.h"
#include <windows.h>

class Win32Timer :public ITimer
{
protected:
	LARGE_INTEGER m_StartTime;
	DWORD m_StartTick;
	LONGLONG m_LastTime;

public:
	Win32Timer();
	~Win32Timer();

	virtual void Reset() override;

	virtual unsigned long getMillisecondTime() override;

};