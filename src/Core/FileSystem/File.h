
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

#ifndef __OAK3D_INCLUDE_CORE_FILE_H__
#define __OAK3D_INCLUDE_CORE_FILE_H__

#include <string>
#include <cstdint>


namespace Oak3D
{
	namespace Core
	{
		class File
		{
		public:
			struct FileImpl;
			enum FileOpenMode
			{
				eFOM_OpenRead = 0,
				eFOM_OpenWrite,
				eFOM_OpenAppend
			};

			File(std::string filepath);
			~File();
			
			static bool Exists(std::string filepath);
			static unsigned long Size(std::string filepath);

			unsigned long Size();
			
			void Open(FileOpenMode eFileOpenMode);
			void Close();

			uint32_t Read(unsigned char *buffer, uint32_t bufferSize, uint32_t bytesToRead, uint32_t offset = 0);
			void Write(uint8_t *buffer, uint32_t bufferSize, uint32_t bytesToWrite, uint32_t offset = 0);
			

			bool IsOpen();

		private:
			FileImpl *m_pImpl;
		};
	}
}

#endif
