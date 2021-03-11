#pragma once


#include <boost/lockfree/queue.hpp>


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @class	LockFreeQueue
///
/// @brief	Lock-Free ť ó���� ���� Ŭ����
////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename super, typename T >
class LockFreeQueueBase
	:
	public super
{
private:
	std::atomic_int m_size; ///< ũ��

public:
	/// ������
	LockFreeQueueBase()
		:
		m_size( 0 )
	{
	}

	/// ������
	LockFreeQueueBase( int size )
		:
		super( size ),
		m_size( 0 )
	{
	}

	/// �Ҹ���
	~LockFreeQueueBase()
	{
		reset();
	}

	/// �� ��ü�� ó�� ���·� �ǵ�����.
	void reset()
	{
		consume_all( [this] ( T const& t )
					 {
						 _delete_if_possible( t );
					 } );
	}

	/// ũ�⸦ ��ȯ�Ѵ�.
	int size() const
	{
		return m_size;
	}

	bool push( T const& t )
	{
		m_size++;
		if ( !super::push( t ) )
		{
			m_size--;
			return false;
		}

		return true;
	}

	bool pop( T& ret )
	{
		if ( !super::pop( ret ) ) return false;

		if ( --m_size < 0 )
		{
			printf( "[LockFreeQueue::pop ] invalid size. [size: %d]", m_size.load() );
			m_size = 0;
		}

		return true;
	}

	template< typename Functor >
	size_t consume_all( Functor& f )
	{
		size_t result = super::consume_all( f );
		if ( ( m_size -= result ) < 0 )
		{
			printf( "[ LockFreeQueue::consume_all ]invalid size. [size: %d]", m_size.load() );
			m_size = 0;
		}

		return result;
	}

	template< typename Functor >
	size_t consume_all( Functor const& f )
	{
		size_t result = super::consume_all( f );
		if ( ( m_size -= result ) < 0 )
		{
			printf( "[[ LockFreeQueue::consume_all ]]invalid size. [size: %d]", m_size.load() );
			m_size = 0;
		}

		return result;
	}

private:
	/// �����ϸ� �����Ѵ�.
	template< typename Type >
	void _delete_if_possible( Type* t )
	{
		if ( t )
		{
			delete t;
			t = nullptr;
		}
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @class	LockFreeQueue
///
/// @brief	Lock-Free ť ó���� ���� Ŭ����
////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename T, size_t size = 0 >
class LockFreeQueue
	:
	public LockFreeQueueBase< boost::lockfree::queue< T, boost::lockfree::capacity< size > >, T >
{
};
