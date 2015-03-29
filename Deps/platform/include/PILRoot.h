#ifndef _PIL_ROOT_H_
#define _PIL_ROOT_H_

#include "PILInclude.h"

namespace PIL
{

	class Root
	{
	public:
		Root()
		{
			mWindowManger = new WindowManager();

			mFileSystem = new FileSystem();

			mTimer = new Timer();
		}
		~Root()
		{
			if (mWindowManger)
				delete mWindowManger;
			if (mFileSystem)
				delete mFileSystem;
			if (mTimer)
				delete mTimer;
		}

		WindowManager* GetWindowManger()
		{
			return mWindowManger;
		}

		FileSystem* GetFileSystem()
		{
			return mFileSystem;
		}

		Timer* GetTimer()
		{
			return mTimer;
		}

	private:
		WindowManager *mWindowManger;
		FileSystem *mFileSystem;
		Timer *mTimer;
	};

}

#endif