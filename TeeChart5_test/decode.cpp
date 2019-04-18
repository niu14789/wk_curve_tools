
#include "stdafx.h"
#include "decode.h"
#include "stdio.h"
#include "718d_crc32.h"
#include "Algo.h"
#include <string.h>
#include "config.h"
#include <math.h>

/* define module debug entry */
#ifdef __DEBUG_GLOBAL_EN__
	#define DEBUG_100HZ(...)	printf("Debug 100HZ Info:"__VA_ARGS__)
#else
	#define DEBUG_100HZ(...)
#endif
/* 100hz process enable */
extern unsigned char Output_100HZ_en;

struct mark2timr_info
{
	unsigned short seq;
	unsigned short week;
	unsigned int ms;
	unsigned int unkown;
};

typedef enum OEM718D_DATA_PRASER_STATE
{
	DATA_PRASER_STATE_SYNC1,	//0-OEM718D_LOG_DATA_HDR_SYNC1
	DATA_PRASER_STATE_SYNC2,	//1-OEM718D_LOG_DATA_HDR_SYNC2
	DATA_PRASER_STATE_SYNC3,	//2-OEM718D_LOG_DATA_HDR_SYNC3
	DATA_PRASER_STATE_HDRLEN,	//3-PRASER LOG DATA HEAD LENGTH
	DATA_PRASER_STATE_HDRDATA,	//4-PRASER LOG DATA HEAD DATA
	DATA_PRASER_STATE_BODYDATA	//5-PRASER LOG DATA BODY DATA
}oem718d_prase_state;

/* Log frame head define */
#define OEM718D_LOG_DATA_HDR_SYNC1		0xAA
#define OEM718D_LOG_DATA_HDR_SYNC2 		0x44
#define OEM718D_LOG_DATA_HDR_SYNC3		0x12

/* log error val define */
unsigned int decode_head_error_cnt = 0u;
unsigned int decode_crc32_error_cnt = 0u;
/* range data save struct */
static RangeCmp_Log_st RangeCmp_Log[6];
/* Gps nav define  */
gps_nav_def gps_nav;
/* log mark2timr count */
static unsigned int log_mark2timr_cnt = 0u;
/* log umkown count */
static unsigned int log_unkown_cnt = 0;
/* init decode state */
static oem718d_prase_state decode_step = DATA_PRASER_STATE_SYNC1;
/* GPS time */
static unsigned short GPS_week;
static unsigned int GPS_ms;
/* */
static unsigned char decode_tmp_buffer[4096];
/*write file ops */
extern int raw_100HZ_write_file(unsigned char *data, unsigned int len);
extern int markinfo_write_file(unsigned char *data, unsigned int len);

void reset_log_parse(void)
{
	/* restart mark2timr count */
	log_mark2timr_cnt = 0;
	/* restart unkown bytes count */
	log_unkown_cnt = 0;
	/* restart parse status */
	decode_step = DATA_PRASER_STATE_SYNC1;
	/* restart decode head error bytes count */
	decode_head_error_cnt = 0;
	/* restart decode crc32 error bytes count */
	decode_crc32_error_cnt = 0;
}
//@ get_log_count
//@ Parm:type:
//@ 
//@
int get_log_count(unsigned int type)
{
	if(type == MSG_ID_LOG_MARK2TIMR)
	{
#if APP_MARK2TIMR_COUNT_EN
		return log_mark2timr_cnt;
#else
		return 0;
#endif/**/
	}

	return -1;
}

void print_cnt_info(void)
{
	DEBUG_100HZ("%d, %d\n",decode_head_error_cnt,decode_crc32_error_cnt);
}
//@ Decode_Raw_Data
//@
//@
unsigned int Decode_Raw_Data(unsigned char *bf, unsigned int len)
{
	/* tmp data for decode */
	unsigned char tmp_data;
	/* store decode index */
	static unsigned int buffer_decode_index = 0u;
	/* one frame length */
	static unsigned int frame_length = 0u;

	/* process data */
	for( unsigned int bindex = 0; bindex < len; bindex++ )
	{
		/* get decode data byte */
		tmp_data = bf[bindex];
		
		/* decode data */
		switch(decode_step)
		{
			case DATA_PRASER_STATE_SYNC1:
				/* RECVICE HEAD1 */
				if ( tmp_data == OEM718D_LOG_DATA_HDR_SYNC1 )
				{
					decode_tmp_buffer[0] = tmp_data;
					decode_step = DATA_PRASER_STATE_SYNC2;
				}
				else
				{
					/* log head error cnt */
					decode_head_error_cnt ++;
					/* restart decode */
					decode_step = DATA_PRASER_STATE_SYNC1;
				}
				break;
			case DATA_PRASER_STATE_SYNC2:
				/* RECVICE HEAD2 */
				if( tmp_data == OEM718D_LOG_DATA_HDR_SYNC2 )
				{				
					decode_tmp_buffer[1] = tmp_data;
					decode_step = DATA_PRASER_STATE_SYNC3;
				}
				else
				{
					/* log head error cnt */
					decode_head_error_cnt ++;
					/* restart decode */
					if ( tmp_data == DATA_PRASER_STATE_SYNC1 )
					{
						decode_tmp_buffer[0] = tmp_data;
						decode_step = DATA_PRASER_STATE_SYNC2;
					}
					else
					{
						decode_step = DATA_PRASER_STATE_SYNC1;
					}
				}
			break;
		case DATA_PRASER_STATE_SYNC3:
			/* RECVICE HEAD3 */
			if ( tmp_data == OEM718D_LOG_DATA_HDR_SYNC3)
			{
				decode_tmp_buffer[2] = tmp_data;
				decode_step = DATA_PRASER_STATE_HDRLEN;
			}
			else
			{
				decode_head_error_cnt ++;
				if ( tmp_data == DATA_PRASER_STATE_SYNC1 )
				{
					decode_tmp_buffer[0] = tmp_data;
					decode_step = DATA_PRASER_STATE_SYNC2;
				}
				else
				{
					decode_step = DATA_PRASER_STATE_SYNC1;
				}
			}
			break;
		case DATA_PRASER_STATE_HDRLEN:
			/* RECVICE HEAD LENGTH */
			decode_tmp_buffer[3] = tmp_data;
			/* decode index count */
			buffer_decode_index = 4;
			/* next decode step */
			decode_step = DATA_PRASER_STATE_HDRDATA;
			break;
		case DATA_PRASER_STATE_HDRDATA:
			/* RECVICE HEAD DATA */
			decode_tmp_buffer[buffer_decode_index++] = tmp_data;
			/*hdr has recv ok*/
			if( buffer_decode_index == decode_tmp_buffer[3] )
			{
				/* one frame length */
				frame_length = *(unsigned short *)&decode_tmp_buffer[8] + decode_tmp_buffer[3] + 4;
				/* next decode step */
				decode_step = DATA_PRASER_STATE_BODYDATA;
				
				if( frame_length >= 4096 )
				{
					/* restart decode */
					buffer_decode_index = 0;
					/* retart decode state */
					decode_step = DATA_PRASER_STATE_SYNC1;
					break;
				}
			}				
			break;
		case DATA_PRASER_STATE_BODYDATA:
			/* RECVICE BODY DATA */
			decode_tmp_buffer[buffer_decode_index++] = tmp_data;
			/*message body has recv ok*/
			if( buffer_decode_index ==  frame_length)
			{
				if( log_message_check(decode_tmp_buffer, buffer_decode_index) )
				{
					log_switch_process(decode_tmp_buffer, buffer_decode_index);
				}
				else
				{
					/* check error and restart */
					decode_crc32_error_cnt ++;
				}
				
				/* restart decode */
				buffer_decode_index = 0;
				/* retart decode state */
				decode_step = DATA_PRASER_STATE_SYNC1;
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

unsigned char getRtkStatus(unsigned char pos_type)
{
	unsigned char status = 0u;
	
	if( pos_type == POS_SOLUTION_NONE )
	{
		//not solution
		status = POS_TYPE_NONE;
	}
	else if( pos_type == POS_SOLUTION_SINGLE )
	{
		// single point position
		status = POS_TYPE_SINGLE;
	}
	else if( pos_type == POS_SOLUTION_PSRDIFF )
	{
		//psr diff status
		status = POS_TYPE_PSRDIFF;
	}
	else if(pos_type == POS_SOLUTION_NARROW_INT)
	{
		// rtk fixed status
		status = POS_TYPE_FIXED;
	}
	else if(pos_type == POS_SOLUTION_NARROW_FLOATE)
	{
		//rtk floate status
		status = POS_TYPE_FLOATE;
	}
	else//other status 
	{
		status = POS_TYPE_NONE;
	}
	
	return status;
}
static void log_switch_process(unsigned char *data, unsigned int len)
{
	log_bestpos_t *bestpos;
	log_bestvel_t *bestvel;
	log_psrdop_t *psrdop;
	/* get msg id and satellite number */
	unsigned short msg_id = *(unsigned short *)&data[4];
	unsigned int svn =  *(unsigned int *)&data[28];
	/* Get  Week Time */
	GPS_week = *(unsigned short *)&data[14];
	GPS_ms = *(unsigned int *)&data[16];
	/* GPS Week Time */
	gps_nav.GPS_week_num = GPS_week;
	gps_nav.GPS_time_ms = GPS_ms;
	gps_nav.unkown = decode_head_error_cnt;
	/* process msg id */
	switch(msg_id)
	{
	case MSG_ID_LOG_RANGECMP:
#if APP_MAKE_100HZ_EN
		/* enable 100hz process */
		if(Output_100HZ_en)
		{
			/* filter error data */
			if( (len - 36) == svn * 24 )
			{
				/* crc check ok frame and then process them */
				Make_100HZ_Process(data, len);
			}
		}
		/* do something */
#endif /* APP_MAKE_100HZ_EN */
		break;
	case MSG_ID_LOG_BESTPOS:
		bestpos = (log_bestpos_t*)&data[28];
		gps_nav.lon = bestpos->lon / 57.2957795;
		gps_nav.lat = bestpos->lat / 57.2957795;
		gps_nav.height = bestpos->hgt + bestpos->undulation;
		gps_nav.svn = bestpos->SVs;
		gps_nav.RTK_flags =  getRtkStatus(bestpos->pos_type);
		markinfo_write_file((unsigned char *)&gps_nav,sizeof(gps_nav));
		break;
	case MSG_ID_LOG_BESTVEL:
#define PI 3.14159265358979323846f
		bestvel = (log_bestvel_t*)&data[28];
		gps_nav.Speed = bestvel->horizonSpeed;
		gps_nav.Vertical_velocity = bestvel->verticalSpeed;	
		gps_nav.Heading = (bestvel->movingDirection * PI)/180;
		markinfo_write_file((unsigned char *)&gps_nav,sizeof(gps_nav));
		break;
	case MSG_ID_LOG_PSRDOP:
		psrdop = (log_psrdop_t *)&data[28];
		gps_nav.PDOP = psrdop->PDOP;
		markinfo_write_file((unsigned char *)&gps_nav,sizeof(gps_nav));
		break;
	case MSG_ID_LOG_HEADING2:
		break;
	case MSG_ID_LOG_MARK2TIMR:
#if APP_MARK2TIMR_COUNT_EN
		struct mark2timr_info mark_temp;
		/* count log mark2timr */
		log_mark2timr_cnt ++;
		mark_temp.seq = log_mark2timr_cnt;
		//mark_temp.week = *(unsigned int *)&data[28];
		//mark_temp.ms = *(unsigned int *)&data[32];
		mark_temp.week = GPS_week;
		mark_temp.ms = GPS_ms;
		mark_temp.unkown = decode_head_error_cnt;
		/*write mark info file */
		markinfo_write_file((unsigned char *)&mark_temp,sizeof(mark_temp));
#endif/* APP_MARK2TIMR_COUNT_EN */
	default:
#if APP_MAKE_100HZ_EN
		/* write data to file */
		raw_100HZ_write_file(data, len);
#endif /* APP_MAKE_100HZ_EN */
		break;
	}
}
/* satellite stystem and wave length table */
unsigned char STATELLITE_SYSTEM[] = {'G','R','S','E','C','J','M','O'};
const static double WAVE_LENGTH[8][2] = {	
							{0.1902936727984,0.2442102134246},/* GPS */
							{0.1873995450970,0.2407704654896},/* GLONASS */
							{0.00,0.00},
							{0.00,0.00},
							{0.1921724324802,0.2485212982753},/* BDS */
							{0.00,0.00},
							{0.00,0.00},
							{0.00,0.00}
							};
//@ decode_satellite_head_info
//@
//@
static void decode_satellite_head_info(unsigned char *data, GPS_head_Info_st  *info)
{
	unsigned int sallite_status = *(unsigned int *)data;
	unsigned char signal_type = (sallite_status & SIGNAL_TYPE_MASK) >> 21;
	/* process satellite system */
	info -> satellite_sys = (sallite_status & SATELLITE_SYSTEM_MASK) >> 16;
	/* process channal */
	switch(info -> satellite_sys)
	{
		case SATELLITE_SYSTEM_GPS:
			switch(signal_type)
			{
				case 0:
					info -> channel = SATELLITE_CHANNAL_L1;
					break;
				case 5:
				case 9:
					info -> channel = SATELLITE_CHANNAL_L2;
					break;
			}
			break;
		case SATELLITE_SYSTEM_GLONASS:
			switch(signal_type)
			{
				case 0:
					info -> channel = SATELLITE_CHANNAL_L1;
					break;
				case 1:
				case 5:
					info -> channel = SATELLITE_CHANNAL_L2;
					break;
			}
			break;
		case SATELLITE_SYSTEM_BDS:
			switch(signal_type)
			{
				case 0:
				case 4:
				case 2:
					info -> channel = SATELLITE_CHANNAL_L1;
					break;
				case 1:
				case 5:
				case 6:
					info -> channel = SATELLITE_CHANNAL_L2;
					break;
			}
			break;
		default:
			break;
	}

	info -> sv_ch_num = (sallite_status & SV_CHANNEL_NUMBER_MASK) >> 5;
}
//@ decode_satellite_item_info
//@
//@
static void decode_satellite_item_info(unsigned char *data, satellite_info_st *item)
{
	unsigned long long psr = 0;
	unsigned int adr = 0;
	int dopper;
	double tmp_adr,wave_lengt,adr_rolls,wave_offset = 0.0,cabil_vaule = 0.0;
	/* process psr */
	psr = (data[3]>>4)|(data[4]<<4)|(data[5]<<12)|(data[6]<<20)|((unsigned long long )data[7]<<28);
	item->item.raw_psr = psr;
	item ->item.psr = psr * 1.0f / 128;
	/* process adr */
	adr = (data[8])|(data[9] << 8)|(data[10] << 16)|(data[11]<< 24);
	item->item.raw_adr = adr;
	tmp_adr = adr *1.0f/ 256;

	wave_lengt = WAVE_LENGTH[item->head.satellite_sys][item->head.channel];

	if( item->head.satellite_sys == SATELLITE_SYSTEM_GLONASS)
	{
		cabil_vaule = -71.95/wave_lengt;
		if(item->head.channel == SATELLITE_CHANNAL_L2)
		{
			wave_lengt += 0.4375 * pow(10.0,(data[17]>>2) * (-1));
			wave_offset = -0.25;
		}
		else
		{
			wave_lengt += 0.5625 * pow(10.0,(data[17]>>2) * (-1));
		}
	}
	/**/
	adr_rolls = (item ->item.psr/wave_lengt + tmp_adr)/MAX_VALUE  ;
	/* correct adr rolls */
	adr_rolls += adr_rolls <=0 ?(-0.5):(0.5);

	item->item.adr = tmp_adr - MAX_VALUE * int(adr_rolls) + cabil_vaule + wave_offset;
	/* process dopper */
	unsigned char tmp_data = data[3] & (1 << 3)? (data[3]|0xf0):(data[3]&0xf);
	/*----*/
    dopper = data[0]|(data[1]<<8)|(data[2]<<16)|((tmp_data)<<24);
	item -> item.raw_dopper = dopper;
	/*---*/
	item->item.dopper = dopper*1.0f/256;
	/* process c/no*/
	item->item.cn = ((data[16]>>5)| ((data[17]&0x3)<<3)) + 20;

	if(item->item.cn < 6)
	{
		item->item.cn_int = 1;
	}
	else
	{
		item->item.cn_int = (item->item.cn-6)/6 + 1;
		if(item->item.cn_int > 9)
		{
			/*max is 9*/
			item->item.cn_int  = 9;
		}
	}
}
//@ fill_raw_data
//@
//@
static void fill_raw_data(unsigned char *start, satellite_info_st *satellite,unsigned char fill_mode)
{
		unsigned long long psr = 0;
		unsigned int dopper = 0,adr = 0;
		
		switch(fill_mode)
		{
			case 1:
				/* process item data */
				psr = (unsigned long long )(satellite->item.psr * 128);
				adr = (unsigned int )(satellite->item.adr);
				dopper = (unsigned int )(satellite->item.dopper * 256);
				break;
			case 2:
				/* process item data */
				psr = satellite->item.raw_psr;
				adr = satellite->item.raw_adr;
				dopper = satellite->item.raw_dopper;
				break;
			default:
				break;
		}
		if( fill_mode == 1 )
		{

		}
		/* fill dopper */
		memcpy(start + 4,(unsigned char *)&dopper, 4);
		/* fill psr */
		start[7] = (start[7] & 0x0f) |((psr & 0xf) << 4);
		psr >>= 4;
		memcpy(start + 8,(unsigned char *)&psr,4);
		/* fill adr */
		memcpy(start + 12,(unsigned char *)&adr,4);
}
//@ make_item_to_raw_data
//@
//@
static int make_item_to_raw_data(unsigned int index, satellite_info_st *satellite)
{
	unsigned char *ptr = NULL;
	unsigned char *ptr_copy = NULL;
	/* item to raw data */
	for(unsigned char pack_index = 1; pack_index < 5;pack_index ++)
	{
		ptr =  RangeCmp_Log[pack_index].data + 32 + RangeCmp_Log[pack_index].index *24;
		/* not same location process */
		if(index != RangeCmp_Log[pack_index].index)
		{
			ptr_copy = RangeCmp_Log[pack_index].data + 32 + index *24;
			
			memcpy(ptr,ptr_copy,24);
		}

		
		RangeCmp_Log[pack_index].index ++;
		/* fill dopper */
		fill_raw_data(ptr, satellite+pack_index, 2);
	}

	return 0;
}
//@ raw_data_save_file
//@
//@
static int raw_data_save_file(unsigned char save_seq)
{
	switch( save_seq )
	{
		case 0:
			for(unsigned char index = 1; index < 6; index++)
			{
				raw_100HZ_write_file(RangeCmp_Log[index].data,RangeCmp_Log[index].len);
			}
			break;
		case 1:
			for(unsigned char index = 0; index < 5; index++)
			{
				raw_100HZ_write_file(RangeCmp_Log[4-index].data,RangeCmp_Log[4-index].len);
			}
			break;
		case 2:
			raw_100HZ_write_file(RangeCmp_Log[0].data,RangeCmp_Log[0].len);
			break;
		case 3:
			raw_100HZ_write_file(RangeCmp_Log[5].data,RangeCmp_Log[5].len);
			break;
	}
	return 0;
}



//@ find_same_satellite
//@
//@
static int find_same_satellite(unsigned int src_index, unsigned int status, unsigned char *find_pack)
{
	unsigned int svn2 = *(unsigned int *)&find_pack[28];
	unsigned int status2 = 0;
	int same_index = -1;

	for(unsigned int index = 0;index < svn2; index++ )
	{	
		status2 = *(unsigned int *)&find_pack[32+index*24];
		if ((( status ^ status2 ) & 0x03E703E0) == 0)
		{
			same_index = index;
			if (index == src_index)
			{
				break;
			}
		}
	}

	return same_index;
}
//@ check_has_same_satellite
//@ 
//@
static int check_has_same_satellite(void)
{
	unsigned char *ptr = RangeCmp_Log[0].data + 32;
	unsigned char *ptr1 = RangeCmp_Log[5].data;
	unsigned int svn = *(unsigned int *)(ptr -4);
	unsigned int svn1 = *(unsigned int *)(ptr1 +28);
	unsigned int status = 0u;

	if( svn != 0 && svn1 != 0)
	{
		/* */
		for(unsigned int svn_index = 0;svn_index < svn; svn_index++)
		{
			status = *(unsigned int *)ptr;

			if( find_same_satellite(svn_index, status, ptr1) >= 0)
			{
				return 0;
			}

			/* update ptr and ptr1 */
			ptr += 24;
		}

		return -1;
	}

	return -1;

}
//@ Make_100HZ_Process
//@ 
//@
void Make_100HZ_Process(unsigned char *pack, unsigned int len)
{
	static unsigned char index = 0, make_100_flag = 0,save_seq = 2,tmp_save_seq = 0;
	static satellite_info_st GPS_Satellite[6];
	unsigned int svn = 0, svn1 = 0, cur_ms = 0;
	int cali_ms;
	unsigned char *ptr = NULL, *ptr1 = NULL;
	/* copy data */
	memcpy(RangeCmp_Log[index].data, pack, len);
	RangeCmp_Log[index].len = len;
	/* update index */
	index =( index + 5 ) % 10;
	/* process */
	if( make_100_flag )
	{
		/*------*/
		svn = *(unsigned int *)&RangeCmp_Log[index].data[28];
		svn1 = *(unsigned int *)&RangeCmp_Log[(index+5)%10].data[28];
		ptr = &RangeCmp_Log[index].data[32];
		/*------*/
		cali_ms = *(unsigned int *)&RangeCmp_Log[5].data[16] - *(unsigned int *)&RangeCmp_Log[0].data[16];
		cali_ms /= 5;

		if( check_has_same_satellite() == 0 )
		{
			/* copy data */
			for(unsigned char pack_index = 1; pack_index < 5; pack_index++)
			{
				cur_ms = *(unsigned int *)&RangeCmp_Log[0].data[16] + cali_ms * pack_index;
				/* copy data */
				memcpy(RangeCmp_Log[pack_index].data, RangeCmp_Log[index].data,RangeCmp_Log[index].len);
				//RangeCmp_Log[pack_index].len = RangeCmp_Log[index].len;
				/**/
				RangeCmp_Log[pack_index].index = 0u;
				/* 100hz time ms */
				memcpy(RangeCmp_Log[pack_index].data+16,(unsigned char *)&cur_ms, 4);
			}
		
			/* 100hz process */
			for( unsigned int svn_index = 0; svn_index < svn; svn_index ++)
			{
				unsigned int status = *(unsigned int *)ptr;
				int find_index;
				/* find same satellite */
				find_index = find_same_satellite(svn_index, status, RangeCmp_Log[(index+5)%10].data);
				/* */
				if( find_index >= 0 )
				{
					ptr1 = RangeCmp_Log[(index+5)%10].data+ 32 + find_index*24;
					/* Decode Satellite Head Info */
					decode_satellite_head_info(ptr, &GPS_Satellite[index].head);
					decode_satellite_head_info(ptr1, &GPS_Satellite[(index+5)%10].head);
					/* Decode Satellite Item Info */
					decode_satellite_item_info(ptr+4, &GPS_Satellite[index]);
					decode_satellite_item_info(ptr1+4,&GPS_Satellite[(index+5)%10]);
					/* 100HZ Process */
					Algo_100HZ_Process( GPS_Satellite, 5 );
					/* fill to  raw data */
					make_item_to_raw_data(svn_index, GPS_Satellite);
				}

				ptr += 24;
			}

			/* fill crc32 */
			for(unsigned char pack_index =1;pack_index < 5;pack_index++)
			{
				unsigned short offset = RangeCmp_Log[pack_index].index *24 + 4;
				/* fill svn number */
				memcpy(RangeCmp_Log[pack_index].data+28,(unsigned char *)&RangeCmp_Log[pack_index].index,4);
				/* fill body length */
				memcpy(RangeCmp_Log[pack_index].data+8,(unsigned char *)&offset,2);
				/* */
				offset += 28;
				/* copy data */
				unsigned int crc32  = CalculateBufferCRC32(offset, RangeCmp_Log[pack_index].data);
				/* 100hz time ms */
				memcpy(RangeCmp_Log[pack_index].data+offset,(unsigned char *)&crc32,4);

				RangeCmp_Log[pack_index].len = offset + 4;
			}
			
			save_seq = tmp_save_seq;
		}
		else
		{
			save_seq = index? 2: 3;
		}
		/* Write file turn round */
		tmp_save_seq ^= 0x01;
	}
	/* Write file */
	raw_data_save_file(save_seq);
	/* to make 100HZ */
	make_100_flag = 1;
}


