#include <sys/types.h>
#include <psxgpu.h>
#include <psxgte.h>
#include "constants.h"
#include "controller.h"
#include "player.h"
#include "comm.h"

// graphics
DISPENV disp[2];
DRAWENV draw[2];
int     frameenv_ind;
u_long  ot[2][OT_LEN];
char    primbuff[2][32768];
char*   nextprim;
void    graphics_init();

// wood
extern const u_int wood_tim[];
TIM_IMAGE           wood_tim_info;
uint32_t            wood_base_x[4] = { SCREEN_W/4, (SCREEN_W/4)*3, -SCREEN_W, SCREEN_W*2};
uint32_t            wood_base_y[4] = { SCREEN_H/2, SCREEN_H/2, SCREEN_H+(SCREEN_H/4), SCREEN_H+(SCREEN_H/4)};
uint32_t            wood_draw_x[4], wood_draw_y[4];
void                wood_init(),
                    wood_update(),
                    wood_draw();
int                 wood_tim_mode;
RECT                wood_tim_prect,
                    wood_tim_crect;

void init() {
	graphics_init();
	controller_init();
	wood_init();
	player_init();
}

void update() {
	controller_update();
	wood_update();
	player_update();
}

void display() {
	// sync & draw bg
	nextprim = primbuff[frameenv_ind];
	DrawSync(0);
	VSync(0);
	PutDispEnv(&disp[frameenv_ind]);
	PutDrawEnv(&draw[frameenv_ind]);

	// draw all primitives
	ClearOTagR(ot[frameenv_ind], OT_LEN);
	nextprim += player_draw(ot[frameenv_ind], nextprim);
	wood_draw();
	DrawOTag(ot[frameenv_ind]+(OT_LEN-1));

	// swap frame buffers
	frameenv_ind = !frameenv_ind;
}

int main() {
	init();

	while (1) {
		update();
		display();
	}

	return 0;
}

void graphics_init() {
	ResetGraph(0);

	SetDefDispEnv(&disp[0], 0, 0,        SCREEN_W, SCREEN_H);
	SetDefDispEnv(&disp[1], 0, SCREEN_H, SCREEN_W, SCREEN_H);
	SetDefDrawEnv(&draw[0], 0, SCREEN_H, SCREEN_W, SCREEN_H);
	SetDefDrawEnv(&draw[1], 0, 0,        SCREEN_W, SCREEN_H);
	frameenv_ind = 0;
	nextprim = primbuff[0];

	setRGB0(&draw[0], 0, 0, 255);
	setRGB0(&draw[1], 0, 0, 255);
	draw[0].isbg = 1;
	draw[1].isbg = 1;

	SetDispMask(1);
}

void wood_init() {
	load_texture((u_long*)wood_tim, &wood_tim_info);
	wood_tim_mode = wood_tim_info.mode;
	wood_tim_prect = *wood_tim_info.prect;
	wood_tim_crect = *wood_tim_info.crect;
	for (int i = 0; i < 4; i++) {
		wood_draw_x[i] = wood_base_x[i];
		wood_draw_y[i] = wood_base_y[i];
	}
}
void wood_update() {
	if (!(padinfo.btn & PAD_RIGHT) && wood_draw_x[0] > 0) {
		wood_draw_x[0]--;
		wood_draw_x[1]--;
		wood_draw_x[2] -= 6;
		wood_draw_x[3] -= 6;
	} else if (!(padinfo.btn & PAD_LEFT) && wood_draw_x[0] < 140) {
		wood_draw_x[0]++;
		wood_draw_x[1]++;
		wood_draw_x[2] += 6;
		wood_draw_x[3] += 6;
	}
}
void wood_draw() {
	POLY_FT4* wood_poly = (POLY_FT4*)nextprim;
	setPolyFT4(wood_poly);
	setXY4(wood_poly,
		wood_draw_x[0], wood_draw_y[0],
		wood_draw_x[1], wood_draw_y[1],
		wood_draw_x[2], wood_draw_y[2],
		wood_draw_x[3], wood_draw_y[3]
	);
	setUVWH(wood_poly, 0, 0, 255, 255);
	setClut(wood_poly, wood_tim_crect.x, wood_tim_crect.y);
	setRGB0(wood_poly, 128, 128, 128);
	wood_poly->tpage = getTPage(wood_tim_mode&0x3, 0, wood_tim_prect.x, wood_tim_prect.y);
	addPrim(ot[frameenv_ind], wood_poly);
	nextprim += sizeof(POLY_FT4);
}