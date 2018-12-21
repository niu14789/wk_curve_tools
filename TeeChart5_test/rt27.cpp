#include "stdafx.h"
#include "string.h"
#include "rt27.h"
/*------------------------------------*/
unsigned char file_buffer[1024*1024*2];//2mB
static FILE * fp = NULL;
/*-------------------------------*/
static FILE * fp_creater_pde = NULL;
static FILE * fp_creater_evt = NULL;
static FILE * wb = NULL;
/*-------------------------------*/
unsigned short 	EventNumber = 0;
unsigned char 	EventSource = 0, EventPort=0;
double 			EventTime = 0.0;
unsigned short 	Week_number;
/* static */
static unsigned char step = 0;
static unsigned char data_buffer_decode[1024];
/*---------------------*/
RT27_event_def rt27_event;
/*--------enter--------*/
int rt27_decode(char *path_rt,char * tmp_file,unsigned int sizetz)
{
	/*-----------------------*/
	char * file_name = path_rt;
	unsigned int len;
	char path_buffer[512];
	/*-----------------------*/
	fopen_s(&fp,file_name,"rb");
	/*-----------------------*/
    if(fp == NULL)
	{
		printf("open file error %s\r\n",file_name);
		return (-1);
	}
	/*--------get name and -------*/
	if( file_licks(file_name,strlen(file_name),path_buffer,sizeof(path_buffer)) != 0 )
	{
		return (-1);
	}
	/*----------------------------*/
	if( 1 )
	{
		/* create always */
		char create_buffer[512];
		/*---------------*/
		sprintf_s(create_buffer,"%s.ent",path_buffer);
		/*-------------------------*/
		fopen_s(&fp_creater_evt,create_buffer,"wb+");
		/*---------------*/
		if( fp_creater_evt == NULL )
		{
			printf("file create fail %s\r\n",create_buffer);
			/* fail */
			return (-1);
		}
	}
	/*-----------------------*/
	char rt27_name[512];
	/*-----------------------*/
	memset(rt27_name,0,sizeof(rt27_name));
	/*-----------------------*/
	sprintf_s(rt27_name,"%s.tmp",path_rt);
	/*-----------------------------*/
	fopen_s(&wb,rt27_name,"wb+");
	/*------------------------*/
	CString show;
    USES_CONVERSION;
	/*---------------------------------------------*/
	show = A2T(rt27_name);
	/*------------------------*/
	if( wb == NULL )
	{
		if( DeleteFile(show) == 0 )
		{
			printf("file can not be deleted %s \r\n",rt27_name);
			return (-1);
		}
		/* open again */
		fopen_s(&wb,rt27_name,"wb+");
	    /*------------------------*/
	    if( wb == NULL )
	    {
			printf("file can not be created %s \r\n",rt27_name);
			return (-1);
		}
	}
	/*---------------------------------------------*/
	SetFileAttributes(show,FILE_ATTRIBUTE_HIDDEN);
	/*-----------------------*/
	/* reset somethings */
	memset(&rt27_event,0,sizeof(rt27_event));
    /* nav end */ 
	while(1)
	{
		/* read */
		len = fread(file_buffer,1,sizeof(file_buffer),fp);		
        /* decode the gps raw and nav data  */   
		raw_gps_parse(file_buffer,len);
		/*--------------------------*/
		if(len < sizeof(file_buffer))
		{
			/*---------------------*/
			if( rt27_event.num == 0 )
			{
				printf("This .RT27 file doesnt have mark point\r\n");
				fclose(fp);
				fclose(wb);
				/*-----------------------*/
				if( fp_creater_evt!= NULL )
				{			
					fclose(fp_creater_evt);
				}
				/* return error */
				return (-2);
			}
			/*---------------------*/
			int tmp = rt27_event.event_per[rt27_event.num-1].index - rt27_event.event_per[0].index + 1;
			/*----------------------------*/
			if( rt27_event.num != tmp )
			{
				printf("lose some point %d\r\n",tmp - rt27_event.num );
				/*----------------------*/
				fclose(fp);
				fclose(wb);
				/*-----------------------*/
				if( fp_creater_evt!= NULL )
				{			
					fclose(fp_creater_evt);
				}
				/* return error */
				return (-3);
				/*----------------------*/
			}
			/*--------------------------*/
			printf("ok %d\r\n",tmp);
			/*--------------------------*/
			if( fp_creater_evt!= NULL )
			{
				fclose(fp_creater_evt);		
			}
			fclose(fp);
			fclose(wb);
			/*------------------------------*/
			strcpy_s(tmp_file,sizetz,rt27_name);
			/*------------------------------*/
			break;
		}
	}
	/*------------------------------*/
	return 0;
}
/*-------------------------------------------------------*/
void raw_gps_parse(unsigned char *buffer,unsigned int len)
{
   	static unsigned int i;
	for(i=0;i<len;i++)
	{
       raw_gps_decode(buffer[i]);
	}
}
/*-------------------------------------------------------*/
void raw_gps_decode(unsigned char d)
{
	static unsigned char step;
	static unsigned char len,data_len = 0;
	static unsigned char status = 0;
	static unsigned char type=0;
	static unsigned int cnt = 0;

	unsigned char sum = 0;

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
			if( d == 0x28 || d == 0x68 || d == 0xE8 || 0xA8 )
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
			if(cnt >= len + (unsigned)2 )
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
				   gps_raw_detail(type,data_buffer_decode,len);
			   }else
			   {
				//   gps_raw_detail(type,data_buffer_decode,len);
                   step = 0;
			   }
			   step = 0;
			   status = 0;
			}
			break;
	}
}
/*--------------------------------------------------------------------------*/
void gps_raw_detail( unsigned char type,unsigned char *data,unsigned char len )
{
	static unsigned char svn = 0;
	static double lat,lon,height;
	unsigned char len_package = 0;
//	unsigned char len_package_single;
//	unsigned char *nav_data;
	static char buffer[2048];
	static char buffer_tmp[8];
	static unsigned int len_write;
	static unsigned int sizeof_test;
    
	static unsigned char buffer_page[20*256];//this buffer max supplys 20 pages.
	static unsigned int buffer_len_page = 0;

	static unsigned char total_page = 0;
	static unsigned char current_page = 0;

	static unsigned char step = 0;

	static unsigned char page_cnt = 0;

	char event_buffer[200];

  switch(type)
  {
	  case 0x57:
        
		  switch(data[0])//raw survey data
		  {
		  case 0:
			  svn = data[21]; 
			  break;
		  case 1: 
			  break;
		  case 2:
				EventSource = data[4];
				EventPort = data[5];
				big2little(data + 6, (char *)&EventNumber, 2);
				big2little(data + 8, (char *)&EventTime, 8);
				/*------------------------------------*/
				if( fp_creater_evt!= NULL )
				{
					memset(event_buffer,0,sizeof(event_buffer));
					sprintf_s(event_buffer,"%d %lf\r\n",EventNumber,EventTime*1000);
					/*------------------------------------*/
					fwrite(event_buffer,1,strlen(event_buffer),fp_creater_evt);
					/*-------------*/
					printf(event_buffer);
					/* write it to the buffer */
					rt27_event.event_per[rt27_event.num].event_time_s = EventTime;
					rt27_event.event_per[rt27_event.num].index = EventNumber;
					/*--------------------------------------------------*/
					rt27_event.num++;
					/*--------------------------------------------------*/
					if( rt27_event.num >= 5000 )
					{
						printf("index over load\r\n");
						/*------------------*/
						rt27_event.num = 0;
					}
				}
				/*------------------------------------*/
				fwrite(&EventNumber,sizeof(EventNumber),1,wb);
				fwrite(&EventTime,sizeof(EventTime),1,wb);
				/*------------------------------------*/
			  break;
		  case 6:

			  //page_buffer[page_buffer_cnt++] = data[1];

			  if( (( data[1] & 0xf0 ) >> 4) == ( data[1] & 0x0f ) && ( data[1] & 0x0f ) == 0x1 ) // this command only has one page 
			  {
                 real_time_gnss_survey_data(data,len);
			  }else
			  { 
				  switch(step)
				  {
				  case 0:
					 if( (( data[1] & 0xf0 ) >> 4 ) == 0x1 ) //first page
					 {
						 buffer_len_page = len;
						 memcpy(buffer_page,data,buffer_len_page);

						 total_page = data[1] & 0xf;//total such as 2 3 6 8

						 current_page = ( data[1] & 0xf0 ) >> 4;//1
						 page_cnt = 0;
						 step = 1;
					 }
					 break;
				  case 1:
					 if( (( data[1] & 0xf0 ) >> 4 ) == ( data[1] & 0x0f ) )
					 {
						/* ok */
						 if( total_page == ( data[1] & 0x0f ) && total_page == (page_cnt + 2) )
						 { 
							 memcpy(buffer_page + buffer_len_page ,data + 4 ,len - 4 );
							 buffer_len_page += len - 4;
							 real_time_gnss_survey_data(buffer_page,buffer_len_page);
							 buffer_len_page = 0;
						 }
						 page_cnt = 0;
						 step = 0;
					 }else if( (( data[1] & 0xf0 ) >> 4) < ( data[1] & 0x0f )  )
					 {
						 if( total_page ==  ( data[1] & 0x0f ) && (current_page + 1) == (( data[1] & 0xf0 ) >> 4) )
						 {
							/* normal page */
							 memcpy(buffer_page + buffer_len_page ,data + 4 ,len - 4 );
							 buffer_len_page += len - 4;
							 current_page = ( data[1] & 0xf0 ) >> 4;//1
							 page_cnt++;

						 }else
						 {
							 step = 0;
							 buffer_len_page = 0;
							 page_cnt = 0;
						 } 
					 }else
					 {
					   /* bad data */
						printf("this is a bad data\n");
						step = 0;
						buffer_len_page = 0;
						page_cnt = 0; 
					 }
					 break;
				  default :
					  break;
				  }
			  }
			  
			  break;
		  default:
			  break;
		  }
		  break;
	  case 0x55:

          switch(data[0])
		  {
			  case 0:
				 // printf("1");
				  break;
			  case 1:
#if 0
				  gps_ephemeris_data.system_timestamps = timestamps;
				  gps_ephemeris_data.system_pps_num = pps_num; 

				  gps_ephemeris_data.PRN = data[1];
 
				  big2little(data+2,(char *)&gps_ephemeris_data.ephemeris_week,2); //2
				  big2little(data+4,(char *)&gps_ephemeris_data.IODC,2); //2

				  gps_ephemeris_data.IODE = data[7];

				  big2little(data + 8,(char *)&gps_ephemeris_data.TOW,4);   // 4
				  big2little(data + 12,(char *)&gps_ephemeris_data.TOC,4);  // 4
				  big2little(data + 16 ,(char *)&gps_ephemeris_data.TOE,4); // 4

				  big2little(data + 20 ,(char *)&gps_ephemeris_data.TGD, 8 ); // 8 
				  big2little(data + 28 ,(char *)&gps_ephemeris_data.AF2, 8 ); // 8
                  big2little(data + 36 ,(char *)&gps_ephemeris_data.AF1, 8 ); // 8
				  big2little(data + 44 ,(char *)&gps_ephemeris_data.AF0, 8 ); // 8
				  big2little(data + 52 ,(char *)&gps_ephemeris_data.CRS, 8 ); // 8
				  big2little(data + 60 ,(char *)&gps_ephemeris_data.DELTAN, 8 ); // 8
				  big2little(data + 68 ,(char *)&gps_ephemeris_data.MSUB0, 8 ); // 8
				  big2little(data + 76 ,(char *)&gps_ephemeris_data.CUC, 8 ); // 8
				  big2little(data + 84 ,(char *)&gps_ephemeris_data.ECCENTRICITY, 8 ); // 8
				  big2little(data + 92 ,(char *)&gps_ephemeris_data.CUS, 8 ); // 8
				  big2little(data + 100 ,(char *)&gps_ephemeris_data.SQRTA, 8 ); // 8
				  big2little(data + 108 ,(char *)&gps_ephemeris_data.CIC, 8 ); // 8
				  big2little(data + 116 ,(char *)&gps_ephemeris_data.OMEGASUB0, 8 ); // 8
				  big2little(data + 124 ,(char *)&gps_ephemeris_data.CIS, 8 ); // 8
				  big2little(data + 132 ,(char *)&gps_ephemeris_data.ISUB0, 8 ); // 8
				  big2little(data + 140 ,(char *)&gps_ephemeris_data.CRC, 8 ); // 8
				  big2little(data + 148 ,(char *)&gps_ephemeris_data.OMEGA, 8 ); // 8
				  big2little(data + 156 ,(char *)&gps_ephemeris_data.OMEGADOT, 8 ); // 8
                  big2little(data + 164 ,(char *)&gps_ephemeris_data.IDOT, 8 ); // 8

				  big2little(data + 172 ,(char *)&gps_ephemeris_data.FLAGS,4); // 4

                  len_gps_ephemeris = sizeof(gps_ephemeris_data);

				  fwrite(&gps_ephemeris_data,1,len_gps_ephemeris,fp_creater_ephemeris_gps);
#endif
				  break;
			  case 2:
#if 0
				  gps_Almanac_data.system_timestamps = timestamps;
				  gps_Almanac_data.system_pps_num = pps_num;

				  gps_Almanac_data.PRN = data[1];

				  big2little(data + 2 ,(char *)&gps_Almanac_data.ALM_DECODE_TIME, 4 ); // 4
				  big2little(data + 6 ,(char *)&gps_Almanac_data.AWN, 2 ); // 2
				  big2little(data + 8 ,(char *)&gps_Almanac_data.TOA, 4 ); // 4
				  /* double */
				  big2little(data + 12 ,(char *)&gps_Almanac_data.SQRTA, 8 ); // 8
				  big2little(data + 20 ,(char *)&gps_Almanac_data.ECCENT, 8 ); // 8
				  big2little(data + 28 ,(char *)&gps_Almanac_data.ISUBO, 8 ); // 8
				  big2little(data + 36 ,(char *)&gps_Almanac_data.OMEGADOT, 8 ); // 8
				  big2little(data + 44 ,(char *)&gps_Almanac_data.OMEGSUBO, 8 ); // 8
				  big2little(data + 52 ,(char *)&gps_Almanac_data.OMEGA, 8 ); // 8
				  big2little(data + 60 ,(char *)&gps_Almanac_data.MSUBO, 8 ); // 8

				  gps_Almanac_data.ALM_HEALTH = data[68];//1

				  len_gps_almance = sizeof(gps_Almanac_data);

				  fwrite(&gps_Almanac_data,1,len_gps_almance,fp_creater_almance_gps);
#endif
				  break;
#if 0
			  case 3:
				  printf("1");
				  break;
			  case 4:
				  printf("1");
				  break;
			  case 5:
				  printf("1"); 
				  break;
			  case 7:
				  printf("1");
				  break;
			  case 8:
				  glonass_almance_data.system_timestamps = timestamps;
				  glonass_almance_data.system_pps_num = pps_num;

				  glonass_almance_data.PRN = data[1];

				  big2little(data + 2 ,(char *)&glonass_almance_data.DAY_NUMBER, 2 ); // 2

				  glonass_almance_data.FDMA_NUMBER = data[4];

				  for(int i = 0 ; i < 8 ; i ++ )
				  {
					  big2little(data + 5 + i * 8 ,(char *)&glonass_almance_data.ECCENTRICITY + i * 8 , 8 ); // 8
				  }

				  glonass_almance_data.HEALTH = data[69]; 
				  /* fp_creater_almance_glonass */

				  len_glonass_almance = sizeof(glonass_almance_data);

                  fwrite(&glonass_almance_data,1,len_glonass_almance,fp_creater_almance_glonass); 
#endif
				  break; 
			  case 9:
#if 0
				  glonass_ephemeris_data.system_timestamps = timestamps;
				  glonass_ephemeris_data.system_pps_num = pps_num;

				  glonass_ephemeris_data.PRN = data[1];

				  big2little(data + 2 ,(char *)&glonass_ephemeris_data.GPS_WEEK_EPH_VALID_REF_TIME, 2 ); // 2
				  big2little(data + 4 ,(char *)&glonass_ephemeris_data.GPS_TIME_EPH_VALID_REF_TIME, 4 ); // 4
				  big2little(data + 8 ,(char *)&glonass_ephemeris_data.GPS_WEEK_EPH_DECODE_REF_TIME, 2 ); // 2
				  big2little(data + 10 ,(char *)&glonass_ephemeris_data.GPS_TIME_EPH_DECODE_REF_TIME, 4 ); // 4
				  big2little(data + 14 ,(char *)&glonass_ephemeris_data.GLONASS_DAY_NUMBER, 2 ); // 2

				  glonass_ephemeris_data.REF_TIME_OF_EPHEMERIS = data[16];//1
				  glonass_ephemeris_data.LEAP_SECONDS = data[17];//1
				  glonass_ephemeris_data.FLAGS = data[18];//1

				  big2little(data + 19 ,(char *)&glonass_ephemeris_data.FRAME_START_TIME, 4 ); // 2

				  glonass_ephemeris_data.ARG_OF_DATA = data[23];//1
				  glonass_ephemeris_data.EPHEMERIS_SOURCE = data[24];//1
				  glonass_ephemeris_data.FDMA = data[25];//1
				  glonass_ephemeris_data.HEALTH = data[26];//1
				  glonass_ephemeris_data.GENERATION = data[27];
				  glonass_ephemeris_data.UDRE = data[28];

				  for(int i = 0 ; i < 14 ; i ++ )
				  {
					  big2little(data + 29 + i * 8 ,(char *)&glonass_ephemeris_data.X + i * 8 , 8 ); // 2
				  }

				  len_glonass_ephemeris = sizeof(glonass_ephemeris_data);

				  fwrite(&glonass_ephemeris_data,1,len_glonass_ephemeris,fp_creater_ephemeris_glonass);
#endif
				  break;
	/*		  case 11:
				  printf("1");
				  break;
			  case 12:
				  printf("1");
				  break;
			  case 14:
				  printf("1");
				  break;
			  case 16:
				  printf("1");
				  break;
			  case 20:
				  printf("1");
				  break;
			  case 21:
				  printf("1");
				  break;
   		      case 22:
				  printf("1");
				  break;*/
			  default:
				 // printf("1");
				  break;
		  }

		  break;
	  case 0x40:
#if 0
           nav_data = &data[3];

           len_package = nav_data[1];//cmd len
           len_package_single = len_package;

		   sizeof_test = sizeof(gps_nav);

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
				  default:
					   break;
			   }
               if( com2_new_version )
			   {
				   if( len_package == 0x7f )
				   {
					   break;
				   }
			   }else
			   {
				   if( len_package == 0x3f )
				   {
					   break;
				   }
			   }
               nav_data = &nav_data[ len_package_single + 2 ];
               len_package += nav_data[1];//cmd len
			   len_package_single = nav_data[1];//cmd len
  		   }
   //    
		   gps_nav.system_pps_num = pps_num;
		   gps_nav.system_timestamps = timestamps;

           fwrite(&gps_nav,1,sizeof(gps_nav) ,fp_creater_nav );

		   if(nav_test)
			{
				//static unsigned long long last_times = 0;
				//static unsigned int reduce = 0;
				 //reduce = gps_nav.system_timestamps - last_times;
				 sprintf_s(buffer,"st : %d \r\n", gps_nav.system_timestamps );//PDOP;//size 4Heading
				 // last_times = gps_nav.system_timestamps;
				  // gps_nav.system_timestamps,
				  // gps_nav.system_pps_num,
				  // gps_nav.GPS_time_ms,
				  // gps_nav.GPS_week_num,
				  // gps_nav.svn,
				  // gps_nav.lat,
				  // gps_nav.lon,
				  // gps_nav.height,
				  // gps_nav.Velocity_flags,
				  // gps_nav.Speed,
				  // gps_nav.Heading,
				  // gps_nav.Vertical_velocity,
				  // gps_nav.PDOP);

			   len_write = strlen(buffer);

			   fwrite(buffer,1,len_write ,fp_creater_txt );
			}
#endif
		  break;
	  default:break;
  }
}

void big2little(unsigned char * src,char * dst,unsigned char len)
{
	for(int i=0;i<len;i++)
	{
		dst[i] = src[ len - 1 - i ];
	}
}
/*----------------------------------------------------------------------------------------------*/
void real_time_gnss_survey_data(unsigned char * data/*beggin from the length */,unsigned int len)//subtype 6
{
   unsigned char record_type = 0;
   unsigned char page_number = 0;
   unsigned char reply_number = 0;
   unsigned char record_interpretation_flags = 0;
   unsigned char block_length = 0;
   unsigned short week_number;
   unsigned int receiver_time;
   unsigned char number_SVs;
   unsigned char number_of_sv;
   unsigned char satellite_id[2];//gps or glosnass , whitch number
   unsigned char EPOCH;
   unsigned int PSEUDORANGE;//
   static double pseu;
   unsigned int doppler_int = 0;
   static float doppler_f;
   
   unsigned char block_type;
   unsigned char track_type;
   unsigned short SNR;
   long long phase;
   unsigned char cycle_slip_count;
   unsigned char measurement_flags;
   unsigned char measurement_flags2;
   unsigned char measurement_flags3;
   unsigned char * rt_p = NULL;
//   char buffer_txt[256];
   unsigned int length_of_total = 0;

   record_type = data[0];//4

   if( record_type == 0x06 )
   {
	   page_number = data[1];//5
	   reply_number = data[2];//6
	   record_interpretation_flags = data[3];//7

	   rt_p = &data[4];
	   block_length = rt_p[0];

       length_of_total += block_length;

	   big2little( rt_p + 1 , (char *)&week_number ,  2);
	   big2little( rt_p + 3 , (char *)&receiver_time ,4);

	   gps_psdu_doppler.GPS_receive_ms = receiver_time;
	   gps_psdu_doppler.week_number = week_number;

	   number_SVs = rt_p[10];
	   EPOCH = rt_p[11];
       /* ------------------------------------ reply ------------------------------------- */
   
	   rt_p += block_length;//0b
	   block_length = rt_p[0];
	   length_of_total += block_length;
	   while( length_of_total < len - 4 )
	   {
		   /* *************************satellite message id oc*************************** */
		   rt_p += block_length;
		   block_length = rt_p[0];
           
		   length_of_total += block_length;

		   satellite_id[0] = rt_p[1];//num
		   satellite_id[1] = rt_p[2];//gps
	       
		   gps_psdu_doppler.num = satellite_id[0];
		   gps_psdu_doppler.id  = satellite_id[1];

		   number_of_sv =  rt_p[4];

		   if(number_of_sv > 2)
		   {
			   return;
		   }

		   for(int i = 0 ; i < number_of_sv ; i++ )
		   {
	/* decode PSEUDORANGE */
			   rt_p += block_length;
			   block_length = rt_p[0];

			   if(rt_p[0] == 0)
			   {
				   return;
			   }

			   length_of_total += block_length;

			   if( i == 0 )
			   {
				   phase = 0;
				   block_type = rt_p[1];
				   track_type = rt_p[2];
				   big2little( rt_p + 3 , (char *)&SNR , 2);
				   big2little( rt_p + 5 , (char *)&PSEUDORANGE  ,4);
				   big2little( rt_p + 9 , (char *)&phase  ,6);

                   if( phase & 0x800000000000 )
				   {
                      phase = 0xffffffffffff - phase + 1;

					  gps_psdu_doppler.phase = ((double)phase / 32768.0f) * (-1);
				   }else
				   {
                      gps_psdu_doppler.phase = ((double)phase / 32768.0f);
				   }

				   gps_psdu_doppler.SNR = (double)SNR / 10.0f;

				   cycle_slip_count = rt_p[15];
				   measurement_flags = rt_p[16];

				   pseu = (double)PSEUDORANGE / 128;

				   gps_psdu_doppler.PSEUDORANGE = pseu;

				   if( measurement_flags & 0x80 )
				   {
					   /* measurement flags2 */
					   measurement_flags2 = rt_p[17];

					   if( measurement_flags2 & 0x80 )
					   {
                           measurement_flags3 = rt_p[18]; 
						   if( measurement_flags & 0x04 )
						   {
							   big2little( rt_p + 19 , (char *)&doppler_int ,  3);

							   if( doppler_int & 0x800000 )
							   {
									doppler_int = 0xffffff - doppler_int + 1;
									doppler_f = ((float)doppler_int / 256) * (-1);
							   }else
							   {
									doppler_f = (float)doppler_int / 256;
							   }
							   
							   gps_psdu_doppler.doppler = doppler_f;
						   }
					   }else
					   {
						   if( measurement_flags & 0x04 )
						   {
							   big2little( rt_p + 18 , (char *)&doppler_int ,  3);

							   if( doppler_int & 0x800000 )
							   {
									doppler_int = 0xffffff - doppler_int + 1;
									doppler_f = ((float)doppler_int / 256) * (-1);
							   }else
							   {
									doppler_f = (float)doppler_int / 256;
							   }
							   
							   gps_psdu_doppler.doppler = doppler_f;
						   }
					   }
				   }else
				   {  
					   if( measurement_flags & 0x04 )
					   {
						   big2little( rt_p + 17 , (char *)&doppler_int ,  3);
                    
						   if( doppler_int & 0x800000 )
						   {
								doppler_int = 0xffffff - doppler_int + 1;
								doppler_f = ((float)doppler_int / 256) * (-1);
						   }else
						   {
                                doppler_f = (float)doppler_int / 256;
						   }
						   
						   gps_psdu_doppler.doppler = doppler_f;
					   }
				   }
			   }else
			   {

			   }
		   }
		   /* printf and save data fp_creater_pde */
		   gps_psdu_doppler.system_timestamps = 0;

		   gps_psdu_doppler.system_pps_num = 0;

		   if( gps_psdu_doppler.system_pps_num > 300 )
		   {
			   gps_psdu_doppler.system_pps_num = 300;
		   }
		   /*--------------------------------------------------------------*/
  //         fwrite(&gps_psdu_doppler,1,sizeof(gps_psdu_doppler),fp_creater_pde);
		   /* --sdf-s-dsf-s-dfs-df-sd-fs-fds-f-sdf*/
#if 0
		    if(raw_test)
			{
			   int len_str;
			   sprintf_s(buffer_txt,"pps_num:%d gps_rec_time:%d num:%d id:%d gps_week:%d ",//pseud:%10.3f doppler:%10.3f\r\n",
				   //gps_psdu_doppler.system_timestamps,
				   gps_psdu_doppler.system_pps_num,
				   gps_psdu_doppler.GPS_receive_ms,
				   gps_psdu_doppler.num,
				   gps_psdu_doppler.id,
				   gps_psdu_doppler.week_number);
	/*			   gps_psdu_doppler.PSEUDORANGE,
				   gps_psdu_doppler.doppler*/

				   len_str = strlen(buffer_txt);

				   sprintf_s(buffer_txt + len_str,"st:%d ",gps_psdu_doppler.system_timestamps);

				   len_str = strlen(buffer_txt);

				   sprintf_s(buffer_txt + len_str,"pseud:%10.3f ",gps_psdu_doppler.PSEUDORANGE);

				   len_str = strlen(buffer_txt);

				   sprintf_s(buffer_txt + len_str,"doppler:%10.3f",gps_psdu_doppler.doppler);

				   len_str = strlen(buffer_txt);

				   sprintf_s(buffer_txt + len_str,"phase:%10.3f",gps_psdu_doppler.phase);

				   len_str = strlen(buffer_txt);

				   sprintf_s(buffer_txt + len_str,"snr:%10.3f\r\n",gps_psdu_doppler.SNR);

				   len_str = strlen(buffer_txt);

				   fwrite(&buffer_txt,1,len_str,fp_creater_pde_txt);
			}
#endif
		   /*-----------------------------------------*/
	   }
	   /* ------------------------------------ reply ------------------------------------- */

   }
}
/*----------------------*/
int file_licks(char * path,unsigned int len,char *lack,unsigned int len2)
{
	unsigned int point = 0;
	/*----------------------------*/
	memset(lack,0,len2);
	/*----------------------------*/
	for( unsigned int i = 0 ; i < len ; i ++ )
	{
		if( path[len - i - 1] == '.' )
		{
			point = i;
			break;
		}
	}
	/*-----------------------------*/
	if( point != 0 )
	{
		if( len2 > len - point )
		{
			memcpy(lack,path,len - point-1);
			/*--------------------------*/
			return 0;
			/*--------------------------*/
		}else
		{
			printf("len error\r\n");
			/*--------------------------*/
			return (-1);
			/*--------------------------*/
		}
	}else
	{
		if( len2 >= len )
		{
			memcpy(lack,path,len);
			/*--------------------------*/
			return 0;
			/*--------------------------*/
		}else
		{
			printf("len error\r\n");
			/*--------------------------*/
			return (-1);
			/*--------------------------*/
		}
	}
	/*-----------------------------*/
}



