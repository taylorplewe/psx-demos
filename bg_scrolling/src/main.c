#include <sys/types.h>
#include <psxgpu.h>
#include "constants.h"

// graphics
DISPENV disp_env[2];
DRAWENV draw_env[2];
int     framebuff_ind;
u_long  ot[2][OT_LEN];
char    primbuff[2][32768];
char*   nextprim;
void    graphics_init();

void init() {
	graphics_init();
}
void update() {

}
void draw() {
	// sync & draw bg
	nextprim = primbuff[framebuff_ind];
	DrawSync(0);
	VSync(0);
	PutDispEnv(&disp_env[framebuff_ind]);
	PutDrawEnv(&draw_env[framebuff_ind]);

	// draw all primitives
	ClearOTagR(ot[framebuff_ind], OT_LEN);
	DrawOTag(ot[framebuff_ind]+(OT_LEN-1));

	// swap frame buffers
	framebuff_ind = !framebuff_ind;
}

int main() {
	init();

	while (1) {
		update();
		draw();
	}

	return 0;
}

// graphics
void graphics_init() {
	ResetGraph(0);

	SetDefDispEnv(&disp_env[0], 0, 0,        SCREEN_W, SCREEN_H);
	SetDefDispEnv(&disp_env[1], 0, SCREEN_H, SCREEN_W, SCREEN_H);
	SetDefDrawEnv(&draw_env[0], 0, SCREEN_H, SCREEN_W, SCREEN_H);
	SetDefDrawEnv(&draw_env[1], 0, 0,        SCREEN_W, SCREEN_H);
	framebuff_ind = 0;
	nextprim = primbuff[0];

	setRGB0(&draw_env[0], 0, 0, 255);
	setRGB0(&draw_env[1], 0, 0, 255);
	draw_env[0].isbg = 1;
	draw_env[1].isbg = 1;

	SetDispMask(1);
}