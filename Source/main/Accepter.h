#pragma once
#include "Thread.h"


class Accepter
	:
	public Thread
{
public:
	// ������
	Accepter();

	// �Ҹ���
	virtual ~Accepter();

	// ������ ������ �����Ѵ�.
	void Begin() override;

	// �����带 �۵���Ų��.
	void Run() override;
};


typedef std::shared_ptr< Accepter > AccepterPtr;
