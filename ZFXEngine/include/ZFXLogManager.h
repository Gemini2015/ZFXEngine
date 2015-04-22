#pragma once

#include "ZFXPrerequisites.h"
#include "ZFXLog.h"

namespace ZFX
{
	class LogManager
	{
	public:
		typedef std::map<String, Log*> Log_Map;
	public:
		LogManager();

		~LogManager();

		Log* CreateLog(String name, LogMaskBit mask = LogLevel_Debug);

		Log* GetLog(String name);

		HRESULT SetCurrentLog(Log* log);

		HRESULT SetCurrentLog(String name);

		Log* GetCurrentLog() const;

		void Print(String msg, LogMaskBit mask = Log_Debug);

		static LogManager& Singleton();

		static LogManager* SingletonPtr();

	private:
		Log_Map mLogMap;
		Log* mCurrentLog;

		static LogManager* Instance;
	};
}