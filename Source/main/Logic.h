#pragma once
#include <memory>
#include "Singleton.h"
#include "TaskManager.h"


class Logic
	:
	public Singleton< Logic >
{
private:
	//TaskManager m_taskManager; // �۾� ������

public:
	// ������
	Logic();

	// �Ҹ���
	virtual ~Logic();

};


typedef std::shared_ptr< Logic > LogicPtr;
