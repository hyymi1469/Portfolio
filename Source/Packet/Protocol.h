#pragma once


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	��Ŷ ���������� �����Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////
enum class Protocol : int
{
	None = 0,

	// �׽�Ʈ�� ��Ŷ
	Test       = 1,
	TestResult = 2,

	// �̺�Ʈ ����
	EventIncrease       = 3,
	EventIncreaseResult = 4,

	// �̺�Ʈ ����
	EventDecrease       = 5,
	EventDecreaseResult = 6,

	// ���� ���� ��û
	RequestDisconnect       = 7,
	RequestDisconnectResult = 8,
};