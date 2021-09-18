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
* File      Srec.c
* Owner     b01802
* Version   1.0   
* Date      Dec-02-2010
* Classification   General Business Information
* Brief     Srecord utilities
********************************************************************************
Revision History:
Version   Date          Author    Description of Changes
1.0       Dec-02-2010   b01802    Initial version
*******************************************************************************/
#include <ctype.h>
#include <string.h>
#include "Types.h"
#include "IO_Map.h"
#include "Errors.h"
#include "Srec.h"
#include "SCI.h"

/******************************************************************************/
static UINT8 GetHexByte(UINT8 *MemByte)
{
  INT8 c;           //used to hold the received ASCII hex byte
  
  *MemByte = 0;
  
  c = getchar();    //get an ASCII hex byte from stdin (upper nybble of byte)
  if (!isxdigit(c))      //is it a valid hex digit
    return(BadHexData);  //no. return an error
    
  //convert the ASCII character to a binary hex nybble
  *MemByte = (isdigit(c) ? c - '0' : c - 'A' + 10) << 4;
  
  c = getchar();    //get an ASCII hex byte from stdin (lower nybble of byte)
  if (!isxdigit(c))      //is it a valid hex digit
    return(BadHexData);  //no. return an error
    
  //convert the ASCII character to a binary hex nybble
  *MemByte += (isdigit(c) ? c - '0' : c - 'A' + 10);
  
  return(noErr);         //return 'no error'
}

/******************************************************************************/
UINT8 RcvSRecord(SRecDataRec *SRecData)
{ 
  UINT8 Error;
    //used to hold the return error code
  UINT8 CheckSum;
    //used to hold calculated checksum value
  UINT8 SRecByte;
    //used to hold memory/address bytes received through GetHexByte()
  UINT32 LoadAddress = 0;
    // holds the converted load address of S-Record
  UINT8 NumDataBytes;
    //holds the number of 'data' bytes in the received S-Record
  UINT8 x;
    //used as a loop counter when receiving 'data' field of the S-Record
  UINT8 AddrBytes;
    //number of bytes in the Srecord address
  UINT8 RecType;
    //holds the character received after the Start-of-Record character (S)


  for(;;)
  {
    while (getchar() != 'S'); //get a character. Start of record character?
    
    
    RecType = getchar();      //yes. get the next character
    
    if (RecType == '0')       //S0 record?
    {
      SRecData->RecType = HeaderRec;    //yes. mark as a header record
      AddrBytes = 2;          //with 2 address bytes
    }
    else if (RecType == '9')  //S9 record?
    {
      SRecData->RecType = EndRec;       //yes. mark as a end record
      AddrBytes = 2;          //with 2 address bytes
    }
    else if (RecType == '8')  //S8 record?
    {
      SRecData->RecType = EndRec;       //yes. mark as a end record
      AddrBytes = 3;          //with 3 address bytes
    }
    else if (RecType == '7')  //S7 record?
    {
      SRecData->RecType = EndRec;       //yes. mark as a end record
      AddrBytes = 4;          //with 4 address bytes
    }
    else if (RecType == '2')  //S2 record?
    {
      SRecData->RecType = DataRec;      //yes. mark as a data record
      AddrBytes = 3;          //with 3 address bytes
    }
    else
      continue;        //we only receive S0, S2, S7, S8 & S9 S-Records
      
    Error = GetHexByte(&SRecData->NumBytes);
    if (Error != noErr)       //get the record length field. valid hex byte?
      return(Error);          //no. return an error
      
    //initialize the checksum with the record length
    CheckSum = SRecData->NumBytes;
    
    //subtract number of bytes in the address+1 to get the length of data field
    NumDataBytes = (SRecData->NumBytes -= (AddrBytes +1));
    
    if (NumDataBytes > MaxSRecLen)  //is the S-Record longer than allowed?
      return(SRecTooLong);          //yes. report an error
      
    for (x = 0; x < AddrBytes; x++)
    {
      Error = GetHexByte(&SRecByte);
      if (Error != noErr)     //get a byte of the load address. valid hex byte?
        return(Error);        //no return an error
      CheckSum += SRecByte;   //add it into the checksum
      LoadAddress = (LoadAddress << 8) + SRecByte;
    }
    
    SRecData->LoadAddr = LoadAddress;
    
    for (x = 0; x < NumDataBytes; x++)  //receive the data field
    {
      Error = GetHexByte(&SRecByte);
      if (Error != noErr)     //get a data field byte. valid hex byte?
        return(Error);        //no return an error
      CheckSum += SRecByte;   //yes. add it in to the checksum
      
      SRecData->Data[x] = SRecByte;
    }
    
    Error = GetHexByte(&SRecByte);
    
    //get the record checksum (last byte of the record). valid hex byte?
    if (Error != noErr)
      return(Error);          //no. return an error
      
    CheckSum = ~CheckSum;     //take 1's compliment of calculated checksum
    if (CheckSum != SRecByte) //calculated checksum == received checksum ?
      return(CheckSumErr);    //no. return an error
    else
      return(noErr);

  }
}
/******************************************************************************/
