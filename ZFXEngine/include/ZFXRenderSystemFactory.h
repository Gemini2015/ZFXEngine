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
		struct RenderSystemInfo
		{
			RenderSystemType type;
			HINSTANCE hDLL;
			RenderSystem* rendersystem;
			RenderSystemInfo(RenderSystemType type)
				: type(type)
				, hDLL(NULL)
				, rendersystem(nullptr)
			{
			}
		};
	public:
		typedef std::map<RenderSystemType, RenderSystemInfo*> RenderSystemInfo_Map;
	public:
		RenderSystemFactory();

		~RenderSystemFactory();

		RenderSystem* CreateRenderSystem(RenderSystemType type);

		void DestroyRenderSystem(RenderSystemType type);

		static RenderSystemFactory& Singleton();

		static RenderSystemFactory* SingletonPtr();

	private:
		RenderSystemInfo_Map mRenderSystemInfoMap;

		static RenderSystemFactory* Instance;
	};
}