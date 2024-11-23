#include "controller.h"

u_char padbuff[2][34];
PADTYPE padinfo;
void controller_init() {
	InitPAD(padbuff[0], 34, padbuff[1], 34);
	padbuff[0][0] = padbuff[0][1] = 0xff;
	padbuff[1][0] = padbuff[1][1] = 0xff;
	StartPAD();
}
void controller_update() {
	padinfo = *((PADTYPE*)padbuff[0]);
}