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

#define FIR_NUM     4

void fir_update(float fir_input);
float get_fir_enc(void);


#endif /* FILTER_H */
