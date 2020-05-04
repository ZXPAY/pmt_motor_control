#include <stdint.h>
#include "fir_filter.h"
#include "control_board.h"

// const float fir_para[] = {0.17878867515878483, 0.07645362875487471, 0.13830445755226606, 0.10668803292702478,
//                           0.10653182358745994, 0.13840365915104513, 0.07637775730114527, 0.17845203470861293};
const float fir_para[] = {0.25, 0.25, 0.25, 0.25};
volatile uint8_t fir_index = 0;
static volatile float fir_output;
static volatile float fir_buf[8] = {0};

void fir_update(float fir_input) {
    fir_buf[fir_index] = fir_input;
    fir_index++;
    if(fir_index == FIR_NUM) {
        /* Update fir_output, about 1.4 us */
        // fir_output = fir_buf[0]*fir_para[0]+fir_buf[1]*fir_para[1]+fir_buf[2]*fir_para[2]+fir_buf[3]*fir_para[3]
        //            + fir_buf[4]*fir_para[4]+fir_buf[5]*fir_para[5]+fir_buf[6]*fir_para[6]+fir_buf[7]*fir_para[7];
        fir_output = fir_buf[0]*fir_para[0]+fir_buf[1]*fir_para[1]+fir_buf[2]*fir_para[2]+fir_buf[3]*fir_para[3];
        fir_input = 0;
    }
}

float get_fir_enc(void) {
    return fir_output;
}
