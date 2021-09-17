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
A PARTICULAR PURPOSE ON ANY HARDWARE, SOFTWARE ORE ADVISE SUPPLIED TO THE PROJECT
BY FREESCALE, AND OR NAY PRODUCT RESULTING FROM FREESCALE SERVICES. IN NO EVENT
SHALL FREESCALE BE LIABLE FOR INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF 
THIS AGREEMENT.

CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands or 
actions by anyone on account of any damage, or injury, whether commercial, 
contractual, or tortuous, rising directly or indirectly as a result of an advise
or assistance supplied CUSTOMER in connection with product, services or goods 
supplied under this Agreement.
********************************************************************************
* File      SCI.h
* Owner     b01802
* Version   1.0 
* Date      Feb-24-2010
* Classification   General Business Information
* Brief     SCI functions
********************************************************************************
* Detailed Description:
********************************************************************************
Revision History:
Version  Date          Author    Description of Changes
1.0      Feb-24-2010   b01802    Initial version
*******************************************************************************/
#ifndef _SCI_H
#define _SCI_H
 
#include "common.h"
#include <hidef.h>

#pragma CODE_SEG RAM_CODE
extern interrupt void SCIISR(void);
#pragma CODE_SEG DEFAULT


/* Typedefs */

/**
* @var uSCIBDH tSCIBDH
*/
/**
* @union uSCIBDH
* SCI Baud Rate Register
*/
typedef union uSCIBDH
{
  tU08   byte;        /**< access register as byte */
  struct
  {
    tU08 sbr8   :1;     /**< msb baud rate select bits: bit 8 */
    tU08 sbr9   :1;     /**< msb baud rate select bits: bit 9 */
    tU08 sbr10  :1;     /**< msb baud rate select bits: bit 10 */
    tU08 sbr11  :1;     /**< msb baud rate select bits: bit 11 */
    tU08 sbr12  :1;     /**< msb baud rate select bits: bit 12 */
    tU08 sbr13  :1;     /**< msb baud rate select bits: bit 13 */
    tU08 sbr14  :1;     /**< msb baud rate select bits: bit 14 */
    tU08 sbr15  :1;     /**< msb baud rate select bits: bit 15 */
  }bit;           /**< access register as bits */
  struct                  /**< alternate bits for SCIASR1 */
  {
    tU08 bkdif  :1;       /**< break detect interrupt flag */
    tU08 berrif :1;       /**< bit error interrupt flag   */
    tU08 berrv  :1;       /**< bit error value */
    tU08        :4;       /**< bit reserved area */
    tU08 rxedgif:1;       /**< receive input active edge interrupt flag */
  }abit;          /**< alternate bits for SCIBDH */
}tSCIBDH;

typedef union uSCIBDL
{
  tU08 byte;				/**< access register as byte */
  struct
  {
    tU08 sbr0  :1;     		/**< lsb baud rate select bits: bit 0 */
    tU08 sbr1  :1;			/**< lsb baud rate select bits: bit 1 */
    tU08 sbr2  :1;			/**< lsb baud rate select bits: bit 2 */
    tU08 sbr3  :1;			/**< lsb baud rate select bits: bit 3 */
    tU08 sbr4  :1;			/**< lsb baud rate select bits: bit 4 */
    tU08 sbr5  :1;			/**< lsb baud rate select bits: bit 5 */
    tU08 sbr6  :1;			/**< lsb baud rate select bits: bit 6 */
    tU08 sbr7  :1;			/**< lsb baud rate select bits: bit 7 */
  }bit;						/**< access register as bits */
  struct
  {
    tU08 bkdie  :1;     	/**< break detect interrupt enable */
    tU08 berrie :1;     	/**< bit error interrupt enable */
    tU08        :5;     	/**< bit reserved area */
    tU08 rxedgie:1;     	/**< receive input active edge interrupt enable */
  }abit;					/**< alternate bits for SCIBDL */
}tSCIBDL;

/**
* @var uSCICR1 tSCICR1
*/
/**
* @union uSCICR1
* SCI Control Register 1
*/
typedef union uSCICR1
{
  tU08   byte;        /**< access register as byte */
  struct
  {
    tU08 pt      :1;      /**< parity type */
    tU08 pe      :1;      /**< parity enable   */
    tU08 ilt     :1;      /**< idle line type */
    tU08 wake    :1;      /**< wake up by address mark/idle */
    tU08 m       :1;      /**< mode select */
    tU08 rsrc    :1;      /**< receiver source */
    tU08 sciswai :1;      /**< stop in wait mode  */
    tU08 loops   :1;      /**< loop mode/single wire mode enable */
  }bit;           /**< access register as bits */
  struct
  {
    tU08 bkdfe   :1;      /**< break detect feature enable */
    tU08 berrm0  :1;      /**< bit error mode */
    tU08 berrm1  :1;      /**< bit error mode */
    tU08         :2;      /**< bit reserved area */
    tU08 tnp     :2;      /**< transmit narrow pulse */
    tU08 iren    :1;      /**< IRDA enable */
  }abit;          /**< alternate bits for SCIACR1 */
}tSCICR1;

/**
* @var uSCICR2 tSCICR2
*/
/**
* @union uSCICR2
* * SCI Control Register 2
*/
typedef union uSCICR2
{
  tU08   byte;				/**< access register as byte */
  struct
  {
    tU08 sbk   :1;      	/**< send break character */
    tU08 rwu   :1;      	/**< receiver wake-up control */
    tU08 re    :1;      	/**< receiver enable */
    tU08 te    :1;      	/**< transmitter enable */
    tU08 ilie  :1;      	/**< idle line interrupt enable */
    tU08 rie   :1;      	/**< receiver interrupt enable   */
    tU08 tcie  :1;      	/**< transmit complete interrupt enable */
    tU08 tie   :1;      	/**< transmit interrupt enable   */
  }bit;						/**< access register as bits */
}tSCICR2;

/**
* @var uSCISR1 tSCISR1
*/
/**
* @union uSCISR1
* SCI Status Register 1
*/
typedef union uSCISR1
{
  tU08   byte;				/**< access register as byte */
  struct
  {
    tU08 pf    :1;      	/**< parity error flag  */
    tU08 fe    :1;      	/**< framing error flag */
    tU08 nf    :1;      	/**< noise error flag */
    tU08 orf   :1;      	/**< overrun error flag - 'or' in specification */
    tU08 idle  :1;      	/**< idle line detected flag */
    tU08 rdrf  :1;      	/**< receive data register full flag */
    tU08 tc    :1;      	/**< transmit complete flag */
    tU08 tdre  :1;      	/**< transmit data register empty flag */
  }bit;						/**< access register as bits */
}tSCISR1;

/**
* @var uSCISR2 tSCISR2
*/
/**
* @union uSCISR2
* SCI status register 2
*/
typedef union uSCISR2
{
  tU08   byte;				/**< access register as byte */
  struct
  {
     tU08 raf    :1;   		/**< receiver active flag */
     tU08 txdir  :1;   		/**< receiver active flag */
     tU08 brk13  :1;   		/**< 13-bit break */
     tU08 rxpol  :1;   		/**< receiver polarity */
     tU08 txpol  :1;   		/**< transmitter polarity */
     tU08        :2;   		/**< bit reserved area */
     tU08 amap   :1;   		/**< alternate register map */
  }bit;						/**< access register as bits */
}tSCISR2;

/**
* @var uSCIDRH tSCIDRH
*/
/**
* @union uSCIDRH
* SCI Data Register hight
*/
typedef union uSCIDRH
{
  tU08   byte;				/**< access register as byte */
  struct
  {
    tU08       :6;      	/**< not used */
    tU08 t8    :1;      	/**< transmit bit 8 */
    tU08 r8    :1;      	/**< receive bit 8   */
  }bit;						/**< access register as bits */
}tSCIDRH;

/**
* @struct tSCI
* sci datastructure
*/
typedef struct          	  /**< sci datastructure  */
  {
  volatile tSCIBDH  scibdh;   /**< sci baud rate high register / alternate status register */
  volatile tSCIBDL  scibdl;   /**< sci baud rate low register  / alternate control register 1 */
  volatile tSCICR1  scicr1;   /**< sci control register 1      / alternate control register 2 */
  volatile tSCICR2  scicr2;   /**< sci control register 2 */
  volatile tSCISR1  scisr1;   /**< sci status register 1 */
  volatile tSCISR2  scisr2;   /**< sci status register 2 */
  volatile tSCIDRH  scidrh;   /**< sci data register high */
  volatile tREG08   scidrl;   /**< sci data register low */
  }tSCI;
  

extern void ResetSCI(void);
extern void InitSCI(void);
extern void OutStr(char * Str);
extern INT8 putchar(INT8 c);
extern INT8 getchar(void);
extern void SetBaud(void);

#endif
/******************************************************************************/
