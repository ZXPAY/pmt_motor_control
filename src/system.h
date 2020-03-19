/**
 * @file system.h
 * @author LiYu87
 * @date 2020.01.07
 * @brief system clock、system power、pinconnect之設定函式。
 *
 * init 為初始化函式，在進入bootloader之前需要對硬體做初始化。
 * deinit 為反初始化函式，在離開bootloader之前呼叫，確保使用者使用環境之暫存器
 * 皆為初始值。
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

#endif  // SYSTEM_H
