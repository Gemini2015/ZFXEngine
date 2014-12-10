#include "ZFXUtil.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdarg.h>

void ZFXLogger::Print(LOG_LEVEL level, const char* fmt, ...)
{
	std::string header = GetLevelString(level);
	if (m_stream.is_open() && !header.empty())
	{
		std::string time = GetClock().GetTimeString(ZFXClock::YMDHS);
		std::ostringstream ostr;
		ostr << "[" << header << " " << time << "] ";
		
		va_list args;
		va_start(args, fmt);
		char buf[1024];
		vsprintf_s(buf, fmt, args);
		va_end(args);
		
		ostr << buf << std::endl;

		m_stream << ostr.str();
		m_stream.flush();
	}
}

void ZFXLogger::Print(const char* str)
{
	std::string header = GetLevelString(m_levelfilter);
	if (m_stream.is_open() && !header.empty() && strlen(str) != 0)
	{
		std::string time = GetClock().GetTimeString(ZFXClock::YMDHS);
		
		m_stream << "[" << header << " " << time << "] ";
		m_stream << str << std::endl;
		m_stream.flush();
	}
}

void ZFXLogger::Print(LOG_LEVEL level, const char* str)
{
	std::string header = GetLevelString(level);
	if (m_stream.is_open() && !header.empty() && strlen(str) != 0)
	{
		std::string time = GetClock().GetTimeString(ZFXClock::YMDHS);

		m_stream << "[" << header << " " << time << "] ";
		m_stream << str << std::endl;
		m_stream.flush();
	}
}

void ZFXLogger::Print(LOG_LEVEL level, std::string filepath, const char* fmt, ...)
{
	std::string header = GetLevelString(level);
	if (!header.empty() && !filepath.empty() && strlen(fmt) != 0)
	{
		std::ofstream tempfile(filepath, std::ios::app);
		if (!tempfile)
		{
			std::string time = GetClock().GetTimeString();

			tempfile << "[" << header << " " << time << "] ";

			va_list args;
			va_start(args, fmt);
			char buf[1024];
			vsprintf_s(buf, fmt, args);
			va_end(args);

			tempfile << buf << std::endl;

			tempfile.flush();
			tempfile.close();
		}
	}
}

void ZFXLogger::Print(std::string str)
{
	std::string header = GetLevelString(m_levelfilter);
	if (m_stream.is_open() && !header.empty() && !str.empty())
	{
		std::string time = GetClock().GetTimeString(ZFXClock::YMDHS);

		m_stream << "[" << header << " " << time << "] ";
		m_stream << str << std::endl;
		m_stream.flush();
	}
}

void ZFXLogger::SetFile(std::string filepath)
{
	if (!filepath.empty())
	{
		m_filepath = filepath;
		m_stream.open(m_filepath, std::ios::out);
	}
}

std::string ZFXLogger::GetLevelString(LOG_LEVEL level)
{
	if (level < m_levelfilter)
	{
		return std::string("");
	}
	std::string res;
	switch (level)
	{
	case LOG_DEBUG:
		res.assign("DEBUG");
		break;
	case LOG_INFO:
		res.assign("INFO");
		break;
	case LOG_WARNNING:
		res.assign("WARNNING");
		break;
	case LOG_ERROR:
		res.assign("ERROR");
		break;
	default:
		res.assign("");
		break;
	}
	return res;
}

void ZFXLogger::SetLogLevel(LOG_LEVEL level)
{
	if (level > LOG_LEVEL_BEGIN && level < LOG_LEVEL_END)
		m_levelfilter = level;
}

ZFXLogger::ZFXLogger()
{
	m_levelfilter = LOG_DEBUG;
	m_filepath.clear();
	m_stream.clear();
}

ZFXLogger::~ZFXLogger()
{
	m_filepath.clear();
	m_stream.flush();
	m_stream.close();
}


/************************************************************************/
/* 
	ZFX Clock
*/
/************************************************************************/
std::string ZFXClock::GetTimeString(TIME_FORMAT format /*= YMD*/)
{
	time_t curtimes;
	time(&curtimes);
	tm t_temp;
	localtime_s(&t_temp, &curtimes);
	char buf[260] = { 0 };
	switch (format)
	{
	case ZFXClock::YMD:
		strftime(buf, 260, "%Y-%m-%d", &t_temp);
		break;
	case ZFXClock::Y_M_D:
		strftime(buf, 260, "%Y_%m_%d", &t_temp);
		break;
	case ZFXClock::YMDHS:
		strftime(buf, 260, "%Y-%m-%d %H:%M:%S", &t_temp);
		break;
	case ZFXClock::YMDHS_FILE:
		strftime(buf, 260, "%Y-%m-%d-%H-%M-%S", &t_temp);
		break;
	case ZFXClock::Y_M_D_H_S:
		strftime(buf, 260, "%Y_%m_%d_%H_%M_%S", &t_temp);
		break;
	case ZFXClock::HS:
		strftime(buf, 260, "%H:%M:%S", &t_temp);
		break;
	case ZFXClock::H_S:
		strftime(buf, 260, "%H_%M_%S", &t_temp);
		break;
	default:
		strftime(buf, 260, "%Y-%m-%d", &t_temp);
		break;
	}
	return std::string(buf);
}

