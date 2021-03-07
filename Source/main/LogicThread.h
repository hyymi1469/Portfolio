#pragma once
#include <map>
#include "Logic.h"
#include "TaskManager.h"
#include "Thread.h"
#include "ThreadManager.h"


class LogicThread
	:
	public Thread
	//public std::enable_shared_from_this< LogicThread >
{
	typedef Thread super;

private:
	TaskManager        m_taskManager; // �۾� ������

public:
	// ������
	LogicThread();

	// �Ҹ���
	virtual ~LogicThread();

	// ������ ������ �����Ѵ�.
	void Begin() override;

	// �����带 �۵���Ų��.
	void Run() override;

	// �� �����忡�� �۾��� �����Ѵ�.
	void RunTask( const TaskManager::Task& task );

};


typedef std::shared_ptr< LogicThread > logicThreadPtr;
