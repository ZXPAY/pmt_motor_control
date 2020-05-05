/**
 * @file filter.h
 * @author Xiang-Guan Deng
 * @brief Filter encoder data
 * @date 2020.xx.xx
 *
 * 將馬達編碼器濾波，濾波器只支援8項
 */
#ifndef FIR_FILTER_H
#define FIR_FILTER_H

#define FIR_NUM                8
#define MV_AVG_WINDOW_SIZE     8

void fir_update(float fir_input);
float get_fir_enc(void);

void mv_avg(uint_fast16_t mv_input);
uint_fast16_t get_mv_avg(void);

#endif /* FILTER_H */
