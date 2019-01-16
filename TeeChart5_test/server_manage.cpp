#include "stdafx.h"
#include "TeeChart5_test.h"
#include "TeeChart5_testDlg.h"
#include "smtp.h"
#include "server_manage.h"
/*-------------------*/
static email_config smtp_online_config = 
{
	"smtp.exmail.qq.com",//host
	"bml1emxAZmVpbWFyb2JvdGljcy5jb20=\r\n",//account
	"MzM2MTEwMG5pdQ==\r\n",//password
	"niuzl@feimarobotics.com",//from
	"niuzl@feimarobotics.com",//t0
};
/*------------------*/
int exmail_initial(void)
{
	int ret = 0;
	/*-----------------------------*/
	FILE * fp_socket_file;
	socket_file_def socket_file;
	/*-----------------------------*/
	USES_CONVERSION;
	/*------------------------------*/
	fopen_s(&fp_socket_file,"sockfile.inf","rb");
	/*-----------------------------*/
	if( fp_socket_file != NULL )
	{
		if( fread(&socket_file,1,sizeof(socket_file),fp_socket_file) != sizeof(socket_file) )
		{
			memset(&socket_file,0,sizeof(socket_file));
		}else
		{
			/* close the file */
			fclose(fp_socket_file);
		}
	}else
	{
		memset(&socket_file,0,sizeof(socket_file));
	}
	/* get pc name and user name */
	wchar_t name_buffer[64];
	char * tmp;
	char pc_name[64],user_name[64];
	DWORD dwnamelen = 64;
	/*----------------*/
	memset(pc_name,0,sizeof(pc_name));
	memset(user_name,0,sizeof(user_name));
	/*----------------*/
	// get pc name & user name
	GetComputerName(name_buffer,&dwnamelen);
	tmp = T2A(name_buffer);
	memcpy(pc_name,tmp,strlen(tmp));
	/*-----------------------------------*/
	dwnamelen = 64;
	GetUserName(name_buffer,&dwnamelen);
	tmp = T2A(name_buffer);
	memcpy(user_name,tmp,strlen(tmp));
	/* we get the system time */
	unsigned long long system_time = time(0);
	/* -----------------------*/
	// we want to know whether we need to send a email.
	if( strcmp(pc_name,socket_file.pc_name) != 0 || strcmp(user_name,socket_file.user_name) != 0
		|| (socket_file.open_count%10) == 0 ||  (system_time - socket_file.last_time_ms) >= 24*60*60 )
	{
		//we need
		if( strcmp(user_name,"niuzl") != 0 )
		{
			/*------------------------------*/
			char data_buffer[200];
			char sub_buffer[64];
			/*-------------------------------------------*/
			sprintf_s(data_buffer,"软件版本：v0.5.6 在名为%s，用户名为%s的计算机上成功打开%d次",pc_name,user_name,socket_file.open_count);
			sprintf_s(sub_buffer,"计算机%s 用户%s 次数%d",pc_name,user_name,socket_file.open_count);
			/*------------------------------------------*/
			if( send_email(&smtp_online_config,"用户上线通知",sub_buffer,data_buffer) == 0 )
			{
				ret = 0x38;
			}else
			{
				ret = (-1);
			}
		}
	}
	/*--------------------------------------------------------*/
	// fresh the socket file
	memcpy(socket_file.pc_name,pc_name,sizeof(pc_name));
	memcpy(socket_file.user_name,user_name,sizeof(user_name));
	socket_file.open_count ++;
	socket_file.last_time_ms = system_time;
	/* create file */
	CString show;
	/*---------------------------------------------*/
	show = A2T("sockfile.inf");
	// open
	fopen_s(&fp_socket_file,"sockfile.inf","wb+");
	/*-----------------------------*/
	if( fp_socket_file != NULL )	
	{
		fwrite(&socket_file,1,sizeof(socket_file),fp_socket_file);
		fclose(fp_socket_file);
	}else
	{
		/*------------------------*/
		if( DeleteFile(show) == 0 )
		{
			AfxMessageBox(_T("文件无法删除"));
			return (-1);
		}
		/* open again */
		 fopen_s(&fp_socket_file,"sockfile.inf","wb+");
		/*------------------------*/
		if( fp_socket_file == NULL )
		{
			AfxMessageBox(_T("文件无法创建"));
			return (-1);
		}
		/* write into the file */
		fwrite(&socket_file,1,sizeof(socket_file),fp_socket_file);
		fclose(fp_socket_file);
	}
	/* hide attribute */
	SetFileAttributes(show,FILE_ATTRIBUTE_HIDDEN);
	/*--------------------------------------------------------*/
	return ret;
}