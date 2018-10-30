
#ifndef __SERVER_MANAGE_H__
#define __SERVER_MANAGE_H__

typedef struct{
	char pc_name[64];
	char user_name[64];
	unsigned long long open_count;
	unsigned long long last_time_ms;
	char rev[112];
}socket_file_def;

int exmail_initial(void);

#endif