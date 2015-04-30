#pragma once

#include "ZFXPrerequisites.h"

namespace ZFX
{
	enum LogMaskBit
	{
		Log_Debug = 0x1,
		Log_Info = 0x2,
		Log_Warning = 0x4,
		Log_Error = 0x8,

		LogLevel_Debug = Log_Debug | Log_Info | Log_Warning | Log_Error,
		LogLevel_Info = Log_Info | Log_Warning | Log_Error,
		LogLevel_Warning = Log_Warning | Log_Error,
		LogLevel_Error = Log_Error,
	};

	class ZFXExport Log
	{
		friend class LogManager;
	public:

		String GetName() const;

		void SetLogMask(uint32 mask);

		uint32 GetLogMask() const;

		void Print(String msg, LogMaskBit mask = Log_Debug);

	private:

		Log(String name, uint32 mask = LogLevel_Debug);

		~Log();

		String GetLogMaskBitString(LogMaskBit mask);

	private:
		String mName;
		String mFileName;
		uint32 mLogMask;
		std::ofstream mStream;
	};

}