#include <memory>
#include <vector>
#include "Environment.h"
#include "IocpManager.h"
#include "IoThread.h"
#include "LogicThread.h"
#include "SessionCheckThread.h"
#include "ThreadManager.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	������
///
/// @return	����
////////////////////////////////////////////////////////////////////////////////////////////////////
IocpManager::IocpManager()
	:m_iocpHandle( nullptr )
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	�Ҹ���
///
/// @return	����
////////////////////////////////////////////////////////////////////////////////////////////////////
IocpManager::~IocpManager()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	IOCP �ڵ�, Worker Thread Pool �����ϴ� �޼���
///
/// @return	����
////////////////////////////////////////////////////////////////////////////////////////////////////
bool IocpManager::Initialize()
{
	m_iocpHandle = CreateIoCompletionPort( INVALID_HANDLE_VALUE, nullptr, 0, 0 );
	if ( !m_iocpHandle )
		return false;

	// �����带 ����� ���� shared �Ҵ�
	std::vector< LogicThread* > logicThreadVec;
	for ( int i = 0; i < Environment::GetSingleton()->GetLogicThreadCount(); ++i )
		logicThreadVec.emplace_back( new LogicThread() );
	
	// Logic(Worker)������ ����
	for ( auto iter : logicThreadVec )
	{
		if ( !ThreadManager::GetSingleton()->CreateThread( "LogicThread", iter ) )
		{
			printf("[IocpManager::Initialize] CreateThread fail!\n");
			return false;
		}
	}


	// I/O������ ����
	std::vector< IoThread* > ioThreadVec;
	for ( int i = 0; i < Environment::GetSingleton()->GetIoThreadCount(); ++i )
		ioThreadVec.emplace_back( new IoThread() );

	for ( auto iter : ioThreadVec )
	{
		if ( !ThreadManager::GetSingleton()->CreateThread( "IoThread", iter ) )
		{
			printf( "[IocpManager::Initialize] CreateThread fail!\n" );
			return false;
		}
	}

	// ���� �ð����� ���� Ȯ��
	SessionCheckThread* sessionCheckThread = new SessionCheckThread();
	if ( !ThreadManager::GetSingleton()->CreateThread( "SessionCheckThread", sessionCheckThread ) )
	{
		printf( "[IocpManager::Initialize] CreateThread fail!\n" );
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	IOCP �ڵ�� ���� �ڵ��� �����Ѵ�.
///
/// @param	socketType     ���� Ÿ��
/// @param	completionKey  ������ ������ ��ġ
/// 
/// @return	bool
////////////////////////////////////////////////////////////////////////////////////////////////////
bool IocpManager::Associate( HANDLE device, ULONG_PTR completionKey )
{
	HANDLE retHandle;

	retHandle = CreateIoCompletionPort( device, m_iocpHandle, completionKey, 0 );
	if ( retHandle != m_iocpHandle )
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	m_iocpHandle�� ��ȯ�Ѵ�.
/// 
/// @return	m_iocpHandle
////////////////////////////////////////////////////////////////////////////////////////////////////
HANDLE IocpManager::GetWorkerIOCPHandle()
{
	return m_iocpHandle;
}
