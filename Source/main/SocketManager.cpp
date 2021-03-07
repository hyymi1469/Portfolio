#include <iostream>
#include <memory>
#include <winsock2.h>
#include "Accepter.h"
#include "Defines.h"
#include "Environment.h"
#include "SocketManager.h"
#include "ThreadManager.h"

#pragma warning(disable : 4996)
#pragma comment(lib, "Ws2_32.lib")


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	������
/// 
/// @return	����
////////////////////////////////////////////////////////////////////////////////////////////////////
SocketManager::SocketManager()
	:m_listenSocket( 0 ), m_sockEvent( 0 ), m_curListenCount( 0 )
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsa ) != 0 )
	{
		printf( "SocketManager::SocketManager WSAStartup error\n" );
		return;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	�Ҹ���
/// 
/// @return	����
////////////////////////////////////////////////////////////////////////////////////////////////////
SocketManager::~SocketManager()
{
	delete m_listenSocket;
	m_listenSocket = nullptr;

	delete m_sockEvent;
	m_sockEvent = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	������ �����ϰ� ������ ���� �� �̺�Ʈ�� �����Ѵ�.
///
/// @param	socketType  ���� Ÿ��
/// 
/// @return	����
////////////////////////////////////////////////////////////////////////////////////////////////////
bool SocketManager::Initialize( const ESocketType socketType )
{
	if ( MAX_LISTEN_COUNT > WSA_MAXIMUM_WAIT_EVENTS )
		return false;

	if ( !m_listenSocket )
		m_listenSocket = new SOCKET[ MAX_LISTEN_COUNT ];

	if ( !m_sockEvent )
		m_sockEvent = new WSAEVENT[ MAX_LISTEN_COUNT ];

	for ( int i = 0; i != MAX_LISTEN_COUNT; ++i )
	{
		m_listenSocket[ i ] = INVALID_SOCKET;
		m_sockEvent[ i ] = INVALID_HANDLE_VALUE;
	}

	if ( socketType == ESocketType::Tcp )
	{
		// ������ �����Ѵ�.(enum class EAcceptType �� ������ ���� _StartTcpListenSock ȣ�� ������ �����ؾ� �Ѵ�.)
		if ( !_StartTcpListenSock( Environment::GetSingleton()->GetGameServerPort() ) )
			return false;


		if ( !ThreadManager::GetSingleton()->CreateThread( "Accepter", new Accepter ) )
		{
			printf( "[SocketManager::Initialize] CreateThread failed!" );
			return false;
		}		
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	������ �����Ѵ�.
///
/// @param	port  ��Ʈ ��ȣ
/// 
/// @return	����
////////////////////////////////////////////////////////////////////////////////////////////////////
bool SocketManager::_StartTcpListenSock( const int port )
{
	int retval = 0;
	SOCKET socket = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED );
	if ( socket == INVALID_SOCKET )
	{
		::WSACleanup();
		return false;
	}

	// ���� ������ ����ϴ� ���α׷��� ���� ����Ǿ����� Ŀ�δܿ��� �ش� ������ ���ε��ؼ� ����ϰ� �ֱ� ������ �߻��ϴ� ������ ���� reuse�ɼ��� ��
	bool reuse = true;
	setsockopt( socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof( reuse ) );
	
	// Event ������ ���� ( Ŭ���̾�Ʈ ���� )
	WSAEVENT listenEvent = WSACreateEvent();
	WSAEventSelect( socket, listenEvent, FD_ACCEPT );

	// Bind
	SOCKADDR_IN serverSockAddr;
	memset( &serverSockAddr, 0, sizeof( serverSockAddr ) );
	serverSockAddr.sin_addr.s_addr = htonl( INADDR_ANY );
	serverSockAddr.sin_family = AF_INET;
	serverSockAddr.sin_port = htons( port );
	retval = bind( socket, (LPSOCKADDR)( &serverSockAddr ), sizeof( serverSockAddr ) );
	if ( retval == SOCKET_ERROR )
	{
		printf( "Bind Error\n" );
		closesocket( socket );
		WSACleanup();
		return false;
	}

	//Listen
	retval = listen( socket, SOMAXCONN );
	if ( retval == SOCKET_ERROR )
	{
		closesocket( socket );
		socket = INVALID_SOCKET;
		WSACleanup();
		return false;
	}
	
	m_listenSocket[ m_curListenCount ] = socket;
	m_sockEvent[ m_curListenCount ] = listenEvent;
	++m_curListenCount;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Accept�Ͽ� Ŭ���̾�Ʈ ���ӽ� ���ο� ������ �Ѱ��ִ� �޼ҵ�
/// 
/// @param	connectIndex  enum class EAcceptType�� �����´�.
/// 
/// @return	����
////////////////////////////////////////////////////////////////////////////////////////////////////
SOCKET SocketManager::AcceptConnection( DWORD& connectIndex )
{
	// Event�� �߻��� ������ ��ٸ�
	DWORD dwIndex = WSAWaitForMultipleEvents( m_curListenCount, m_sockEvent, FALSE, INFINITE, FALSE );
	connectIndex = dwIndex - WSA_WAIT_EVENT_0;

	WSAResetEvent( m_sockEvent[ connectIndex ] );

	// Network �󿡼� �߻��� Event�� ã��
	WSANETWORKEVENTS networkEvents;
	WSAEnumNetworkEvents( m_listenSocket[ connectIndex ], m_sockEvent[ connectIndex ], &networkEvents );

	if ( networkEvents.lNetworkEvents & FD_ACCEPT )
		return accept( m_listenSocket[ connectIndex ], NULL, NULL );
	
	return INVALID_SOCKET;
}
