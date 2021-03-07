#pragma once
#pragma warning( disable:4786 )
#pragma warning( disable:4996 )


#include <string>
#include <Windows.h>
#include "../main/Defines.h"
#include "Protocol.h"

class Packet
{
public:
	// ������
	Packet();

	// ������
	Packet( Protocol protocolId );

	// ������
	Packet( const Packet& sourcePacket );

	// �Ҹ���
	virtual ~Packet();


protected:
	typedef struct HEADER
	{
		char*           startCharacter;
		unsigned short* dataSize;
		int*            protocolId;
	} HEADER;

	HEADER       m_packetHeader;                   // ��Ŷ �ش�
	char         m_arrPacketBuffer[ BUFFER_SIZE ]; // ��Ŷ ����
	char*        m_dataField;                      // �����Ͱ� ����Ǵ� ���� ���� ������
	char*        m_readPosition;                   // �������� �д� ��ġ
	char*        m_writePosition;                  // �������� ���� ��ġ
	char*        m_endOfDataField;                 // �����Ͱ� ����Ǵ� ������ ���� ������
	unsigned int m_receivedSize;                   // ���� �������� ��

public:
	// ��Ŷ�� ��ȿ���� Ȯ���Ѵ�
	bool IsValidPacket();

	// �������� �ѹ��� �����Ѵ�.
	Packet& SetProtocolId( const Protocol protocolId );

	// �������� �ѹ��� ��ȯ�Ѵ�.
	int GetProtocolId();
	
	// ����� ������ ������ ����� ��ȯ�Ѵ�.
	unsigned short GetDataFieldSize();

	// ��Ŷ �� ũ�⸦ ��ȯ�Ѵ�.
	int GetPacketSize();

	// ���� ��Ŷ ũ�⸦ ��ȯ�Ѵ�.
	int	GetReceivedSize();

	// ��Ŷ�� �ʱ�ȭ�Ѵ�.
	void Clear( int bufferSize = BUFFER_SIZE );

	// ���� ��Ŷ ���۸� ��ȯ�Ѵ�.
	char* GetPacketBuffer();

	// Buffer �� ���� �����͸� �����Ѵ�.
	void CopyToBuffer( const char* buff, const int size );

	// ��Ŷ�� �ִ� ������ ������ �д´�.
	void ReadData( void* buffer, int size );

	// ��Ŷ�� �ִ� ������ ������ �д´�.
	void ReadData( std::string& strDestination, int size );

	// ��Ŷ ���ۿ� �����͸� ����.
	void WriteData( const void* buffer, int size );

	// Packet Ŭ������ �б� ��ġ �����͸� ��Ŷ ������ ���� �������� �ʱ� ��ġ�� �̵� ��Ų��.
	void ResetReadPt();

	// = ������ ���۷�����
	Packet& operator = ( Packet& packet );

	// ������ ���۷�����
	Packet& operator << ( bool source );
	Packet& operator >> ( bool& destination );

	Packet& operator << ( char source );
	Packet& operator >> ( char& destination );

	Packet& operator << ( unsigned short source );
	Packet& operator >> ( unsigned short& destination );

	Packet& operator << ( unsigned int source );
	Packet& operator >> ( unsigned int& destination );

	Packet& operator << ( short source );
	Packet& operator >> ( short& destination );

	Packet& operator << ( int source );
	Packet& operator >> ( int& destination );

	Packet& operator << ( float source );
	Packet& operator >> ( float& destination );

	Packet& operator << ( const std::string& source );
	Packet& operator >> ( std::string& destination );
};
