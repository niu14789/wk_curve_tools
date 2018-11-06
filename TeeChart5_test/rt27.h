
#pragma once

/*-------------------------------*/
int rt27_decode(char *path_rt,char * tmp_file);
int file_licks(char * path,unsigned int len,char *lack,unsigned int len2);
void raw_gps_parse(unsigned char *buffer,unsigned int len);
void raw_gps_decode(unsigned char d);
void big2little(unsigned char * src,char * dst,unsigned char len);
void gps_raw_detail( unsigned char type,unsigned char *data,unsigned char len );
void real_time_gnss_survey_data(unsigned char * data/*beggin from the length */,unsigned int len);
/* gps new version */
typedef struct gps_psdu_doppler_s
{
    unsigned long long system_timestamps;//size 8//ϵͳͬ��ʱ���?
	unsigned int system_pps_num;//size 4//��Ӧpps��
	/* from ID 0x01 */
	unsigned int GPS_receive_ms;//size 4//gps time
    /*  */
	double PSEUDORANGE;//8α��
    float doppler;//4������
	unsigned char num;//1
	unsigned char id;//1
	unsigned short week_number;//2
	double phase;//8
	double SNR;//8
}gps_psdu_doppler_def;

static gps_psdu_doppler_def gps_psdu_doppler;
/*---------------------*/
typedef struct
{
	double event_time_s;
	unsigned int index;
	unsigned int rev;
}Event_persional_info_def;
typedef struct 
{
	unsigned int num;
	Event_persional_info_def event_per[5000];
}RT27_event_def;

extern RT27_event_def rt27_event;