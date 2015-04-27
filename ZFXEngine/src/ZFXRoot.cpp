#include "ZFXRoot.h"
#include "PILInclude.h"

#include "ZFXRenderSystem.h"
#include "ZFXRenderWindow.h"
#include "ZFXLogManager.h"

namespace ZFX
{
	Root* Root::Instance = NULL;

	Root::Root(String projectName)
		: mName(projectName)
		, mRenderSystem(nullptr)
		, mDefaultWindow(nullptr)
		, mLogManager(nullptr)
		, mTimer(nullptr)
		, mIsInitialised(false)
	{
		Instance = this;

		if (PIL::Root::SingletonPtr() == nullptr)
		{
			mPILRoot = new PIL::Root();
		}

		if (LogManager::SingletonPtr() == nullptr)
		{
			mLogManager = new LogManager();
			Log* log = mLogManager->CreateLog(mName);
			mLogManager->SetCurrentLog(log);
		}

		mTimer = mPILRoot->GetTimer();

		mLogManager->Print("ZFX Root Created", Log_Info);
	}

	Root::~Root()
	{
		ShutDown();

		if (mLogManager)
			delete mLogManager;

		if (mPILRoot)
			delete mPILRoot;
	}

	RenderWindow* Root::Init(bool useDefaultWindow, String windowTitle /*= "ZFX Render Window"*/)
	{
		if (mRenderSystem == nullptr)
		{
			StringBuffer sb;
			sb << "No RenderSystem ";
			mLogManager->Print(sb.str(), Log_Error);
			throw std::runtime_error("No RenderSystem");
		}

		if (useDefaultWindow)
		{
			mDefaultWindow = mRenderSystem->Init(useDefaultWindow, windowTitle);
		}
		mTimer->Reset();
		mIsInitialised = true;
		mLogManager->Print("ZFX Root Initialised.", Log_Info);
		return mDefaultWindow;
	}

	bool Root::IsInitialied() const
	{
		return mIsInitialised;
	}

	void Root::ShutDown()
	{
		mIsInitialised = false;
		mLogManager->Print("Root Shutdown.", Log_Info);
	}

	RenderWindow* Root::GetDefaultWindow()
	{
		return mDefaultWindow;
	}

	PIL::Timer* Root::GetTimer()
	{
		return mTimer;
	}

	LogManager* Root::GetLogManager()
	{
		return mLogManager;
	}

	void Root::SetRenderSystem(RenderSystem* rs)
	{
		if (rs == nullptr)
			return;
		mRenderSystem = rs;
		StringBuffer sb;
		sb << "Set RenderSystem " << rs->GetName();
		mLogManager->Print(sb.str(), Log_Info);
	}

	RenderSystem* Root::GetRenderSystem()
	{
		return mRenderSystem;
	}

	Root& Root::Singleton()
	{
		assert(Instance);
		return *Instance;
	}

	Root* Root::SingletonPtr()
	{
		return Instance;
	}

}