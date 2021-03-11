#pragma once
#include "LogicThread.h"
#include "Session.h"


class User
	:
	public Session
{
	typedef Session super;

private:
	LogicThread* m_logicThread; // ���� �ش� ������ Ȱ�� ���� ���� ������

public:
	// ������
	User();

	// �Ҹ���
	virtual ~User();

	void OnDestroy() override;

	// �ش� Ŭ������ ������� �����Ѵ�.
	void Reset() override;

	// Ȱ�� ���� ���������带 �����Ѵ�.
	void SetLogicThread( LogicThread* logicThread );

	// Ȱ�� ���� ���������带 ��ȯ�Ѵ�.
	LogicThread* GetLogicThread();

	// ��Ŷ�� �޾Ƽ� �����ϰ� ó���Ѵ�.
	int PacketHandler( const char* pRecvBuffer, const int nRecvSize ) override;
};

