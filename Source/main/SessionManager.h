#pragma once
#include <list>
#include <map>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <WinSock2.h>
#include "Defines.h"
#include "LockFreeQueue.h"
#include "Session.h"
#include "Singleton.h"


class User;
class SessionManager
	:
	public Singleton< SessionManager >
{
private:

	// ���� �����Ҵ� Ǯ
	typedef LockFreeQueue< Session*, FULL_USER_OBJ_POOL > UserObjectPoolQue;

	// Ȱ�� ���� ��
	typedef std::map< INT64, Session* >                   ActiveObjectPoolMap;

private:
	UserObjectPoolQue    m_userObjectPoolQue;   // ���� ������Ʈ Ǯ�� �̸� �����ϰ� ��� ���� ť
	ActiveObjectPoolMap  m_activeObjectPoolMap; // Ȱ�� ���� ���� ������Ʈ�� ��� ���� ��
	std::mutex           m_mutex;               // ���ؽ�
	std::atomic< INT64 > m_curMaxSessionNum;    // ���� ���� �ְ� ��

public:
	// ������
	SessionManager();

	// �Ҹ���
	virtual ~SessionManager();

	// ����Ǯ�� �����Ѵ�.
	void Initialize();

	// ���ο� Session ��ü�� ��´�.
	void CreateNewSession( SOCKET socket );

	// ������Ʈ�� ��ȯ�Ѵ�.
	void PushObjPool( User* user );

	// ������ ��Ŷ ���� �ð��� Ȯ�� �� ������ �ڸ���.
	void CheckAndDisconnect();

	// Ȱ�� ���� ������Ʈ�� ����.
	void PopActiveObject( const INT64 activeSessionNum );

private:
	// ������Ʈ Ǯ�� �ִ´�.
	void _PushObjPool( const int objCount );

	// ����Ǯ���� ���� �� Ǯ�� ��������� �߰��� 200�� Ǯ�� �ִ´�.
	User* _PopObjPool();
};

