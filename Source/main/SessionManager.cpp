#include "IocpManager.h"
#include "SessionManager.h"
#include "ThreadManager.h"
#include "User.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	������
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
SessionManager::SessionManager()
	:
	m_curMaxSessionNum( 0 )
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	�Ҹ���
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
SessionManager::~SessionManager()
{
	Session* session = nullptr;
	while ( m_userObjectPoolQue.pop( session ) )
	{
		delete session;
		session = nullptr;
	}
	
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		for ( auto iter : m_activeObjectPoolMap )
		{
			Session* session = iter.second;
			delete session;
			session = nullptr;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	����Ǯ�� �����Ѵ�.
/// 
/// @return	bool
////////////////////////////////////////////////////////////////////////////////////////////////////
void SessionManager::Initialize()
{
	_PushObjPool( USER_OBJ_POOL );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	���ο� Session ��ü�� ��´�.
///
/// @param	socket  ���� ���� ����
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
void SessionManager::CreateNewSession( SOCKET socket )
{
	// Linger off -> ���� ������ ���� ���� ������� ��� ���� ����
	struct linger lingerOpt;
	lingerOpt.l_onoff = 1;
	lingerOpt.l_linger = 0;

	//int timeOutValue = 3000;
	setsockopt( socket, SOL_SOCKET, SO_LINGER, (char*)&lingerOpt, sizeof( lingerOpt ) );
	//setsockopt( socket, SOL_SOCKET, SO_RCVTIMEO, (const char*) &timeOutValue, sizeof( timeOutValue ) );

	int socklen = socklen = BUFFER_SIZE; // 16������ CPU�� �б� ���� ���� ��
	setsockopt( socket, SOL_SOCKET, SO_RCVBUF, (char*)&socklen, sizeof( socklen ) );
	setsockopt( socket, SOL_SOCKET, SO_SNDBUF, (char*)&socklen, sizeof( socklen ) );

	User* user = _PopObjPool();
	if ( !user )
	{
		printf( "[SessionManager::CreateNewSession] user == nullptr!\n" );
		return;
	}

	user->Reset();
	user->SetSocket( socket );
	user->SetLastRecvTime( std::time( nullptr ) );
	user->SetLogicThread( (LogicThread*)( ThreadManager::GetSingleton()->GetIdleThread( "LogicThread" ) ) );
	user->GetLogicThread()->IncreaseTaskCount();
	
	if ( !IocpManager::GetSingleton()->Associate( (HANDLE)( socket ), (ULONG_PTR)( user ) ) )
	{
		user->OnDestroy();

		printf("[SessionManager::CreateNewSession] create session fail!\n");

		return;
	}

	if ( !user->OnCreate() )
	{
		user->OnDestroy();

		printf( "[SessionManager::CreateNewSession] OnCreate fail!\n" );

		return;
	}

	if ( !user->IsConnected() )
	{
		user->OnDestroy();

		printf( "[SessionManager::CreateNewSession] IsConnected fail!\n" );

		return;
	}

	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_activeObjectPoolMap.emplace( user->GetSessionNum(), user );

		time_t curTime = time( nullptr );
		struct tm* localTime = localtime( &curTime );
		printf( "[%04d-%02d-%02d %02d:%02d:%02d] Connect, sessionNum : %d \n",
				localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
				localTime->tm_hour, localTime->tm_min, localTime->tm_sec,
				user->GetSessionNum() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	������Ʈ�� ��ȯ�Ѵ�.
///
/// @param	user  ��ȯ�� ������Ʈ
/// 
/// @return	bool
////////////////////////////////////////////////////////////////////////////////////////////////////
void SessionManager::PushObjPool( User* user )
{
	if ( !user )
		return;

	if ( !m_userObjectPoolQue.push( user ) )
	{
		delete user;
		user = nullptr;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Ȱ�� ���� ������Ʈ�� ����.
///
/// @param	activeSessionNum  Ȱ�� ���� ���� �ѹ�
/// 
/// @return	bool
////////////////////////////////////////////////////////////////////////////////////////////////////
void SessionManager::PopActiveObject( const INT64 activeSessionNum )
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_activeObjectPoolMap.erase( activeSessionNum );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	������Ʈ Ǯ�� �ִ´�.
///
/// @param	objCount  �߰��� ������Ʈ ī��Ʈ
/// 
/// @return	bool
////////////////////////////////////////////////////////////////////////////////////////////////////
void SessionManager::_PushObjPool( const int objCount )
{
	for ( int i = 0; i < objCount; ++i )
	{
		User* user = new User();
		user->SetSessionNum( ++m_curMaxSessionNum );
		if ( !m_userObjectPoolQue.push( user ) )
		{
			delete user;
			user = nullptr;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	����Ǯ���� ���� �� Ǯ�� ��������� �߰��� 200�� Ǯ�� �ִ´�.
///
/// @param	session  ������ ������ �ִ´�.
/// 
/// @return	bool
////////////////////////////////////////////////////////////////////////////////////////////////////
User* SessionManager::_PopObjPool()
{
	// ���� pop�� ������ ���, Ǯ�� �߰��� 200 �Ҵ��Ѵ�.
	Session* session = nullptr;
	if ( !m_userObjectPoolQue.pop( session ) )
	{
		printf( "m_userObjectPoolQue count : %d\n", m_userObjectPoolQue.size() );
		session = new User();
		session->SetSessionNum( ++m_curMaxSessionNum );
		m_userObjectPoolQue.push( session );
		_PushObjPool( EXTRA_USER_OBJ_POOL );
	}
	return (User*)( session );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	������ ��Ŷ ���� �ð��� Ȯ�� �� ������ �ڸ���.
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
void SessionManager::CheckAndDisconnect()
{
	Session* session = nullptr;
	const INT64 curTime = std::time( nullptr );
	std::list< Session* > deleteCandidateList;
	
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		for ( auto iter = m_activeObjectPoolMap.begin(); iter != m_activeObjectPoolMap.end(); ++iter )
		{
			Session* session = iter->second;
			if ( !session )
				continue;

			const INT64 lastRecvTime = session->GetLastRecvTime();

			if ( !lastRecvTime )
			{
				deleteCandidateList.emplace_back( session );
				continue;
			}

			const INT64 diffTime = curTime - session->GetLastRecvTime();
			if ( DISCONNECT_SESSION_SEC < diffTime )
			{
				deleteCandidateList.emplace_back( session );
				continue;
			}
		}

		for ( auto iter : deleteCandidateList )
		{
			Session* session = iter;
			if ( !session )
				continue;

			User* user = (User*)( session );
			//user->OnDestroy();
			user->CloseConnection();
			m_activeObjectPoolMap.erase( user->GetSessionNum() );
			PushObjPool( user );
			continue;
		}
	}
	
}
