#include <sys/types.h>
#include <psxgpu.h>
#include <psxgte.h>
#include "constants.h"
#include "controller.h"
#include "ship.h"

// graphics
DISPENV disp[2];
DRAWENV draw[2];
int frameenv_ind;
u_long ot[2][OT_LEN];
char primbuff[2][32768];
char* nextprim;

void init_graphics() {
	ResetGraph(0);

	SetDefDispEnv(&disp[0], 0, 0, SCREEN_W, SCREEN_H);
	SetDefDispEnv(&disp[1], 0, SCREEN_H, SCREEN_W, SCREEN_H);
	SetDefDrawEnv(&draw[0], 0, SCREEN_H, SCREEN_W, SCREEN_H);
	SetDefDrawEnv(&draw[1], 0, 0, SCREEN_W, SCREEN_H);
	frameenv_ind = 0;
	nextprim = primbuff[0];

	setRGB0(&draw[0], 0, 0, 255);
	setRGB0(&draw[1], 0, 0, 255);
	draw[0].isbg = 1;
	draw[1].isbg = 1;

	SetDispMask(1);
}
void init() {
	init_graphics();
	controller_init();
	ship_init();
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
	ship_draw(ot[frameenv_ind], nextprim);
	DrawOTag(ot[frameenv_ind]+(OT_LEN-1));

	// swap frame buffers
	frameenv_ind = !frameenv_ind;
}

void update() {
	controller_update();
	ship_update();
}

int main() {
	init();

	while (1) {
		update();
		display();
	}

	return 0;
}