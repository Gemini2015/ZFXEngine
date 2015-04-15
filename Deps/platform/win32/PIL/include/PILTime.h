#ifndef _PIL_TIME_H_
#define _PIL_TIME_H_

#include "PILPrerequisites.h"

#if defined(PLATFORM_LINUX)

#include <sys/time.h>

#endif

namespace PIL
{

	class Timer
	{
	public:
		Timer();
		~Timer();

		void Reset();
		uint32 GetMillisecondTime();

	protected:

#if defined(PLATFORM_WIN32)

		LARGE_INTEGER m_StartTime;
		DWORD m_StartTick;
		LONGLONG m_LastTime;

#elif defined(PLATFORM_LINUX) 
		struct timeval m_StartTime;

#endif

	};

}

#endif
