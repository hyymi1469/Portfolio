#pragma once
#include <map>
#include <time.h> // tmep ymi
#include "../main/Singleton.h"

class EventManager
	:
	public Singleton< EventManager >
{
	// �̺�Ʈ �׽�Ʈ ��<sessionId, eventCount >
	typedef long long Int64;
	typedef std::map< int, Int64 > TestEventMap;

public:
	TestEventMap m_testEventMap; // ������ �׽�Ʈ�� ���� �̺�Ʈ ��
	clock_t      m_start;        // temp ymi
	clock_t      m_end;          // temp ymi

public:
	// ������
	EventManager();

	// �Ҹ���
	virtual ~EventManager();

	// �̺�Ʈ �ʿ� �ش� Ű�� ���� �߰��Ѵ�.
	void SetEventMapCount( const int key, const int value );

	// ������ �׽�Ʈ�� ���� �������� ������Ų��.
	void IncreaseTestCount();

	// ������ �׽�Ʈ�� ���� �������� ��ȯ�Ѵ�.
	int GetTestCount();

	double GetTestTime();
};

