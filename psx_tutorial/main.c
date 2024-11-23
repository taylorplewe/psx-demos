#include <sys/types.h>
#include <stdio.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>
#include <psxapi.h>
#include <psxpad.h>

#define OTLEN 8
#define BOX_SIZE 64
#define SCRN_WIDTH 320
#define SCRN_HEIGHT 240
#define BOX1_SPEED 2
#define BOX2_SPEED 4
#define NUM_BOXES 2

DISPENV disp[2];
DRAWENV draw[2];
int db;
u_long ot[2][OTLEN];

char primbuff[2][32768];
char* nextprim;

typedef struct {
	int x;
	int y;
	int hspeed;
	int vspeed;
	int width;
	int height;
	int r, g, b;
} Box;
Box* boxes[NUM_BOXES];
char* boxbuff[sizeof(Box)*NUM_BOXES];

TILE* tile;
SPRT* sprt;
DR_TPAGE* tpri;

int sprtX = 40;

TIM_IMAGE ti;
extern const uint32_t garden64_1_tim[];

u_char padbuff[2][34];
PADTYPE padinfo;

void ship_init();
void ship_update();
void loadTexture(const uint32_t* tim, TIM_IMAGE* tparam) {
	GetTimInfo(tim, tparam);
	LoadImage(tparam->prect, tparam->paddr);
	DrawSync(0);
	if (tparam->mode & 0x8) {
		LoadImage(tparam->crect, tparam->caddr);
		DrawSync(0);
	}
}
void initBoxes() {
	boxes[0] = (Box*)boxbuff;
	boxes[1] = (Box*)boxbuff+sizeof(Box);

	boxes[0]->x = 32;
	boxes[0]->y = 32;
	boxes[0]->hspeed = BOX1_SPEED;
	boxes[0]->vspeed = BOX1_SPEED;
	boxes[0]->width = BOX_SIZE;
	boxes[0]->height = BOX_SIZE;
	boxes[0]->r = 240;
	boxes[0]->g = 240;
	boxes[0]->b = 120;

	boxes[1]->x = 98;
	boxes[1]->y = 120;
	boxes[1]->hspeed = BOX1_SPEED;
	boxes[1]->vspeed = BOX1_SPEED;
	boxes[1]->width = BOX_SIZE;
	boxes[1]->height = BOX_SIZE;
	boxes[1]->r = 120;
	boxes[1]->g = 240;
	boxes[1]->b = 120;
}
void init() {
	ResetGraph(0);
	loadTexture(garden64_1_tim, &ti);

	SetDefDispEnv(&disp[0], 0, 0, SCRN_WIDTH, SCRN_HEIGHT);
	SetDefDispEnv(&disp[1], 0, SCRN_HEIGHT, SCRN_WIDTH, SCRN_HEIGHT);
	SetDefDrawEnv(&draw[0], 0, SCRN_HEIGHT, SCRN_WIDTH, SCRN_HEIGHT);
	SetDefDrawEnv(&draw[1], 0, 0, SCRN_WIDTH, SCRN_HEIGHT);

	setRGB0(&draw[0], 50, 200, 50);
	setRGB0(&draw[1], 50, 200, 50);
	draw[0].isbg = 1;
	draw[1].isbg = 1;
	// draw[0].tpage = getTPage(ti.mode&0x3, 0, ti.prect->x, ti.prect->y);
	// draw[1].tpage = getTPage(ti.mode&0x3, 0, ti.prect->x, ti.prect->y);

	PutDispEnv(&disp[0]);
	SetDispMask(1);
	PutDrawEnv(&draw[0]);

	InitPAD(padbuff[0], 34, padbuff[1], 34);
	padbuff[0][0] = padbuff[0][1] = 0xff;
	padbuff[1][0] = padbuff[1][1] = 0xff;
	StartPAD();

	FntLoad(960, 0);
	FntOpen(0, 8, 320, 224, 0, 100);

	nextprim = primbuff[0];

	db = 0;
	
	initBoxes();
	ship_init();
}

void getInput() {
	padinfo = *((PADTYPE*)padbuff[0]);

	if (padinfo.stat) {
		return;
	}

	if (!(padinfo.btn & PAD_CROSS)) {
		sprtX++;
	}
}
void display() {
	DrawSync(0);
	VSync(0);
	
	PutDispEnv(&disp[db]);
	PutDrawEnv(&draw[db]);

	DrawOTag(ot[db]+(OTLEN-1));

	db = !db;
	nextprim = primbuff[db];
}

void updateBox(Box* box) {
	box->x += box->hspeed;
	box->y += box->vspeed;

	if (box->x + BOX_SIZE > SCRN_WIDTH) {
		box->x = SCRN_WIDTH - BOX_SIZE;
		box->hspeed *= -1;
	} else if (box->x < 0) {
		box->x = 0;
		box->hspeed *= -1;
	}
	if (box->y + BOX_SIZE > SCRN_HEIGHT) {
		box->y = SCRN_HEIGHT - BOX_SIZE;
		box->vspeed *= -1;
	} else if (box->y < 0) {
		box->y = 0;
		box->vspeed *= -1;
	}
	
	tile = (TILE*)nextprim;
	setTile(tile);
	setXY0(tile, box->x, box->y);
	setWH(tile, BOX_SIZE, BOX_SIZE);
	setRGB0(tile, box->r, box->g, box->b);
	addPrim(ot[db], tile);
	nextprim += sizeof(TILE);
}

int main() {
	init();

	while (1) {
		getInput();
		FntPrint(0, "Hello California");
		FntFlush(-1);

		ClearOTagR(ot[db], OTLEN);

		for (int i = 0; i < NUM_BOXES; i++) {
			updateBox(boxes[i]);
		}

		sprt = (SPRT*)nextprim;
		setSprt(sprt);
		setXY0(sprt, sprtX, 40);
		setUV0(sprt, 0, 0);
		setWH(sprt, 64, 64);
		setClut(sprt, ti.crect->x, ti.crect->y);
		setRGB0(sprt, 128, 128, 128);
		addPrim(ot[db], sprt);
		nextprim += sizeof(SPRT);

		tpri = (DR_TPAGE*)nextprim;
		setDrawTPage( tpri, 0, 1, 
			getTPage(ti.mode&0x3, 0, ti.prect->x, ti.prect->y));
		addPrim( ot[db], tpri );
		nextprim += sizeof(DR_TPAGE);

		ship_update();

		display();
	}

	return 0;
}


/// Fixed-point arithmetic (ship)
#define mul_frac(val, frac) (val*frac)>>12
#define div_frac(val, div) (val*ONE)/div
#define SHIP_TURN_SPEED 25

uint32_t ship_x, ship_y;
uint32_t ship_velox, ship_veloy;
int ship_angle;
SVECTOR ship_triangle[] = {
	{  0, -20, 0},
	{ 10,  20, 0},
	{-10,  20, 0}
};
void ship_init() {
	ship_x = ONE*(disp[0].disp.w>>1);
	ship_y = ONE*(disp[0].disp.h>>1);
	ship_angle = 0;
}
void ship_control() {
	if (!(padinfo.btn & PAD_UP)) {
		ship_velox += (isin(ship_angle))/128;
		ship_veloy -= (icos(ship_angle))/128;
	} else if (!(padinfo.btn & PAD_DOWN)) {
		ship_velox -= (isin(ship_angle))/64;
		ship_veloy += (icos(ship_angle))/64;
	}
	if (!(padinfo.btn & PAD_LEFT)) {
		ship_angle -= SHIP_TURN_SPEED;
	} else if (!(padinfo.btn & PAD_RIGHT)) {
		ship_angle += SHIP_TURN_SPEED;
	}
}
void ship_move() {
	ship_x += ship_velox;
	ship_y += ship_veloy;
	if (ship_x > ONE*disp[0].disp.w) {
		ship_velox *= -1;
	} else if (ship_x < 0) {
		ship_velox *= -1;
	}
	if (ship_y > ONE*disp[0].disp.h) {
		ship_veloy *= -1;
	} else if (ship_y < 0) {
		ship_veloy *= -1;
	}
}
void ship_display() {
	SVECTOR v[3];
	POLY_F3* tri;
	// rotate ship based on angle
	for (int i = 0; i < 3; i++) {
		v[i].vx = (
			(
				(
					ship_triangle[i].vx * icos(ship_angle)
				) - (
					ship_triangle[i].vy * isin(ship_angle)
				)
			) >> 12
		) + (ship_x >> 12);
		v[i].vy = (
			(
				(
					ship_triangle[i].vy * icos(ship_angle)
				) + (
					ship_triangle[i].vx * isin(ship_angle)
				)
			) >> 12
		) + (ship_y >> 12);
	}
	tri = (POLY_F3*)nextprim;
	setPolyF3(tri);
	setRGB0(tri, 0xff, 0xff, 0xff);
	setXY3(tri,
		v[0].vx, v[0].vy,
		v[1].vx, v[1].vy,
		v[2].vx, v[2].vy
	);
	addPrim(ot[db], tri);
	nextprim += sizeof(POLY_F3);
	
}
void ship_update() {
	ship_control();
	ship_move();
	ship_display();
}
///