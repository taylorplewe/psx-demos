#include <stdint.h>
#include <sys/types.h>
#include <psxgpu.h>
#include <psxgte.h>
#include "cooldog.h"
#include "constants.h"
#include "controller.h"
#include "comm.h"

#define CLDG_WIDTH  32
#define CLDG_HEIGHT 32
#define TURN_SPEED  20

typedef struct {
	uint32_t x, y;
	int angle;
} CLDG;
CLDG cooldog;

extern const u_long cooldog_tim[];
TIM_IMAGE         cooldog_tim_info;

void cooldog_init() {
	cooldog.x = (SCREEN_W>>1)<<12;
	cooldog.y = (SCREEN_H>>1)<<12;
	load_texture(cooldog_tim, &cooldog_tim_info);
}
void cooldog_update() {
	if (!(padinfo.btn & PAD_LEFT)) {
		cooldog.angle -= TURN_SPEED;
	} else if (!(padinfo.btn & PAD_RIGHT)) {
		cooldog.angle += TURN_SPEED;
	}
}
uint32_t draw_x[4];
uint32_t draw_y[4];
void _cooldog_calc_points() {
	int cldg_sin = isin(cooldog.angle);
	int cldg_cos = icos(cooldog.angle);

	uint32_t orig_x[4] = {
		-CLDG_WIDTH/2, CLDG_WIDTH/2, -CLDG_WIDTH/2, CLDG_WIDTH/2
	};
	uint32_t orig_y[4] = {
		-CLDG_HEIGHT/2, -CLDG_HEIGHT/2, CLDG_HEIGHT/2, CLDG_HEIGHT/2
	};

	for (int i = 0; i < 4; i++) {
		draw_x[i] = ((orig_x[i] * cldg_cos) - (orig_y[i] * cldg_sin)) >> 12;
		draw_y[i] = ((orig_y[i] * cldg_cos) + (orig_x[i] * cldg_sin)) >> 12;
	}
}
void cooldog_draw(u_long* ot, char* nextprim) {
	_cooldog_calc_points();
	// sort cooldog's poly
	const uint32_t x = cooldog.x>>12;
	const uint32_t y = cooldog.y>>12;
	POLY_FT4* cooldog_poly = (POLY_FT4*)nextprim;
	nextprim += sizeof(POLY_FT4);
	setPolyFT4(cooldog_poly);
	setXY4(cooldog_poly,
		draw_x[0] + (cooldog.x>>12), draw_y[0] + (cooldog.y>>12),
		draw_x[1] + (cooldog.x>>12), draw_y[1] + (cooldog.y>>12),
		draw_x[2] + (cooldog.x>>12), draw_y[2] + (cooldog.y>>12),
		draw_x[3] + (cooldog.x>>12), draw_y[3] + (cooldog.y>>12)
	);
	// setUVWH(cooldog_poly, 0, 0, CLDG_WIDTH, CLDG_HEIGHT);
	setUV4(cooldog_poly,
		CLDG_WIDTH, 0,
		0,          0,
		CLDG_WIDTH, CLDG_HEIGHT,
		0,          CLDG_HEIGHT
	);
	setClut(cooldog_poly, cooldog_tim_info.crect->x, cooldog_tim_info.crect->y);
	setRGB0(cooldog_poly, 128, 128, 128);
	cooldog_poly->tpage = getTPage(cooldog_tim_info.mode&0x3, 0, cooldog_tim_info.prect->x, cooldog_tim_info.prect->y);
	addPrim(ot, cooldog_poly);
	nextprim += sizeof(POLY_FT4);
}