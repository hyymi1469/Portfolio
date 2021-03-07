#pragma once
#include <string>
#include "Singleton.h"

class Environment
	:
	public Singleton< Environment >

{
private:
	std::string m_gameServerIp;     // ���Ӽ��� ������
	int         m_gameServerPort;   // ���Ӽ��� ��Ʈ
	short       m_logicThreadCount; // �� ������ ����
	short       m_ioThreadCount;    // I/O ������ ����

public:
	// ������
	Environment();

	// �Ҹ���
	virtual ~Environment();

	// ���Ӽ��� �����Ǹ� �����Ѵ�.
	void SetGameServerIp( const std::string& gameServerIp );

	// ���Ӽ��� �����Ǹ� ��ȯ�Ѵ�.
	std::string GetGameServerIp();

	// ���Ӽ��� ��Ʈ�� �����Ѵ�.
	void SetGameServerPort( const int gameServerPort );

	// ���Ӽ��� ��Ʈ�� ��ȯ�Ѵ�.
	int GetGameServerPort() const;

	// �� ������ ������ �����Ѵ�.
	void SetLogicThreadCount( const short roomThreadCount );

	// �� ������ ������ ��ȯ�Ѵ�.
	int GetLogicThreadCount() const;

	// I/O ������ ������ �����Ѵ�.
	void SetIoThreadCount( const short ioThreadCount );

	// I/O ������ ������ ��ȯ�Ѵ�.
	int GetIoThreadCount() const;
};

