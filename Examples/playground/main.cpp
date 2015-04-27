#include "ZFX.h"

int main()
{
	ZFX::Root* root = new ZFX::Root("playground");
	try
	{
		root->Init(true);
	}
	catch (std::runtime_error e)
	{
		std::cout << e.what() << std::endl;
	}


	if (root != nullptr)
		delete root;
	std::cout << "press any key to continue." << std::endl;
	getchar();
	return 0;
}