;********************************************************************************
;* Freescale Semiconductor Inc.
;* (c) Copyright 2004-2005 Freescale Semiconductor, Inc.
;* ALL RIGHTS RESERVED.
;********************************************************************************
;Services performed by FREESCALE in this matter are performed AS IS and without 
;any warranty. CUSTOMER retains the final decision relative to the total design 
;and functionality of the end product. FREESCALE neither guarantees nor will be 
;held liable by CUSTOMER for the success of this project.
;FREESCALE DISCLAIMS ALL WARRANTIES, EXPRESSED, IMPLIED OR STATUTORY INCLUDING, 
;BUT NOT LIMITED TO, IMPLIED WARRANTY OF MERCHANTABILITY OR FITNESS FOR 
;A PARTICULAR PURPOSE ON ANY HARDWARE, SOFTWARE OR ADVISE SUPPLIED TO THE PROJECT
;BY FREESCALE, AND OR NAY PRODUCT RESULTING FROM FREESCALE SERVICES. IN NO EVENT
;SHALL FREESCALE BE LIABLE FOR INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF 
;THIS AGREEMENT.
;
;CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands or 
;actions by anyone on account of any damage, or injury, whether commercial, 
;contractual, or tortuous, rising directly or indirectly as a result of an advise
;or assistance supplied CUSTOMER in connection with product, services or goods 
;supplied under this Agreement.
;********************************************************************************
;* File      StartS12.s
;* Owner     b01802
;* Version   1.0   
;* Date      Dec-02-2010
;* Classification   General Business Information
;* Brief     Startup file
;********************************************************************************
;* Detailed Description:
;*
;*    Tested on: DEMO9S12PFAME, DEMO9S12HY64, TWR-S12G128
;*    
;*    This startup file makes a decision whether the bootloader or user
;*    application is executed. 
;********************************************************************************
;Revision History:
;Version   Date          Author    Description of Changes
;1.0       Dec-02-2010   b01802    Initial version
;********************************************************************************
;
  xref  main
;
  xdef  _BootStart
;

AppResetVect:   equ     $effe ; here is stored reset vector of user application
StackTop:       equ     $3900


_BootStart:

  ; ---------------------------------------------------------------------------------
  ; DEMO9S12PFAME - pin PP1 - pushbutton SW501
  
  ;movb  #$02, $025C     ; enable pull up resistor on PP1 
  
  ;nop   ;wait a few cycles for stabilization of the signal
  ;nop
  ;nop
  ;nop
  ;nop
  
  ;brclr $0259, #$02, GoBoot  ; if PP0 == 0 then start the bootloader
                            ; if PP0 == 1 then start the application
  
  ;movb  #$00, $025C  ; disable pull up resistor on pin PP0 - restore default state  
  
  ldd   AppResetVect
  cpd   #$ffff
  beq   GoBoot          ; if the application reset vector is not available
                        ; then start the bootloader
  ldx   AppResetVect
  jmp    0,x              ; jump to the application
  
  
GoBoot:
  lds  #StackTop          
  jmp  main
  
;********************************************************************************
