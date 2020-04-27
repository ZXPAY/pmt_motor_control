/*
 * @brief	IO redirection support
 *
 * This file adds re-direction support to the library for various
 * projects. It can be configured in one of 3 ways - no redirection,
 * redirection via a UART, or redirection via semihosting. If DEBUG
 * is not defined, all printf statements will do nothing with the
 * output being throw away. If DEBUG is defined, then the choice of
 * output is selected by the DEBUG_SEMIHOSTING define. If the
 * DEBUG_SEMIHOSTING is not defined, then output is redirected via
 * the UART. If DEBUG_SEMIHOSTING is defined, then output will be
 * attempted to be redirected via semihosting. If the UART method
 * is used, then the Board_UARTPutChar and Board_UARTGetChar
 * functions must be defined to be used by this driver and the UART
 * must already be initialized to the correct settings.
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */


#include <stdio.h>

#define ITM_Port8(n)    (*((volatile unsigned char *) (0xE0000000 + 4 * n)))
#define ITM_Port16(n)   (*((volatile unsigned short *) (0xE0000000 + 4 * n)))
#define ITM_Port32(n)   (*((volatile unsigned long *) (0xE0000000 + 4 * n)))

#define DEMCR           (*((volatile unsigned long *) (0xE000EDFC)))
#define TRCENA          0x01000000

/* Write to SWO */
void _ttywrch(int ch)
{
	if (DEMCR & TRCENA) {
		while (ITM_Port32(0) == 0) {}
		ITM_Port8(0) = ch;
	}
}

