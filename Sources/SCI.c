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
contractual, or tortuous, rising directly or indirectly as a result of an advise
or assistance supplied CUSTOMER in connection with product, services or goods 
supplied under this Agreement.
********************************************************************************
* File      SCI.c
* Owner     b01802
* Version   1.0
* Date      Dec-02-2010
* Classification   General Business Information
* Brief     SCI functions
********************************************************************************
Revision History:
Version   Date          Author    Description of Changes
1.0       Dec-02-2010   b01802    Initial version
*******************************************************************************/
#include <string.h>
#include "Types.h"
#include "IO_Map.h"
#include "SCI.h"
#include "Srec.h"

/******************************************************************************/
#define Baud9600 2604
#define Baud38400 651
#define Baud57600 434
#define Baud115200 217

#define XOn 0x11
#define XOff 0x13

#define RxBufSize 96
#define TxBufSize 64

#define XOnCount RxBufSize - 8
#define XOffCount (RxBufSize-16)+2

/******************************************************************************/
UINT8 RxBuff[RxBufSize];  //receive queue
UINT8 TxBuff[TxBufSize];  //transmit queue
UINT8 RxIn;               //next available location in the Rx queue
UINT8 RxOut;              //next character to be removed from the Rx queue
UINT8 TxIn;               //next available location in the Tx queue
UINT8 TxOut;              //next character to be sent from the Tx queue
volatile UINT8 RxBAvail;  //number of bytes left in the Rx queue
volatile UINT8 TxBAvail;  //number of bytes left in the Tx queue
#define SIZE_OF_PACKETrx  5       //size of receiver buffer - constant or you can use variable size of packet
#define SIZE_OF_PACKETtx  10      //size of receiver buffer - constant or you can use variable size of packet

volatile unsigned char packet_lenght;
volatile unsigned char sc0_data_in;
volatile unsigned char buffer[SIZE_OF_PACKETrx];

volatile unsigned char pointer_to_buffer = 0;

volatile unsigned char buffer_full = 0;              //flag - full buffer

volatile unsigned char *ptr_g;                       //global pointer to data

volatile unsigned char i,ninth_bit;

unsigned char packet[SIZE_OF_PACKETtx];

UINT8 XOffSent;
  //flag indicating that an XOff character was sent (== 1, XOff was sent)
  
UINT8 XOffRcvd;
  //flag indicating that an XOff character was received (== 1, XOff was received)
  
UINT8 SendXOff;
  //flag indicating that the TxISR should send an XOff the next time it's called
  
UINT8 SendXOn;

/******************************************************************************/

INT8 getchar(void);
INT8 putchar(INT8 c);
void OutStr(INT8 * Str);

/******************************************************************************/
void OutStr(INT8 * Str)
{
  //output 1 character at a time untill we reach the end of the string
  while (*Str != 0)
    putchar(*Str++);
}

/******************************************************************************/
INT8 putchar(INT8 c)
{
 while (TxBAvail == 0);     //if there's no room in the xmit queue wait here
 
 DisableInterrupts;
 TxBuff[TxIn++] = (UINT8)c; //put the char in the buffer, inc buffer index
 if (TxIn == TxBufSize)     //buffer index go past the end of the buffer?
  TxIn = 0;                 //yes. wrap around to the start
 TxBAvail--;                //one less character available in the buffer
 if (XOffRcvd == 0)         //if an XOff has not been received...
  SCI0CR2 |= SCI0CR2_TIE_MASK;//enable SCI transmit interrupts
 EnableInterrupts;
 
 return(c);                 //return the character we sent
}

/******************************************************************************/
INT8 getchar(void)
{
 UINT8 c;                   //holds the character we'll return
 
 //if there's no characters in the Rx buffer wait here
 while (RxBAvail == RxBufSize);
 
 DisableInterrupts;
 
 //get a character from the Rx buffer & advance the Rx index
 c = RxBuff[RxOut++];
 if (RxOut == RxBufSize)    //index go past the physical end of the buffer?
  RxOut = 0;                //yes wrap around to the start
 RxBAvail++;                //1 more byte available in the receive buffer
 
 //if an XOff was previously sent & the Rx buffer
 //has more than XOnCount bytes available
 if ((XOffSent != 0) & (RxBAvail >= XOnCount))
 {
   SendXOn = 1;
   SCI0CR2 |= SCI0CR2_TIE_MASK; //enable SCI transmit interrupts
 }
 EnableInterrupts;
 
 return(c);     //return the character retrieved from receive buffer
}

/******************************************************************************/
void InitSCI(void)
{
   //SCI Baud Rate SBR[15:0] = bus clock / (16*Baud rate) 
   //or SBR[15:1] = bus clock / (2*Baud rate) for case of IREN=1(Infrared)
   //in this case bus clock = 25MHz and IREN = 0, so SBR[15:0] = 25000000 / 9600 = 2604                     //------------------------------------------------------
   SCI0BDH = (unsigned char) (Baud9600 >> 8);  
   SCI0BDL = (unsigned char) Baud9600 & 0xFF;  
                     //------------------------------------------------------
   SCI0CR1 = 0b00000100;    // SCI Control Register 1       
                     /*  0b10000100
                      *    ||||||||__ PT     Parity Type Bit /1=Odd parity,0=Even parity
                      *    |||||||___ PE     Parity Enable Bit /1=Parity function enabled
                      *    ||||||____ ILT    Idle Line Type Bit /Idle character bit count begins after (0= start)1=stop bit
                      *    |||||_____ WAKE   Wakeup Condition Bit /1=Address mark wakeup,0=Idle line wakeup
                      *    ||||______ M      Data Format Mode Bit /One start bit (0=eight data bits)1=nine data bits, one stop bit
                      *    |||_______ RSRC   Receiver Source Bit /1=Receiver input connected externally to transmitter
                      *    ||________ SCIWAI SCI Stop in Wait Mode Bit /1=SCI disabled in wait mode
                      *    |_________ LOOPS  Loop Select Bit /1=Loop operation enabled
                      */
                     // LOOPS=0 Normal operation
                     // LOOPS=1,RSRC=0 Loop mode with Rx input internally connected to Tx output 
                     // LOOPS=RSRC=1 Single-wire mode with Rx input internally connected to TXD pin, RXD pin 
                     
   /* The TIE is set by putchar and cleared by the ISR. */
                     //------------------------------------------------------                      
   SCI0CR2 = 0x2C;    // SCI Control Register 2       
                     /*  0b00101100
                      *    ||||||||__ SBK  Send Break Bit /1=Transmit break characters
                      *    |||||||___ RWU  Receiver Wakeup Bit /1=enables the wakeup function receiver interrupt requests.
                      *    ||||||____ RE   Receiver Enable Bit /1=Receiver enabled
                      *    |||||_____ TE   Transmitter Enable Bit /1=Transmitter enabled
                      *    ||||______ ILIE Idle Line Interrupt Enable Bit /1=IDLE interrupt requests enabled
                      *    |||_______ RIE  Receiver Full Interrupt Enable Bit /1=RDRF and OR interrupt requests enabled
                      *    ||________ TCIE Transmission Complete Interrupt Enable Bit /1=TC interrupt requests enabled
                      *    |_________ TIE  Transmitter Interrupt Enable Bit /1=TDRE interrupt requests enabled
                      */
                     // RDRF=receive data register full flag                             
                     // OR=overrun flag                          
                     // TC=transmission complete flag
                     // TDRE=transmit data register empty flag                                                          
                     //------------------------------------------------------
  RxIn = RxOut = TxIn = TxOut = 0;  //set the Rx & Tx queue indexes to zero
  RxBAvail = RxBufSize;     //the receive buffer is empty
  TxBAvail = TxBufSize;     //the transmit buffer is empty
  XOffSent = 0;             //XOff character has not been sent
  XOffRcvd = 0;             //XOff character has not been received
  SendXOff = 0;             //Don't send an XOff
}
 
/******************************************************************************/
void SetBaud(void)
{
  INT8 c;
  
  OutStr("\r\n1.) 9600\r\n"); //display baud rate selection menu
  OutStr("2.) 38400\r\n");
  OutStr("? ");
  c = getchar();              //get choice
  (void)putchar(c);           //echo choice
  
  //display message at old baudrate
  OutStr("\r\nChange Terminal Baud Rate and Press Return");
  
  //now wait until the last character has been shifted out
  while ((SCI0SR1 & SCI0SR1_TC_MASK) == 0);
  
  switch (c) {
  
  case '1':
    SCI0BDH = (unsigned char) (Baud9600 >> 8);  
    SCI0BDL = (unsigned char) Baud9600 & 0xFF; 
    break;
  
  case '2':
    SCI0BDH = (unsigned char) (Baud38400 >> 8);  
    SCI0BDL = (unsigned char) Baud38400 & 0xFF;
    break;
    
  default:                  //invalid choice returns to main menu
    break;
  }
  
  (void)getchar();          //and wait for the user to change the baud rate
  
  OutStr("\r\n");
  OutStr("Passed\r\n");
}

/******************************************************************************/
#pragma CODE_SEG RAM_CODE
static void RxISR(void)
{ 
  UINT8 c;
  
  c = SCI0DRL;
  
  if (c == XOff)                //host want us to stop sending data?
  {
    SCI0CR2 &= ~SCI0CR2_TIE_MASK; //yes. disable transmit interrupts
    XOffRcvd = 1; //let putchar know that it may continue to place characters
                  //in the buffer but not enable Xmit interrupts
    return;
  }
  else if (c == XOn)            //host want us to start sending data?
  {
    if (TxBAvail != TxBufSize)  //anything left in the Tx buffer?
    SCI0CR2 |= SCI0CR2_TIE_MASK;  //yes. enable transmit interrupts
    XOffRcvd = 0;
    return;
  }
  
  //if the number of bytes available in the Rx buff
  //is < XOffCount & an XOff hasn't been sent 
  if ((RxBAvail <= XOffCount) && (XOffSent == 0))
  {
    SendXOff = 1; //set flag to send an XOff, stoping the host from sending data
    XOffSent = 1; //set the flag showing that we've queued up to send an XOff
    
    //enable transmit interrupts,
    //so the TxISR will be called next time the TDRE bit is set
    SCI0CR2 |= SCI0CR2_TIE_MASK;
  }
  
  if (RxBAvail != 0)            //if there are bytes available in the Rx buffer
  {
    RxBAvail--;                 //reduce the count by 1
    RxBuff[RxIn++] = c;         //place the received byte in the buffer
    if (RxIn == RxBufSize)      //reached the physical end of the buffer?
    RxIn = 0;                   //yes. wrap around to the start
  }
}
#pragma CODE_SEG DEFAULT



         /******************************************************************************/
#pragma CODE_SEG RAM_CODE
static void TxISR(void)
{
  if (SendXOn != 0)             //request to send an XOn to the host?
  {
    SendXOn = 0;
    XOffSent = 0;               //reset the XOff flag
    SCI0DRL = XOn;               //send the character
    if (TxBAvail == TxBufSize)  //anything in the Tx buffer?
    SCI0CR2 &= ~SCI0CR2_TIE_MASK; //no. disable transmit interrupts
  }
  else if (SendXOff != 0)       //request to send an XOff to the host?
  {
    SendXOff = 0;               //yes, clear the request
    SCI0DRL = XOff;              //send the character
    if (TxBAvail == TxBufSize)  //anything in the Tx buffer?
    SCI0CR2 &= ~SCI0CR2_TIE_MASK; //no. disable transmit interrupts
  }
  else
  {
    SCI0DRL = TxBuff[TxOut++];   //remove a character from the buffer & send it
    
    if (TxOut == TxBufSize)     //reached the physical end of the buffer?
      TxOut = 0;                //yes. wrap around to the start
      
    if (++TxBAvail == TxBufSize)  //anything left in the Tx buffer?
      SCI0CR2 &= ~SCI0CR2_TIE_MASK; //no. disable transmit interrupts
   }
}
#pragma CODE_SEG DEFAULT

/******************************************************************************/
//==============================================================================
//SCI interrupt runtine
//==============================================================================
#pragma CODE_SEG RAM_CODE
interrupt 20 void SCI0_Isr(void)  
{ 
//---------------
//protection against randomly flag clearing
 unsigned char scicr2,scisr1;
 scisr1 = SCI0SR1;                          // save status register actual status
 scicr2 = SCI0CR2;                          // save control register actual status
//---------------

//******************************************************************************
//Interrupt runtine RECEIVER part
//******************************************************************************

//if receiver interrupt is enabled and corresponding interrupt flag is set
 if((scicr2 & SCI0CR2_RIE_MASK) && ((scisr1 & (SCI0SR1_OR_MASK | SCI0SR1_RDRF_MASK))))
  {
    if(scisr1 & SCI0SR1_OR_MASK)            // if overrun error do nothing/something
      {
        (void)SCI0DRL;                            // clear interrupt flag 
      }
    else
    {
         RxISR();
    }
  }
  

//******************************************************************************
//Interrupt runtine TRANCEIVER part
//******************************************************************************

  //if tranceiver interrupt is enabled and corresponding interrupt flag is set
  if((scicr2 & SCI0CR2_TIE_MASK) && ((scisr1 & SCI0SR1_TDRE_MASK)))
  { 
      TxISR();
  }
}
