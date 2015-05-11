#include "GLDemo.h"

int main()
{
	GLDemo* demo = new GLDemo();

	demo->Setup();

	demo->Run();

	demo->Shutdown();

	std::cout << "press any key to continue." << std::endl;
	getchar();
	return 0;
}