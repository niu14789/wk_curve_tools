

#ifndef __SMTP_H__
#define __SMTP_H__


typedef struct {
	char * host;
	char * account;
	char * password;
	char * from;
	char * to;
}email_config;

int send_email(email_config *config,char * s_str_name,char * subject,char * data_send);


#endif