#include "ZFX.h"
#include "PILInclude.h"
#include <conio.h>


int main()
{
	ZFX::Root* root = new ZFX::Root("playground");
	ZFX::RenderSystemFactory* rsf = ZFX::RenderSystemFactory::SingletonPtr();
	try
	{
		ZFX::RenderSystem* rs = rsf->CreateRenderSystem(ZFX::RST_OPENGL);
		root->SetRenderSystem(rs);
		root->Init(true);
		ZFX::RenderWindow* w = root->GetDefaultWindow();
		w->SetHidden(false);
		PIL::WindowManager* wm = PIL::Root::Singleton().GetWindowManger();

		std::cout << "handling message. press 'q' to quit." << std::endl;
		while (1)
		{
			wm->HandleMessage();

			w->SwapBuffers(false);
			if (_kbhit())
			{
				char c = _getch();
				if(c == 'q')
					break;
			}
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	if (root != nullptr)
		delete root;
	std::cout << "press any key to continue." << std::endl;
	getchar();
	return 0;
}