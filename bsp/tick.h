/**
 * @file tick.h
 * @author Xiang-Guan Deng
 * @brief tick, timeout implement file
 * @date 2020.xx.xx
 *
 * It's hardware depentdent. User should implement these function
 */

#ifndef TICK_H
#define TICK_H


void init_hw_tick(void);
void enable_tick(void);
void disable_tick(void);


#endif /* TICK_H */
