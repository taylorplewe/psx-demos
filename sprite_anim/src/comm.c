#include "comm.h"

void load_texture(const uint32_t* tim_data, TIM_IMAGE* tim) {
	GetTimInfo(tim_data, tim);
	LoadImage(tim->prect, tim->paddr);
	DrawSync(0);
	if (tim->mode & 0x8) {
		LoadImage(tim->crect, tim->caddr);
		DrawSync(0);
	}
}