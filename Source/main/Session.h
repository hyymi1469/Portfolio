#pragma once
#include <ctime>
#include <string>
#include <WinSock2.h>
#include "../Packet/Packet.h"
#include "Defines.h"
#include "Enum.h"


class Session
{
	// �������ǵ��� Ȯ��
	typedef struct OVERLAPPEDEX : OVERLAPPED
	{
		ESessionIoFlag flags;
	} OVERLAPPEDEX;

protected:
	SOCKET       m_socket;             // ����� ���� ����
	char*        m_receiveBuffer;      // ���� �������� ����(�Ϲ� �迭�� ���ÿ����̹Ƿ� �ʹ� ����. �����Ҵ��ؼ� �������� �����)
	int          m_receivedPacketSize; // ���� �������� ũ��
	std::string  m_ip;                 // ip
	int          m_port;               // port
	const int    m_recvBufferSize;     // ������ ũ��
	HANDLE       m_recvEvent;          // Recv �̺�Ʈ
	int			 m_sendingPacketSize;  // ���� send ���� ��Ŷ�� ������
	OVERLAPPEDEX m_overlappedRecv;     // �������ǵ� recvȮ��
	OVERLAPPEDEX m_overlappedSend;     // �������ǵ� sendȮ��
	INT64        m_lastRecvTime;       // ��Ŷ�� ���������� ���� �ð�(�� ����)
	int          m_sessionNum;         // ���� �ѹ�
	
public:
	// ������
	Session();

	// �Ҹ���
	virtual ~Session();

	// ������ ����Ѵ�.
	virtual void OnDestroy();

	// �ش� Ŭ������ ������� �����Ѵ�.
	virtual void Reset();

	// ������ �����Ѵ�.
	void SetSocket( SOCKET socket );

	// ���� ���ῡ ���� ������ �Ѵ�.
	bool OnCreate();

	// recv�� ��û�Ѵ�.(�̿ϼ�)
	bool Receive();

	// session������ �Ǿ����� Ȯ���Ѵ�.
	bool IsConnected();

	// ��Ŷ�� �����Ѵ�.
	void Dispatch( const DWORD bytesTransferred, OVERLAPPED* ov );

	// ���� ��Ŷ�� �����Ѵ�.(���� OV�� recv�� ��� ����)
	void DispatchReceive( const DWORD bytesTransferred );

	// ��Ʈ��ȣ�� ��ȯ�Ѵ�.
	int GetPort();

	// ���ǳѹ��� ��ȯ�Ѵ�.
	int GetSessionNum();

	// ���ǳѹ��� �����Ѵ�.
	void SetSessionNum( const int sessionNum );

	// ��Ŷ�� ���������� ���� �ð��� ��ȯ�Ѵ�.
	INT64 GetLastRecvTime();

	// ��Ŷ�� ���������� ���� �ð��� �����Ѵ�.
	void SetLastRecvTime( const INT64 lastRecvTime );

	// ���ǿ� �Ҵ�� ������ ����
	void CloseConnection();

	// ��Ŷ�� ������.
	void SendPacket( Packet& packet );

	// ��Ŷ�� �����ϰ� ó���Ѵ�.
	virtual int PacketHandler( const char* pRecvBuffer, const int nRecvSize ) = 0;
};

