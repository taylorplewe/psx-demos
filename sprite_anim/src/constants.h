#include <psxgpu.h>

#define SCREEN_W 320
#define SCREEN_H 240
#define OT_LEN 8

typedef struct {
	int num_frames;
	int frame_time;
	DVECTOR frame_locs[];
} ANIM;