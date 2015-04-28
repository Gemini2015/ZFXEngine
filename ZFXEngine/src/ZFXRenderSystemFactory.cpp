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
		RenderSystem_Map::iterator it = mRenderSystemMap.begin();
		while (it != mRenderSystemMap.end())
		{
			if (it->second)
				delete it->second;
			it++;
		}
		mRenderSystemMap.clear();
	}

	RenderSystem* RenderSystemFactory::CreateRenderSystem(RenderSystemType type)
	{
		RenderSystem* rs = nullptr;
		switch (type)
		{
		case ZFX::RST_OPENGL:
		{
			PIL::FileSystem* fs = PIL::Root::Singleton().GetFileSystem();
			String dlllib = "OpenGL.dll";
			if (!fs->IsFileOrDirExist(dlllib))
			{
				StringBuffer sb;
				sb << dlllib << " Not found.";
				LogManager::Singleton().Print(sb.str(), Log_Error);
				throw std::invalid_argument("File Not Found");
			}
			HINSTANCE mDLL = LoadLibrary(dlllib.c_str());
			CREATERENDERDEVICE createdevice = (CREATERENDERDEVICE)GetProcAddress(mDLL, "CreateRenderDevice");
			if (createdevice)
			{
				if (!FAILED(createdevice(&rs)) && rs != nullptr)
				{
					mRenderSystemMap[rs->GetName()] = rs;
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