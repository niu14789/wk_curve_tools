#include "stdafx.h"
#include <stdio.h>
#include "base64.h"
#include <winsock.h>
#include "smtp.h"
//#pragma  comment(  linker,  "/subsystem:\"windows\"  /entry:\"mainCRTStartup\""  )

#pragma comment(lib, "Ws2_32.lib")

#define ENABLE_FILE  0

/*--------------------------------------*/
#if ENABLE_FILE
static FILE * socket_file;
#endif
/*--------------------------------------*/
int init_socket();
void socket_close();
SOCKET socket_connect(const char * hostname, int port);
int get_sockbuf();
int send_socket(const char *data);
 
SOCKET sockfd = 0;

//static unsigned int test_start = 0;
static char sub_buffer[20*1024*2024];

int send_socket(const char *data)
{
	if (send(sockfd, data, strlen(data), 0) < 0)
	{
#if ENABLE_FILE
		fwrite("发送失败!\r\n",1,strlen("发送失败!\r\n"),socket_file);
#endif
		return (-1);
	}
#if ENABLE_FILE
	fwrite(data,1,strlen(data),socket_file);
#endif
	return 0;
}
int get_sockbuf()
{
	static char buf[1024] = { 0 };
	memset(buf, 0, sizeof(buf));
	if (recv(sockfd, buf, sizeof(buf), 0) < 0)
	{
		return (-1);//error
	}
	/*-------------*/
#if ENABLE_FILE
	fwrite(buf,1,strlen(buf),socket_file);
#endif
	/*------ok------*/
	return 0;
}
 
int init_socket()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return -1;
	}
 
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}
	return 0;
}
 
SOCKET socket_connect(const char * hostname, int port)//连接套接字
{
	struct hostent*host;
	unsigned long ip = 0;
 
	if (init_socket() == -1)
		return 0;
	SOCKET st = socket(AF_INET, SOCK_STREAM, 0);
	if (st == 0)
		return 0;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
 
	
	ip = inet_addr(hostname);
	if (ip == INADDR_NONE)
	{
		host = gethostbyname(hostname);
		/*---------------------------*/
		if( host == NULL )
		{
			return 0;//fail 
		}
		/*---------------------------*/
		addr.sin_addr = *((struct in_addr*)(host->h_addr_list[0]));
	}
	else
	{
		addr.sin_addr.s_addr = inet_addr(hostname);
	}
 
	if (connect(st, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
#if ENABLE_FILE
		fwrite("连接失败!\r\n",1,strlen("连接失败!\r\n"),socket_file);
#endif
		return 0;
	}
	else
	{
		return st;
	}
}
 
void socket_close()
{
	closesocket(sockfd);
	WSACleanup();
}

/* send a message */
int send_email(email_config *config,char * s_str_name,char * subject,char * data_send)
{
	char data[1024] = { 0 };
	char base64_data[2048] = { 0 };
	int i = 0;
#if ENABLE_FILE
	socket_file = fopen("socket.txt","wb++");
#endif
	init_socket();
	/*-------------------------------------*/
	sockfd = socket_connect(config->host, 25);
    // connect ok?
	if( sockfd == 0 )
	{
		return (-1);
	}
	//接收服务器信息
	if( get_sockbuf() == (-1) )
	{
		return (-1);
	}
    /*----------------*/
	memset(sub_buffer,0,sizeof(sub_buffer));
	/*----------------*/
	sprintf_s(sub_buffer,"helo %s\r\n",config->host);
	//发送helo信息
	if( send_socket(sub_buffer) == (-1) )
	{
		return (-1);
	}
	//接收服务器应答
	if( get_sockbuf() == (-1) )
	{
		return (-1);
	}
 
	//发送登录请求
	if( send_socket("auth login\r\n") == (-1) )
	{
		return (-1);
	}
	//接收服务器应答
	if( get_sockbuf() == (-1) )
	{
		return (-1);
	}
 
	//发送编码后的邮箱账号
	//memset(data, 0, sizeof(data));
	//strcpy(data, config->account);
	//base64_encode(data, strlen(data), base64_data);
	//i = strlen(base64_data);
	//base64_data[i] = '\r';
	//base64_data[i+1] = '\n';
	if( send_socket(config->account) == (-1) )
	{
		return (-1);
	}
	//接收服务器应答
	if( get_sockbuf() == (-1) )
	{
		return (-1);
	}
 
	//发送编码后的邮箱密码
	//memset(data, 0, sizeof(data));
	//memset(base64_data, 0, sizeof(base64_data));
	//strcpy(data, config->password);
	//base64_encode(data, strlen(data), base64_data);
	//i = strlen(base64_data);
	//base64_data[i] = '\r';
	//base64_data[i + 1] = '\n';
    /* send */
	if( send_socket(config->password) == (-1) )
	{
		return (-1);
	}
	//接收服务器应答
	if( get_sockbuf() == (-1) )
	{
		return (-1);
	}
    /*----------------*/
	memset(sub_buffer,0,sizeof(sub_buffer));
	/*----------------*/
	sprintf_s(sub_buffer,"MAIL FROM:<%s>\r\n",config->from);
	//mail from
	if( send_socket(sub_buffer) == (-1) )
	{
		return (-1);
	}
	//接收服务器应答
	if( get_sockbuf() == (-1) )
	{
		return (-1);
	}
    /*----------------*/
	memset(sub_buffer,0,sizeof(sub_buffer));
	/*----------------*/
	sprintf_s(sub_buffer,"rcpt to:<%s>\r\n",config->to); 
	//rcpt to
	if( send_socket(sub_buffer) == (-1) )
	{
		return (-1);
	}
	//接收服务器应答
	if( get_sockbuf() == (-1) )
	{
		return (-1);
	}
 
	//正文
	if( send_socket("data\r\n") == (-1) )
	{
		return (-1);
	}
	/* pc sender name */
	memset(sub_buffer,0,sizeof(sub_buffer));
	/*----------------*/
	sprintf_s(sub_buffer,"From:\"%s\"<%s>\r\n",s_str_name,config->from);
	/*------------------------------------------*/
	//mail from
	if( send_socket(sub_buffer) == (-1) )
	{
		return (-1);
	}
	//接收服务器应答
	if( get_sockbuf() == (-1) )
	{
		return (-1);
	}
	/*--------------------------------*/
	memset(sub_buffer,0,sizeof(sub_buffer));
	/*--------------------------------*/
	sprintf_s(sub_buffer,"subject:%s\r\n",subject);
	/*--------------------------------*/
	if( send_socket(sub_buffer) == (-1) )
	{
		return (-1);
	}
    /*----------------*/
	memset(sub_buffer,0,sizeof(sub_buffer));
	/*----------------*/
	sprintf_s(sub_buffer,"\r\n%s\r\n.\r\n",data_send);
	/* send */
	if( send_socket(sub_buffer) == (-1) )
	{
		return (-1);
	}
	//接收服务器应答
	if( get_sockbuf() == (-1) )
	{
		return (-1);
	}
	//退出
	if( send_socket("quit\r\n") == (-1) )
	{
		return (-1);
	}
	//接收服务器应答
	if( get_sockbuf() == (-1) )
	{
		return (-1);
	}
	/* close */
	socket_close();
#if ENABLE_FILE
	/*--------*/
	fclose(socket_file);
#endif
	/* return */
	return 0;
}