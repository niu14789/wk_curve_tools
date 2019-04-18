#ifndef __718D_CRC32_H__
#define __718D_CRC32_H__

/* extern function */
unsigned int  CalculateBlockCRC32(unsigned char crc_data, unsigned int  ulCRC);

unsigned int  CalculateBufferCRC32(unsigned long ulCount,unsigned char *ucBuffer );
/* check crc */
unsigned char log_message_check(unsigned char *buffer, unsigned int len);

#endif /*__718D_CRC32_H__*/