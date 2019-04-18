#ifndef __718D_APP_H__
#define __718D_APP_H__

/* extern function */
int App_100Hz_Main( const char *raw_file_path, const char *out_file_path );

int raw_100HZ_write_file(unsigned char *data, unsigned int len);

int markinfo_write_file(unsigned char *data, unsigned int len);
#endif/* __718D_APP_H__ */