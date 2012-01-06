
#include "Core/Config/Oak3DConfig.h"

#if OAK3D_OS == OAK3D_OS_WINDOWS

#include <windows.h>
#include <cstdint>
#include "Thread.h"

namespace Oak3D
{
	namespace Core
	{
		struct Thread::ThreadImpl
		{
			uint32_t threadId;
		};

		Thread::Thread(uint32_t (pThreadFunc)(void *), void *pData)
		{
			m_pImpl = new ThreadImpl;
			::CreateThread(NULL,						// default security attributes
				0,										// use default stack size  
				(LPTHREAD_START_ROUTINE)pThreadFunc,	// thread function name
				pData,									// argument to thread function 
				0,										// use default creation flags 
				(LPDWORD)&m_pImpl->threadId);			// returns the thread identifier
		}

		void Thread::Sleep(uint32_t ms)
		{
			::Sleep(ms);
		}

	}	// namespace Core
}	// namespace Oak3D

#endif // OAK3D_OS == OAK3D_OS_WINDOWS