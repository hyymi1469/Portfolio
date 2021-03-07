#pragma once
#include "SessionManager.h"
#include "Thread.h"
#include "ThreadManager.h"


class SessionCheckThread
	:
	public Thread
	//public std::enable_shared_from_this< LogicThread >
{
	typedef Thread super;

public:
	// ������
	SessionCheckThread();

	// �Ҹ���
	virtual ~SessionCheckThread();

	// ������ ������ �����Ѵ�.
	void Begin() override;

	// �����带 �۵���Ų��.
	void Run() override;
};


typedef std::shared_ptr< SessionCheckThread > SessionCheckThreadPtr;
