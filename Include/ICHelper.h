/* **************************************************************************
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 *
 *            openSource wireless Platform for sensors and Internet of Things
 * **************************************************************************
 *  FileName:        ICHelper.h
 *  Dependencies:    Microchip configs files
 *  Module:          FlyPort PRO
 *  Compiler:        Microchip C30 v3.12 or higher
 *
 *  Author               Rev.    Date              Comment
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Stefano Saccucci     1.0     05/13/2014		   First release  (core team)
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  Software License Agreement
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  This is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License (version 2) as published by
 *  the Free Software Foundation AND MODIFIED BY OpenPicus team.
 *
 *  ***NOTE*** The exception to the GPL is included to allow you to distribute
 *  a combined work that includes OpenPicus code without being obliged to
 *  provide the source code for proprietary components outside of the OpenPicus
 *  code.
 *  OpenPicus software is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *
 * Warranty
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * WE ARE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 **************************************************************************/
#include "HWlib.h"

#define IC1	(0)
#define IC2	(1)
#define IC3	(2)
#define IC4	(3)
#define IC5	(4)
#define IC6	(5)
#define IC7	(6)
#define IC8	(7)
#define IC9	(8)

#define IC_SIZE 5

#define RISING_EDGE 	0x03
#define FALLING_EDGE 	0x02
#define BOTH_EDGE 		0x01

#define EVERY_EVENT			0x00
#define	EVERY_SECOND_EVENT	0x20
#define	EVERY_THIRD_EVENT	0x40
#define	EVERY_FOURTH_EVENT	0x60

#define SINGLE_MODULE	0x0000
#define DOUBLE_MODULE	0x0100

/*****************************************************************************
	Input Capture function declarations
*****************************************************************************/
void ICPin(int, BYTE );
int ICOn(BYTE, BYTE, BYTE, BYTE, int);
void ICOff(BYTE);
unsigned long ICValue(BYTE, BYTE);
unsigned long ICEdge(BYTE, BYTE);
BYTE ICCounter(BYTE);
float ICDecode(unsigned long, unsigned long, BYTE);


