#include "stdafx.h"
#include "App.h"
#include <stdio.h>
#include <direct.h>
#include <io.h>
#include "decode.h"
#include <string.h>
#include "config.h"

/* 100HZ data file point */
static FILE *Filep_100HZ = NULL;
/* raw file point */
static FILE *Filep_Raw = NULL;
/* mark info */
static FILE *Filep_MarkInfo = NULL;

static unsigned char rawdata[1024];
unsigned char Output_100HZ_en = 0;
extern unsigned int decode_head_error_cnt;
//@ App_100Hz_Main
//@ raw_file_path:Raw File Path
//@ out_file_path:100HZ File Path
int App_100Hz_Main( const char *raw_file_path, const char *out_file_path , char * path_create)
{
	/* process do work flag */
	static unsigned char loop_flag = 1u;
	unsigned char temp_file[128], nlength, nbytes,temp_fix[32] ;
	/* read raw data buffer */
	loop_flag = 1;
	/* restart parse seq */
	reset_log_parse();
	/* check raw_file_path prama and check out file path */
	if((raw_file_path == NULL) ||( _access(raw_file_path, 0) == -1))
	{
		/* check file access */
		DEBUG_GLOBAL("USAGE:%s Must Give\n", "raw_file");
		return -1;
	} 
	char * ptr = strrchr((char *)raw_file_path,'.');
	/* open raw file */
	fopen_s(&Filep_Raw,raw_file_path, "rb");

	if( out_file_path != NULL)
	{
		/* create 100hz file */
		fopen_s(&Filep_100HZ, out_file_path, "wb+");
		Output_100HZ_en = 1;
	}

	/* temp file output */
	nlength = strlen(raw_file_path);
	/* */
	memcpy(temp_file, raw_file_path, nlength);
	temp_file[nlength] = '\0';
	char *ptr1 = strrchr((char *)temp_file, '.');
	nbytes = strlen(ptr1);
	memcpy(temp_fix,ptr1,nbytes+1);
	/* */
	memcpy(temp_file+nlength-nbytes,".temp", 5);
	memcpy(temp_file+nlength-nbytes+5,temp_fix, nbytes+1);
	/* create 100hz file */
	fopen_s(&Filep_MarkInfo, (const char *)temp_file, "wb+");

	while(loop_flag)
	{
		static size_t nlength = 0;
		nlength = fread(rawdata, 1, sizeof(rawdata), Filep_Raw);

		if( nlength > 0)
		{
			/* decode raw process and make 100HZ */
			Decode_Raw_Data( rawdata, nlength);
		}
		else
		{
			DEBUG_GLOBAL("UnkownBytes:%d\n",decode_head_error_cnt);
			DEBUG_GLOBAL("MARK2TIMRCnt:%d\n",get_log_count(MSG_ID_LOG_MARK2TIMR));

			/* close opened file*/
			fclose(Filep_Raw);
			/*   */
			if(Output_100HZ_en)
			{
				fclose(Filep_100HZ);
			}
			/*   */
			fclose(Filep_MarkInfo);
			/* exit loop work */
			loop_flag = 0;
		}
	}

	memcpy(path_create,temp_file,strlen((const char *)temp_file));

	return 0;
}

int raw_100HZ_write_file(unsigned char *data, unsigned int len)
{	
	if(Output_100HZ_en)
	{
		return fwrite(data,1,len,Filep_100HZ);
	}
	else
	{
		return -1;
	}
}

int markinfo_write_file(unsigned char *data, unsigned int len)
{	
	return fwrite(data,1,len,Filep_MarkInfo);
}
