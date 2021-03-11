#include <iostream>
#include <cstdio>
#include <unordered_map>
#include "Environment.h"
#include "SessionManager.h"
#include "Singleton.h"
#include "IocpManager.h"
#include "SocketManager.h"
#include "Enum.h"
#include "Defines.h"

#pragma warning(disable : 4996)

typedef std::unordered_map< std::string, std::string > IniStrMap;


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	ȯ�溯���� �����Ѵ�.
///
/// @param	iniStrMap  ������ ȯ�溯�� ��
/// 
/// @return	����
////////////////////////////////////////////////////////////////////////////////////////////////////
bool _SetEnvironment( const IniStrMap& iniStrMap )
{
	auto iter = iniStrMap.find( "ServerIP" );
	if ( iter == iniStrMap.end() )
		return false;
	Environment::GetSingleton()->SetGameServerIp( iter->second );

	iter = iniStrMap.find( "Port" );
	if ( iter == iniStrMap.end() )
		return false;
	Environment::GetSingleton()->SetGameServerPort( atoi( iter->second.c_str() ) );

	iter = iniStrMap.find( "LogicThreadCount" );
	if ( iter == iniStrMap.end() )
		return false;
	Environment::GetSingleton()->SetLogicThreadCount( atoi( iter->second.c_str() ) );

	iter = iniStrMap.find( "IoThreadCount" );
	if ( iter == iniStrMap.end() )
		return false;
	Environment::GetSingleton()->SetIoThreadCount( atoi( iter->second.c_str() ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	����
///
/// @return	����
////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	// 1. GameServer�� ������ ini������ �ҷ����� �� ���ش�.
	// ini�ҷ����� �Լ��� windows�Լ��� ������ ���������� �ȵ��ư� �� �����Ƿ� fopen���
	FILE* file = fopen( GAME_SERVER_INI_PATH, "r" );
	if ( !file )
		return printf("GameServer.ini Read Error!!!\n");

	IniStrMap iniStrMap;
	while ( feof( file ) == 0 )
	{
		char str[ 200 ] = "";
		std::fgets( str, 200, file );

		std::string tempStr = str;
		int index = tempStr.find( "=" );
		if ( index < 0 )
			continue;

		std::string tempStrPair1 = tempStr.substr( 0, index );
		std::string tempStrPair2 = tempStr.substr( index + 1, tempStr.size() );

		iniStrMap.emplace( tempStrPair1, tempStrPair2 );
	}

	if ( !_SetEnvironment( iniStrMap ) )
		return false;

	// 2. IOCP�� ���� ���� �غ� �Ѵ�.(IO��Ʈ ����, ��Ŀ ������Ǯ ����)
	if ( !IocpManager::GetSingleton()->Initialize() )
		return printf("IocpManager Initialize Failed!");

	// 4. �����Ҵ� ����Ǯ ����
	SessionManager::GetSingleton()->Initialize();

	// 5. Accept������ ����� Accept����
	if ( !SocketManager::GetSingleton()->Initialize( ESocketType::Tcp ) )
		return printf( "SocketManager Initialize Failed!" );


	// ���� �����尡 �ȳ������� ���
	HANDLE writeEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	WaitForSingleObject( writeEvent, INFINITE );     // ���� �ϷḦ ��ٸ�(Write�̺�Ʈ ���������� ���)


	Environment::Release();
	return 0;
} 
