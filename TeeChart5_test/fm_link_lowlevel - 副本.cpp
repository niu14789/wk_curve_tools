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
/*------------------*/
unsigned char buffer[4096];
unsigned char step = 0;
unsigned char receive_buf[270];
/*-------------*/
const char * string_chese_table[20][32] = 
{
	{"imu1","imu1","ax:","ay:","az:","gx:","gy:","gz:","tmp:"},
	{"imu2","imu2","ax:","ay:","az:","gx:","gy:","gz:","tmp:"},
	{"imu3","imu3","ax:","ay:","az:","gx:","gy:","gz:","tmp:"},
	{"g_mrs","转速电流","转速1:","电流1:","转速2:","电流2","转速3:","电流3","转速4:","电流4:"},
	{"_baro_data","气压计","气压值:"},
	{"sonar_m","超声波","高度(m):"},
	{"ublox_msg","ublox_GPS","星数：","PDOP:"},
	{"bd930_msg","BD930_GPS","星数：","PDOP:"},
	{"_mag_data","磁力计3","x：","y:","z:"},
	{"_mag_ems1","磁力计1","x：","y:","z:"},
	{"_mag_board","磁力计0","x：","y:","z:"},
    {"version_unit","版本号","GPS板：","底板:","磁力计1:","磁力计2:","超声波:","单云台:","五云台:","电调1：","电调2：","电调3：","电调4：",
	 "飞控:","视觉：","CPLD:","FPGA:","电池版本:"},
	{"battery_msg","智能电池","1#电压：","1#剩余:","2#电压：","2#剩余:","3#电压：","3#剩余:"},
	{"led_status","LED状态","1#:","2#:","3#:","4#:"},
	{"cali_status","标定状态","电机标定:","水不标定：","磁力计标定："},
	{"gimbal_msg","拍照计数","热靴计数：","执行计数:"},
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
	int ret;
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
void CTeeChart5_testDlg::fm_link_send(unsigned char ID , unsigned char *data,unsigned int len_p)
{
	/*-------------------------*/
	unsigned char buffer[300];
	int len;
	/* create package */
	len = fmlink_package_create(buffer,sizeof(buffer),ID,data,len_p);
	/* send */
	send(buffer,len);
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
	/* show */
	c_name.Format(_T("%S"),list->title);
	/* add to combox */
	m_combox_param_show.AddString(c_name);
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
	for( int i = 0 ; i < 16 ; i ++ )
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
						sprintf(chese_list.chese[chese_list.total_line].show_data + strlen(chese_list.chese[chese_list.total_line].show_data),
							"%s%d ",string_chese_table[i][j+2],show_uint8[j]);
					}
					break;
				case 0x82:
					show_int16 = (short *)list->raw_data;
					/* show */
					for( j = 0 ; j < num ; j ++ )
					{
						sprintf(chese_list.chese[chese_list.total_line].show_data + strlen(chese_list.chese[chese_list.total_line].show_data),
							"%s%d ",string_chese_table[i][j+2],show_int16[j]);
					}
					break;
				case 0x02:
					show_uint16 = (unsigned short *)list->raw_data;
					/* show */
					for( j = 0 ; j < num ; j ++ )
					{
						sprintf(chese_list.chese[chese_list.total_line].show_data + strlen(chese_list.chese[chese_list.total_line].show_data),
							"%s%d ",string_chese_table[i][j+2],show_uint16[j]);
					}
					break;
				case 0x84:
					show_int32 = (int *)list->raw_data;
					/* show */
					for( j = 0 ; j < num ; j ++ )
					{
						sprintf(chese_list.chese[chese_list.total_line].show_data + strlen(chese_list.chese[chese_list.total_line].show_data),
							"%s%d ",string_chese_table[i][j+2],show_int32[j]);
					}
					break;
				case 0x04:
					show_uint32 = (unsigned int *)list->raw_data;
					/* show */
					for( j = 0 ; j < num ; j ++ )
					{
						sprintf(chese_list.chese[chese_list.total_line].show_data + strlen(chese_list.chese[chese_list.total_line].show_data),
							"%s%d ",string_chese_table[i][j+2],show_uint32[j]);
					}
					break;
				case 0x05:
					show_float = (float *)list->raw_data;
					/* show */
					for( j = 0 ; j < num ; j ++ )
					{
						sprintf(chese_list.chese[chese_list.total_line].show_data + strlen(chese_list.chese[chese_list.total_line].show_data),
							"%s%5.2f ",string_chese_table[i][j+2],show_float[j]);
					}
					break;
				default :
				break;
			}
			/*---check if---*/
			update_check_result(&chese_list.chese[chese_list.total_line]);
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
int CTeeChart5_testDlg::update_check_result( SYS_LIST_CHESE_DETAIL_DEF * check_ese)
{
	unsigned char *show_uint8,*show_uint8_l,*show_uint8_u;
	char *show_int8,*show_int8_l,*show_int8_u;
	unsigned short *show_uint16,*show_uint16_l,*show_uint16_u;
	short *show_int16, *show_int16_l, *show_int16_u;
	unsigned int *show_uint32,*show_uint32_l,*show_uint32_u;
	int *show_int32,*show_int32_l,*show_int32_u;
	float *show_float,*show_float_l,*show_float_u;
	int i;
	/*---check if---*/
	if( check_ese->check_if == 1 )
	{
		/* defautl is ok */
		sprintf(check_ese->result,"PASS");
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
								sprintf(check_ese->result,"参数%d异常",i);
								break;
							}
						}else
						{
							 if( check_ese->check_type == 1 )
							 {
								 if( !(( show_int8[i] >= show_int8_l[i] ) && ( show_int8[i] <= show_int8_u[i] )) )
								 {
									check_ese->check_ok = 0;
									sprintf(check_ese->result,"参数%d异常",i);
									break;
								 }
							 }else if( check_ese->check_type == 2 )
							 {
								 if( !(( show_int8[i] >= show_int8_l[i] ) && ( show_int8[i] <= show_int8_u[i] ) && show_int8[i] != 0 ) )
								 {
									check_ese->check_ok = 0;
									sprintf(check_ese->result,"参数%d异常",i);
									break;
								 }
							 }
						}
					}
					break
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
								sprintf(check_ese->result,"参数%d异常",i);
								break;
							}
						}else
						{
							 if( check_ese->check_type == 1 )
							 {
								 if( !(( show_uint8[i] >= show_uint8_l[i] ) && ( show_uint8[i] <= show_uint8_u[i] )) )
								 {
									check_ese->check_ok = 0;
									sprintf(check_ese->result,"参数%d异常",i);
									break;
								 }
							 }else if( check_ese->check_type == 2 )
							 {
								 if( !(( show_uint8[i] >= show_uint8_l[i] ) && ( show_uint8[i] <= show_uint8_u[i] ) && show_uint8[i] != 0 ) )
								 {
									check_ese->check_ok = 0;
									sprintf(check_ese->result,"参数%d异常",i);
									break;
								 }
							 }
						}
					}
					break;
				case 0x82:
				case 0x02:
					show_uint16 = (unsigned short *)check_ese->raw_data;
					/* show */
					for( i = 0 ; i < check_ese->num ; i ++ )
					{
						if( show_uint16[i] == 0 )
						{
							check_ese->check_ok = 0;
							sprintf(check_ese->result,"参数%d异常",i);
							break;
						}
					}
					break;
				case 0x84:
				case 0x04:
					show_uint32 = (unsigned int *)check_ese->raw_data;
					/* show */
					for( i = 0 ; i < check_ese->num ; i ++ )
					{
						if(  show_uint32[i] == 0 )
						{
							check_ese->check_ok = 0;
							sprintf(check_ese->result,"参数%d异常",i);
							break;
						}
					}
					break;
				case 0x05:
					show_float = (float *)check_ese->raw_data;
					/* show */
					for( i = 0 ; i < check_ese->num ; i ++ )
					{
						if( show_float[i] == 0 )
						{
							check_ese->check_ok = 0;
							sprintf(check_ese->result,"参数%d异常",i);
							break;
						}
					}
					break;
				default :
				break;
			}
		}else if( check_ese->check_type == 1 || check_ese->check_type == 2 )
		{

		}
	}else
	{
		sprintf(check_ese->result,"----");
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
	sprintf(buffer,"%d",list->index);
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
		update_check_result(&chese_list.chese[i]);
		Check_list_one(&chese_list.chese[i]);
	}
	/* reset semphter */
	Set_sempher(0);
}