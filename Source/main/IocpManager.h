#pragma once
#include <winsock2.h>
#include "Singleton.h"


class IocpManager
	:
	public Singleton< IocpManager >
{
private:
	HANDLE m_iocpHandle;  // IOCP �ڵ�

public:
	// ������
	IocpManager();

	// �Ҹ���
	virtual ~IocpManager();

	// IOCP �ڵ�, Worker Thread Pool �����ϴ� �޼���
	bool Initialize();

	// �ڵ�� ���� �ڵ��� �����Ѵ�.
	bool Associate( HANDLE hDevice, ULONG_PTR pCompletionKey );

	// m_iocpHandle�� ��ȯ�Ѵ�.
	HANDLE GetWorkerIOCPHandle();
};
