#pragma once
#include <boost/utility.hpp>
#include <condition_variable>
#include <ctime>
#include <iostream>
#include <mutex>
#include <sstream>
#include <sys/timeb.h>
#include "LockFreeQueue.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @class	TaskManager
///
/// @brief	�۾� ������ ���� Ŭ����
////////////////////////////////////////////////////////////////////////////////////////////////////
class TaskManager
	:
	public boost::noncopyable // ���縦 ���� ���� ��!
{
	// boost::noncopyable �� ��ӹ����� �Ʒ� �� ���� private�� ���� ���ص� ���� ���� �� ����!
	//TaskManager( const TaskManager& );
	//const TaskManager& operator=( const TaskManager& ){};


public:
	/// �۾� Ÿ�� ����
	typedef std::function< void() > Task;

	///< �۾� Ÿ�� ����
	typedef Task TaskExt;

public:
	/// ���� �۾� ť Ÿ�� ����
	typedef LockFreeQueue< TaskExt*, 256 > FixedTaskQueue;

	/// ���� �۾� ť Ÿ�� ����
	typedef LockFreeQueue< TaskExt* > VariantTaskQueue;

	/// �۾� ó�� �Լ� Ÿ�� ����
	typedef std::function< void( TaskExt* ) > TaskConsumer;

private:
	std::string             m_name;                 ///< �̸�
	int                     m_threadId;             ///< ������ �ĺ���
	std::mutex              m_mutex;            ///< ��� ���ؽ�
	std::condition_variable m_conditionVariable;    ///< ���� ����
	FixedTaskQueue          m_taskPool;             ///< �۾� ��ü Ǯ
	FixedTaskQueue          m_taskQueue;            ///< �۾� ť
	VariantTaskQueue        m_subTaskQueue;         ///< ���� �۾� ť
	TaskConsumer            m_taskConsumer;         ///< �۾� ó����
	bool                    m_hasTask;              ///< �½�ũ ���� ����
	std::atomic_int         m_taskCount;            ///< �½�ũ ����
	bool                    m_logEnabled;           ///< �α� Ȱ��ȭ ����
	__int64                 m_nextLogTime;          ///< ���� �α� �ð�
	bool                    m_useConditionVariable; ///< ���� ���� ��� ����

public:
	/// ������
	TaskManager();

	/// �Ҹ���
	~TaskManager();

	// �ʱ�ȭ�Ѵ�.
	void Initialize( const std::string& name, int threadId, bool useConditionVariable = true );

	/// �� ��ü�� ó�� ���·� �ǵ�����.
	void Reset( bool logEnabled = false, void* ownerPtr = nullptr, const std::string& ownerInfo = "" );

	/// �����带 �����Ѵ�.
	void SetThreadId( int threadId ) {
		m_threadId = threadId;
	}

	/// �۾� ó���ڸ� �����Ѵ�.
	void SetTaskConsumer( const TaskConsumer& taskConsumer ) {
		m_taskConsumer = taskConsumer;
	}

	/// �α� Ȱ��ȭ ���θ� �����Ѵ�.
	void SetLogEnabled( bool logEnabled ) {
		m_logEnabled = logEnabled;
	}

	/// �̸��� ��ȯ�Ѵ�.
	const std::string& GetName() const {
		return m_name;
	}

	/// �����带 ��ȯ�Ѵ�.
	int GetThreadId() const {
		return m_threadId;
	}

	/// �۾� ó���ڸ� ��ȯ�Ѵ�.
	const TaskConsumer& GetTaskConsumer() const {
		return m_taskConsumer;
	}

	/// �½�ũ ������ ��ȯ�Ѵ�.
	int GetTaskCount() const;

	/// ��� �����忡�� �۾��� �����Ѵ�.
	void RunTask( const Task& task, bool blocking = false );

	/// �����Ѵ�.
	int Update( const __int64& curTime );

	/// �۾��� ���� ������ ��� �� �����Ѵ�.
	void WaitAndUpdate();

private:
	/// �۾��� �ִ´�.
	void _PushTask( TaskExt* taskExt );

	int _GetCurrentThreadId();

private:
	static bool ms_useConditionVariable; ///< ���� ���� ��� ����

public:
	/// ���� ���� ��� ���θ� �����Ѵ�.
	static void SetUseConditionVariable( bool useConditionVariable ) {
		ms_useConditionVariable = useConditionVariable;
	}
};
