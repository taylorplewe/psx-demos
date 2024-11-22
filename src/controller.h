#include <sys/types.h>
#include <psxpad.h>
#include <psxapi.h> // InitPAD(), StartPAD()

extern u_char padbuff[2][34];
extern PADTYPE padinfo;
void controller_init();
void controller_update();