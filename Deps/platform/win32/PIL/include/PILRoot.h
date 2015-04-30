#ifndef _PIL_ROOT_H_
#define _PIL_ROOT_H_

#include "PILWindowManager.h"
#include "PILFileSystem.h"
#include "PILTime.h"

namespace PIL
{

	class PILExport Root
	{
	public:
		static Root *Instance;
	public:
		Root()
		{
			mWindowManger = new WindowManager();

			mFileSystem = new FileSystem();

			mTimer = new Timer();

			Instance = this;
		}
		~Root()
		{
			if (mWindowManger)
				delete mWindowManger;
			if (mFileSystem)
				delete mFileSystem;
			if (mTimer)
				delete mTimer;

			Instance = NULL;
		}

		WindowManager* GetWindowManger() const
		{
			return mWindowManger;
		}

		FileSystem* GetFileSystem() const
		{
			return mFileSystem;
		}

		Timer* GetTimer() const
		{
			return mTimer;
		}

		static Root* SingletonPtr()
		{
			return Instance;
		}

		static Root& Singleton()
		{
			assert(Instance != NULL);
			return *Instance;
		}

	private:
		WindowManager *mWindowManger;
		FileSystem *mFileSystem;
		Timer *mTimer;
	};
}

#endif