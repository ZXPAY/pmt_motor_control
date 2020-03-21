#include "hal_tick.h"
#include "MKV30F12810.h"                // NXP::Device:Startup:MKV30F12810_startup
#include "MKV30F12810_features.h"       // NXP::Device:Startup:MKV30F12810_startup

/* PIT2 interrupt every 1 ms */
void PIT2_IRQHandler(void) {
    inc_tick();

    /* clear flag */
    PIT->CHANNEL[2].TFLG = PIT_TFLG_TIF_MASK;
}

