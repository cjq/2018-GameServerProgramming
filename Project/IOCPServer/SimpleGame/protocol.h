//#include "stdafx.h"
#define SERVERPORT		4000

#define MAX_BUFF_SIZE	4096
#define MAX_PACKET_SIZE 255

#define BOARD_WIDTH		800
#define BOARD_HEIGHT	800
#define SPACESIZE		10

#define VIEW_RADIUS		7

#define	MOVE_TIME		1000

#define MAX_USER		10

#define NPC_START		100
#define NUM_OF_NPC		200'100

#define MAX_STR_SIZE	100

#define CS_UP			1
#define CS_DOWN			2
#define CS_LEFT			3
#define CS_RIGHT		4
#define CS_CHAT			5

#define SC_POS				1
#define SC_PUT_PLAYER		2
#define SC_REMOVE_PLAYER	3
#define SC_CHAT				4

constexpr int SPACE_X = BOARD_WIDTH / SPACESIZE;
constexpr int SPACE_Y = BOARD_HEIGHT / SPACESIZE;

#pragma pack (push, 1)

struct cs_packet_up {
	BYTE size;
	BYTE type;
};

struct cs_packet_down {
	BYTE size;
	BYTE type;
};

struct cs_packet_left {
	BYTE size;
	BYTE type;
};

struct cs_packet_right {
	BYTE size;
	BYTE type;
};

struct cs_packet_chat {
	BYTE size;
	BYTE type;
	WCHAR message[MAX_STR_SIZE];
};

struct sc_packet_pos {
	BYTE size;
	BYTE type;
	WORD id;
	SHORT x;
	SHORT y;
};

struct sc_packet_put_player {
	BYTE size;
	BYTE type;
	WORD id;
	SHORT x;
	SHORT y;
};
struct sc_packet_remove_player {
	BYTE size;
	BYTE type;
	WORD id;
};

struct sc_packet_chat {
	BYTE size;
	BYTE type;
	WORD id;
	WCHAR message[MAX_STR_SIZE];
};

#pragma pack (pop)