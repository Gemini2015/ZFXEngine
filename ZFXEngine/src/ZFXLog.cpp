#include "ZFXLog.h"
#include "PILInclude.h"

namespace ZFX
{
	Log::Log(String name, uint32 mask /*= LogLevel_Debug*/)
	{
		mName = name;
		String timestr = PIL::Root::Singleton().GetTimer()->GetTimeString(PIL::TimeStringFormat::YMD);
		std::ostringstream ostr;
		ostr << mName << "-" << timestr << ".log";
		mFileName = ostr.str();
		mLogMask = mask;
		mStream.open(mFileName.c_str(), std::ios::out);
	}

	Log::~Log()
	{
		mStream.flush();
		mStream.close();
	}

	String Log::GetName() const
	{
		return mName;
	}

	void Log::SetLogMask(uint32 mask)
	{
		mLogMask = mask;
	}

	uint32 Log::GetLogMask() const
	{
		return mLogMask;
	}

	void Log::Print(String msg, LogMaskBit mask /*= Log_Debug*/)
	{
		if (msg.empty())
			return;
		if (!mStream.is_open())
			return;
		if ((mLogMask & mask) == 0)
			return;

		String maskstr = GetLogMaskBitString(mask);
		String timestr = PIL::Root::Singleton().GetTimer()->GetTimeString(PIL::TimeStringFormat::YMDHS);

		mStream << "[" << timestr << "][" << maskstr << "]" << msg << std::endl;
		mStream.flush();
	}

	String Log::GetLogMaskBitString(LogMaskBit mask)
	{
		String ret = "";
		switch (mask)
		{
		case ZFX::Log_Debug:
		{
			ret = "Debug";
		}
			break;
		case ZFX::Log_Info:
		{
			ret = "Info";
		}
			break;
		case ZFX::Log_Warning:
		{
			ret = "Warning";
		}
			break;
		case ZFX::Log_Error:
		{
			ret = "Error";
		}
			break;
		default:
		{
			ret = "Debug";
		}
			break;
		}
		return ret;
	}

}