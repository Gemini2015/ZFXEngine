#pragma once

#include "ZFXPrerequisites.h"

namespace ZFX
{
	class Root
	{
	public:
		explicit Root(String projectName);

		~Root();

		RenderWindow* Init(bool useDefaultWindow, String windowTitle = "ZFX Render Window");

		bool IsInitialied() const;

		void ShutDown();

		RenderWindow* GetDefaultWindow();

		PIL::Timer* GetTimer();

		LogManager* GetLogManager();

		void SetRenderSystem(RenderSystem* rs);

		RenderSystem* GetRenderSystem();

		static Root& Singleton();

		static Root* SingletonPtr();

	private:
		String mName;
		PIL::Root* mPILRoot;
		RenderSystem* mRenderSystem;
		RenderSystemFactory* mRenderSystemFactory;
		RenderWindow* mDefaultWindow;
		LogManager* mLogManager;
		PIL::Timer* mTimer;

		bool mIsInitialised;

		static Root* Instance;
	};
}