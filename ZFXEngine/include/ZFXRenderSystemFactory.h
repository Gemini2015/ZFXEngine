#pragma once

#include "ZFXPrerequisites.h"

namespace ZFX
{
	enum RenderSystemType
	{
		RST_OPENGL,
	};

	class ZFXExport RenderSystemFactory
	{
	public:
		typedef std::map<String, RenderSystem*> RenderSystem_Map;
	public:
		RenderSystemFactory();

		~RenderSystemFactory();

		RenderSystem* CreateRenderSystem(RenderSystemType type);

		void DestroyRenderSystem(RenderSystemType type);

		static RenderSystemFactory& Singleton();

		static RenderSystemFactory* SingletonPtr();

	private:
		RenderSystem_Map mRenderSystemMap;

		static RenderSystemFactory* Instance;
	};
}