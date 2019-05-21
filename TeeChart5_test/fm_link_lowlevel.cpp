#include "stdafx.h"
#include "TeeChart5_test.h"
#include "TeeChart5_testDlg.h"
#include "afxdialogex.h"
#include "CSeries.h"
#include "tchart1.h"
#include "C_open.h"
#include "CLegend0.h"
#include "math.h"
#include "C_PROCOTOL_CREATE.h"
#include <Dbt.h>
#include "m_com_num.h"
/*------------------*/
extern COM_MANAGE_DEF COM;
extern SYS_LIST_CHESE_DEF chese_list;
extern unsigned int combox_clear_flags;
extern unsigned char show_taps_flags;
LINE_MULIT_DEF line_mulit[128];
/*------------------*/
COMBOX_PARAM_RT_DEF combox_rt_line;
LINE_CONFIG_DEF line_cmd;
/*------------------*/
unsigned char buffer[4096];
unsigned char step = 0;
unsigned char receive_buf[270];
unsigned char lvs[10];
/* int */
int line_num_last = 0xffff;
int line_num = 0;
int i_last = 0xffff;
int j_last = 0xffff;
int mulit_cnt = 0;
/*-------------*/
extern unsigned char motor_check_flags;
/*-------------*/
const char * string_chese_table[23][32] = 
{
	{"imu1","imu1","ax:","ay:","az:","gx:","gy:","gz:","tmp:"},
	{"imu2","imu2","ax:","ay:","az:","gx:","gy:","gz:","tmp:"},
	{"imu3","imu3","ax:","ay:","az:","gx:","gy:","gz:","tmp:"},
	{"g_mrs","转速电流","转速1:","电流1:","转速2:","电流2","转速3:","电流3","转速4:","电流4:"},
	{"_baro_data","气压计","气压值:"},
	{"sonar_m","超声波","高度(m):"},
	{"ublox_msg","ublox_GPS","星数：","PDOP:"},
	{"bd930_msg","930/718D","星数：","PDOP:"},
	{"_mag_data","磁力计3","x：","y:","z:"},
	{"_mag_ems1","磁力计1","x：","y:","z:"},
	{"_mag_board","磁力计0","x：","y:","z:"},
    {"version_unit","D系列版本号","GPS板：","底板:","磁力计1:","磁力计2:","超声波:","单云台:","五云台:","电调1：","电调2：","电调3：","电调4：",
	 "飞控:","视觉：","CPLD:","FPGA:","电池版本:"},
	{"battery_msg","智能电池","1#电压：","1#剩余:","2#电压：","2#剩余:","3#电压：","3#剩余:"},
	{"led_status","LED状态","1#:","2#:","3#:","4#:"},
	{"cali_status","标定状态","磁力计:","水平标定：","电机标定："},
	{"gimbal_msg","拍照计数","热靴计数：","执行计数:"},
	{"_airspeed","空速计"," "},
	{"version_v100","V系列版本号","GPS板","载荷","空速计","电调1","电调2","电调3","底板","倾转1","倾转2","倾转3","磁力计1","磁力计2","舵面控制板1"
	,"舵面控制板2","舵面控制板3","405","rev2","rev3","rev4","rev5","rev6","radio","rev8","飞控"},
	{"version_vion","V100视觉版本","AP","CPLD","FPGA","电池","rev1","rev2","rev3","rev4","rev5","rev6","rev7","rev8"},
	{"airspeed_temperature","空速计温度","(度):"},
	{"diff_press_pa_raw","空速计动压","(pa):"},
	{"plane_type_g","机型","V"},
	{"plane_propeller_g","浆类型","浆叶数："},
};
int CTeeChart5_testDlg::radio_sms(unsigned char data)
{
	switch(step)
	{
		case 0:
			if( data == 0x46 )
			{
				step = 1;
			}else
			{
				step = 0;
			}
			break;
		case 1:
			if( data == 0x4D )
			{
				step = 2;
			}else
			{
				step = 0;
			}
			break;
		case 2:
			if( data == 0x55 )
			{
				step = 3;
			}else
			{
				step = 0;
			}
			break;
		case 3:
			if( data == 0x01 )
			{
				step = 4;
			}else
			{
				step = 0;
			}
			break;
		case 4:
			if( data == 0x00 )
			{
				step = 5;
			}else
			{
				step = 0;
			}
			break;
		case 5:
			if( data == 0x11 )
			{
				step = 6;
			}else
			{
				step = 0;
			}
			break;
		case 6:
			if( data == 0x00 )
			{
				step = 7;
			}else
			{
				step = 0;
			}
			break;
		case 7:
			if( data == 0xAA )
			{
				step = 8;
			}else
			{
				step = 0;
			}
			break;
		case 8:
			if( data == 0x55 )
			{
				step = 0;
				/* return ok */
				return 0;
			}else
			{
				step = 0;
			}
			break;
		default:
			break;
	}
	/*-----------------*/
	return (-1);
}
/* radio check */
int CTeeChart5_testDlg::check_radio(unsigned char * data,unsigned int len)
{
   int ret;
   /*--get the radio status--*/
   for( unsigned int i = 0 ; i < len ; i ++ )
   {
	   ret = radio_sms(data[i]);
   }
   /* return */
   return ret;
}
/*-------fmlink--------*/
const unsigned char CRC16_TABLE[256] = {
	50, 124, 137, 0, 237, 217, 104, 119, 0, 0, 0, 
	89,  0,  0,  0,  0,  0,  0,  0,  0, 214,    
	159, 220, 168, 24, 23, 170, 144, 67, 115, 39,   
	246, 185, 104, 237, 244, 222, 212, 9, 254, 230, 
	28, 28, 132, 221, 232, 11, 153, 41, 39, 0,    
	0, 0, 0, 15, 3, 0, 230, 0, 0, 0,   
	153, 183, 51, 82, 118, 148, 21, 0, 243, 124,
	0, 0, 38, 20, 158, 152, 143, 0, 0, 0,
    106, 49, 22, 143, 140, 5, 150, 0, 231, 183,
	63, 54, 0, 0, 0, 0, 0, 0, 0, 175,
	102, 158, 208, 56, 93, 138, 108, 32, 185, 84,
	34, 0, 124, 237, 4, 76, 128, 56, 116, 134,
	237, 203, 250, 87, 203, 220, 25, 226, 46, 29,
	223, 85, 6, 229, 203, 1,  0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 154, 49, 0, 134,
	219, 208, 188, 84, 22, 19, 21, 134, 0, 78,
	68, 189, 127, 154, 21, 21, 144, 1, 234, 73,
	181, 22, 83, 167, 138, 234, 240, 47, 189, 52,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,  0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 204, 49, 170, 44, 83, 46,  0};

//************************************************************************************
/*
 *  if the crc data will be  same as the data calc,then return 0;
 *
 *  else return fail none-true
 *
 */
unsigned short CTeeChart5_testDlg::holder_check_crc(unsigned short crc_origin_one,unsigned char *data,int len)
{
  /* resave it now */
	unsigned short crc;

    crc = crc16_accumulate_buffer(len,data);

	if(crc_origin_one!=0)
	{
		if(crc == crc_origin_one)
			return 0;/* matchs ok */
	}

	return crc;
}

//************************************************************************************
unsigned short CTeeChart5_testDlg::crc16_accumulate(unsigned char data,unsigned short crc_data)
{
	unsigned char ch;
		ch = (unsigned char)(data ^ (unsigned char)(crc_data & 0x00ff));
		ch = (unsigned char)(ch ^ (ch << 4));
		crc_data=((crc_data >> 8) ^ (ch << 8) ^ (ch << 3) ^ (ch >> 4));
		return crc_data;
}
//************************************************************************************

//************************************************************************************
unsigned short CTeeChart5_testDlg::crc16_accumulate_buffer(unsigned short buffer_length,unsigned char* data_buffer)
{
	unsigned short i;
	unsigned short crc=0xf00f;
	for( i=1;i<buffer_length;i++)
	{
	crc=crc16_accumulate(data_buffer[i],crc );

	}
	crc=crc16_accumulate( CRC16_TABLE[data_buffer[5]],crc);
	return crc;
}
/* fm link create buffer */
int CTeeChart5_testDlg::fmlink_package_create(unsigned char * buffer,unsigned int buffer_len,unsigned char ID,unsigned char * payload,unsigned int p_len)
{
	unsigned char cache[256+8];
	static unsigned int seq = 0;
	unsigned short crc;
	cache[0] = 0x46;
	cache[1] = 0x4e;
	cache[2] = 0xff;
	cache[3] = p_len;
	cache[4] = 0xb4;
	cache[5] = ID;
	memcpy(&cache[6],payload,p_len);
	
	crc = holder_check_crc(0,cache,p_len+6);

	cache[6+p_len] = crc & 0xff;

	cache[6+p_len+1] = crc >> 8;

	memcpy(buffer,cache,8+p_len);

	return 8+p_len;
}
/*--------------------*/
int CTeeChart5_testDlg::link_autopilot(unsigned char * data,unsigned len)
{
	int ret = 0;
	/* for link loop */
	for( unsigned int i = 0 ; i < len ; i ++ )
	{
		ret = gs_d_icr(data[i]);
	}
	/* return */
	return ret;
}
/* decode the gs data */
int CTeeChart5_testDlg::gs_d_icr(unsigned char c)
{
	static unsigned short cnt2 = 0,crc = 0,cnt = 0;
	static unsigned char step_head = 0;
	static unsigned char step_head2 = 0;

	switch(step_head)
	{
	 case 0:
		 if(c == 0x46)
		 {
			 receive_buf[0] = c;
			 step_head = 1;
		 }
		 else{
			 step_head = 0;
		 }
		 break;
	 case 1:
		 if(c == 0x4d||c == 0x4e)
		 {
			 receive_buf[1] = c;
			 step_head = 2;
		 }
	     else
		 {
			step_head = 0;
		 }
		 break;
	 case 2:
		 if( c == 0x7e )
		 {
		     receive_buf[2] = c;
		     step_head = 3;
		 }else
		 {
			 step_head = 0;
		 }
		 break;
	 case 3:
		 receive_buf[3]=c;
	     step_head = 4;
		 break;
	 case 4:
         receive_buf[4]  = c;
		 step_head = 5;
		 break;
	 case 5:
		 receive_buf[5] = c;
		 step_head = 6;
		 cnt = 0;
		 break;

	 case 6:
		 if((receive_buf[3]+2) > cnt )
		 {
			 receive_buf[6+cnt] = c;
			 cnt++;
		 }
		 if((receive_buf[3]+2) == cnt)
		 {
			 memcpy(&crc,receive_buf+6+(receive_buf[3]),2);
			 cnt = 0;
			 if(crc == holder_check_crc(0,receive_buf,(receive_buf[3]+6)))
			 {	
				 /* get data */
				 fm_data_decode(receive_buf[5],&receive_buf[6],receive_buf[3]);
				 /* return ok */
				 return 0;
			 }
					 
			 step_head = 0;
		 }
		 break;
	 default :
		 step_head = 0;
		 break;
	}
	/* return */
	return (-1);
}
/*------------------*/
extern COM_MANAGE_DEF COM;
/*------------------*/
/* create */
int CTeeChart5_testDlg::fm_link_send(unsigned char ID , unsigned char *data,unsigned int len_p)
{
	/*-------------------------*/
	unsigned char buffer[300];
	int len;
	/* create package */
	len = fmlink_package_create(buffer,sizeof(buffer),ID,data,len_p);
	/* send */
	return send(buffer,len);
}
/* link autopilot thread */
void CTeeChart5_testDlg::link_autopilot_thread(void)
{
	unsigned char buffer = 0xff;//link id
	/*-------------------------*/
	fm_link_send(0xff,&buffer,1);
}
/* decode and check */
void CTeeChart5_testDlg::fm_link_handle(unsigned char * data,unsigned int len)
{
    /* check radio */
	if( COM.radio_status == 0 )
	{
		/* not connect */
		if( check_radio(data,len) == 0 )
		{
			/* check ok */
			m_taps.SetWindowTextW(_T("提示信息：找到电台，正在连接飞机"));
		    /* check radio */
			COM.radio_status = 1;
			/*------------*/
			KillTimer(1);
			/* start */
			SetTimer(2,1000,NULL);
			/*-----------*/
			COM.show_flags = 1;
		}
	}
	/*-- check the normal data --*/
	if( link_autopilot(data,len) == 0 )
	{
		if( COM.show_flags == 0 )
		{
			/* check ok */
			m_taps.SetWindowTextW(_T("提示信息：找到电台，正在连接飞机"));
		    /* check radio */
			COM.radio_status = 1;
			/*------------*/
			KillTimer(1);
			/* start */
			SetTimer(2,1000,NULL);
			/* clear */
			COM.show_flags = 1;
		}
	}
}
/* unit data */
void CTeeChart5_testDlg::unit_data_thread(void)
{
	unsigned char buffer[3];//link id
	/*-------------------------*/
	buffer[0] = 0xfe;
	buffer[1] = COM.now_sw;
	buffer[2] = COM.now_num;
	/*-------------------------*/
	fm_link_send(0xff,buffer,3);
}
/* set unit num */
void CTeeChart5_testDlg::set_unit_num(unsigned int now_sw,unsigned int now_num)
{
	COM.now_sw  = now_sw;
	COM.now_num = now_num;
}
/* list updata ; */
void CTeeChart5_testDlg::List_refresh(unsigned int mode)
{
	if( mode == 0 )
	{

	}
}
/*--------------*/
void CTeeChart5_testDlg::List_refresh_line(SYS_LIST_DETAIL * list)
{
	/*---------------*/
	USES_CONVERSION;
	/* */
	CString c_name,c_chese,c_index,c_data;
    /* check */
	list_transfer(list);
	/* chese */

}
/*-------------*/
void CTeeChart5_testDlg::List_delete_all(void)
{
	m_list_ctrl.DeleteAllItems();
	chese_list.total_line = 0;
}
/* string_chese_table */
int CTeeChart5_testDlg::list_transfer(SYS_LIST_DETAIL * list)
{
	unsigned short type;
	unsigned short num;
	int j;
	unsigned char *show_uint8;
	short *show_int16;
	unsigned short *show_uint16;
	int *show_int32;
	unsigned int *show_uint32;
	float *show_float;
	/*---------------*/
	for( int i = 0 ; i < 23 ; i ++ )
	{
		if( strstr(list->title,string_chese_table[i][0]) != NULL )
		{
			/* copy data */
			memcpy(chese_list.chese[chese_list.total_line].title_chese,string_chese_table[i][1],strlen(string_chese_table[i][1]));
			/* ----- */
			memcpy(chese_list.chese[chese_list.total_line].title,list->title,sizeof(list->title));
			/* copy raw data */
			memcpy(chese_list.chese[chese_list.total_line].raw_data,list->raw_data,sizeof(list->raw_data));
			/* find the check item*/
			chese_list.chese[chese_list.total_line].type = list->data_type;
			chese_list.chese[chese_list.total_line].num = list->data_num;
			/*--------------------*/
			chese_list.chese[chese_list.total_line].index = chese_list.total_line;
			/*--------------------*/
			type = list->data_type;
			num = list->data_num;
			/* clear show data */
			memset(chese_list.chese[chese_list.total_line].show_data,0,sizeof(chese_list.chese[chese_list.total_line].show_data));
			/*--------------------*/
			switch(type)
			{
				case 0x81:
					memcpy(chese_list.chese[chese_list.total_line].show_data,list->raw_data,num);
					break;
				case 0x01:
					show_uint8 = (unsigned char *)list->raw_data;
					/* show */
					for( j = 0 ; j < num ; j ++ )
					{
						int leng = strlen(chese_list.chese[chese_list.total_line].show_data);
						sprintf_s(chese_list.chese[chese_list.total_line].show_data + leng,
							sizeof(chese_list.chese[chese_list.total_line].show_data)-leng,
							"%s%d ",string_chese_table[i][j+2],show_uint8[j]);
					}
					break;
				case 0x82:
					show_int16 = (short *)list->raw_data;
					/* show */
					for( j = 0 ; j < num ; j ++ )
					{
						int leng = strlen(chese_list.chese[chese_list.total_line].show_data);
						sprintf_s(chese_list.chese[chese_list.total_line].show_data + leng,
							sizeof(chese_list.chese[chese_list.total_line].show_data) - leng,
							"%s%d ",string_chese_table[i][j+2],show_int16[j]);
					}
					break;
				case 0x02:
					show_uint16 = (unsigned short *)list->raw_data;
					/* show */
					for( j = 0 ; j < num ; j ++ )
					{
						int leng = strlen(chese_list.chese[chese_list.total_line].show_data);
						sprintf_s(chese_list.chese[chese_list.total_line].show_data + leng,
							sizeof(chese_list.chese[chese_list.total_line].show_data) - leng,
							"%s%d ",string_chese_table[i][j+2],show_uint16[j]);
					}
					break;
				case 0x84:
					show_int32 = (int *)list->raw_data;
					/* show */
					for( j = 0 ; j < num ; j ++ )
					{
						int leng = strlen(chese_list.chese[chese_list.total_line].show_data);
						sprintf_s(chese_list.chese[chese_list.total_line].show_data + leng,
							sizeof(chese_list.chese[chese_list.total_line].show_data) - leng,
							"%s%d ",string_chese_table[i][j+2],show_int32[j]);
					}
					break;
				case 0x04:
					show_uint32 = (unsigned int *)list->raw_data;
					/* show */
					for( j = 0 ; j < num ; j ++ )
					{
						int leng = strlen(chese_list.chese[chese_list.total_line].show_data);
						sprintf_s(chese_list.chese[chese_list.total_line].show_data + leng,
							sizeof(chese_list.chese[chese_list.total_line].show_data) - leng,
							"%s%d ",string_chese_table[i][j+2],show_uint32[j]);
					}
					break;
				case 0x05:
					show_float = (float *)list->raw_data;
					/* show */
					for( j = 0 ; j < num ; j ++ )
					{
						int leng = strlen(chese_list.chese[chese_list.total_line].show_data);
						sprintf_s(chese_list.chese[chese_list.total_line].show_data + leng,
							sizeof(chese_list.chese[chese_list.total_line].show_data) - leng,
							"%s%5.2f ",string_chese_table[i][j+2],show_float[j]);
					}
					break;
				default :
				break;
			}
			/*---check if---*/
			update_check_result(&chese_list.chese[chese_list.total_line],1);
			Check_list_one(&chese_list.chese[chese_list.total_line]);
			/* line ++ */
			chese_list.total_line++;
			/* return ok */
			return 0;
		}
	}
	/* return err */
	return (-1);
}
/* update check result */
int CTeeChart5_testDlg::update_check_result( SYS_LIST_CHESE_DETAIL_DEF * check_ese,unsigned int mode)
{
	unsigned char *show_uint8,*show_uint8_l,*show_uint8_u,*show_uint8_m;
	char *show_int8,*show_int8_l,*show_int8_u,*show_int8_m;
	unsigned short *show_uint16,*show_uint16_l,*show_uint16_u,*show_uint16_m;
	short *show_int16, *show_int16_l, *show_int16_u,*show_int16_m;
	unsigned int *show_uint32,*show_uint32_l,*show_uint32_u,*show_uint32_m;
	int *show_int32,*show_int32_l,*show_int32_u,*show_int32_m;
	float *show_float,*show_float_l,*show_float_u,*show_float_m,*show_float_m_now,*show_float_r_now,*float_limit_m,*float_limit_e,*float_limit_r;
	unsigned int i;
	/*---check if---*/
	if( check_ese->check_if == 1 )
	{
		/* check by people */
		if( motor_check_flags == 4 )
		{
			if( strstr( check_ese->title , "g_mrs") != NULL )
			{
				check_ese->check_type = 4;//check and ok
				sprintf_s(check_ese->result,"通过");
			    check_ese->check_ok = 1;//ok
			    return 0;
			}
		}
		if( motor_check_flags == 5 )
		{
			if( strstr( check_ese->title , "g_mrs") != NULL )
			{
				check_ese->check_type = 5;
				sprintf_s(check_ese->result,"未通过");
				check_ese->check_ok = 0;//err
				return 0;
			}
		}
		/* take a picture */
		if( m_check_camera.GetCheck() && mode == 1 )
		{
			if( strstr( check_ese->title , "battery_msg") != NULL )
			{
				/* take a photo */
				take_a_photo();
			}
		}
		/* defautl is ok */
		sprintf_s(check_ese->result,"PASS");
		check_ese->check_ok = 1;//ok
		/*---------------*/
		if( 1 )//check_ese->check_type == 0 )//pass with none '0'
		{
			switch(check_ese->type)
			{
				case 0x81:
					show_int8   =  (char *)check_ese->raw_data;
					show_int8_l =  (char *)check_ese->limit_low;
					show_int8_u =  (char *)check_ese->limit_high;
					/* show */
					for( i = 0 ; i < check_ese->num ; i ++ )
					{
						if( check_ese->check_type == 0 )
						{
							if( show_int8[i] == 0 )
							{
								check_ese->check_ok = 0;
								sprintf_s(check_ese->result,"参数%d异常",i);
								break;
							}
						}else
						{
							 if( check_ese->check_type == 1 )
							 {
								 if( !(( show_int8[i] >= show_int8_l[i] ) && ( show_int8[i] <= show_int8_u[i] )) )
								 {
									check_ese->check_ok = 0;
									sprintf_s(check_ese->result,"参数%d异常",i);
									break;
								 }
							 }else if( check_ese->check_type == 2 )
							 {
								 if( !(( show_int8[i] >= show_int8_l[i] ) && ( show_int8[i] <= show_int8_u[i] ) && show_int8[i] != 0 ) )
								 {
									check_ese->check_ok = 0;
									sprintf_s(check_ese->result,"参数%d异常",i);
									break;
								 }
							 }
							 else if( check_ese->check_type == 3 )
							 {
								 if( check_ese->mean_rms_num < 10 )
								 {
									 if( mode == 1 )
									 {
										 sprintf_s(check_ese->result,"收集中%d/10",check_ese->mean_rms_num+1);
									 }else
									 {
										 sprintf_s(check_ese->result,"收集中%d/10",check_ese->mean_rms_num);
									 }
									 /* check */
									 check_ese->check_ok = 0;
									 /* save data */
									 show_int8_m = (char *)&check_ese->data_mean_rms[i];
									 /* save */
									 show_int8_m[check_ese->mean_rms_num] = *show_int8;
								 }
							 }
						}
					}
					/* fresh or update */
					if( mode == 1 && check_ese->mean_rms_num < 10 )
					{
						/*----------------------*/
						check_ese->mean_rms_num++;
						/*----------------------*/
					}
					break;
				case 0x01:
					show_uint8   =  (unsigned char *)check_ese->raw_data;
					show_uint8_l =  (unsigned char *)check_ese->limit_low;
					show_uint8_u =  (unsigned char *)check_ese->limit_high;
					/* show */
					for( i = 0 ; i < check_ese->num ; i ++ )
					{
						if( check_ese->check_type == 0 )
						{
							if( show_uint8[i] == 0 )
							{
								check_ese->check_ok = 0;
								sprintf_s(check_ese->result,"参数%d异常",i);
								break;
							}
						}else
						{
							 if( check_ese->check_type == 1 )
							 {
								 if( !(( show_uint8[i] >= show_uint8_l[i] ) && ( show_uint8[i] <= show_uint8_u[i] )) )
								 {
									check_ese->check_ok = 0;
									sprintf_s(check_ese->result,"参数%d异常",i);
									break;
								 }
							 }else if( check_ese->check_type == 2 )
							 {
								 if( !(( show_uint8[i] >= show_uint8_l[i] ) && ( show_uint8[i] <= show_uint8_u[i] ) && show_uint8[i] != 0 ) )
								 {
									check_ese->check_ok = 0;
									sprintf_s(check_ese->result,"参数%d异常",i);
									break;
								 }
							 }
							 else if( check_ese->check_type == 3 )
							 {
								 if( check_ese->mean_rms_num < 10 )
								 {
									 if( mode == 1 )
									 {
										 sprintf_s(check_ese->result,"收集中%d/10",check_ese->mean_rms_num+1);
									 }else
									 {
										 sprintf_s(check_ese->result,"收集中%d/10",check_ese->mean_rms_num);
									 }
									 /* check */
									 check_ese->check_ok = 0;
									 /* save data */
									 show_uint8_m = (unsigned char *)&check_ese->data_mean_rms[i];
									 /* save */
									 show_uint8_m[check_ese->mean_rms_num] = *show_uint8;
								 }
							 }
						}
					}
					/* fresh or update */
					if( mode == 1 && check_ese->mean_rms_num < 10 )
					{
						/*----------------------*/
						check_ese->mean_rms_num++;
						/*----------------------*/
					}
					break;
				case 0x82:
				    show_int16   =  (short *)check_ese->raw_data;
					show_int16_l =  (short *)check_ese->limit_low;
					show_int16_u =  (short *)check_ese->limit_high;
					/* show */
					for( i = 0 ; i < check_ese->num ; i ++ )
					{
						if( check_ese->check_type == 0 )
						{
							if( show_int16[i] == 0 )
							{
								check_ese->check_ok = 0;
								sprintf_s(check_ese->result,"参数%d异常",i);
								break;
							}
						}else
						{
							 if( check_ese->check_type == 1 )
							 {
								 if( !(( show_int16[i] >= show_int16_l[i] ) && ( show_int16[i] <= show_int16_u[i] )) )
								 {
									check_ese->check_ok = 0;
									sprintf_s(check_ese->result,"参数%d异常",i);
									break;
								 }
							 }else if( check_ese->check_type == 2 )
							 {
								 if( !(( show_int16[i] >= show_int16_l[i] ) && ( show_int16[i] <= show_int16_u[i] ) && show_int16[i] != 0 ) )
								 {
									check_ese->check_ok = 0;
									sprintf_s(check_ese->result,"参数%d异常",i);
									break;
								 }
							 }
							 else if( check_ese->check_type == 3 )
							 {
								 if( check_ese->mean_rms_num < 10 )
								 {
									 if( mode == 1 )
									 {
										 sprintf_s(check_ese->result,"收集中%d/10",check_ese->mean_rms_num+1);
									 }else
									 {
										 sprintf_s(check_ese->result,"收集中%d/10",check_ese->mean_rms_num);
									 }
									 /* check */
									 check_ese->check_ok = 0;
									 /* save data */
									 show_int16_m = (short *)&check_ese->data_mean_rms[i];
									 /* save */
									 show_int16_m[check_ese->mean_rms_num] = *show_int16;
								 }
							 }
						}
					}
					/* fresh or update */
					if( mode == 1 && check_ese->mean_rms_num < 10 )
					{
						/*----------------------*/
						check_ese->mean_rms_num++;
						/*----------------------*/
					}
					break;
				case 0x02:
					show_uint16   =  (unsigned short *)check_ese->raw_data;
					show_uint16_l =  (unsigned short *)check_ese->limit_low;
					show_uint16_u =  (unsigned short *)check_ese->limit_high;
					/* show */
					for( i = 0 ; i < check_ese->num ; i ++ )
					{
						if( check_ese->check_type == 0 )
						{
							if( show_uint16[i] == 0 )
							{
								check_ese->check_ok = 0;
								sprintf_s(check_ese->result,"参数%d异常",i);
								break;
							}
						}else
						{
							 if( check_ese->check_type == 1 )
							 {
								 if( !(( show_uint16[i] >= show_uint16_l[i] ) && ( show_uint16[i] <= show_uint16_u[i] )) )
								 {
									check_ese->check_ok = 0;
									sprintf_s(check_ese->result,"参数%d异常",i);
									break;
								 }
							 }else if( check_ese->check_type == 2 )
							 {
								 if( !(( show_uint16[i] >= show_uint16_l[i] ) && ( show_uint16[i] <= show_uint16_u[i] ) && show_uint16[i] != 0 ) )
								 {
									check_ese->check_ok = 0;
									sprintf_s(check_ese->result,"参数%d异常",i);
									break;
								 }
                             }
							 else if( check_ese->check_type == 3 )
							 {
								 if( check_ese->mean_rms_num < 10 )
								 {
									 if( mode == 1 )
									 {
										 sprintf_s(check_ese->result,"收集中%d/10",check_ese->mean_rms_num+1);
									 }else
									 {
										 sprintf_s(check_ese->result,"收集中%d/10",check_ese->mean_rms_num);
									 }
									 /* check */
									 check_ese->check_ok = 0;
									 /* save data */
									 show_uint16_m = (unsigned short *)&check_ese->data_mean_rms[i];
									 /* save */
									 show_uint16_m[check_ese->mean_rms_num] = *show_uint16;
								 }
							 }
						}
					}
					/* fresh or update */
					if( mode == 1 && check_ese->mean_rms_num < 10 )
					{
						/*----------------------*/
						check_ese->mean_rms_num++;
						/*----------------------*/
					}
					break;
				case 0x84:
				    show_int32   =  (int *)check_ese->raw_data;
					show_int32_l =  (int *)check_ese->limit_low;
					show_int32_u =  (int *)check_ese->limit_high;
					/* show */
					for( i = 0 ; i < check_ese->num ; i ++ )
					{
						if( check_ese->check_type == 0 )
						{
							if( show_int32[i] == 0 )
							{
								check_ese->check_ok = 0;
								sprintf_s(check_ese->result,"参数%d异常",i);
								break;
							}
						}else
						{
							 if( check_ese->check_type == 1 )
							 {
								 if( !(( show_int32[i] >= show_int32_l[i] ) && ( show_int32[i] <= show_int32_u[i] )) )
								 {
									check_ese->check_ok = 0;
									sprintf_s(check_ese->result,"参数%d异常",i);
									break;
								 }
							 }else if( check_ese->check_type == 2 )
							 {
								 if( !(( show_int32[i] >= show_int32_l[i] ) && ( show_int32[i] <= show_int32_u[i] ) && show_int32[i] != 0 ) )
								 {
									check_ese->check_ok = 0;
									sprintf_s(check_ese->result,"参数%d异常",i);
									break;
								 }
                             }
							 else if( check_ese->check_type == 3 )
							 {
								 if( check_ese->mean_rms_num < 10 )
								 {
									 if( mode == 1 )
									 {
										 sprintf_s(check_ese->result,"收集中%d/10",check_ese->mean_rms_num+1);
									 }else
									 {
										 sprintf_s(check_ese->result,"收集中%d/10",check_ese->mean_rms_num);
									 }
									 /* check */
									 check_ese->check_ok = 0;
									 /* save data */
									 show_int32_m = (int *)&check_ese->data_mean_rms[i];
									 /* save */
									 show_int32_m[check_ese->mean_rms_num] = *show_int32;
								 }
							 }
						}
					}
					/* fresh or update */
					if( mode == 1 && check_ese->mean_rms_num < 10 )
					{
						/*----------------------*/
						check_ese->mean_rms_num++;
						/*----------------------*/
					}
					break;				
				case 0x04:
					show_uint32   =  (unsigned int *)check_ese->raw_data;
					show_uint32_l =  (unsigned int *)check_ese->limit_low;
					show_uint32_u =  (unsigned int *)check_ese->limit_high;
					/* show */
					for( i = 0 ; i < check_ese->num ; i ++ )
					{
						if( check_ese->check_type == 0 )
						{
							if( show_uint32[i] == 0 )
							{
								check_ese->check_ok = 0;
								sprintf_s(check_ese->result,"参数%d异常",i);
								break;
							}
						}else
						{
							 if( check_ese->check_type == 1 )
							 {
								 if( !(( show_uint32[i] >= show_uint32_l[i] ) && ( show_uint32[i] <= show_uint32_u[i] )) )
								 {
									check_ese->check_ok = 0;
									sprintf_s(check_ese->result,"参数%d异常",i);
									break;
								 }
							 }else if( check_ese->check_type == 2 )
							 {
								 if( !(( show_uint32[i] >= show_uint32_l[i] ) && ( show_uint32[i] <= show_uint32_u[i] ) && show_uint32[i] != 0 ) )
								 {
									check_ese->check_ok = 0;
									sprintf_s(check_ese->result,"参数%d异常",i);
									break;
								 }
                             }
							 else if( check_ese->check_type == 3 )
							 {
								 if( check_ese->mean_rms_num < 10 )
								 {
									 if( mode == 1 )
									 {
										 sprintf_s(check_ese->result,"收集中%d/10",check_ese->mean_rms_num+1);
									 }else
									 {
										 sprintf_s(check_ese->result,"收集中%d/10",check_ese->mean_rms_num);
									 }
									 /* check */
									 check_ese->check_ok = 0;
									 /* save data */
									 show_uint32_m = (unsigned int *)&check_ese->data_mean_rms[i];
									 /* save */
									 show_uint32_m[check_ese->mean_rms_num] = *show_uint32;
								 }
							 }
						}
					}
					/* fresh or update */
					if( mode == 1 && check_ese->mean_rms_num < 10 )
					{
						/*----------------------*/
						check_ese->mean_rms_num++;
						/*----------------------*/
					}
					break;
				case 0x05:
					show_float   =  (float *)check_ese->raw_data;
					show_float_l =  (float *)check_ese->limit_low;
					show_float_u =  (float *)check_ese->limit_high;
					show_float_m_now = (float *)check_ese->mean_now;
					show_float_r_now = (float *)check_ese->rms_now;
					float_limit_m = (float *)check_ese->limit_mean;
					float_limit_e = (float *)check_ese->limit_mean_error;
					float_limit_r = (float *)check_ese->limit_rms;
					/* show */
					for( i = 0 ; i < check_ese->num ; i ++ )
					{
						if( check_ese->check_type == 0 )
						{
							if( show_float[i] == 0 )
							{
								check_ese->check_ok = 0;
								sprintf_s(check_ese->result,"参数%d异常",i);
								break;
							}
						}else
						{
							 if( check_ese->check_type == 1 )
							 {
								 if( !(( show_float[i] >= show_float_l[i] ) && ( show_float[i] <= show_float_u[i] )) )
								 {
									check_ese->check_ok = 0;
									sprintf_s(check_ese->result,"参数%d异常",i);
									break;
								 }
							 }else if( check_ese->check_type == 2 )
							 {
								 if( !(( show_float[i] >= show_float_l[i] ) && ( show_float[i] <= show_float_u[i] ) && show_float[i] != 0 ) )
								 {
									check_ese->check_ok = 0;
									sprintf_s(check_ese->result,"参数%d异常",i);
									break;
								 }
                             }
							 else if( check_ese->check_type == 3 )
							 {
								 /* save data */
								 show_float_m = (float *)&check_ese->data_mean_rms[i];
								 /*------------------------*/
								 if( check_ese->mean_rms_num < 10 )
								 {
									 if( mode == 1 )
									 {
										 sprintf_s(check_ese->result,"收集中%d/10",check_ese->mean_rms_num+1);
									 }else
									 {
										 sprintf_s(check_ese->result,"收集中%d/10",check_ese->mean_rms_num);
									 }
									 /*--------------*/
									 check_ese->check_ok = 0;
									 /* save */
								     show_float_m[check_ese->mean_rms_num] = show_float[i];
								 }else
								 {
									 if( mode == 1 )
									 {
										 for( int ti = 0 ; ti < 9 ; ti ++ )
										 {
											 show_float_m[ti] = show_float_m[ti+1];
										 }
										 /*-----------------*/
										 show_float_m[9] = show_float[i];
									 }
									 /*-----------------*/
									 MeanCal((float *)&check_ese->data_mean_rms[i],10,&show_float_m_now[i],&show_float_r_now[i]);
									 /* check */
									 if( !(( show_float_m_now[i] >= ( float_limit_m[i] - float_limit_e[i] )) && 
										   ( show_float_m_now[i] <= ( float_limit_m[i] + float_limit_e[i] ) )) )
									 {
										check_ese->check_ok = 0;
										sprintf_s(check_ese->result,"均值%d异常",i);
										break;
									 }
									 /*-----------*/
									 if( show_float_r_now[i] > float_limit_r[i] )
									 {
										check_ese->check_ok = 0;
										sprintf_s(check_ese->result,"方差%d异常",i);
										break;
									 }
								 }
							 } 
						}
					}
					/* fresh or update */
					if( mode == 1 && check_ese->mean_rms_num < 10 )
					{
						/*----------------------*/
						check_ese->mean_rms_num++;
						/*----------------------*/
					}
					break;
				default :
				  break;
			}
		}
	}else
	{
		sprintf_s(check_ese->result,"----");
	}
	return 0;
}
/* add one list */
void CTeeChart5_testDlg::Check_list_one(SYS_LIST_CHESE_DETAIL_DEF * list)
{
	unsigned int index;
	char buffer[16];
	CString c_name,c_chese,c_index,c_data,c_result;
	/*---------*/
	USES_CONVERSION;
	/*--------*/
	c_chese = A2T(list->title_chese);
	/*--------*/
	sprintf_s(buffer,"%d",list->index);
	/*--------*/
	c_index = A2T(buffer);
	/*--------*/
	c_data = A2T(list->show_data);
	/*--------*/
	c_result = A2T(list->result);
	/*--------*/
	Set_sempher(1);
	/* index */
	index = list->index;
	///*--------*/
	/*--------*/
	m_list_ctrl.InsertItem(index, c_index);
	m_list_ctrl.SetItemText(index, 1, c_chese);
	m_list_ctrl.SetItemText(index, 2, c_result);
	m_list_ctrl.SetItemText(index, 3, c_data);
	/*------------------------------------------*/
	if( list->check_if )
	{
		m_list_ctrl.SetCheck(index, 1);
	}else
	{
		m_list_ctrl.SetCheck(index, 0);
	}
	/*----*/
	Set_sempher(0);
}
/* up data check result list */
void CTeeChart5_testDlg::refresh_result_all(void)
{   
	Set_sempher(1);
	/* reset */
	m_list_ctrl.DeleteAllItems();
	/* check all button */
	/* ip */
	for( unsigned int i = 0 ; i < chese_list.total_line ; i ++ )
	{
		update_check_result(&chese_list.chese[i],0);
		Check_list_one(&chese_list.chese[i]);
	}
	/* reset semphter */
	Set_sempher(0);
}
/* add data to draw combox */
void CTeeChart5_testDlg::push_data_to_dram_lines( node_def * node ,unsigned int mode )
{
	CString show;
	/*-------------*/
	USES_CONVERSION;
	/*-------------*/
	if( mode == 1 )
	{
		m_combox_param_show.ResetContent();
		memset(&combox_rt_line,0,sizeof(combox_rt_line));
	}else if( mode == 0 )
	{
		return;
	}
	/*-------------------*/
	if( node->type == _CB_RT_ )
	{
		/*-----------------------------------*/
		combox_rt_line.rt_line[combox_rt_line.com_line_num].param_type = node->size >> 16;
		combox_rt_line.rt_line[combox_rt_line.com_line_num].param_num  = node->size & 0xffff;
		combox_rt_line.rt_line[combox_rt_line.com_line_num].index_raw  = node->line_num;
		/*-----------------------------------*/
		for( unsigned int i = 0 ; i < combox_rt_line.rt_line[combox_rt_line.com_line_num].param_num ;i++ )
		{
			sprintf_s(combox_rt_line.rt_line[combox_rt_line.com_line_num].name[i],"%s_D%d",node->name,i);
			/* transfer */
			show = A2T(combox_rt_line.rt_line[combox_rt_line.com_line_num].name[i]);
			/* COMBOX add line */
			m_combox_param_show.AddString(show);
			/*----------------*/
			m_combox_param_show.SetCurSel(combox_rt_line.combox_item_num);
			/* item ++*/
			combox_rt_line.combox_item_num++;
		}
		/* line */
		combox_rt_line.com_line_num++;
	}
}
/* get line group and index */
int CTeeChart5_testDlg::Get_line_group_index(unsigned int seq)
{
	if( combox_clear_flags != 0 )
	{
		return (-1);
	}
	/*----------*/
	unsigned int sum = 0;
	unsigned int group,index;
	CString show;
	/* transfer */
	USES_CONVERSION;
	/* get data */
	for( unsigned int i = 0 ; i < combox_rt_line.com_line_num ; i++ )
	{
		sum +=  combox_rt_line.rt_line[i].param_num;
		/* sum */
		if( sum > seq )
		{
			index = combox_rt_line.rt_line[i].param_num - (sum - seq);
			group = combox_rt_line.rt_line[i].index_raw;
			/*--------------------*/
			for( int j = 0 ; j < line_cmd.line_num ; j++ )
			{
                if( line_cmd.line_gid[i][0] == group && line_cmd.line_gid[i][1] == index )
                {
                	/* already exist */
                	return (-1);
                }
			}
			/* set line struct */
			if( m_check_hold.GetCheck() )
			{
				if( line_cmd.line_num < 10 )
				{
					/*-----------------*/
					line_cmd.line_gid[line_cmd.line_num][0] = group;
					line_cmd.line_gid[line_cmd.line_num][1] = index;
					/*----------------*/
					line_cmd.line_num++;
				}else
				{
					MessageBox(_T("最多支持10条曲线"),_T("tips"),0);
					return (-1);
				}
			}else
			{
				line_cmd.line_num = 1;
				/*-----------------*/
				line_cmd.line_gid[0][0] = group;
				line_cmd.line_gid[0][1] = index;
				/* clear all line first */
				//===========================================dsfsdfsddfghdjdsfasdaf
				clear_all_exist_lines(1);
				/*-----------------------*/
			}
			/* allcate line source */
			//-----------=-=-=-=-=-=-=-=-=-=-=
			unsigned int line_source = 0;
			/* get source */
			if( Get_line10_source(&line_source) != 0 )
			{
				MessageBox(_T("最多支持10条曲线"),_T("tips"),0);
				return (-1);
			}
			/*-------*/
			combox_rt_line.rt_line[i].line_source[index] = line_source;
			/* show legend */
	        Legend_handle(1);
			/* put title */
			show = A2T(combox_rt_line.rt_line[i].name[index]);
			/* put title */
			CSeries line_cfs = (CSeries)m_chart.Series(line_source);
			/* -- show title -- */
			line_cfs.put_Title(show);
			/* show on */
			combox_rt_line.rt_line[i].line_ok[index] = 1;
			/*---------------*/
			unsigned int colorR = (unsigned char)rand();
			unsigned int colorG = (unsigned char)rand();
			unsigned int colorB = (unsigned char)rand();
			/*-----------*/
			unsigned long cols = (colorB<<16)|(colorG<<8)|(colorR);
			/* put color */
			Legend_handle(1);
			/*-----------------------------*/
			line_cfs.put_Title(show);
			/* show legend */
			line_cfs.put_ShowInLegend(1);
			/* put color */
			line_cfs.put_Color(cols);
			/* tips */
			m_taps.SetWindowTextW(_T("提示信息：正在设置曲线......"));
			/*-------------*/
			line_cmd.cmd = 0xfd;
			/* start thread */
			SetTimer(4,500,NULL);
			show_taps_flags = 1;
			/* --------------- */
			break;
		}
	}
	return 0;
}
/*---------------------*/
int CTeeChart5_testDlg::Get_line10_source(unsigned int *num)
{
	/*----------------------*/
	for( int i = 0 ; i < 10 ; i ++ )
	{
		if( lvs[i] == 0 )
		{
			lvs[i] = 1;
			*num = i;
			return 0;
		}
	}
	/*   */
	return (-1);;
}
/*---------------------*/
void CTeeChart5_testDlg::clear_all_exist_lines(unsigned int mode)
{
	for( unsigned int i = 0 ; i < combox_rt_line.com_line_num ; i++ )
	{
		for( unsigned int j = 0 ; j < combox_rt_line.rt_line[i].param_num ; j++ )
		{
			combox_rt_line.rt_line[i].line_ok[j] = 0;//clear
		}
	}
	/* clear lines */
	for( int t = 0 ; t < 10 ; t ++ )
	{
		/* create line */
		CSeries lineS = (CSeries)m_chart.Series(t);
		/* clear */
		lineS.Clear();
	}
	/*------------*/
	for( unsigned int i = 0 ; i < 10 ; i ++ )
	{
		CSeries line_cfs = (CSeries)m_chart.Series(i);
		/* setting */
		line_cfs.Clear();
		line_cfs.put_ShowInLegend(0);
		lvs[i] = 0;
	}
	/*--------------*/
    Legend_handle(0);//hide
	/*-------------*/
	if( mode == 0 )
	{
		/* tips */
		m_taps.SetWindowTextW(_T("提示信息：正在设置曲线......"));
		/*-------------*/
		line_cmd.cmd = 0xfd;
		line_cmd.line_num = 0;
		/* start thread */
		SetTimer(4,500,NULL);
		show_taps_flags = 1;
	}
}
/* set line num thread */
void CTeeChart5_testDlg::Set_line_output_thread(void)
{
	unsigned int num = line_cmd.line_num * 2 + 4;
	/*----------------------*/
	memcpy(&buffer,&line_cmd,num);
	/*----------------------*/
    fm_link_send(0xff,(unsigned char *)buffer,num );
}
/*------------*/
void CTeeChart5_testDlg::Force_cache_wave( unsigned char * line_raw , unsigned int len )
{
	int real_leg = len - 2;
	int width = 0;
	/* search */
	/*-------------*/
	for( int j = 0 ; j < line_cmd.line_num ; j ++ )
	{
		for( unsigned int i = 0 ; i < combox_rt_line.com_line_num ; i ++ )
		{
			if( combox_rt_line.rt_line[i].index_raw == line_cmd.line_gid[j][0] )//line->line_gid[0][0] )
			{
				unsigned int index = line_cmd.line_gid[j][1];
				/*--------------------------------------*/
				/* get width */
				Force_Q_wave(i,index,&line_raw[2 + width]);
				/*------------------*/
				if( combox_rt_line.rt_line[i].param_type > 4 )
				{
					width += 4;
				}else
				{
					width += combox_rt_line.rt_line[i].param_type;
				}
				/*----------------------------------*/
				break;
			}
		}
	}
}
/*--------------------------*/
void CTeeChart5_testDlg::Force_Q_wave(unsigned int line_g , unsigned int index , unsigned char * data )
{
	double tmp;
	if( combox_rt_line.rt_line[line_g].line_available_point[index] == 200 )
	{
		for( int i = 0 ; i < 199 ; i ++ )
		{
			combox_rt_line.rt_line[line_g].line_data[index][i] = combox_rt_line.rt_line[line_g].line_data[index][i+1];
		}
		/*------------*/
		combox_rt_line.rt_line[line_g].line_available_point[index] --;
	}
	switch( combox_rt_line.rt_line[line_g].param_type )
	{
		case 0x81:
			combox_rt_line.rt_line[line_g].line_data[index][combox_rt_line.rt_line[line_g].line_available_point[index]] = (double)(*((char *)data));
			break;
		case 0x01:
			combox_rt_line.rt_line[line_g].line_data[index][combox_rt_line.rt_line[line_g].line_available_point[index]] = (double)(*((unsigned char *)data));
			break;
		case 0x82:
			combox_rt_line.rt_line[line_g].line_data[index][combox_rt_line.rt_line[line_g].line_available_point[index]] = (double)(*((short *)data));
			break;
		case 0x02:
			combox_rt_line.rt_line[line_g].line_data[index][combox_rt_line.rt_line[line_g].line_available_point[index]] = (double)(*((unsigned short *)data));
			break;
		case 0x84:
			combox_rt_line.rt_line[line_g].line_data[index][combox_rt_line.rt_line[line_g].line_available_point[index]] = (double)(*((int *)data));
			break;
		case 0x04:
			combox_rt_line.rt_line[line_g].line_data[index][combox_rt_line.rt_line[line_g].line_available_point[index]] = (double)(*((unsigned int *)data));
			break;
		case 0x05:
			tmp = (double)(*((float *)data));
			combox_rt_line.rt_line[line_g].line_data[index][combox_rt_line.rt_line[line_g].line_available_point[index]] = tmp;
			break;
		default :
			break;
	}
	/* --- */
	if( combox_rt_line.rt_line[line_g].line_available_point[index] < 200 )
	{
	   combox_rt_line.rt_line[line_g].line_available_point[index]++;
	}
//	/*  */
//	int i;
//	/*-------------*/
//	for( i = 0 ; i < mulit_cnt ; i++ )
//	{
//	    /* macol line */
//		if( line_mulit[i].li == line_g && line_mulit[i].lj == index )
//		{
//			break;
//		}
//	}
//	/*------------*/
//	if( i == mulit_cnt )
//	{
//		line_mulit[mulit_cnt].li = line_g;
//		line_mulit[mulit_cnt].lj = index;
//		line_mulit[mulit_cnt].lin_srv = mulit_cnt;
//		combox_rt_line.rt_line[line_g].line_source[index] = mulit_cnt;
//		mulit_cnt++;
//	}
//	/* draw lines */
////	Draw_simple_line(combox_rt_line.rt_line[line_g].line_data[index],combox_rt_line.rt_line[line_g].line_available_point);
	Draw_all_lines();
}
/* draw all lines */
void CTeeChart5_testDlg::Draw_all_lines(void)
{
	for( unsigned int i =  0 ; i < combox_rt_line.com_line_num ; i ++ )
	{
		for( int j = 0 ; j < 32 ; j ++ )
		{
			if( combox_rt_line.rt_line[i].line_ok[j] == 1 && combox_rt_line.rt_line[i].line_available_point[j] > 0 )
			{
				Draw_simple_line(combox_rt_line.rt_line[i].line_data[j],combox_rt_line.rt_line[i].line_available_point[j],combox_rt_line.rt_line[i].line_source[j],j);
			}
		}
	}
}

/*  draw sample line */
void CTeeChart5_testDlg::Draw_simple_line(double * line_table , unsigned int len,int i,int j)
{
	//for( int t = 0 ; t < 10 ; t ++ )
	//{
	//	/* create line */
	//	CSeries lineS = (CSeries)m_chart.Series(t);

	//	lineS.Clear();
	//}

	CSeries lineSeries1 = (CSeries)m_chart.Series(i);

	lineSeries1.Clear();

    for( unsigned int i = 0 ; i < len ; i++)
    {
		lineSeries1.AddXY((double)i, line_table[i],NULL,NULL);
    }
}

void CTeeChart5_testDlg::MeanCal(float * a , unsigned char b , float *m , float *r )
{
	float mean=0.0f;
	float rms=0.0f;
	unsigned char i;

	mean=0.0f;

	for( i = 0 ; i < b ; i++ )
	{
		mean += a[i];
	}

	mean=mean/((float)b);

	rms = 0.0f;

	for( i = 0 ; i < b ; i++ )
	{
		rms+=(a[i]-mean)*(a[i]-mean);
	}
	/* creater */
	rms = sqrtf( rms ) / ( ( float )b );  // false formula
	/*-------------*/
	*m = mean;   //  均值
	*r = rms;    //  方差
}