/*
 * common.h
 *
 *  Created on: Jul 28, 2015
 *      Author: B52932
 */

#ifndef COMMON_H_
#define COMMON_H_

/* Global definitions */
#define TRUE  1
#define FALSE 0

/**
* @var typedef unsigned char tU08
* unsigned 8 bits definition
*/
typedef unsigned char   tU08;   /**< unsigned 8 bit definition */

/**
* @var typedef unsigned char tU16
* unsigned 16 bits definition
*/
typedef unsigned short    tU16;   /**< unsigned 16 bit definition */

/**
* @var uREG08 tREG08
* register 8 bits
*/

/**
* @union uREG08
* register 8 bits
*/
typedef union uREG08    	/**< 8 bit register with byte and bit access*/
  {
  tU08  byte;           	/**< access whole register e.g. var.byte   = 0xFF;*/
  struct
    {
    tU08 _0 :1;				/**< bit 0 */
    tU08 _1 :1;				/**< bit 1 */
	tU08 _2 :1;				/**< bit 2 */
    tU08 _3 :1;				/**< bit 3 */
    tU08 _4 :1;				/**< bit 4 */
    tU08 _5 :1;				/**< bit 5 */
    tU08 _6 :1;				/**< bit 6 */
    tU08 _7 :1;				/**< bit 7 */
    }bit;					/**< access bit at a time  e.g. var.bit._7 = 1;   */
  }tREG08;


/**
* @var uREG16 tREG16
* register 16 bits
*/

/**
* @union uREG16
* register 16 bits
*/
typedef union uREG16   		/**< 16 bit register with word and byte access*/
  {
  tU16 word;           		/**< access whole word */
  struct
    {
    tREG08 msb;				/**< msb byte */
    tREG08 lsb;				/**< lsb byte */
    }byte;					/**< access byte at a time */
  }tREG16;

// TODO Error signals
  
  /* Prototypes */


#endif /* COMMON_H_ */
