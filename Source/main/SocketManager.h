#pragma once
#include <winsock2.h>
#include <windows.h>
#include <mswsock.h>
#include "Singleton.h"
#include "Enum.h"


class SocketManager
	:
	public Singleton< SocketManager >
{
private:
	SOCKET*   m_listenSocket;    // ������ Listen ���� ������
	WSAEVENT* m_sockEvent;       // ������ �̺�Ʈ ��ü ������
	char      m_curListenCount;  // ���� Listen �ϴ� ����
	//Acceptor  m_acceptor;        // accept

public:
	// ������
	SocketManager();

	// �Ҹ���
	virtual ~SocketManager();

	// ������ �����Ѵ�.
	bool Initialize( const ESocketType socketType );

	// Accept�Ͽ� Ŭ���̾�Ʈ ���ӽ� ���ο� ������ �Ѱ��ִ� �޼ҵ�
	SOCKET AcceptConnection( DWORD& connectIndex );

private:
	// ������ �����Ѵ�.(TCP)
	bool _StartTcpListenSock( const int port );
};
