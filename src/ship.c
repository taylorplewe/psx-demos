#include "ship.h"
#include "constants.h"

#define SHIP_TURN_SPEED 20

typedef struct {
	uint32_t x, y;
	int angle;
} SHIP;
SHIP    ship;
SVECTOR ship_base_triangle[] = {
	{   0,-50, 0},
	{  10, 10, 0},
	{ -10, 10, 0}
};
SVECTOR ship_triangle[3];
int     ship_cos;
int     ship_sin;

void ship_init() {
	ship.angle = 0;
	ship.x = (SCREEN_W >> 1)<<12;
	ship.y = (SCREEN_H >> 1)<<12;
}

void ship_turn() {
	if (!(padinfo.btn & PAD_RIGHT)) {
		ship.angle += SHIP_TURN_SPEED;
	} else if (!(padinfo.btn & PAD_LEFT)) {
		ship.angle -= SHIP_TURN_SPEED;
	}
	ship_sin = isin(ship.angle);
	ship_cos = icos(ship.angle);
}
void ship_move() {
	if (!(padinfo.btn & PAD_UP)) {
		ship.x += ship_sin;
		ship.y -= ship_cos;
	} else if (!(padinfo.btn & PAD_DOWN)) {
		ship.x -= ship_sin;
		ship.y += ship_cos;
	}
}
void ship_update() {
	ship_turn();
	ship_move();
}

void _ship_calc_triangle() {
	for (int i = 0; i < 3; i++) {
		ship_triangle[i].vx =
			((ship_base_triangle[i].vx * ship_cos) - (ship_base_triangle[i].vy * ship_sin)) >> 12;
		ship_triangle[i].vy =
			((ship_base_triangle[i].vy * ship_cos) + (ship_base_triangle[i].vx * ship_sin)) >> 12;
	}
}
void ship_draw(u_long* ot, char* nextprim) {
	_ship_calc_triangle();
	POLY_F3* ship_poly = (POLY_F3*)nextprim;
	setPolyF3(ship_poly);
	setRGB0(ship_poly, 0xff, 0xff, 0xff);
	setXY3(ship_poly,
		ship_triangle[0].vx + (ship.x>>12), ship_triangle[0].vy + (ship.y>>12),
		ship_triangle[1].vx + (ship.x>>12), ship_triangle[1].vy + (ship.y>>12),
		ship_triangle[2].vx + (ship.x>>12), ship_triangle[2].vy + (ship.y>>12)
	);
	addPrim(ot, ship_poly);
	nextprim += sizeof(POLY_F3);
}