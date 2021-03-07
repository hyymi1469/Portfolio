#pragma once
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <vector>
#include <cstdio>
#include "LogicThread.h"
#include "Singleton.h"
#include "Thread.h"


typedef std::set< std::string >                   ThreadNameSet;     // ������ �̸��� ���ǵǾ� �ִ� ��
typedef std::vector< HANDLE >                     ThreadPoolVector;  // ������Ǯ ����
typedef std::map< std::string, ThreadPoolVector > ThreadMap;         // ������ �̸�, ������ ������

class ThreadManager
	:
	public Singleton< ThreadManager >
{
private:
	ThreadNameSet   m_threadNameSet;  // ������ �̸� ��� ��
	ThreadMap       m_threadMap;      // ������ ���� ��
	std::mutex      m_mutex;          // ���ؽ�

public:
	// ������
	ThreadManager();

	// �Ҹ���
	virtual ~ThreadManager();

	// �����带 �����Ѵ�.
	bool CreateThread( const std::string& threadName, Thread* threadPtr );
	
	// �����带 �����Ѵ�.( beginthreadexVer)
	HANDLE BeginCreateThread(
		LPTHREAD_START_ROUTINE startAddress,
		LPVOID                 param,
		UINT&                  threadId );

	// ���� ���� ���� �����带 ��ȯ�Ѵ�.
	Thread* GetIdleThread( const std::string& threadStr );
	/*
	// �׽�Ʈ
	ThreadPoolVector GetThreadPool( const std::string& threadStr );

	void AcquireWaitingRoom();
	*/
};
