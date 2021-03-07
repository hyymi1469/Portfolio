#include "Packet.h"
#include <stdio.h>
#include <assert.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	������
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
Packet::Packet()
	: m_dataField( nullptr ), m_readPosition( nullptr ), m_writePosition( nullptr ), m_receivedSize( 0 )
{
	Clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	������
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
Packet::Packet( Protocol protocolId )
	: m_dataField( nullptr ), m_readPosition( nullptr ), m_writePosition( nullptr ), m_receivedSize( 0 )
{
	Clear();
	SetProtocolId( protocolId );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	������
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
Packet::Packet( const Packet& sourcePacket )
	: m_dataField( nullptr ), m_readPosition( nullptr ), m_writePosition( nullptr ), m_receivedSize( 0 )
{
	int offset = 0;

	Clear();

	::CopyMemory( m_arrPacketBuffer, sourcePacket.m_arrPacketBuffer, BUFFER_SIZE );
	assert( m_readPosition <= m_endOfDataField && "ReadPosition�� DataField�� �Ѿ���ϴ�." );

	// �д� ��ġ ����
	offset = (int)( sourcePacket.m_readPosition - sourcePacket.m_dataField );
	m_readPosition += offset;
	assert( m_readPosition <= m_endOfDataField && "ReadPosition�� DataField�� �Ѿ���ϴ�." );

	// ���� ��ġ ����
	offset = (int)( sourcePacket.m_writePosition - sourcePacket.m_dataField );
	m_writePosition += offset;
	assert( m_writePosition <= m_endOfDataField && "WritePosition�� DataField�� �Ѿ���ϴ�." );

	m_receivedSize = sourcePacket.m_receivedSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	�Ҹ���
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
Packet::~Packet()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	��Ŷ�� ��ȿ���� Ȯ���Ѵ�
/// 
/// @return	��Ŷ ��ȿ ����
////////////////////////////////////////////////////////////////////////////////////////////////////
bool Packet::IsValidPacket()
{
	if ( m_receivedSize < PACKET_HEADERSIZE )
		return false;

	if ( m_receivedSize < PACKET_HEADERSIZE + GetDataFieldSize() )
	{
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	�������� �ѹ��� �����Ѵ�.
///
/// @param	protocolId  �������� �ĺ���
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
Packet& Packet::SetProtocolId( const Protocol protocolId )
{
	*m_packetHeader.protocolId = (int)( protocolId );
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	�������� �ѹ��� ��ȯ�Ѵ�.
/// 
/// @return	�������� �ĺ���
////////////////////////////////////////////////////////////////////////////////////////////////////
int Packet::GetProtocolId()
{
	return *m_packetHeader.protocolId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	����� ������ ������ ����� ��ȯ�Ѵ�.
/// 
/// @return	����� ������ ������ ������
////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned short Packet::GetDataFieldSize()
{
	return *m_packetHeader.dataSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	��Ŷ �� ũ�⸦ ��ȯ�Ѵ�.
/// 
/// @return	��Ŷ ũ��
////////////////////////////////////////////////////////////////////////////////////////////////////
int Packet::GetPacketSize()
{
	return ( GetDataFieldSize() + PACKET_HEADERSIZE );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	���� ��Ŷ ũ�⸦ ��ȯ�Ѵ�.
/// 
/// @return	���� ��Ŷ ũ��
////////////////////////////////////////////////////////////////////////////////////////////////////
int	Packet::GetReceivedSize()
{
	return m_receivedSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	��Ŷ�� �ʱ�ȭ�Ѵ�.
///
/// @param	bufferSize  ���� ������
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
void Packet::Clear( int bufferSize )
{
	memset( m_arrPacketBuffer, 0, bufferSize );

	// ��Ŷ ��� �ʱ�ȭ
	m_packetHeader.startCharacter = (char*)m_arrPacketBuffer;

	m_packetHeader.dataSize = (unsigned short*)( m_arrPacketBuffer + PACKET_START_POS );
	m_packetHeader.protocolId = (int*)( m_arrPacketBuffer + PACKET_HEADER_READ );

	m_dataField = &m_arrPacketBuffer[ PACKET_HEADERSIZE ];

	// �������� ��ġ ������ �ʱ�ȭ
	m_readPosition = m_dataField;
	m_writePosition = m_dataField;
	m_endOfDataField = &m_arrPacketBuffer[ bufferSize ];

	assert( m_readPosition <= m_endOfDataField && "ReadPosition�� DataField�� �Ѿ���ϴ�." );

	m_receivedSize = PACKET_HEADERSIZE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	���� ��Ŷ ���۸� ��ȯ�Ѵ�.
/// 
/// @return	���� ��Ŷ ����
////////////////////////////////////////////////////////////////////////////////////////////////////
char* Packet::GetPacketBuffer()
{
	return (char*)( m_arrPacketBuffer );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Buffer �� ���� �����͸� �����Ѵ�.
///
/// @param	buff  ����
/// @param	size  ũ��
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
void Packet::CopyToBuffer( const char* buff, const int size )
{
	Clear();

	m_receivedSize = 0;

	::CopyMemory( m_arrPacketBuffer + m_receivedSize, buff, size );
	m_receivedSize += size;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	��Ŷ�� �ִ� ������ ������ �д´�.
///
/// @param	buff  ����
/// @param	size  ũ��
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
void Packet::ReadData( void* buffer, int size )
{
	if ( m_readPosition + size > m_dataField + GetDataFieldSize() ||
		 m_readPosition + size > m_endOfDataField )
	{
		throw( 1 );
	}

	::CopyMemory( buffer, m_readPosition, size );
	m_readPosition += size;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	��Ŷ�� �ִ� ������ ������ �д´�.
///
/// @param	strDestination  ���� string
/// @param	size            ũ��
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
void Packet::ReadData( std::string& strDestination, int size )
{
	if ( m_readPosition + size > m_dataField + GetDataFieldSize() ||
		 m_readPosition + size > m_endOfDataField )
	{
		printf("[Packet::ReadData]Wrong Read Date\n");
		throw( 1 );
	}

	strDestination = m_readPosition;
	m_readPosition += size;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	��Ŷ ���ۿ� �����͸� ����.
///
/// @param	buffer  �� ����
/// @param	size    ũ��
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
void Packet::WriteData( const void* buffer, int size )
{
	if ( m_writePosition + size > m_endOfDataField )
	{
		printf("[Packet::WriteData]Packet WriteData exception protocol number = [ % d ]\n", GetProtocolId() );
#ifdef _DEBUG
		assert( 0 && "WritePacket SizeOver!!!!!!!!!!!!!!!!!!!!\n" );
#endif
		return;
	}

	::CopyMemory( m_writePosition, buffer, size );
	m_writePosition += size;
	m_receivedSize += size;

	// ��Ŷ�� ������ ũ�� 
	*m_packetHeader.dataSize += size;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Packet Ŭ������ �б� ��ġ �����͸� ��Ŷ ������ ���� �������� �ʱ� ��ġ�� �̵� ��Ų��.
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
void Packet::ResetReadPt()
{
	m_readPosition = m_dataField;
}

// = ������ ���۷�����
Packet& Packet::operator = ( Packet& packet )
{
	if ( this == &packet )
		return *this;

	::CopyMemory( m_arrPacketBuffer, packet.GetPacketBuffer(), packet.GetPacketSize() );

	m_packetHeader.startCharacter = (char*)m_arrPacketBuffer;
	m_packetHeader.dataSize       = (unsigned short*)( m_arrPacketBuffer + PACKET_START_POS );
	m_packetHeader.protocolId     = (int*)( m_arrPacketBuffer + PACKET_HEADER_READ );

	m_dataField = &m_arrPacketBuffer[ 4 + 2 ];

	// �������� ��ġ ������ �ʱ�ȭ
	m_readPosition = m_dataField;
	m_writePosition = m_dataField;
	m_endOfDataField = &m_arrPacketBuffer[ BUFFER_SIZE ];

	assert( m_readPosition <= m_endOfDataField && "ReadPosition�� DataField�� �Ѿ���ϴ�." );

	m_receivedSize = packet.m_receivedSize;

	return *this;
}

Packet& Packet::operator << ( bool source )
{
	WriteData( &source, sizeof( bool ) );

	return *this;
}

Packet& Packet::operator >> ( bool& destination )
{
	ReadData( &destination, sizeof( bool ) );

	return *this;
}

Packet& Packet::operator << ( char source )
{
	WriteData( &source, sizeof( char ) );

	return *this;
}

Packet& Packet::operator >> ( char& destination )
{
	ReadData( &destination, sizeof( char ) );

	return *this;
}

Packet& Packet::operator << ( unsigned short source )
{
	WriteData( &source, sizeof( unsigned short ) );

	return *this;
}

Packet& Packet::operator >> ( unsigned short& destination )
{
	ReadData( &destination, sizeof( unsigned short ) );

	return *this;
}


Packet& Packet::operator << ( unsigned int source )
{
	WriteData( &source, sizeof( unsigned int ) );

	return *this;
}

Packet& Packet::operator >> ( unsigned int& destination )
{
	ReadData( &destination, sizeof( unsigned int ) );

	return *this;
}

Packet& Packet::operator << ( short source )
{
	WriteData( &source, sizeof( short ) );

	return *this;
}

Packet& Packet::operator >> ( short& destination )
{
	ReadData( &destination, sizeof( short ) );

	return *this;
}


Packet& Packet::operator << ( int source )
{
	WriteData( &source, sizeof( int ) );

	return *this;
}

Packet& Packet::operator >> ( int& destination )
{
	ReadData( &destination, sizeof( int ) );

	return *this;
}

Packet& Packet::operator << ( float source )
{
	WriteData( &source, sizeof( float ) );

	return *this;
}

Packet& Packet::operator >> ( float& destination )
{
	ReadData( &destination, sizeof( float ) );

	return *this;
}

Packet& Packet::operator << ( const std::string& source )
{
	WriteData( (LPCTSTR)( source.c_str() ), (int)( source.size() ) + 1 );

	return *this;
}

Packet& Packet::operator >> ( std::string& destination )
{
	int strLength = strlen( m_readPosition ) + 1;

	ReadData( destination, strLength );

	return *this;
}
