#pragma once

#include <sys/types.h>
#include <psxgpu.h>
#include <psxgte.h>
#include "controller.h"

void ship_init();
void ship_update();
void ship_draw(u_long* ot, char* nextprim);