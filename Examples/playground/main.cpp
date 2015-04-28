#include "ZFX.h"

int main()
{
	ZFX::Root* root = new ZFX::Root("playground");
	ZFX::RenderSystemFactory* rsf = ZFX::RenderSystemFactory::SingletonPtr();
	try
	{
		ZFX::RenderSystem* rs = rsf->CreateRenderSystem(ZFX::RST_OPENGL);
		root->SetRenderSystem(rs);
		root->Init(true);
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