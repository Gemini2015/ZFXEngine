#ifndef _PIL_TIME_H_
#define _PIL_TIME_H_

#include "PILPrerequisites.h"

#if defined(PLATFORM_LINUX)

#include <sys/time.h>

#endif

namespace PIL
{
	enum TimeStringFormat
	{
		HS,			// h:m:s
		H_S,		// h_m_s
		YMD,		// y-m-d
		Y_M_D,		// y_m_d
		YMDHS,		// y-m-d h:m:s
		YMDHS_FILE,	// y-m-d-h-m-s
		Y_M_D_H_S,	// y_m_d_h_m_s
	};

	class PILExport Timer
	{
	public:
		Timer();
		~Timer();

		void Reset();

		uint32 GetMillisecondTime();

		std::string GetTimeString(TimeStringFormat fmt, struct tm* time = nullptr);

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
