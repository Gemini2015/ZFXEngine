#ifndef _PIL_FILE_SYSTEM_H_
#define _PIL_FILE_SYSTEM_H_

#include "PILPrerequisites.h"
#include <vector>
#include <string>

#if defined(PLATFORM_WIN32)

#include <direct.h>
#include <io.h>

#elif defined(PLATFORM_LINUX)

#include <limits.h>
#include <unistd.h>
#include <stdlib.h>

#endif

namespace PIL
{
	typedef struct PILExport FileInfo_t {
		uint32		attrib;
		int64		time_write;
		uint32		size;
		std::string name;
	}FileInfo;

	typedef std::vector<FileInfo> FileInfo_Vec;
	typedef std::vector<std::string> FilePath_Vec;

	class PILExport FileSystem
	{
	public:
		FileSystem() {};
		~FileSystem() {};

		// File function

		// Directory function
		inline bool IsRelativePath(const std::string path)
		{
			if (path.empty())
				return true;
#if defined(PLATFORM_WIN32)
			if (isalpha(uint8(path[0])) && path[1] == ':')
				return false;
			else return true;
#else
			return path[0] != '/' && path[0] != '\\';
#endif
		}

		inline bool IsFileOrDirExist(const std::string path)
		{
			if (path.empty())
				return false;
#if defined(PLATFORM_WIN32)
			if (_access(path.c_str(), 0) == 0)
				return true;
			else return false;
#else
			if (access(path.c_str(), F_OK) == 0)
				return true;
			else return false;
#endif
		}

		inline std::string RelativeToAbsPath(const std::string relpath)
		{
			if (IsRelativePath(relpath))
			{
#if defined(PLATFORM_WIN32)
				char buf[260];
				if(_fullpath(buf, relpath.c_str(), 260) != NULL)
					return std::string(buf);
				else return "";
#else
				char buf[260];
				if(realpath(relpath.c_str(), buf) != NULL)
					return std::string(buf);
				else return "";
#endif
			}
			else return relpath;
		}

		HRESULT CreateDir(const std::string path);

		HRESULT RemoveDir(const std::string path);

		HRESULT RenameDir(const std::string oldname, const std::string newname);

		HRESULT GetFileList(const std::string path, FileInfo_Vec *filelist, bool recursive = false, const std::string filter = "*.*");
	};
}

#endif
