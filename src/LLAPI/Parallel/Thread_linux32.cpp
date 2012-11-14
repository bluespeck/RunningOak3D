#include <ctime>
#include <thread>

#include "Thread.h"

namespace ro3d
{
namespace Core
{
	struct Thread::ThreadImpl
		{
			std::thread *m_pThread;
			
			ThreadImpl(uint32_t (pThreadFunc)(void *), void *pData)
			{
				m_pThread = new std::thread(pThreadFunc, pData);
			}
			
			~ThreadImpl()
			{
				delete m_pThread;
			}
		};

		// --------------------------------------------------------------------------------
		void Thread::CreateThread(uint32_t (pThreadFunc)(void *), void *pData)
		{
			m_pImpl = new ThreadImpl(pThreadFunc, pData);
		}

		// --------------------------------------------------------------------------------
		Thread::~Thread()
		{
			if(m_pImpl)
			{
				Join();
				delete m_pImpl;
			}
		}


		// --------------------------------------------------------------------------------
		void Thread::Sleep(uint32_t ms)
		{
			//m_pImpl->m_pThread->sleep_for(std::chrono::milliseconds(ms));
			timespec ts = {ms / 1000, (ms % 1000) * 1000};
			::nanosleep(&ts, nullptr);
			
		}

		// --------------------------------------------------------------------------------
		void Thread::Join()
		{
			m_pImpl->m_pThread->join();
			delete m_pImpl;
			m_pImpl = nullptr;
		}
		
} // end namespace Core
} // end namespace ro3d