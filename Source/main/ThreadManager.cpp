#include <process.h>
#include "ThreadManager.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	������
/// 
/// @return	����
////////////////////////////////////////////////////////////////////////////////////////////////////
ThreadManager::ThreadManager()
{
	m_threadNameSet.emplace( "LogicThread");
	m_threadNameSet.emplace( "IoThread" );
	m_threadNameSet.emplace( "Accepter");
	m_threadNameSet.emplace( "SessionCheckThread" );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	�Ҹ���
/// 
/// @return	����
////////////////////////////////////////////////////////////////////////////////////////////////////
ThreadManager::~ThreadManager()
{
	for ( auto iter : m_threadMap )
	{
		for ( auto iter2 : iter.second )
		{
			delete iter2;
			iter2 = nullptr;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	�����带 �����Ѵ�.
///
/// @param	threadName       �����ϰ��� �ϴ� ������ �̸�
/// @param	threadParentPtr  ������ �������� Ŭ����
/// 
/// @return	bool
////////////////////////////////////////////////////////////////////////////////////////////////////
bool ThreadManager::CreateThread( const std::string& threadName, Thread* threadPtr )
{
	if ( !threadPtr )
	{
		printf("[ThreadManager::CreateThread]ThreadParentPtr is Nullptr. %s", threadName.c_str() );
		return false;
	}

	auto iter = m_threadNameSet.find( threadName );
	if ( iter == m_threadNameSet.end() )
	{
		printf( "[ThreadManager::CreateThread] thread create Fail! threadName : %s", threadName.c_str() );
		return false;
	}

	m_threadMap[ threadName ].emplace_back( threadPtr );
	
	//threadPtr->BeginThread();
	threadPtr->Begin();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	�����带 �����Ѵ�.( beginthreadexVer)
///
/// @param	startAddress     ������ �Լ�
/// @param	param            ������ ��ġ
/// @param  threadId         ������ �ĺ��� 
/// 
/// @return	bool
////////////////////////////////////////////////////////////////////////////////////////////////////
HANDLE ThreadManager::BeginCreateThread(
	LPTHREAD_START_ROUTINE startAddress,
	LPVOID                 param,
	UINT&                  threadId )
{
	HANDLE threadHandle;

	threadHandle = (HANDLE) _beginthreadex(
								nullptr,
								0,
								(THREAD_START) startAddress,
								param,
								0,
								&threadId );
	if ( !threadHandle )
	{
		puts( "_beginthreadex() error" );
		exit( 1 );
	}

	return threadHandle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	���� ���� ���� �����带 ��ȯ�Ѵ�.
///
/// @param	threadStr  ������ ������ �̸�
/// 
/// @return	ThreadPtr
////////////////////////////////////////////////////////////////////////////////////////////////////
Thread* ThreadManager::GetIdleThread( const std::string& threadStr )
{
	int minLogicCount = 999999999;
	Thread* idleThread = nullptr;

	{
		std::unique_lock< std::mutex > lock( m_mutex );
		auto threadIter = m_threadMap.find( threadStr );
		if ( threadIter == m_threadMap.end() )
		{
			printf("[ThreadManager::GetIdleThread] m_threadMap is end!!! %s\n", threadStr.c_str() );
			return nullptr;
		}

		for ( auto iter : threadIter->second )
		{
			Thread* thread = (Thread*)( iter );
			if ( !thread )
			{
				printf("[ThreadManager::GetIdleThread]");
				continue;
			}

			if ( thread->GetTaskCount() < minLogicCount )
			{
				minLogicCount = thread->GetTaskCount();
				idleThread = thread;
			}
		}
	}

	return idleThread;
}

// �׽�Ʈ
/*
ThreadPoolVector ThreadManager::GetThreadPool( const std::string& threadStr )
{
	int minLogicCount = 999999999;
	Thread* idleThread = nullptr;

	{
		std::unique_lock< std::mutex > lock( m_mutex );
		auto threadIter = m_threadMap.find( threadStr );
		if ( threadIter == m_threadMap.end() )
		{
			printf( "[ThreadManager::GetIdleThread] m_threadMap is end!!! %s\n", threadStr.c_str() );
			return threadIter->second;
		}


		return threadIter->second;
	}
}

void ThreadManager::AcquireWaitingRoom()
{
	
}
*/