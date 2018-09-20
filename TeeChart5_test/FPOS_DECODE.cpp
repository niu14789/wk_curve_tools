#include "stdafx.h"
#include "TeeChart5_test.h"
#include "TeeChart5_testDlg.h"

typedef struct
{
	double lon;
	double lat;
	double height;
	double CT1;
	double CT2;
	double CT3;
	double CT4;
	double CT5;
	double PITCH;
	double ROLL;
	double YAW;
	unsigned int index;
	unsigned int rev;
}FPOS_DEF;

typedef struct
{
    double lon;
	double lat;
	double height;
	double PITCH;
	double ROLL;
	double YAW;
	unsigned int index;
	unsigned int rev;
}POS_DEF;

FPOS_DEF fpos;
POS_DEF  pos;
static unsigned int position = 0;
char fpos_buffer[5*1024*1024];
char one_line_buffer[1024];
/*---------------------*/
int CTeeChart5_testDlg::fpos_process(char *path,char *path_tmp,unsigned int mode)
{
	FILE * rb = fopen(path,"rb");
	/*------------------------*/
	if( rb == NULL )
	{
		MessageBox(_T("文件无法打开"),_T("tips"),0);
		return (-1);
	}
	/*-----------------------*/
	char fpos_name[200];
	/*-----------------------*/
	memset(fpos_name,0,sizeof(fpos_name));
	/*-----------------------*/
	sprintf(fpos_name,"%s.tmp",path);
	/*-----------------------------*/
	FILE * wb = fopen(fpos_name,"wb+");
	/*------------------------*/
	CString show;
    USES_CONVERSION;
	/*---------------------------------------------*/
	show = A2T(fpos_name);
	/*------------------------*/
	if( wb == NULL )
	{
		if( DeleteFile(show) == 0 )
		{
			MessageBox(_T("文件无法删除"),_T("tips"),0);
			return (-1);
		}
		/* open again */
		wb = fopen(fpos_name,"wb+");
	    /*------------------------*/
	    if( wb == NULL )
	    {
			MessageBox(_T("文件无法创建"),_T("tips"),0);
			return (-1);
		}
	}
	/*---------------------------------------------*/
	SetFileAttributes(show,FILE_ATTRIBUTE_HIDDEN);
	/*-----------------------*/
	int len = fread(fpos_buffer,1,sizeof(fpos_buffer),rb);
	/*-----------------------*/
	if( len == sizeof(fpos_buffer) )
	{
		MessageBox(_T("一些数据可能已丢失"),_T("tips"),0);
	}
	/*----------------------*/
	if( len == 0 )
	{
		MessageBox(_T("文件为空"),_T("tips"),0);
		/*------------------------------------*/
		return (-1);
	}
	/*----------------------*/
	position = 0;
	/*----------------------*/
	while(1)
	{
		int ret = pos_read_one_line(fpos_buffer,len,one_line_buffer,sizeof(one_line_buffer));
		/*------------------------------*/
		if( ret == 0 )
		{
			if( mode == 0 )
			{
				/* ok */
				if( sscanf(one_line_buffer,"%d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
					&fpos.index,&fpos.lon,&fpos.lat,&fpos.height,&fpos.CT1,&fpos.CT2,&fpos.CT3,&fpos.CT4,&fpos.CT5,&fpos.PITCH,&fpos.ROLL,&fpos.YAW) == 12 )
				{
					fwrite(&fpos,1,sizeof(fpos),wb);
				}else
				{
				   MessageBox(_T("FPOS文件错误"),_T("tips"),0);
				   return (-1);
				}
			}else if(mode == 1 )
			{
				//pos
				/* ok */
				if( sscanf(one_line_buffer,"%d %lf %lf %lf %lf %lf %lf",
					&pos.index,&pos.lon,&pos.lat,&pos.height,&pos.PITCH,&pos.ROLL,&pos.YAW) == 7 )
				{
					fwrite(&pos,1,sizeof(pos),wb);
				}else
				{
				   MessageBox(_T("POS文件错误"),_T("tips"),0);
				   return (-1);
				}
			}else
			{
				MessageBox(_T("未知的POS文件"),_T("tips"),0);
				return (-1);
			}
			/*---------------------------------------*/
		}else if( ret == 1 )
		{
			fclose(rb);
			fclose(wb);
			/* copy new path */
			memcpy( path_tmp , fpos_name , strlen(fpos_name));
			/*---------------*/
			return 0;
		}else if( ret == (-1) )
		{
			/* file error */
			return (-1);
		}
	}
	/*----------------------*/
}
/* read one line */
int CTeeChart5_testDlg::pos_read_one_line(char * buffer,unsigned int len,char * dst,unsigned int dst_len)
{
	unsigned int dst_cnt = 0;
	/*-----------------------------------*/
	memset(dst,0,dst_len);
	/*-----------------------------------*/
	while(1)
	{
	   if( buffer[position] == 0x0d && buffer[position+1] == 0x0A )
	   {
		   position += 2;
		   dst_cnt = 0;
		   return 0;
	   }else
	   {
		   if( dst_cnt >= dst_len )
		   {
			   MessageBox(_T("FPOS文件错误"),_T("tips"),0);
			   return (-1);
		   }
		   /*---------------------------*/
		   dst[dst_cnt] = buffer[position];
		   /*---------------------------*/
		   dst_cnt++ ;
	   }
	   /* incremeter */
	   position++;
	   /* end file ? */
	   if( position >= len )
	   {
		   /* file end */
		   return 1;
	   }
	}
}