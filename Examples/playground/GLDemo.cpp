#include "GLDemo.h"
#include "PILInclude.h"
#include "ZFX.h"
#include <conio.h>

GLDemo::GLDemo()
{

}

GLDemo::~GLDemo()
{

}

void GLDemo::Setup()
{
	mRoot = new ZFX::Root("GLDemo");
	ZFX::RenderSystemFactory* rsf = ZFX::RenderSystemFactory::SingletonPtr();
	try
	{
		mRenderSystem = rsf->CreateRenderSystem(ZFX::RST_OPENGL);
		mRoot->SetRenderSystem(mRenderSystem);
		mRenderWindow = mRoot->Init(true);
		mRenderWindow->SetHidden(false);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	InitVAO();
}

void GLDemo::Run()
{
	PIL::WindowManager* wm = PIL::Root::Singleton().GetWindowManger();
	if (wm == nullptr) return;

	std::cout << "Demo run. press 'q' to quit." << std::endl;

	while (1)
	{
		wm->HandleMessage();
		Draw();
		mRenderWindow->SwapBuffers(false);

		if (_kbhit())
		{
			char c = _getch();
			if(c == 'q')
				break;
		}
	}
}

void GLDemo::Shutdown()
{
	if (mRoot != nullptr)
		delete mRoot;
}

void GLDemo::InitVAO()
{

}

void GLDemo::Draw()
{

}
