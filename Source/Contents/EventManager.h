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

private:
	TestEventMap m_testMap; // ������ �׽�Ʈ�� ���� �̺�Ʈ ��
	clock_t      m_start;   // temp ymi
	clock_t      m_end;     // temp ymi

public:
	// ������
	EventManager();

	// �Ҹ���
	virtual ~EventManager();

	// �׽�Ʈ�� ���� ����� ��ȯ�Ѵ�.
	int GetTestMapSize();

	// �̺�Ʈ �ʿ� �ش� Ű�� ���� �߰��Ѵ�.
	void EmplaceTestMap( const int key, const int value );

	// �ð� ����� ��ȯ�Ѵ�.
	double GetTestTime();

	TestEventMap& GetTestMap()
	{
		return m_testMap;
	};
};

