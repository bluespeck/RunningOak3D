#pragma once

#include <string>
#include <vector>

#include "Path.h"
#include "Utils/Types.h"

namespace oakvr
{
	namespace core
	{
		namespace io
		{
			class Directory
			{
			public:
				struct DirEntry
				{
					oakvr::core::io::path::PathType path;
					enum class EntryType
					{
						file,
						directory
					} type;
				};
			
				struct DirectoryImpl;
						
				Directory(const oakvr::core::io::path::PathType &path);
				Directory(const Directory &dir) = delete;
				Directory(Directory &&dir);
				~Directory() = default;

			
				std::vector<DirEntry> GetEntryList();			
				std::vector<oakvr::core::io::path::PathType> GetFileList(bool bRecursive = false);

				static bool Exists(const oakvr::core::io::path::PathType &path);
				static std::vector<DirEntry> GetEntryList(const oakvr::core::io::path::PathType &path);
				static std::vector<oakvr::core::io::path::PathType> GetFileList(const oakvr::core::io::path::PathType &path, bool bRecursive = false);

			private:
			

				up<DirectoryImpl> m_pImpl;
			};
		}	// namespace io
	}	// namespace core
}	// namespace oakvr
