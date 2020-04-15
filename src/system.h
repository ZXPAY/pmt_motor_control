/**
 * @file system.h
 * @author Deng Xiang-Guan
 * @date 2020.01.07
 * @brief system clock初始化和預設systick設定為1K
 */

#ifndef SYSTEM_H
#define SYSTEM_H

/**
 * @brief 初始化 system clock
 */
void system_clock_init(void);

/**
 * @brief 反初始化 system clock
 */
void system_clock_deinit(void);

/**
 * @brief 初始化 systick clock
 */
void systick_init(void);

#endif  /* SYSTEM_H */
