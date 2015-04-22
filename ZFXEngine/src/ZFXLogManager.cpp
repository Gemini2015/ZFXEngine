#include "ZFXLogManager.h"

namespace ZFX
{

	LogManager* LogManager::Instance = nullptr;

	LogManager::LogManager()
	{
		Instance = this;
		mLogMap.clear();
	}

	LogManager::~LogManager()
	{
		Log_Map::iterator it = mLogMap.begin();
		while (it != mLogMap.end())
		{
			delete it->second;
			it++;
		}
		mLogMap.clear();
	}

	ZFX::Log* LogManager::CreateLog(String name, LogMaskBit mask /*= LogLevel_Debug*/)
	{
		Log_Map::iterator it = mLogMap.find(name);
		if (it != mLogMap.end())
			return it->second;
		Log* log = new Log(name, mask);
		mLogMap[name] = log;
		return log;
	}

	HRESULT LogManager::SetCurrentLog(Log* log)
	{
		if(log == nullptr)
			return E_INVALIDARG;

		mCurrentLog = log;
		Log_Map::iterator it = mLogMap.find(log->GetName());
		if (it == mLogMap.end())
		{
			mLogMap[log->GetName()] = log;
		}
		return S_OK;
	}

	HRESULT LogManager::SetCurrentLog(String name)
	{
		if (name.empty())
			return E_INVALIDARG;
		Log_Map::iterator it = mLogMap.find(name);
		if (it == mLogMap.end())
			return E_INVALIDARG;
		mCurrentLog = it->second;
		return S_OK;
	}

	ZFX::Log* LogManager::GetCurrentLog() const
	{
		return mCurrentLog;
	}

	void LogManager::Print(String msg, LogMaskBit mask /*= Log_Debug*/)
	{
		if (mCurrentLog == nullptr)
			return;
		mCurrentLog->Print(msg, mask);
	}

	LogManager& LogManager::Singleton()
	{
		assert(Instance != nullptr);
		return *Instance;
	}

	LogManager* LogManager::SingletonPtr()
	{
		return Instance;
	}

}