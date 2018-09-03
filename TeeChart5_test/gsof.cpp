#include "stdafx.h"
#include "TeeChart5_test.h"
#include "TeeChart5_testDlg.h"
#include "afxdialogex.h"
#include "CSeries.h"
#include "tchart1.h"

typedef struct gps_nav_struct_s{
	unsigned long long system_timestamps;//size 8
	unsigned int system_pps_num;//size 4
	/* from ID 0x01 */
	unsigned int GPS_time_ms;//size 4
	unsigned short GPS_week_num;//size 2
	unsigned char svn;//size 1
	unsigned char position_flag1;//size 1
	unsigned char position_flag2;//size 1
	unsigned char init_num;//size 1
	unsigned short rev1;//size 2
	/* from ID 0x02 */
	double lat;//size 8
	double lon;//size 8
	double height;//size 8
	/* from ID 0x08 */
	unsigned char rev_a[7];//size 2
	unsigned char Velocity_flags;//size 1
	float Speed;//size 4
	float Heading;//size 4
	float Vertical_velocity;//size 4
	float Local_heading;//size 4
	/* from ID 0x09 */
	float PDOP;//size 4Heading
	float HDOP;//size 4
	float VDOP;//size 4
	float TDOP;//size 4
	unsigned char rev_b[6];//size 8
    unsigned char RTK_flags;
	unsigned char RTK_condition;
}gps_nav_def;

static unsigned char data_buffer_decode[1024];

static gps_nav_def  gps_nav;
static gps_tes_def  gsof_msg;
static unsigned int cnt_show = 0;

void CTeeChart5_testDlg::raw_gps_decode(gsof_show_def * show , unsigned char d)
{
	static unsigned char step;
	static unsigned char len = 0,data_len = 0;
	static unsigned char type=0;
	static unsigned char status;
	static unsigned int cnt = 0;
	static double last_gps_time  = 0;
	unsigned char sum = 0;
	/* total */
	gsof_msg.total_size++;
	/*-----------------*/
    switch(step)
	{
		case 0:
			if( d == 0x02 )
			{
				step = 1;
			}else
			{
				step = 0;
			}
			break;
		case 1:
			if( d == 0x28 || d == 0x68 || d == 0xE8 )
			{
				step = 2;
				status = d;
			}else
			{
				step = 0;
			}
			break;
		case 2:
			if( d == 0x55 || d == 0x57 ||  d == 0x40 )
			{
				step = 3;
			}else
			{
				step = 0;
			}
            type = d;
			break;
        case 3:
			len = d;
			step = 4;
			cnt = 0;
			break;
		case 4:
            data_buffer_decode[cnt] = d;
			cnt++;
			if(cnt >= (unsigned int)(len + 2) )
			{ 
               for(int i=0;i<len;i++)
			   {
                  sum += data_buffer_decode[i];
			   }
			   sum += status;
               sum += type;
			   sum += len;

               if(sum == data_buffer_decode[len] && 0x3 == data_buffer_decode[len+1])
			   {
				   /* check ok */
				   gps_raw_detail(show,type,data_buffer_decode,len);
				   step = 0;
			   }else
			   {
                   step = 0;
			   }
			   step = 0;
			}
			break;
	}
}

void CTeeChart5_testDlg::gps_raw_detail( gsof_show_def * show,unsigned char type,unsigned char *data,unsigned char len )
{
	static double lat,lon,height;
	unsigned char len_package = 0;
	unsigned char len_package_single;
	unsigned char *nav_data;
	unsigned char num;
	static char buffer[2048];
	static double last_gps_time = 0;
  switch(type)
  {
	  case 0x40:
		  /* transfer */
           nav_data = &data[3];
		   num = data[0];

           len_package = nav_data[1];//cmd len

           len_package_single = len_package;

		   while( 1 )
		   {
			   switch(*nav_data)
			   {
				  case 0x01:
                        big2little(nav_data+2,(char *)&gps_nav.GPS_time_ms,sizeof(gps_nav.GPS_time_ms)); //4
                        big2little(nav_data+6,(char *)&gps_nav.GPS_week_num,sizeof(gps_nav.GPS_week_num));//2
						gps_nav.svn = nav_data[8];//1
						gps_nav.position_flag1 = nav_data[9];//1
						gps_nav.position_flag2 = nav_data[10];//1
						gps_nav.init_num = nav_data[11];//1
						/*-----------*/
					  break;
				  case 0x02:
					    big2little(nav_data+2,(char *)&gps_nav.lat,8);
						big2little(nav_data+10,(char *)&gps_nav.lon,8);
						big2little(nav_data+18,(char *)&gps_nav.height,8);		                        
					  break;
				  case 0x08:
					  gps_nav.Velocity_flags = nav_data[2];//1
					  big2little(nav_data+3,(char *)&gps_nav.Speed,sizeof(gps_nav.Speed));//4
					  big2little(nav_data+7,(char *)&gps_nav.Heading,sizeof(gps_nav.Heading));//4
					  big2little(nav_data+11,(char *)&gps_nav.Vertical_velocity,sizeof(gps_nav.Vertical_velocity));//4
					  big2little(nav_data+15,(char *)&gps_nav.Local_heading,sizeof(gps_nav.Local_heading));//4
					  break;
				  case 0x09:
					  big2little(nav_data+2,(char *)&gps_nav.PDOP,sizeof(gps_nav.PDOP));//4 
					  big2little(nav_data+6,(char *)&gps_nav.HDOP,sizeof(gps_nav.HDOP));//4
                      big2little(nav_data+10,(char *)&gps_nav.VDOP,sizeof(gps_nav.VDOP));//4
					  big2little(nav_data+14,(char *)&gps_nav.TDOP,sizeof(gps_nav.TDOP));//4
					  break;
				  case 0x0C://position sigma
					  break;
				  case 0x26:
					  gps_nav.RTK_flags = nav_data[6];
					  gps_nav.RTK_condition = nav_data[7];
					  break;
				  default:
					   break;
			   }
            
				if( len_package == 0x7f )
				{
					show[cnt_show].time = (double)gps_nav.GPS_time_ms;
					show[cnt_show].vgx = (double)gps_nav.Speed;
					
					if( last_gps_time != 0 )
					{
						show[cnt_show].time_sigma = (double)gps_nav.GPS_time_ms - last_gps_time;
					}
					last_gps_time = (double)gps_nav.GPS_time_ms;

					cnt_show ++;
					gsof_msg.ok_package++;
					break;
				}
			    /*---------------------*/
                nav_data = &nav_data[ len_package_single + 2 ];
                len_package += nav_data[1];//cmd len
			    len_package_single = nav_data[1];//cmd len
  		   }

		   //memset(buffer,0,sizeof(buffer));

		   //sprintf_s(buffer,"%-4d%12d    %3.7f    %3.7f    %3.3f    %3d    %3d\r\n",
		   //gps_nav.GPS_week_num,gps_nav.GPS_time_ms,(float)gps_nav.lat*57.3f,(float)gps_nav.lon*57.3f,(float)gps_nav.height,gps_nav.RTK_condition,gps_nav.RTK_flags);

		   //fwrite(buffer,1,strlen(buffer) ,fp );
		   break;
      }
}
void CTeeChart5_testDlg::big2little(unsigned char * src,char * dst,unsigned char len)
{
	for(int i=0;i<len;i++)
	{
		dst[i] = src[ len - 1 - i ];
	}
}

void CTeeChart5_testDlg::clear_cnt(void)
{
	cnt_show = 0;
	memset(&gsof_msg,0,sizeof(gsof_msg));
}
//gps_tes_def  gsof_msg
gps_tes_def * CTeeChart5_testDlg::gsof_transfer( gsof_show_def * show , unsigned char * buffer , unsigned int len )
{

	for( unsigned int i = 0 ; i < len ; i ++ )
	{
		raw_gps_decode(show,buffer[i]);
	}

	gsof_msg.none_data = gsof_msg.total_size - gsof_msg.ok_package*148;

	gsof_msg.per = (float)(gsof_msg.ok_package*148) / (float)gsof_msg.total_size;

	return &gsof_msg;
}