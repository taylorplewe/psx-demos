#include <stdio.h>
#include "ship.h"
#include "constants.h"
#include "comm.h"

#define SHIP_TURN_SPEED 20

typedef struct {
	uint32_t x, y;
	int      angle;
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

void _ship_turn() {
	if (padinfo.type == 0x7) return;
	if (!(padinfo.btn & PAD_RIGHT)) {
		ship.angle += SHIP_TURN_SPEED;
	} else if (!(padinfo.btn & PAD_LEFT)) {
		ship.angle -= SHIP_TURN_SPEED;
	}
	ship_sin = isin(ship.angle);
	ship_cos = icos(ship.angle);

	printf("ship.angle: %i\n", ship.angle);
}
void _ship_turn_joystick() {
	if (padinfo.type != 0x7) return;
	// check if left stick is out of deadzone
	if (padinfo.rs_x < 128-JOYSTICK_DEADZONE
		|| padinfo.rs_x > 128+JOYSTICK_DEADZONE
		|| padinfo.rs_y < 128-JOYSTICK_DEADZONE
		|| padinfo.rs_y > 128+JOYSTICK_DEADZONE
	) {
		// calculate new sin & cos based off angle from atan2(rs_y-128, rs_x-128)
		int8_t srs_x = (int8_t)padinfo.rs_x - 128;
		int8_t srs_y = (int8_t)padinfo.rs_y - 128;
		long new_angle = (fastAtan2(srs_y, srs_x) + 1024) % ONE;

		ship_sin = isin(new_angle);
		ship_cos = icos(new_angle);
	}
}
void _ship_move() {
	if (padinfo.type == 0x7) return;
	if (!(padinfo.btn & PAD_UP)) {
		ship.x += ship_sin;
		ship.y -= ship_cos;
	} else if (!(padinfo.btn & PAD_DOWN)) {
		ship.x -= ship_sin;
		ship.y += ship_cos;
	}
}
void _ship_move_joystick() {
	if (padinfo.type != 0x7) return;
	// check if left stick is out of deadzone
	if (padinfo.ls_x < 128-JOYSTICK_DEADZONE || padinfo.ls_x > 128+JOYSTICK_DEADZONE) {
		int8_t sls_x = (int8_t)padinfo.ls_x - 128;
		ship.x += sls_x<<5;
	}
	if (padinfo.ls_y < 128-JOYSTICK_DEADZONE || padinfo.ls_y > 128+JOYSTICK_DEADZONE) {
		int8_t sls_y = (int8_t)padinfo.ls_y - 128;
		ship.y += sls_y<<5;
	}
}
void ship_update() {
	_ship_turn();
	_ship_turn_joystick();
	_ship_move();
	_ship_move_joystick();
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