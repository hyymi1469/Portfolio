#pragma once
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <winsock2.h>


typedef unsigned( __stdcall* THREAD_START )( void* );

class Thread
{
protected:
	bool               m_isStarted;     // ������ ���ۿ���
	unsigned int       m_threadId;      // �������� ID��
	HANDLE	           m_threadHandle;  // �������� �ڵ鰪
	std::atomic< int > m_taskCount;     // �۾� ���� 

public:
	// ������
	Thread();

	// �Ҹ���
	virtual ~Thread();

	// ������ ������ �غ��Ѵ�.(_beginthreadex Ver)
	void BeginThread();

	// ������ �����Ѵ�.(_beginthreadex Ver)
	static DWORD HandleRunner( LPVOID param );

	// �����带 �۵���Ų��.
	virtual void Run() = 0;

	// ������ ������ �����Ѵ�.
	virtual void Begin() = 0;

	// ����� ó������
	void End();

	// logic������ ��ȯ�Ѵ�.
	int GetTaskCount();

	// logic������ ������Ų��.
	void IncreaseTaskCount();

	// logic������ ���ҽ�Ų��.
	void DecreaseTaskCount();

	// ������ �ĺ��ڸ� ��ȯ�Ѵ�.
	unsigned int GetThreadId();
};


typedef std::shared_ptr< Thread > ThreadPtr;
