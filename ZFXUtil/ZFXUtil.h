#ifndef _ZFXUTIL_H_
#define _ZFXUTIL_H_

#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <sstream>


enum LOG_LEVEL
{
	LOG_LEVEL_BEGIN = 1,
	
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNNING,
	LOG_ERROR,

	LOG_LEVEL_END,
};


class ZFXLogger
{

private:
	std::string m_filepath;
	std::ofstream m_stream;
	LOG_LEVEL m_levelfilter;

public:
	ZFXLogger();
	~ZFXLogger();
	void SetFile(std::string filepath);
	void SetLogLevel(LOG_LEVEL level);
	std::string GetLevelString(LOG_LEVEL level);
	void Print(LOG_LEVEL level, std::string filepath, const char* fmt, ...);
	void Print(LOG_LEVEL level, const char *fmt, ...);
	void Print(LOG_LEVEL level, const char* str);
	void Print(const char* str);
	void Print(std::string str);
};

class ZFXClock
{
public:
	enum TIME_FORMAT
	{
		YMD,	// y-m-d
		Y_M_D = 1,	// y_m_d
		YMDHS,	// y-m-d h:m:s
		YMDHS_FILE, // y-m-d-h-m-s
		Y_M_D_H_S,	// y_m_d_h_m_s
		HS,		// h:m:s
		H_S,	// h_m_s

	};
	std::string GetTimeString(TIME_FORMAT format = YMD);
};

ZFXClock* GetClock();
ZFXLogger* GetLogger();

ZFXLogger* GetLogger()
{
	static ZFXLogger* logger;
	if (!logger)
	{
		logger = new ZFXLogger();
		if (!logger)
		{
			//error
		}
		
		std::ostringstream ostr;
		ostr << "ZFXLog-" << GetClock()->GetTimeString(ZFXClock::YMDHS_FILE) << ".log";
		
		logger->SetFile(ostr.str());
	}
	return logger;
}

ZFXClock* GetClock()
{
	static ZFXClock* clock;
	if (clock == NULL)
	{
		clock = new ZFXClock();
	}
	return clock;
}

#endif