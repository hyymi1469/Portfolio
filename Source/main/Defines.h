#pragma once


#define GAME_SERVER_INI_PATH    "./GameServer.ini"  // GameServer.ini�� �о� �� ��θ� �����Ѵ�.
#define MAX_LISTEN_COUNT	    1                   // ������ listen ������ �����Ѵ�.( WSA_MAXIMUM_WAIT_EVENTS ���� �������� ���� 64�� ���Ϸ� ������ ��)
#define FULL_USER_OBJ_POOL      60000               // �ִ� ���� ������Ʈ Ǯ�� �����Ѵ�.
#define USER_OBJ_POOL           2000               // ���� ������Ʈ Ǯ�� �����Ѵ�.
#define EXTRA_USER_OBJ_POOL     200                 // ���� ������Ʈ Ǯ�� �߰��� �����Ѵ�.
#define BUFFER_SIZE             9600                // �ۼ��� ���� ������(��ǻ�Ͱ� �б� ������ 16������ �Ѵ�.)
#define	PACKET_HEADERSIZE       6                   // ��Ŷ �ش� ������
#define	PACKET_START_POS        0                   // ��Ŷ �б� ���� ����
#define	PACKET_HEADER_READ      2                   // ��Ŷ ��� �б� ����
#define	DISCONNECT_SESSION_SEC  180                 // ������ ���� �ּ� �ð�( �� ���� )