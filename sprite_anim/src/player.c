#include <stdint.h>
#include <psxgpu.h>
#include <psxgte.h>
#include "player.h"
#include "constants.h"
#include "comm.h"
#include "controller.h"

typedef enum {
	IDLE,
	WALK
} PLR_STATE;
typedef struct {
	uint32_t x, y;
	int      state;
	ANIM*    anim;
	int      frame_num;
	int      frame_counter;
} PLR;

ANIM anim_idle = {
	.num_frames = 1,
	.frame_time = 1,
	.frame_locs = {
		{0, 0}
	},
};
ANIM anim_walk = {
	.num_frames = 6,
	.frame_time = 5,
	.frame_locs = {
		{32*1, 0},
		{32*2, 0},
		{32*3, 0},
		{32*4, 0},
		{32*5, 0},
		{32*6, 0}
	}
};
PLR       plr;
TIM_IMAGE plr_tim;

extern const uint32_t iron_knight_tim[];

void plr_init() {
	load_texture(iron_knight_tim, &plr_tim);
	plr.x = (SCREEN_W>>2)<<12;
	plr.y = (SCREEN_H>>1)<<12;
	plr.frame_num = plr.frame_counter = 0;
}

void plr_anim() {
	plr.frame_counter = (plr.frame_counter + 1) % plr.anim->frame_time;
	if (plr.frame_counter == 0) {
		plr.frame_num = (plr.frame_num + 1) % plr.anim->num_frames;
	}
}
void plr_walk(int right) {
	plr.state = WALK;
	plr.anim = &anim_walk;
	if (right) {
		plr.x += ONE;
	} else {
		plr.x -= ONE;
	}
}
void plr_idle() {
	plr.state = IDLE;
	plr.anim = &anim_idle;
}
void plr_update() {
	if (!(padinfo.btn & PAD_RIGHT)) {
		plr_walk(1);
	} else if (!(padinfo.btn & PAD_LEFT)) {
		plr_walk(0);
	} else {
		plr_idle();
	}
	plr_anim();
}
void plr_draw(u_long* ot, char* nextprim) {
	// sort knight image
	SPRT*   plr_sprt = (SPRT*)nextprim;
	setSprt(plr_sprt);
	setXY0( plr_sprt, plr.x>>12, plr.y>>12);
	setUV0( plr_sprt, plr.anim->frame_locs[plr.frame_num].vx, plr.anim->frame_locs[plr.frame_num].vy);
	setWH(  plr_sprt, 32, 48);
	setRGB0(plr_sprt, 128, 128, 128);
	setClut(plr_sprt, plr_tim.crect->x, plr_tim.crect->y);
	addPrim(ot, plr_sprt);
	nextprim += sizeof(SPRT);

	// sort texture page
	DR_TPAGE* tpage = (DR_TPAGE*)nextprim;
	setDrawTPage(tpage, 0, 1, getTPage(plr_tim.mode&0x3, 0, plr_tim.prect->x, plr_tim.prect->y));
	addPrim(ot, tpage);
	nextprim += sizeof(DR_TPAGE);
}