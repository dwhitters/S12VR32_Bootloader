/*******************************************************************************
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2005 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
********************************************************************************
Services performed by FREESCALE in this matter are performed AS IS and without
any warranty. CUSTOMER retains the final decision relative to the total design
and functionality of the end product. FREESCALE neither guarantees nor will be
held liable by CUSTOMER for the success of this project.
FREESCALE DISCLAIMS ALL WARRANTIES, EXPRESSED, IMPLIED OR STATUTORY INCLUDING,
BUT NOT LIMITED TO, IMPLIED WARRANTY OF MERCHANTABILITY OR FITNESS FOR
A PARTICULAR PURPOSE ON ANY HARDWARE, SOFTWARE OR ADVISE SUPPLIED TO THE PROJECT
BY FREESCALE, AND OR NAY PRODUCT RESULTING FROM FREESCALE SERVICES. IN NO EVENT
SHALL FREESCALE BE LIABLE FOR INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF 
THIS AGREEMENT.

CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands or
actions by anyone on account of any damage, or injury, whether commercial, 
contractual, or tortuous, rising directly or indirectly as result of the advise
or assistance supplied CUSTOMER in connection with product, services or goods 
supplied under this Agreement.
********************************************************************************
* File      main.c
* Owner     b01802
* Version   1.0   
* Date      Dec-02-2010
* Classification   General Business Information
* Brief     Serial bootloader for families S12G, S12P, S12HY and S12HA
********************************************************************************
* Detailed Description:
*
*   Tested on: DEMO9S12PFAME, DEMO9S12HY64 
********************************************************************************
Revision History:
Version   Date          Author    Description of Changes
1.0       Dec-02-2010   b01802    Initial version
*******************************************************************************/
#include <hidef.h>  //common defines and macros
#include "Types.h"
#include "Regdef.h"
#include "SCI.h"
#include "Errors.h"
#include "PFlash.h"
#include "Srec.h"
#include "PartID.h"

#pragma MESSAGE DISABLE C5909
#pragma MESSAGE DISABLE C12056

/******************************************************************************/
//Protect the bootloader at 0xF000-0xFFFF:

const unsigned char flash_array[] @0xFF08 = {0xFF,0xFF,0xFF,0xFF,0xCF,0xFF,0xFF,0xFE};
/*
0xFF08-0xFF0B - reserved
0xFF0C - P-Flash Protection byte = 0xCF (Bootloader at 0xF000-0xFFFF is protected)
0xFF0D - D-Flash Protection byte = 0xFF (default state, protection is disabled)
0xFF0E - Flash Nonvolatile byte = 0xFF (default state, watchdog is disabled)
0xFF0F - Flash Security byte = 0xFE (default unsecured state)

Preload command file MUST contain command FLASH NOUNSECURE!
*/

/******************************************************************************/
#define RESET_VEC_SRC       0x03FFE0
#define RESET_VEC_DST       0x03EFF8

#define CPU12IVBR           0x3F00
#define SCI0Ch              0xD6

#define __SEG_START_REF(a)  __SEG_START_ ## a
#define __SEG_END_REF(a)    __SEG_END_ ## a
#define __SEG_SIZE_REF(a)   __SEG_SIZE_ ## a
#define __SEG_START_DEF(a)  extern INT8 __SEG_START_REF (a) []
#define __SEG_END_DEF(a)    extern INT8 __SEG_END_REF   (a) []
#define __SEG_SIZE_DEF(a)   extern INT8 __SEG_SIZE_REF  (a) []

__SEG_START_DEF (RAM_CODE);
__SEG_END_DEF   (RAM_CODE);
__SEG_SIZE_DEF  (RAM_CODE);


/******************************************************************************/
static UINT8 EraseFlash(void)
{    
  switch (PARTID)
  {
    //S12P
    case MASK_0M01N:
          //erase whole P-Flash memory except bootloader area
          return(PFlash_EraseSectorBySector(0x020000UL, 0x03EFDFUL));
          
    //S12HY, S12HA
    case MASK_0M34S:
          //erase whole P-Flash memory except bootloader area
          return(PFlash_EraseSectorBySector(0x030000UL, 0x03EFDFUL));
          
    //S12G
    case MASK_0N51A:
          //erase whole P-Flash memory except bootloader area
          return(PFlash_EraseSectorBySector(0x020000UL, 0x03EFDFUL));
          
    case MASK_0N95B:
          //erase whole P-Flash memory except bootloader area
          return(PFlash_EraseSectorBySector(0x004000UL, 0x03EFDFUL));
          
    case MASK_0N48A:
          //erase whole P-Flash memory except bootloader area
          return(PFlash_EraseSectorBySector(0x038000UL, 0x03EFDFUL));
          
    case MASK_1N48A:
          //erase whole P-Flash memory except bootloader area
          return(PFlash_EraseSectorBySector(0x038000UL, 0x03EFDFUL));
    
    case MASK_0N11N:
          //erase whole P-Flash memory except bootloader area
          return(PFlash_EraseSectorBySector(0x038000UL, 0x03EFDFUL));
          
    default:
          return(UnknownPartID);
  }       
}

/******************************************************************************/
static UINT8 ProgramFlash(void)
{
  SRecDataRec ProgSRec;
  UINT8 Error = noErr; 

  for(;;)
  {
    Error = RcvSRecord(&ProgSRec);
    if (Error != noErr) //go get an S-Record, return if there was an error 
      break;
    
    if (ProgSRec.RecType == EndRec) // S7, S* or S9 record? 
      break;                        // yes. return 
      
    else if (ProgSRec.RecType == HeaderRec)       //S0 record?
      continue;                     //yes. just ignore it
      
    else                            //a data record was received 
    {
        
      if ((ProgSRec.LoadAddr & 0x0000001FUL) != 0) //S-Record address alligned?
        return(SRecOddError);       //yes. return
      if (ProgSRec.NumBytes != 32)  //S-Record constant length 32?
        return(SRecOddError);       //yes. return
      
      
      
      if (ProgSRec.LoadAddr == RESET_VEC_SRC)
      {
        //Program reset vector to address 0xEFFE
        if(Error = PFlash_Program(RESET_VEC_DST, (UINT16 *)&ProgSRec.Data[24]))
          return(Error);
      } 
      else 
      {
        //is the address within a physical flash?
        switch (PARTID)
        {
          //S12P 128k flash
          case MASK_0M01N:
                if (!((ProgSRec.LoadAddr >= 0x020000UL) && (ProgSRec.LoadAddr <= 0x03EFDFUL)))
                  return(SRecRangeError);
                break;
          
          //S12HY, S12HA 64k flash
          case MASK_0M34S:
                if (!((ProgSRec.LoadAddr >= 0x030000UL) && (ProgSRec.LoadAddr <= 0x03EFDFUL)))
                  return(SRecRangeError);
                break;
          
          //S12G 128k flash
          case MASK_0N51A:
                if (!((ProgSRec.LoadAddr >= 0x020000UL) && (ProgSRec.LoadAddr <= 0x03EFDFUL)))
                  return(SRecRangeError);
                break;
          
          case MASK_0N95B:
                if (!((ProgSRec.LoadAddr >= 0x004000UL) && (ProgSRec.LoadAddr <= 0x03EFDFUL)))
                  return(SRecRangeError);
                break;
                
          case MASK_0N48A:
                if (!((ProgSRec.LoadAddr >= 0x038000UL) && (ProgSRec.LoadAddr <= 0x03EFDFUL)))
                  return(SRecRangeError);
                break;
                
          case MASK_1N48A:
                if (!((ProgSRec.LoadAddr >= 0x038000UL) && (ProgSRec.LoadAddr <= 0x03EFDFUL)))
                  return(SRecRangeError);
                break;

          case MASK_0N11N:
                if (!((ProgSRec.LoadAddr >= 0x038000UL) && (ProgSRec.LoadAddr <= 0x03EFDFUL)))
                  return(SRecRangeError);
                break;
          
          default:
                return(UnknownPartID);
        } 
        
        
        //address is OK, program the record to flash
        if(Error = PFlash_Program(ProgSRec.LoadAddr, (UINT16 *)&ProgSRec.Data))
          return(Error);
        if(Error = PFlash_Program(ProgSRec.LoadAddr+8, (UINT16 *)&ProgSRec.Data[8]))
          return(Error);
        if(Error = PFlash_Program(ProgSRec.LoadAddr+16, (UINT16 *)&ProgSRec.Data[16]))
          return(Error);
        if(Error = PFlash_Program(ProgSRec.LoadAddr+24, (UINT16 *)&ProgSRec.Data[24]))
          return(Error);
      }
    
    //feedback to serial port for each correctly programmed S-Record
    (void)putchar('*');
    
    }
  }
 
  return(Error);

}

/******************************************************************************/
static void CopyCodeToRAM(void)
{  
  UINT8 *Src;
  UINT8 *Dst;
  UINT16  SegSize;
  UINT16  x;
  
  //RAM code resides in Flash from 0xfc00 - 0xfeff
  Src = (UINT8 *)__SEG_START_REF(RAM_CODE);
  Dst = (UINT8 *)0x3D00;          //copied to RAM at 0x3D00
  SegSize = (UINT16)__SEG_SIZE_REF(RAM_CODE);
 
  for (x = 0; x < SegSize; x++)   //just copy a byte at a time
    *Dst++ = *Src++;
}

#if 0
/******************************************************************************/
static void InterruptModuleSetup(void)
{
  //set the CPU12X interrupt vector base address in RAM
  IVBR = (CPU12IVBR >> 8);
  *(UINT16 *)(CPU12IVBR + SCI0Ch) = (UINT16)SCIISR;
}
#endif

/******************************************************************************/
static void PLL_Init(void)
{
  //set 25MHz bus clock
  //PEI mode, internal 1MHz RC oscillator is used
  CPMUSYNR = 0x58;
  CPMUPOSTDIV = 0x00;
  while(!(CPMUFLG && CPMUFLG_LOCK_MASK));
}

/******************************************************************************/
static void AppExecute(void)
{
  CPMUCOP = 0x01;     //enable watchdog
  CPMUARMCOP = 0x00;
  //value written to ARMCOP register different from 0xAA or 0x55 will reset
  //the MCU immediately. The pushbutton on pin PP0 is supposed to be in default
  //state ('1'), so user application will be started.
}

/******************************************************************************/
void main(void) {

  INT8 c;
  MODRR2_MODRR = 0x06; //route SCI0 to Port S
  
  PLL_Init();           //set bus clock 25MHz
  
  FCLKDIV = 0x18;       //set flash prescaler
  
  CopyCodeToRAM();
  
  //InterruptModuleSetup();
  
  InitSCI();            //initialize SCI
 
  EnableInterrupts;     //enable interrupts for the SCI
 
  OutStr("\f\r\nS12 Bootloader v1.0\r\n");    // sign-on

  for(;;)
  {
    //display menu
    OutStr("\r\na.) Erase Flash\r\n");
    OutStr("b.) Program Flash\r\n");
    OutStr("c.) Set Baud Rate\r\n");
    OutStr("d.) Execute Application\r\n");
    OutStr("? ");
    c = getchar();      //get choice
    (void)putchar(c);   //echo choice
    OutStr("\r\n");     //next line
    
    switch (c) {
    
    case 'a':
      c = EraseFlash();
      if (c != 0)    //go erase all the Flash (except the bootloader)
        OutStr(GetErrorString(c));    // and report an error if there was one
      else
        OutStr("\r\nErased successfully!\r\n");
      break;
    
    case 'b':
      c = ProgramFlash();
      if (c != 0)    //go program the Flash
        OutStr(GetErrorString(c));    //and report an error if there was one
      else
        OutStr("\r\nDownloaded successfully!\r\n");
      break;
      
    case 'c':
      SetBaud();     //go set the SCI baud rate
      break;
    
    case 'd':
      AppExecute();  //go execute the application
      break;
    
    default:
      break;
     
    }
   
  }
  
}
/******************************************************************************/
