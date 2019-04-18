#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__
/* */
#define APP_MAKE_100HZ_EN			1u
/* */
#define APP_MARK2TIMR_COUNT_EN		1u
/* Enable global debug */
#define __DEBUG_GLOBAL_EN__			1u
#define __VERSION_GLOBAL_EN__		1u

#if __DEBUG_GLOBAL_EN__
	#define DEBUG_GLOBAL(...)		printf("#GPS> "__VA_ARGS__)
#else
	#define DEBUG_GLOBAL(...)		
#endif /* __DEBUG_GLOBAL_EN__ */

#if __VERSION_GLOBAL_EN__
	#define VERSION_OUTPUT(...)		printf(__VA_ARGS__)
#else
	#define VERSION_OUTPUT(...)
#endif/* __VERSION_GLOBAL_EN__*/

#pragma pack(push,1)
typedef struct gps_nav_struct_s{
	//unsigned long long system_timestamps;//size 8
	//unsigned int system_pps_num;//size 4
	/* from ID 0x01 */
	unsigned short GPS_week_num;//size 2
	unsigned int GPS_time_ms;//size 4
	unsigned char svn;//size 1
	unsigned char RTK_flags;
	//unsigned char position_flag1;//size 1
	//unsigned char position_flag2;//size 1
	//unsigned char init_num;//size 1
	//unsigned short rev1;//size 2
	/* from ID 0x02 */
	double lat;//size 8
	double lon;//size 8
	double height;//size 8
	/* from ID 0x08 */
	//unsigned char rev_a[7];//size 2
	//unsigned char Velocity_flags;//size 1
	double Speed;//size 4
	double Heading;//size 4
	double Vertical_velocity;//size 4
	//float Local_heading;//size 4
	/* from ID 0x09 */
	float PDOP;//size 4Heading
	//float HDOP;//size 4
	//float VDOP;//size 4
	//float TDOP;//size 4
	//unsigned char rev_b[6];//size 8
	//unsigned char RTK_condition;
	unsigned int unkown;
}gps_nav_def;
#pragma pack(pop)

#endif /*__APP_CONFIG_H__*/