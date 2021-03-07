#pragma once
#include <map>
#include "../main/Singleton.h"

class EventManager
	:
	public Singleton< EventManager >
{
	// �̺�Ʈ �׽�Ʈ ��<sessionId, eventCount >
	typedef long long Int64;
	typedef std::map< int, Int64 > TestEventMap;

private:
	TestEventMap m_testEventMap; // ������ �׽�Ʈ�� ���� �̺�Ʈ ��

public:
	// ������
	EventManager();

	// �Ҹ���
	virtual ~EventManager();

	// �̺�Ʈ �ʿ� �ش� Ű�� ���� �߰��Ѵ�.
	void SetEventMapCount( const int key, const int value );

};

