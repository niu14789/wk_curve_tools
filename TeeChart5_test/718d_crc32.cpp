#include "stdafx.h"
#include "718d_crc32.h"

/* Calculates the CRC-32 of a block of data all at once */
#define CRC32_POLYNOMIAL 0xEDB88320L

//@	log message CRC check
//@ return 1:ok
//@	return 0:err
unsigned char log_message_check(unsigned char *buffer, unsigned int len)
{
	return (CalculateBufferCRC32(len-4,buffer) == *(unsigned int  *)&buffer[len-4]);
}

//@ CRC32Value:Calculate a CRC value to be used by CRC calculation functions.
//@ 
//@
static unsigned int  CRC32Value(unsigned int  i)
{
	unsigned int  j;
	unsigned int  ulCRC;
	ulCRC = i;

	for ( j = 8 ; j > 0; j-- )
	{
		if ( ulCRC & 1 )
			ulCRC = ( ulCRC >> 1 ) ^ CRC32_POLYNOMIAL;
		else
			ulCRC >>= 1;
		}
	return ulCRC;
}
//@ CalculateBlockCRC32
//@ 
//@
unsigned int  CalculateBlockCRC32(unsigned char crc_data, unsigned int  ulCRC)
{
	unsigned int  ulTemp1;
	unsigned int  ulTemp2;

	ulTemp1 = ( ulCRC >> 8 ) & 0x00FFFFFF;
	ulTemp2 = CRC32Value( ((unsigned int ) (ulCRC ^ crc_data) ) & 0xff );
	ulCRC = ulTemp1 ^ ulTemp2;

	return( ulCRC );
}

//@ CalculateBufferCRC32
//@ 
//@
unsigned int  CalculateBufferCRC32(unsigned long ulCount,unsigned char *ucBuffer ) 
{
	unsigned int  ulTemp1;
	unsigned int  ulTemp2;
	unsigned int  ulCRC = 0;
	
	while ( ulCount-- != 0 )
	{
		ulTemp1 = ( ulCRC >> 8 ) & 0x00FFFFFFL;
		ulTemp2 = CRC32Value( ((int) ulCRC ^ *ucBuffer++ ) & 0xff );
		ulCRC = ulTemp1 ^ ulTemp2;
	}
	
	return( ulCRC );
}