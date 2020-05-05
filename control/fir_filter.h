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
#define MV_AVG_MAX_WINDOW      32

void fir_update(float fir_input);
float get_fir_enc(void);


/* Moving average structure */
typedef struct _mv_avg_t {
    int_fast16_t mv_buf[MV_AVG_MAX_WINDOW];
    uint_fast8_t mv_index;
    uint_fast8_t wd_sz;       /* window size */
    uint_fast8_t shift;
    int_fast16_t mv_output;
}mv_avg_t;

void init_mv_avg(mv_avg_t *mv_avg, uint_fast8_t window_size);
void set_mv_avg(mv_avg_t *mv_avg, int_fast16_t mv_input);
int_fast16_t get_mv_avg(mv_avg_t *mv_avg);
void deinit_mv_avg(mv_avg_t *mv_avg);

#endif /* FILTER_H */
