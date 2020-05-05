#include <stdint.h>
#include "fir_filter.h"
#include "control_board.h"

// const float fir_para[] = {0.1729849303169786, 0.07671098704435053, 0.13553251466712268, 0.11354916782850921,
//                           0.11381076804573631, 0.13643788566902515, 0.0773712822646857, 0.17360265220328586};
// const float fir_para[] = {0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125};
// const float fir_para[] = {0.25, 0.25, 0.25, 0.25};
const float fir_para[] = {0.3159478704231127, 0.18406092903906668, 0.1841058246534395, 0.31588537447889564};

static volatile float fir_output;
static volatile float fir_buf[8] = {0};

void fir_update(float fir_input) {
    for(uint8_t i=FIR_NUM-1;i>0;i--) {
        fir_buf[i] = fir_buf[i-1];
    }
    fir_buf[0] = fir_input;
    fir_output = fir_buf[0]*fir_para[0]+fir_buf[1]*fir_para[1]+fir_buf[2]*fir_para[2]+fir_buf[3]*fir_para[3]
                 + fir_buf[4]*fir_para[4]+fir_buf[5]*fir_para[5]+fir_buf[6]*fir_para[6]+fir_buf[7]*fir_para[7];
}

float get_fir_enc(void) {
    return fir_output;
}