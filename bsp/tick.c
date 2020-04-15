#include "hal_tick.h"
#include "tick.h"
#include "control_board.h"
#include "cortex_m4.h"

void init_hw_tick(void) {
    /* enable clock source */
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
    /* enable, and do not stop in DEBUG mode */
    PIT->MCR = 0;
    /* Below PIT use in timeout and ticks */
    /* reload PIT2 every 1 ms (1000 Hz), SYSTEM_CLOCK_FREQUENCY = 72MHz */
    PIT->CHANNEL[2].LDVAL = 72000000 / 1000;
    /* enable PIT2 timer and enable interrupt */
    PIT->CHANNEL[2].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;
}

void enable_tick(void) {
    __enable_irqn(PIT2_IRQn);
}

void disable_tick(void) {
    __disable_irqn(PIT2_IRQn);
}

/* PIT2 interrupt every 1 ms */
void PIT2_IRQHandler(void) {
    inc_tick();

    /* clear flag */
    PIT->CHANNEL[2].TFLG = PIT_TFLG_TIF_MASK;
}

