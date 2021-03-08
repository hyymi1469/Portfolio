#include "TaskManager.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	������
/// 
/// @param	name					�̸�
/// @param	thread					������
/// @param	useConditionVariable	���� ���� ��� ����
////////////////////////////////////////////////////////////////////////////////////////////////////
TaskManager::TaskManager()
	:
	m_hasTask( false ),
	m_taskCount( 0 ),
	m_logEnabled( false ),
	m_nextLogTime( 0 )
{
	m_taskConsumer = [ this ] ( TaskExt* taskExt )
	{
		( *taskExt )( );

		*taskExt = nullptr;

		if ( !m_taskPool.push( taskExt ) )
		{
			delete taskExt;
			taskExt = nullptr;
		}

		m_taskCount.fetch_sub( 1 );

	};
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	������
/// 
/// @param	name					�̸�
/// @param	thread					������
/// @param	useConditionVariable	���� ���� ��� ����
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
void TaskManager::Initialize( const std::string& name, int threadId, bool useConditionVariable/* = true*/ )
{
	m_name = name;
	m_threadId = threadId;
	m_useConditionVariable = useConditionVariable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @breif	�Ҹ���
////////////////////////////////////////////////////////////////////////////////////////////////////
TaskManager::~TaskManager()
{
	Reset();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	�� ��ü�� ó�� ���·� �ǵ�����.
///
/// @param	logEnabled	�α� Ȱ��ȭ ����
/// @param	ownerPtr	������ ������
/// @param	ownerInfo	������ ����
///
/// @return	��ȯ �� ����
////////////////////////////////////////////////////////////////////////////////////////////////////
void TaskManager::Reset( bool logEnabled /*= false*/, void* ownerPtr /*= nullptr*/, const std::string& ownerInfo /*= ""*/ )
{
	if ( !logEnabled )
	{
		m_taskQueue.consume_all( std::default_delete< TaskExt >() );
	}
	else
	{
		m_taskQueue.consume_all( [ownerPtr, ownerInfo] ( TaskExt* task )
								 {
									 delete task;
									 task = nullptr;
								 } );
	}

	m_taskPool.consume_all( std::default_delete< TaskExt >() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	�½�ũ ������ ��ȯ�Ѵ�.
///
/// @return	�½�ũ ����
////////////////////////////////////////////////////////////////////////////////////////////////////
int TaskManager::GetTaskCount() const
{
	return m_taskCount.load();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	��� �����忡�� �۾��� �����Ѵ�.
///
/// @param	task		�۾�
/// @param	callerInfo	ȣ���� ����
/// @param	blocking	��ŷ ����
///
/// @return	��ȯ �� ����
////////////////////////////////////////////////////////////////////////////////////////////////////
void TaskManager::RunTask( const Task& task, bool blocking/* = false*/ )
{
	if ( blocking && m_threadId == _GetCurrentThreadId() )
	{
		task();
		return;
	}

	TaskExt* taskPtr = nullptr;
	if ( !m_taskPool.pop( taskPtr ) )
		taskPtr = new TaskExt();

	if ( !blocking )
	{
		*taskPtr = task;

		_PushTask( taskPtr );
		return;
	}

	std::atomic< bool > completion( false );
	*taskPtr = [&task, &completion] ()
	{
		task();
		completion.store( true );
	};

	_PushTask( taskPtr );

	while ( !completion.load() )
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	�����Ѵ�.
///
/// @param	curTime	���� �ð�
/// 
/// @return	ó���� �۾� ��
////////////////////////////////////////////////////////////////////////////////////////////////////
int TaskManager::Update( const __int64& curTime )
{
	if ( m_logEnabled && m_nextLogTime <= curTime )
	{
		int taskCount = GetTaskCount();
		if ( taskCount )
		{
			printf( "[TaskManager::Update] [name: %s, taskCount: %d]", m_name.c_str(), GetTaskCount() );
		}
		m_nextLogTime = curTime + 1000 * 10;
	}

	int taskCount = 0;

	taskCount += m_taskQueue.consume_all( m_taskConsumer );
	taskCount += m_subTaskQueue.consume_all( m_taskConsumer );

	if ( m_useConditionVariable )
		m_hasTask = ( m_taskCount > 0 );

	return taskCount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	�۾��� ���� ������ ��� �� �����Ѵ�.
///
/// @return	��ȯ �� ����
////////////////////////////////////////////////////////////////////////////////////////////////////
void TaskManager::WaitAndUpdate()
{
	if ( m_useConditionVariable )
	{
		if ( m_taskQueue.empty() && m_subTaskQueue.empty() )
		{
			std::unique_lock< std::mutex > lock( m_mutex );

			m_conditionVariable.wait(
				lock,
				[this] ()
				{
					return m_hasTask;
				} );
		}
	}

	struct timeb tb;
	ftime( &tb );
	const __int64 curTime = ( std::time( nullptr ) * 1000 ) + tb.millitm;
	Update( curTime );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	�۾��� �ִ´�.
///
/// @param	taskExt	Ȯ�� �۾�
///
/// @return	��ȯ �� ����
////////////////////////////////////////////////////////////////////////////////////////////////////
void TaskManager::_PushTask( TaskExt* taskExt )
{
	bool result = false;
	if ( m_subTaskQueue.empty() )
		result = m_taskQueue.push( taskExt );

	if ( !result )
		while ( !m_subTaskQueue.push( taskExt ) )
			std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );

	m_taskCount.fetch_add( 1 );
	if ( m_useConditionVariable )
	{
		{
			std::unique_lock< std::mutex > lock( m_mutex );
			m_hasTask = true;
		}

		m_conditionVariable.notify_one();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	���� �������� id���� �޾� ��
///
/// @param	taskExt	Ȯ�� �۾�
///
/// @return	��ȯ �� ����
////////////////////////////////////////////////////////////////////////////////////////////////////
int TaskManager::_GetCurrentThreadId()
{
	std::stringstream ss;
	ss << std::this_thread::get_id();
	return atoi( ss.str().c_str() );
}
