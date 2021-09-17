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
* File      DeviceInfo.h
* Owner     b01802
* Version   1.0 
* Date      Feb-24-2010
* Classification   General Business Information
* Brief     Definiton of used register
********************************************************************************
* Detailed Description:
********************************************************************************
Revision History:
Version  Date          Author    Description of Changes
1.0      Feb-24-2010   b01802    Initial version
*******************************************************************************/
#ifndef _REGDEF_H
#define _REGDEF_H

#define IVBR        (*(volatile UINT8 *)  0x00000120)
#define CPMUSYNR    (*(volatile UINT8 *)  0x00000034)
#define CPMUPOSTDIV (*(volatile UINT8 *)  0x00000036)
#define CPMUFLG     (*(volatile UINT8 *)  0x00000037)
#define CPMUCOP     (*(volatile UINT8 *)  0x0000003C)
#define CPMUARMCOP  (*(volatile UINT8 *)  0x0000003F)
#define FCLKDIV     (*(volatile UINT8 *)  0x00000100)
#define SCIDRL      (*(volatile UINT8 *)  0x000000CF)
#define SCICR1      (*(volatile UINT8 *)  0x000000CA)
#define SCICR2      (*(volatile UINT8 *)  0x000000CB)
#define SCISR1      (*(volatile UINT8 *)  0x000000CC)
#define FSTAT       (*(volatile UINT8 *)  0x00000106)
#define FCCOBIX     (*(volatile UINT8 *)  0x00000102)
#define FCCOB       (*(volatile UINT16 *) 0x0000010A)
#define SCIBDH       (*(volatile UINT8 *) 0x000000C8)
#define SCIBDL       (*(volatile UINT8 *) 0x000000C9)
#define PARTID      (*(volatile UINT16 *) 0x00003380)

#define SCICR2_SBK_MASK                 1
#define SCICR2_RWU_MASK                 2
#define SCICR2_RE_MASK                  4
#define SCICR2_TE_MASK                  8
#define SCICR2_ILIE_MASK                16
#define SCICR2_RIE_MASK                 32
#define SCICR2_TCIE_MASK                64
#define SCICR2_TIE_MASK                 128

#define SCISR1_PF_MASK                  1
#define SCISR1_FE_MASK                  2
#define SCISR1_NF_MASK                  4
#define SCISR1_OR_MASK                  8
#define SCISR1_IDLE_MASK                16
#define SCISR1_RDRF_MASK                32
#define SCISR1_TC_MASK                  64
#define SCISR1_TDRE_MASK                128

#define FSTAT_MGSTAT0_MASK              1
#define FSTAT_MGSTAT1_MASK              2
#define FSTAT_MGBUSY_MASK               8
#define FSTAT_FPVIOL_MASK               16
#define FSTAT_ACCERR_MASK               32
#define FSTAT_CCIF_MASK                 128

#define CPMUFLG_LOCK_MASK               8

#if (defined(_MC9S12ZVL32_H)) /* S12ZVL32 specific definitions */
/* CPMU definitions */
#define CPMU_ADDR 0x06C4	/* Address in which CPMUSYNR is declared */
#define VCO_MIN 32000000
#define VCO_MID 48000000
#define VCO_MAX 64000000
#define POSTDIV_MAX 31
#define SYNDIV_MAX 63
#define REFDIV_MAX 15
#define REF_MIN_TH1 1000000
#define REF_MIN_TH2 2000000
#define REF_MID_TH  6000000
#define REF_MAX_TH 12000000
#define SELECT_VCO_RANGE(x) (x <= VCO_MID)? 0 : 1 
#define SELECT_REF_RANGE(x) ( (((x >= REF_MIN_TH1) && (x <= REF_MIN_TH2))? 0 : 1) ^ ((x <= REF_MID_TH)? 0 : 3) | ((x <= REF_MAX_TH)? 0 : 1))
/* SCI definitions */
#define SCI0 					0x0700
#define SCI1 					0x0710
#define LIN_PHY_INTEGRATED		1						/* LIN physical layer integrated */
#define SCI_ROUTED 				SCI0					/* Channel routed to LINPHY */
#define MODIFY_ROUTING 			MODRR0_S0L0RR			/* Register that routes SCI0 to external pins */
#define ROUTING_VALUE			0b110					/* Value to write to the register */
/* SPI definitions */
#define SPPR_MAX	7
#define SPR_MAX		7
#define SPI_ADDR	0x0780
#endif /* END of S12ZVL32 specific definitions */

#if 1 // (defined(_MC9S12VR64_H)) S12VR64 specific definitions 
/* CPMU definitions */
#define CPMU_ADDR 0x0034		/* Address in which CPMUSYNR is declared */
#define VCO_MIN 32000000
#define VCO_MID 48000000
#define VCO_MAX 50000000
#define POSTDIV_MAX 31
#define SYNDIV_MAX 63
#define REFDIV_MAX 15
#define REF_MIN_TH1 1000000
#define REF_MIN_TH2 2000000
#define REF_MID_TH  6000000
#define REF_MAX_TH 12000000
#define MODRR2_MODRR (*(volatile UINT8 *)  0x0000024F)
#define SELECT_VCO_RANGE(x) (x <= VCO_MID)? 0 : 1 
#define SELECT_REF_RANGE(x) ( (((x >= REF_MIN_TH1) && (x <= REF_MIN_TH2))? 0 : 1) ^ ((x <= REF_MID_TH)? 0 : 3) | ((x <= REF_MAX_TH)? 0 : 1))
/* SCI definitions */
#define SCI0 			0x00C8
#define SCI1 			0x00D0
#define LIN_PHY_INTEGRATED		1				/* LIN physical layer integrated */
#define SCI_ROUTED 		SCI0					/* Channel routed to LINPHY */
#define MODIFY_ROUTING 	MODRR2_MODRR			/* Register that routes SCI0 to external pins */
#define ROUTING_VALUE	0b1100					/* Value to write to the register */
/* SPI definitions */
#define SPPR_MAX	7
#define SPR_MAX		7
#define SPI_ADDR	0x00D8
#endif /* END of S12VR64 specific definitions */

#if (defined(_MC9S12ZVML128_H)) /* S12ZVML128 specific definitions */
/* CPMU definitions */
#define CPMU_ADDR 0x06C4		/* Address in which CPMUSYNR is declared */
#define VCO_MIN 32000000	
#define VCO_MIN_TH 48000000
#define VCO_MID_TH  80000000
#define VCO_MAX	   100000000
#define POSTDIV_MAX 31
#define SYNDIV_MAX 63
#define REFDIV_MAX 15
#define REF_MIN_TH1 1000000
#define REF_MIN_TH2 2000000
#define REF_MID_TH  6000000
#define REF_MAX_TH 12000000
#define SELECT_VCO_RANGE(x) ( ((x <= VCO_MIN_TH)? 0 : 1) | ((x > VCO_MID_TH) ? 0b10:0) ) 
#define SELECT_REF_RANGE(x) ( (((x >= REF_MIN_TH1) && (x <= REF_MIN_TH2))? 0 : 1) ^ ((x <= REF_MID_TH)? 0 : 3) | ((x <= REF_MAX_TH)? 0 : 1))
/* SCI definitions */
#define SCI0 			0x0700
#define SCI1 			0x0710
#define LIN_PHY_INTEGRATED		1				/* LIN physical layer integrated */
#define SCI_ROUTED 		SCI0					/* Channel routed to LINPHY */
#define MODIFY_ROUTING 	MODRR0_S0L0RR			/* Register that routes SCI0 to external pins */
#define ROUTING_VALUE	0b110					/* Value to write to the register */
/* SPI definitions */
#define SPPR_MAX	7
#define SPR_MAX		7
#define SPI_ADDR	0x0780
#endif /* END of S12ZVML128 specific definitions */

#if (defined(_MC9S12ZVC192_H)) /* S12ZVC192 specific definitions */
/* CPMU definitions */
#define CPMU_ADDR 0x06C4		/* Address in which CPMUSYNR is declared */
#define VCO_MIN 32000000
#define VCO_MID 48000000
#define VCO_MAX 64000000
#define POSTDIV_MAX 31
#define SYNDIV_MAX 63
#define REFDIV_MAX 15
#define REF_MIN_TH1 1000000
#define REF_MIN_TH2 2000000
#define REF_MID_TH  6000000
#define REF_MAX_TH 12000000
#define SELECT_VCO_RANGE(x) (x <= VCO_MID)? 0 : 1 
#define SELECT_REF_RANGE(x) ( (((x >= REF_MIN_TH1) && (x <= REF_MIN_TH2))? 0 : 1) ^ ((x <= REF_MID_TH)? 0 : 3) | ((x <= REF_MAX_TH)? 0 : 1))
/* SCI definitions */
#define SCI0 			0x0700
#define SCI1 			0x0710
#define LIN_PHY_INTEGRATED		0				/* This device does not have LINPHY */
/* SPI definitions */
#define SPPR_MAX	7
#define SPR_MAX		7
#define SPI_ADDR	0x0780
#endif /* END of S12ZVC192 specific definitions */

#if (defined(_MC9S12ZVH_H)) /* S12ZVH specific definitions */
/* CPMU definitions */
#define CPMU_ADDR 0x06C4		/* Address in which CPMUSYNR is declared */
#define VCO_MIN 32000000
#define VCO_MID 48000000
#define VCO_MAX 64000000
#define POSTDIV_MAX 31
#define SYNDIV_MAX 63
#define REFDIV_MAX 15
#define REF_MIN_TH1 1000000
#define REF_MIN_TH2 2000000
#define REF_MID_TH  6000000
#define REF_MAX_TH 12000000
#define SELECT_VCO_RANGE(x) (x <= VCO_MID)? 0 : 1 
#define SELECT_REF_RANGE(x) ( (((x >= REF_MIN_TH1) && (x <= REF_MIN_TH2))? 0 : 1) ^ ((x <= REF_MID_TH)? 0 : 3) | ((x <= REF_MAX_TH)? 0 : 1))
/* SCI definitions */
#define SCI0 			0x0700
#define SCI1 			0x0710
#define LIN_PHY_INTEGRATED		0				/* This device does not have LINPHY */
/* SPI definitions */
#define SPPR_MAX	7
#define SPR_MAX		7
#define SPI_ADDR	0x0780
#endif /* END of S12ZVH specific definitions */

#endif /* HW_DEF_H_ */

/******************************************************************************/

