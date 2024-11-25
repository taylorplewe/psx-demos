#include <stdint.h>
#include <psxgpu.h>
#include <psxgte.h>
#include "player.h"
#include "constants.h"
#include "comm.h"
#include "controller.h"

#define PLR_WIDTH  32
#define PLR_HEIGHT 48

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
	int      facing;
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
	.frame_time = 4,
	.frame_locs = {
		{32*1, 0},
		{32*2, 0},
		{32*3, 0},
		{32*4, 0},
		{32*5, 0},
		{32*6, 0}
	}
};
PLR plr;

extern const uint32_t plr_tim[];
TIM_IMAGE             plr_tim_info;

void player_init() {
	load_texture(plr_tim, &plr_tim_info);
	plr.x = ((SCREEN_W>>1) - (PLR_WIDTH>>1))<<12;
	plr.y = (SCREEN_H>>1)<<12;
	plr.frame_num = plr.frame_counter = 0;
	plr.facing = 1;
}

void plr_anim() {
	plr.frame_counter = (plr.frame_counter + 1) % plr.anim->frame_time;
	if (plr.frame_counter == 0) {
		plr.frame_num = (plr.frame_num + 1) % plr.anim->num_frames;
	}
}
void plr_walk(int facing) {
	plr.state = WALK;
	plr.anim = &anim_walk;
	plr.facing = facing;
}
void plr_idle() {
	plr.state = IDLE;
	plr.anim = &anim_idle;
}
void player_update() {
	if (!(padinfo.btn & PAD_RIGHT)) {
		plr_walk(1);
	} else if (!(padinfo.btn & PAD_LEFT)) {
		plr_walk(-1);
	} else {
		plr_idle();
	}
	plr_anim();
}
uint32_t player_draw(u_long* ot, char* nextprim) {
	// sort player's poly
	const uint32_t x = plr.x>>12;
	const uint32_t y = plr.y>>12;
	POLY_FT4* player_poly = (POLY_FT4*)nextprim;
	setPolyFT4(player_poly);
	setXY4(player_poly,
		(plr.x>>12),             (plr.y>>12),
		(plr.x>>12) + PLR_WIDTH, (plr.y>>12),
		(plr.x>>12),             (plr.y>>12) + PLR_HEIGHT,
		(plr.x>>12) + PLR_WIDTH, (plr.y>>12) + PLR_HEIGHT
	);
	uint16_t u = plr.anim->frame_locs[plr.frame_num].vx;
	if (plr.facing != 1) u += PLR_WIDTH;
	setUVWH(player_poly, u, plr.anim->frame_locs[plr.frame_num].vy, PLR_WIDTH * plr.facing, PLR_HEIGHT);
	setClut(player_poly, plr_tim_info.crect->x, plr_tim_info.crect->y);
	setRGB0(player_poly, 128, 128, 128);
	player_poly->tpage = getTPage(plr_tim_info.mode&0x3, 0, plr_tim_info.prect->x, plr_tim_info.prect->y);
	addPrim(ot, player_poly);
	return sizeof(POLY_FT4);
}