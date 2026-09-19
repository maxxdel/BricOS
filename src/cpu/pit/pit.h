#pragma once

#define IRQ_TIMER               0
#define PIT_DATA_0              0x40
#define PIT_DATA_1              0x41
#define PIT_DATA_2              0x42
#define PIT_COMMAND             0x43
#define PIT_MODE3_SQUARE_WAVE   0x36

#include "../../kernel/utils/utils.h"

void init_timer(unsigned int frequency);