/** \file ICHelper.c
 *  \brief library for input capturing functions
 */

/**
\addtogroup 
@{
*/

/* **************************************************************************																					
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 * 
 *            openSource wireless Platform for sensors and Internet of Things	
 * **************************************************************************
 *  FileName:        ICHelper.c
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
/// @cond debug

#include "ICHelper.h"

unsigned char icflag[]={0,0,0,0,0,0,0,0,0};
unsigned long icvalue[]={0,0,0,0,0,0,0,0,0};
BYTE ic_cnt[]={0,0,0,0,0,0,0,0,0};
BYTE ic_cnt_index[]={0,0,0,0,0,0,0,0,0};
BYTE ic_index[]={0,0,0,0,0,0,0,0,0};
unsigned long ic_pin[]={0,0,0,0,0,0,0,0,0};
BYTE ic_port[]={0,0,0,0,0,0,0,0,0};

unsigned long ic1_val[IC_SIZE];
unsigned long ic1_edge[IC_SIZE];

unsigned long ic2_val[IC_SIZE];
unsigned long ic2_edge[IC_SIZE];

unsigned long ic3_val[IC_SIZE];
unsigned long ic3_edge[IC_SIZE];

unsigned long ic4_val[IC_SIZE];
unsigned long ic4_edge[IC_SIZE];

unsigned long ic5_val[IC_SIZE];
unsigned long ic5_edge[IC_SIZE];

unsigned long ic6_val[IC_SIZE];
unsigned long ic6_edge[IC_SIZE];

unsigned long ic7_val[IC_SIZE];
unsigned long ic7_edge[IC_SIZE];

unsigned long ic8_val[IC_SIZE];
unsigned long ic8_edge[IC_SIZE];

unsigned long ic9_val[IC_SIZE];
unsigned long ic9_edge[IC_SIZE];

#if ((defined FLYPORT_G) || (defined FLYPORTGPRS))

	const BYTE ICPos[] = {
		18, 0,28, 0, 0,
		 0,13, 7, 0, 6, 
		 0, 8,24,20,11,
		 0,12, 0, 4,14,
		 3,29,25,10, 0,
		 17,0,16, 0, 0,
		 0, 0, 0, 0, 0,
		 0, 0, 0, 0
		};

#elif FLYPORT_ETH

	const BYTE ICPos[] = {
		18, 0,28, 0, 0,
		 0, 9, 7, 0, 6, 
		 0, 8,24,22,11,
		 0,26, 0, 4,14,
		 3, 1,23,10, 0,
		17, 0,16, 0, 0,
		21, 0, 0, 0, 0,
		 0, 0, 0, 0
		};

#endif

/*********************************************************/
/*Remappable inputs registers for Input Capturing function
IC1-IC2-IC3-IC4-IC5-IC6-IC7-IC8-IC9	capture ****/						
int *RPICRs[]	= {
	(int*) 0x068E, (int*) 0x068E,
	(int*) 0x0690, (int*) 0x0690,
	(int*) 0x0692, (int*) 0x0692,
	(int*) 0x0694, (int*) 0x0694,
	(int*) 0x069E
	};					
	
/*********************************************************/
/*ICXCON1 registers for IC modules*****/				
int *ICCON1Rs [] = {
	(int*) 0x0140, (int*) 0x0148, (int*) 0x0150,
	(int*) 0x0158, (int*) 0x0160, (int*) 0x0168,
	(int*) 0x0170, (int*) 0x0178, (int*) 0x0180
	};

/*********************************************************/
/*ICXCON2 registers for IC modules*****/				
int *ICCON2Rs [] = {
	(int*) 0x0142, (int*) 0x014A, (int*) 0x0152,
	(int*) 0x015A, (int*) 0x0162, (int*) 0x016A,
	(int*) 0x0172, (int*) 0x017A, (int*) 0x0182
	};
	
/*********************************************************/
/*PORT registers for IC modules*****/
int *IC_PORT[]={(int *)0x02C2,(int *)0x02CA,(int *)0x02D2,
				(int *)0x02DA,(int *)0x02E2,(int *)0x02EA,
				(int *)0x02F2};	
/// @endcond



/**
\defgroup Input Capture
@{

*/

/**
 * ICXInterrupt - Save value of the dedicated timer stored in ICXBUF.
 * \param None
 * \return None
 */
/***********ISR's for input capture modules *********/
/****************************************************/

#ifdef IC1
/***********ISR for input capture module 1 *********/
void __attribute__((interrupt, no_auto_psv)) _IC1Interrupt(void)
{
    if(ic_cnt[IC1]==1)
            IEC0bits.IC1IE = 0;
    ic_index[IC1] = ic_cnt_index[IC1]-ic_cnt[IC1];
    ic1_val[ic_index[IC1]]=IC1BUF;
	ic1_edge[ic_index[IC1]] = (*IC_PORT[ic_port[IC1]])&ic_pin[IC1];
    if(((*ICCON2Rs[IC1])&0x0100)>0)
    {
            unsigned long timer_32bit;
            timer_32bit = IC2BUF;
            timer_32bit = timer_32bit <<16;
            ic1_val[ic_index[IC1]] = ic1_val[ic_index[IC1]] | timer_32bit;
    }
    ic_cnt[IC1]=ic_cnt[IC1]-1;
    IFS0bits.IC1IF = 0;
}
#endif

#ifdef IC2
/***********ISR for input capture module 2 *********/
void __attribute__((interrupt, no_auto_psv)) _IC2Interrupt(void)
{
    if(ic_cnt[IC2]==1)
        IEC0bits.IC2IE = 0;
    ic_index[IC2] = ic_cnt_index[IC2]-ic_cnt[IC2];
    ic2_val[ic_index[IC2]]=IC2BUF;
	ic2_edge[ic_index[IC2]] = (*IC_PORT[ic_port[IC2]])&ic_pin[IC2];
    ic_cnt[IC2]=ic_cnt[IC2]-1;
    IFS0bits.IC2IF = 0;
}
#endif

#ifdef IC3
/***********ISR for input capture module 3 *********/
void __attribute__((interrupt, no_auto_psv)) _IC3Interrupt(void)
{
    if(ic_cnt[IC3]==1)
        IEC2bits.IC3IE = 0;
    ic_index[IC3] = ic_cnt_index[IC3]-ic_cnt[IC3];
    ic3_val[ic_index[IC3]]=IC3BUF;
	ic3_edge[ic_index[IC3]] = (*IC_PORT[ic_port[IC3]])&ic_pin[IC3];
    if(((*ICCON2Rs[IC3])&0x0100)>0)
    {
            unsigned long timer_32bit;
            timer_32bit = IC4BUF;
            timer_32bit = timer_32bit <<16;
            ic3_val[ic_index[IC3]] = ic3_val[ic_index[IC3]] | timer_32bit;
    }
    ic_cnt[IC3]=ic_cnt[IC3]-1;
    IFS2bits.IC3IF = 0;
}
#endif

#ifdef IC4
/***********ISR for input capture module 4 *********/
void __attribute__((interrupt, no_auto_psv)) _IC4Interrupt(void)
{
    if(ic_cnt[IC4]==1)
        IEC2bits.IC4IE = 0;
    ic_index[IC4] = ic_cnt_index[IC4]-ic_cnt[IC4];
	ic4_edge[ic_index[IC4]] = (*IC_PORT[ic_port[IC4]])&ic_pin[IC4];
    ic4_val[ic_index[IC4]]=IC4BUF;
    ic_cnt[IC4]=ic_cnt[IC4]-1;
    IFS2bits.IC4IF = 0;
}
#endif

#ifdef IC5
/***********ISR for input capture module 5 *********/
void __attribute__((interrupt, no_auto_psv)) _IC5Interrupt(void)
{
    if(ic_cnt[IC5]==1)
        IEC2bits.IC5IE = 0;
    ic_index[IC5] = ic_cnt_index[IC5]-ic_cnt[IC5];
    ic5_val[ic_index[IC5]]=IC5BUF;
	ic5_edge[ic_index[IC5]] = (*IC_PORT[ic_port[IC5]])&ic_pin[IC5];
    if(((*ICCON2Rs[IC5])&0x0100)>0)
    {
            unsigned long timer_32bit;
            timer_32bit = IC6BUF;
            timer_32bit = timer_32bit <<16;
            ic5_val[ic_index[IC5]] = ic5_val[ic_index[IC5]] | timer_32bit;
    }
    ic_cnt[IC5]=ic_cnt[IC5]-1;
    IFS2bits.IC5IF = 0;
}
#endif

#ifdef IC6
/***********ISR for input capture module 6 *********/
void __attribute__((interrupt, no_auto_psv)) _IC6Interrupt(void)
{
    if(ic_cnt[IC6]==1)
        IEC2bits.IC6IE = 0;
    ic_index[IC6] = ic_cnt_index[IC6]-ic_cnt[IC6];
	ic6_edge[ic_index[IC6]] = (*IC_PORT[ic_port[IC6]])&ic_pin[IC6];
    ic6_val[ic_index[IC6]]=IC6BUF;
    ic_cnt[IC6]=ic_cnt[IC6]-1;
    IFS2bits.IC6IF = 0;
}
#endif

#ifdef IC7
/***********ISR for input capture module 7 *********/
void __attribute__((interrupt, no_auto_psv)) _IC7Interrupt(void)
{
    if(ic_cnt[IC7]==1)
        IEC1bits.IC7IE = 0;
    ic_index[IC7] = ic_cnt_index[IC7]-ic_cnt[IC7];
    ic7_val[ic_index[IC7]]=IC7BUF;
	ic7_edge[ic_index[IC7]] = (*IC_PORT[ic_port[IC7]])&ic_pin[IC7];
    if(((*ICCON2Rs[IC7])&0x0100)>0)
    {
            unsigned long timer_32bit;
            timer_32bit = IC8BUF;
            timer_32bit = timer_32bit <<16;
            ic7_val[ic_index[IC7]] = ic7_val[ic_index[IC7]] | timer_32bit;
    }
    ic_cnt[IC7]=ic_cnt[IC7]-1;
    IFS1bits.IC7IF = 0;
}
#endif

#ifdef IC8
/***********ISR for input capture module 8 *********/
void __attribute__((interrupt, no_auto_psv)) _IC8Interrupt(void)
{
    if(ic_cnt[IC8]==1)
        IEC1bits.IC8IE = 0;
    ic_index[IC8] = ic_cnt_index[IC8]-ic_cnt[IC8];
	ic8_edge[ic_index[IC8]] = (*IC_PORT[ic_port[IC8]])&ic_pin[IC8];
    ic8_val[ic_index[IC8]]=IC8BUF;
    ic_cnt[IC8]=ic_cnt[IC8]-1;
    IFS1bits.IC8IF = 0;
}
#endif

#ifdef IC9
/***********ISR for input capture module 9 *********/
void __attribute__((interrupt, no_auto_psv)) _IC9Interrupt(void)
{
    if(ic_cnt[IC9]==1)
        IEC5bits.IC9IE = 0;
    ic_index[IC9] = ic_cnt_index[IC9]-ic_cnt[IC9];
	ic9_edge[ic_index[IC9]] = (*IC_PORT[ic_port[IC9]])&ic_pin[IC9];
    ic9_val[ic_index[IC9]]=IC9BUF;
    ic_cnt[IC9]=ic_cnt[IC9]-1;
    IFS5bits.IC9IF = 0;
}
#endif

/**
\defgroup Input Capture
@{

*/

/**
 * void ICPin(int io, BYTE ic_module) - Initializes the specified pin for Input Capture operations.
 * \param io - specifies the pin.
 * \param ic_module - specifies which IC module the pin must be initialized to. The valid parameters are the following:
 <UL>
	<LI><B>IC1</B> Input Capture 1.</LI>
	<LI><B>IC2</B> Input Capture 2.</LI>
	<LI><B>IC3</B> Input Capture 3.</LI>
	<LI><B>IC4</B> Input Capture 4.</LI>
	<LI><B>IC5</B> Input Capture 5.</LI>
	<LI><B>IC6</B> Input Capture 6.</LI>
	<LI><B>IC7</B> Input Capture 7.</LI>
	<LI><B>IC8</B> Input Capture 8.</LI>
	<LI><B>IC9</B> Input Capture 9.</LI>
</UL>
 * \return None
 */ 
void ICPin(int io, BYTE ic_module) 
{	
    io--;
    if (ICPos[io] != 0)
    {
		__builtin_write_OSCCONL(OSCCON & 0xBF);	// Unlock registers
		if(ic_module==IC9)
			*RPICRs[ic_module] = (*RPICRs[ic_module] & 0x003F) | (ICPos[io]<<8);
		else if(!(ic_module %2))
			*RPICRs[ic_module] = (*RPICRs[ic_module] & 0x3F00) | (ICPos[io]);//low byte register address
		else
			*RPICRs[ic_module] = (*RPICRs[ic_module] & 0x003F) | (ICPos[io]<<8);//high byte register address
		__builtin_write_OSCCONL(OSCCON | 0x40);
    
		WORD PORT_pin = 0;
		WORD PORT_reg = 0;
		
		#if ((defined FLYPORT_G)||(defined FLYPORTGPRS))
		
			switch(ICPos[io])
			{
				case 18:
					PORT_pin = 0b0000000000100000; //RB5
					PORT_reg = 1;
				break;
				case 28:
					PORT_pin = 0b0000000000010000; //RB4
					PORT_reg = 1;
				break;
				case 13:
					PORT_pin = 0b0000000000000100; //RB2
					PORT_reg = 1;
				break;
				case 7:
					PORT_pin = 0b0000000010000000; //RB7
					PORT_reg = 1;
				break;
				case 6:
					PORT_pin = 0b0000000001000000; //RB6
					PORT_reg = 1;
				break;
				case 8:
					PORT_pin = 0b0000000100000000; //RB8
					PORT_reg = 1;
				break;
				case 24:
					PORT_pin = 0b0000000000000010; //RD1
					PORT_reg = 3;
				break;
				case 20:
					PORT_pin = 0b0000000000100000; //RD5
					PORT_reg = 3;
				break;
				case 11:
					PORT_pin = 0b0000000000000001; //RD0
					PORT_reg = 3;
				break;
				case 12:
					PORT_pin = 0b0000100000000000; //RD11
					PORT_reg = 3;
				break;
				case 4:
					PORT_pin = 0b0000001000000000; //RD9
					PORT_reg = 3;
				break;
				case 14:
					PORT_pin = 0b0100000000000000; //RB14
					PORT_reg = 1;
				break;
				case 3:
					PORT_pin = 0b0000010000000000; //RD10
					PORT_reg = 3;
				break;
				case 29:
					PORT_pin = 0b1000000000000000; //RB15
					PORT_reg = 1;
				break;
				case 25:
					PORT_pin = 0b0000000000010000; //RD4
					PORT_reg = 3;
				break;
				case 10:
					PORT_pin = 0b0000000000010000; //RF4
					PORT_reg = 5;
				break;
				case 17:
					PORT_pin = 0b0000000000100000; //RF5
					PORT_reg = 5;
				break;
				case 16:
					PORT_pin = 0b0000000000001000; //RF3
					PORT_reg = 5;
				break;
			}
			
		#elif FLYPORT_ETH

			switch(ICPos[io])
			{
				case 18:
					PORT_pin = 0b0000000000100000; //RB5
					PORT_reg = 1;
				break;
				case 28:
					PORT_pin = 0b0000000000010000; //RB4
					PORT_reg = 1;
				break;
				case 9:
					PORT_pin = 0b0000001000000000; //RB9
					PORT_reg = 1;
				break;		
				case 7:
					PORT_pin = 0b0000000010000000; //RB7
					PORT_reg = 1;
				break;
				case 6:
					PORT_pin = 0b0000000001000000; //RB6
					PORT_reg = 1;
				break;
				case 8:
					PORT_pin = 0b0000000100000000; //RB8
					PORT_reg = 1;
				break;
				case 24:
					PORT_pin = 0b0000000000000010; //RD1
					PORT_reg = 3;
				break;
				case 22:
					PORT_pin = 0b0000000000001000; //RD3
					PORT_reg = 3;
				break;		
				case 11:
					PORT_pin = 0b0000000000000001; //RD0
					PORT_reg = 3;
				break;
				case 26:
					PORT_pin = 0b0000000010000000; //RG7
					PORT_reg = 6;
				break;
				case 4:
					PORT_pin = 0b0000001000000000; //RD9
					PORT_reg = 3;
				break;
				case 14:
					PORT_pin = 0b0100000000000000; //RB14
					PORT_reg = 1;
				break;
				case 3:
					PORT_pin = 0b0000010000000000; //RD10
					PORT_reg = 3;
				break;
				case 1:
					PORT_pin = 0b0000000000000010; //RB1
					PORT_reg = 1;
				break;
				case 23:
					PORT_pin = 0b0000000000000100; //RD2
					PORT_reg = 3;
				break;
				case 10:
					PORT_pin = 0b0000000000010000; //RF4
					PORT_reg = 5;
				break;
				case 17:
					PORT_pin = 0b0000000000100000; //RF5
					PORT_reg = 5;
				break;
				case 16:
					PORT_pin = 0b0000000000001000; //RF3
					PORT_reg = 5;
				break;
				case 21:
					PORT_pin = 0b0000000001000000; //RG6
					PORT_reg = 6;
				break;
			}

		#endif
		
		ic_pin[ic_module] = PORT_pin;
		ic_port[ic_module] = PORT_reg;
	}
}

/**
 * int ICOn(BYTE ic_module, BYTE active_edge, BYTE num_edge, BYTE mask_edge, int doublemodule) - Set up and switch on the input capture module.
 * \param BYTE ic_module - specifies the IC module. The valid parameters are the following:
	 <UL>
		<LI><B>IC1</B> Input Capture 1.</LI>
		<LI><B>IC2</B> Input Capture 2.</LI>
		<LI><B>IC3</B> Input Capture 3.</LI>
		<LI><B>IC4</B> Input Capture 4.</LI>
		<LI><B>IC5</B> Input Capture 5.</LI>
		<LI><B>IC6</B> Input Capture 6.</LI>
		<LI><B>IC7</B> Input Capture 7.</LI>
		<LI><B>IC8</B> Input Capture 8.</LI>
		<LI><B>IC9</B> Input Capture 9.</LI>
	</UL>
 * \param active_edge - specifies the edge that triggers the interrupt. The valid parameters are the following:
	<UL>
		<LI><B>RISING_EDGE</B></LI>
		<LI><B>FALLING_EDGE</B></LI>
		<LI><B>BOTH_EDGE</B></LI>
	</UL>
 * \param num_edge - number of events you want store.
 * \param mask_edge - mask for interrupt events. The valid parameters are the following:
	<UL>
		<LI><B>EVERY_EVENT</B> the interrupt triggers for every event</LI>
		<LI><B>EVERY_SECOND_EVENT</B> the interrupt triggers for every two events</LI>
		<LI><B>EVERY_THIRD_EVENT</B> the interrupt triggers for every three events</LI>
		<LI><B>EVERY_FOURTH_EVENT</B> the interrupt triggers for every four events</LI>
	</UL>
 * \param  doublemodule - specifies if the module selected is provided with a 32 bit timer.
 <B>NOTE:</B> Only the odd module (no IC9) can have a 32 timer and once a module is used with a 32 bit timer the successive module to even one, can't be used for further operations.
	The valid parameters are the following:
	<UL>
		<LI><B>SINGLE_MODULE </B> 16 bit timer</LI>	
		<LI><B>DOUBLE_MODULE </B> 32 bit timer.</LI>
	</UL>
* \return the status of operation
	<UL>
		<LI><B>0 </B> the operation was successful</LI>	
		<LI><B>-1 </B> the operation was unsuccessful</LI>	
	</UL>
 */ 
int ICOn(BYTE ic_module, BYTE active_edge, BYTE num_edge, BYTE mask_edge, int doublemodule)
{
	*ICCON1Rs[ic_module] = 0x0000;
    *ICCON2Rs[ic_module] = 0x0000;

    unsigned int data = 0;
    data = 0x1C00 + mask_edge + active_edge;

    *ICCON1Rs[ic_module] = data;
    *ICCON2Rs[ic_module] = 0x0000;

    if(doublemodule)
    {
            /***** 32 bit timer enabling****/
            /***** ICXCON2 odd module setting****/
            *ICCON2Rs[ic_module] = doublemodule;
            //ic_module++;
            /***** ICXCON1/2 even module setting****/
            *ICCON1Rs[ic_module+1] = 0x0000;//clear ICXCON1 even module register
            *ICCON1Rs[ic_module+1] = data;//set ICXCON1 even module register
            *ICCON2Rs[ic_module+1] = doublemodule;//set ICXCON2 even module register
    }
	
    ic_cnt[ic_module]=num_edge;
    ic_cnt_index[ic_module]=num_edge;

    switch(ic_module)
    {
        case 0:
                IFS0bits.IC1IF = 0; // Clear the IC1 interrupt status flag
                IEC0bits.IC1IE = 1; // Enable IC1 interrupts
                break;
        case 1:
                IFS0bits.IC2IF = 0; // Clear the IC2 interrupt status flag
                IEC0bits.IC2IE = 1; // Enable IC2 interrupts
                break;
        case 2:
                IFS2bits.IC3IF = 0; // Clear the IC3 interrupt status flag
                IEC2bits.IC3IE = 1; // Enable IC3 interrupts
                break;
        case 3:
                IFS2bits.IC4IF = 0; // Clear the IC4 interrupt status flag
                IEC2bits.IC4IE = 1; // Enable IC4 interrupts
                break;
        case 4:
                IFS2bits.IC5IF = 0; // Clear the IC5 interrupt status flag
                IEC2bits.IC5IE = 1; // Enable IC5 interrupts
                break;
        case 5:
                IFS2bits.IC6IF = 0; // Clear the IC6 interrupt status flag
                IEC2bits.IC6IE = 1; // Enable IC6 interrupts
                break;
        case 6:
                IFS1bits.IC7IF = 0; // Clear the IC7 interrupt status flag
                IEC1bits.IC7IE = 1; // Enable IC7 interrupts
                break;
        case 7:
                IFS1bits.IC8IF = 0; // Clear the IC8 interrupt status flag
                IEC1bits.IC8IE = 1; // Enable IC8 interrupts
                break;
        case 8:
                IFS5bits.IC9IF = 0; // Clear the IC9 interrupt status flag
                IEC5bits.IC9IE = 1; // Enable IC9 interrupts
                break;
        default:
                return -1;
}
return 0;
}



/**
* void ICOff(BYTE ic_module) - Switch off the input capture module.
* \param BYTE ic_module - specifies the IC module. The valid parameters are the following:
 <UL>
	<LI><B>IC1</B> Input Capture 1.</LI>
	<LI><B>IC2</B> Input Capture 2.</LI>
	<LI><B>IC3</B> Input Capture 3.</LI>
	<LI><B>IC4</B> Input Capture 4.</LI>
	<LI><B>IC5</B> Input Capture 5.</LI>
	<LI><B>IC6</B> Input Capture 6.</LI>
	<LI><B>IC7</B> Input Capture 7.</LI>
	<LI><B>IC8</B> Input Capture 8.</LI>
	<LI><B>IC9</B> Input Capture 9.</LI>
</UL>
* \return None
*/ 
void ICOff(BYTE ic_module)
{
    //*ICCON1Rs[ic_module] = *ICCON1Rs[ic_module] & 0xFFF8;
	int T32=*ICCON2Rs[ic_module];
	if((T32&0x0100)>0)
	{
		*ICCON1Rs[ic_module+1] = 0x0000;
		*ICCON2Rs[ic_module+1] = 0x0000;
	}
	*ICCON1Rs[ic_module] = 0x0000;
	*ICCON2Rs[ic_module] = 0x0000;
}

/**
* unsigned long ICValue(BYTE ic_module, BYTE edge_event) - Get the timer value.
* \param BYTE ic_module - specifies the IC module. The valid parameters are the following:
 <UL>
	<LI><B>IC1</B> Input Capture 1.</LI>
	<LI><B>IC2</B> Input Capture 2.</LI>
	<LI><B>IC3</B> Input Capture 3.</LI>
	<LI><B>IC4</B> Input Capture 4.</LI>
	<LI><B>IC5</B> Input Capture 5.</LI>
	<LI><B>IC6</B> Input Capture 6.</LI>
	<LI><B>IC7</B> Input Capture 7.</LI>
	<LI><B>IC8</B> Input Capture 8.</LI>
	<LI><B>IC9</B> Input Capture 9.</LI>
</UL>
* \param edge_event - specifies which event must return
* \return 
	<UL>
		<LI><B>the IC event timer</B></LI>
		<LI><B>0 </B> the operation was successful</LI>	
		<LI><B>-1 </B> the operation was unsuccessful</LI>	
	</UL>
*/ 
unsigned long ICValue(BYTE ic_module, BYTE edge_event)
{
    switch(ic_module)
    {
        case 0:
                return ic1_val[edge_event];
        case 1:
                return ic2_val[edge_event];
        case 2:
                return ic3_val[edge_event];
        case 3:
                return ic4_val[edge_event];
        case 4:
                return ic5_val[edge_event];
        case 5:
                return ic6_val[edge_event];
        case 6:
                return ic7_val[edge_event];
        case 7:
                return ic8_val[edge_event];
        case 8:
                return ic9_val[edge_event];
        default:
                return -1;
    }
    return -1;
}

/**
* unsigned long ICEdge(BYTE ic_module, BYTE edge_event) - Get the edge value.
* \param BYTE ic_module - specifies the IC module. The valid parameters are the following:
 <UL>
	<LI><B>IC1</B> Input Capture 1.</LI>
	<LI><B>IC2</B> Input Capture 2.</LI>
	<LI><B>IC3</B> Input Capture 3.</LI>
	<LI><B>IC4</B> Input Capture 4.</LI>
	<LI><B>IC5</B> Input Capture 5.</LI>
	<LI><B>IC6</B> Input Capture 6.</LI>
	<LI><B>IC7</B> Input Capture 7.</LI>
	<LI><B>IC8</B> Input Capture 8.</LI>
	<LI><B>IC9</B> Input Capture 9.</LI>
</UL>
* \param edge_event - specifies which event must return
* \return 
	<UL>
		<LI><B>0 </B> falling edge</LI>	
		<LI><B>>0 </B> rising edge</LI>	
		<LI><B>-1 </B> the operation was unsuccessful</LI>	
	</UL>
*/ 
unsigned long ICEdge(BYTE ic_module, BYTE edge_event)
{
    switch(ic_module)
    {
        case 0:
                return ic1_edge[edge_event];
        case 1:
                return ic2_edge[edge_event];
        case 2:
                return ic3_edge[edge_event];
        case 3:
                return ic4_edge[edge_event];
        case 4:
                return ic5_edge[edge_event];
        case 5:
                return ic6_edge[edge_event];
        case 6:
                return ic7_edge[edge_event];
        case 7:
                return ic8_edge[edge_event];
        case 8:
                return ic9_edge[edge_event];
        default:
                return -1;
    }
    return -1;
}

/**
* BYTE ICCounter(BYTE ic_module) - Get the counter value. It starts from num_edge (see ICOn) and arrives to 0. When it is 0, the input capture sequence is finished.
* \param BYTE ic_module - specifies the IC module. The valid parameters are the following:
 <UL>
	<LI><B>IC1</B> Input Capture 1.</LI>
	<LI><B>IC2</B> Input Capture 2.</LI>
	<LI><B>IC3</B> Input Capture 3.</LI>
	<LI><B>IC4</B> Input Capture 4.</LI>
	<LI><B>IC5</B> Input Capture 5.</LI>
	<LI><B>IC6</B> Input Capture 6.</LI>
	<LI><B>IC7</B> Input Capture 7.</LI>
	<LI><B>IC8</B> Input Capture 8.</LI>
	<LI><B>IC9</B> Input Capture 9.</LI>
</UL>
* \return the counter value
*/ 
BYTE ICCounter(BYTE ic_module)
{
    return ic_cnt[ic_module];
}
/**
*float ICDecode(unsigned long data1, unsigned long data2, BYTE ic_module) - Get the difference of time between two edges/events.
* \param data1 - timer value of the first edge/event
* \param data2 - timer value of the second edge/event
* \param BYTE ic_module - specifies the IC module. The valid parameters are the following:
 <UL>
	<LI><B>IC1</B> Input Capture 1.</LI>
	<LI><B>IC2</B> Input Capture 2.</LI>
	<LI><B>IC3</B> Input Capture 3.</LI>
	<LI><B>IC4</B> Input Capture 4.</LI>
	<LI><B>IC5</B> Input Capture 5.</LI>
	<LI><B>IC6</B> Input Capture 6.</LI>
	<LI><B>IC7</B> Input Capture 7.</LI>
	<LI><B>IC8</B> Input Capture 8.</LI>
	<LI><B>IC9</B> Input Capture 9.</LI>
</UL>
* \return the time in us
*/ 
float ICDecode(unsigned long data1, unsigned long data2, BYTE ic_module)
{
	float data;
	if(data2>data1)
		data = data2 - data1;
	else
	{
		int T32=*ICCON2Rs[ic_module];
		if((T32&0x0100)>0)
			data = (0xFFFFFFFF-(data1+1)) + data2;
		else
			data = (0x00010000-data1) + data2;
	}
	/*conversion factor*/
	data *= 62.5;
	data /=1000;
	return data;//time in us
}
