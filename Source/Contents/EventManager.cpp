#include "EventManager.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	생성자
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
EventManager::EventManager()
{
	m_start = clock(); //시간 측정 시작 YMI
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	소멸자
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
EventManager::~EventManager()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	테스트용 맵의 사이즈를 반환한다.
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
int EventManager::GetTestMapSize()
{
	return m_testMap.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	이벤트 맵에 해당 키와 값을 추가한다.
///
/// @param	key    키 값
/// @param	value  벨류 값
/// 
/// @return	None
////////////////////////////////////////////////////////////////////////////////////////////////////
void EventManager::EmplaceTestMap( const int key, const int value )
{
	m_testMap[ key ] += value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	시간 기록을 반환한다.
/// 
/// @return	double
////////////////////////////////////////////////////////////////////////////////////////////////////
double EventManager::GetTestTime()
{
	m_end = clock();
	return (double)( m_end - m_start );
}