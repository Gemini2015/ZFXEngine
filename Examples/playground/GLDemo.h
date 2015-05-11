#pragma once

#include "ZFXPrerequisites.h"

class GLDemo
{
public:
	GLDemo();

	~GLDemo();

	void Setup();

	void Run();

	void Shutdown();

private:
	void InitVAO();

	void Draw();

private:
	ZFX::Root* mRoot;
	ZFX::RenderSystem* mRenderSystem;
	ZFX::RenderWindow* mRenderWindow;
};