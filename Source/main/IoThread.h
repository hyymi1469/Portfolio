#pragma once
#include "LogicThread.h"
#include "Thread.h"
#include "ThreadManager.h"


class IoThread
	:
	public Thread
	//public std::enable_shared_from_this< LogicThread >
{
	typedef Thread super;

public:
	// ������
	IoThread();

	// �Ҹ���
	virtual ~IoThread();

	// ������ ������ �����Ѵ�.
	void Begin() override;

	// �����带 �۵���Ų��.
	void Run() override;

};


typedef std::shared_ptr< IoThread > IoThreadPtr;
