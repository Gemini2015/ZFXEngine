#include "ZFXRenderSystemFactory.h"
#include "PILInclude.h"
#include "ZFXLogManager.h"
#include "ZFXRenderSystem.h"

namespace ZFX
{

	RenderSystemFactory* RenderSystemFactory::Instance = NULL;

	RenderSystemFactory::RenderSystemFactory()
	{
		Instance = this;
	}

	RenderSystemFactory::~RenderSystemFactory()
	{
		RenderSystemInfo_Map::iterator it = mRenderSystemInfoMap.begin();
		while (it != mRenderSystemInfoMap.end())
		{
			if (it->second)
			{
				RenderSystemInfo* info = it->second;
				if (info == nullptr) return;
				RELEASERENDERDEVICE releaserenderdevice = (RELEASERENDERDEVICE)GetProcAddress(info->hDLL, "ReleaseRenderDevice");
				if (releaserenderdevice)
				{
					releaserenderdevice(&(info->rendersystem));
				}
				delete info;
			}
			it++;
		}
		mRenderSystemInfoMap.clear();
	}

	RenderSystem* RenderSystemFactory::CreateRenderSystem(RenderSystemType type)
	{
		RenderSystem* rs = nullptr;
		switch (type)
		{
		case ZFX::RST_OPENGL:
		{
			RenderSystemInfo* info = new RenderSystemInfo(RST_OPENGL);
			info->type = RST_OPENGL;
			PIL::FileSystem* fs = PIL::Root::Singleton().GetFileSystem();
			String dlllib = "OpenGL.dll";
			if (!fs->IsFileOrDirExist(dlllib))
			{
				StringBuffer sb;
				sb << dlllib << " Not found.";
				LogManager::Singleton().Print(sb.str(), Log_Error);
				throw std::invalid_argument("File Not Found");
			}
			info->hDLL = LoadLibrary(dlllib.c_str());
			if (info->hDLL == NULL) return nullptr;
			CREATERENDERDEVICE createdevice = (CREATERENDERDEVICE)GetProcAddress(info->hDLL, "CreateRenderDevice");
			if (createdevice)
			{
				if (!FAILED(createdevice(&rs)) && rs != nullptr)
				{
					info->rendersystem = rs;
					mRenderSystemInfoMap[RST_OPENGL] = info;
				}
			}
		}
			break;
		default:
			break;
		}
		return rs;
	}

	void RenderSystemFactory::DestroyRenderSystem(RenderSystemType type)
	{
		RenderSystemInfo_Map::iterator it = mRenderSystemInfoMap.find(type);
		if (it == mRenderSystemInfoMap.end())
			return;
		RenderSystemInfo* info = it->second;
		if (info == nullptr) return;
		RELEASERENDERDEVICE releaserenderdevice = (RELEASERENDERDEVICE)GetProcAddress(info->hDLL, "ReleaseRenderDevice");
		if (releaserenderdevice)
		{
			releaserenderdevice(&(info->rendersystem));
		}
		delete info;
		mRenderSystemInfoMap.erase(it);
	}

	RenderSystemFactory& RenderSystemFactory::Singleton()
	{
		assert(Instance);
		return *Instance;
	}

	RenderSystemFactory* RenderSystemFactory::SingletonPtr()
	{
		return Instance;
	}

}