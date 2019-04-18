#ifndef __718D_DECODE_H__
#define __718D_DECODE_H__

/* Tracking status */
#define TRACKING_STATUS_MASK 0x0000001F
/* sv channel number */
#define SV_CHANNEL_NUMBER_MASK 0x000003E0
/* Phase lock flag */
#define PHASE_LOCK_FLAG_MASK 0x00000400
/* Parity known flag */
#define PARITY_KNOWN_FLAG_MASK 0x00000800
/* Code locked flag */
#define CODE_LOCKED_FLAG_MASK 0x00001000
/* Correlator type */
#define CORRELATOR_TYPE_MASK 0x0000E000
/* satellite system */
/*0 = GPS		4 = BeiDou	*/
/*1 = GLONASS	5 = QZSS	*/
/*2 = SBAS		6 = NavIC	*/
/*3 = Galileo	7 = Other	*/
#define SATELLITE_SYSTEM_MASK 0x00070000
/* Grouping */
#define GROUPING_MASK 0x00100000
/* singal type */
#define SIGNAL_TYPE_MASK 0x03E00000
/* Primary L1 channel */
#define PRIMARY_L1_CHANNEL_MASK 0x08000000
/* Carrier phase measurement */
#define CARRIER_PHASE_MEASUR_MASK 0x10000000
/* Digital filtering on signal */
#define DIGITAL_FILTER_MASK 0x20000000
/* PRN lock flag */
#define PRN_LOCK_FLAG_MASK 0x40000000
/* Channel assignment */
#define CHANNEL_ASSIGNMENT_MASK 0x80000000

/* satellite system define */
#define SATELLITE_CHANNAL_L1		0
#define SATELLITE_CHANNAL_L2		1

#define SATELLITE_SYSTEM_GPS		0
#define SATELLITE_SYSTEM_GLONASS	1
#define SATELLITE_SYSTEM_BDS		4

#define MAX_VALUE		8388608	
/* message id type define */
#define MSG_ID_LOG_RANGECMP		140u
#define MSG_ID_LOG_ALMANAC		73u
#define MSG_ID_LOG_BDSALMANAC	1584u
#define MSG_ID_LOG_BDSEPHEMERS	1696u
#define MSG_ID_LOG_GLOALMANAC	718u
#define MSG_ID_LOG_GLOEPHEMERS	723u
#define MSG_ID_LOG_GPSEPHEM		7u
#define MSG_ID_LOG_MARK2TIMR	616u

#define MSG_ID_LOG_BESTPOS		42u
#define MSG_ID_LOG_BESTVEL		99u
#define MSG_ID_LOG_PSRDOP		174u
#define MSG_ID_LOG_HEADING2		1335u

/* define bestpos.pos_type */
#define POS_SOLUTION_NONE						0
#define POS_SOLUTION_SINGLE						16
#define POS_SOLUTION_PSRDIFF					17
#define POS_SOLUTION_L1_INT						48
#define POS_SOLUTION_WIDE_INT					49
#define POS_SOLUTION_NARROW_INT					50
#define POS_SOLUTION_L1_FLOATE					32
#define POS_SOLUTION_NARROW_FLOATE				34
/* position solution type */
typedef enum 
{
	POS_TYPE_NONE,//0 -> no solution
	POS_TYPE_SINGLE,//1 -> single point position
	POS_TYPE_PSRDIFF,//2 -> Pseudorange differential solution
	POS_TYPE_FIXED=4,//4 -> RTK fixed ambiguities solution
	POS_TYPE_FLOATE//5 -> RTK floate ambiguities solution
}OEM718D_POS_TYPE;

typedef struct log_rangecmp_st
{
	unsigned char channel;
	unsigned char *signal_type;
	unsigned char satellite_sys;
	unsigned char sv_ch_num;
}GPS_head_Info_st;

/* Gps Item Define */
typedef struct
{
	unsigned long long raw_psr;
	unsigned int raw_adr;
	int raw_dopper;
	double psr;
	double adr;
	double dopper;
	unsigned char cn;
	unsigned char cn_int;
}GPS_Item_Info_st;

typedef struct satellite_info
{
	GPS_head_Info_st head;
	GPS_Item_Info_st item;
}satellite_info_st;

typedef struct log_range_cmp
{
	unsigned char data[2048];
	unsigned int  len;
	unsigned int index;
}RangeCmp_Log_st;

#pragma pack(push,1)
 typedef struct
{
	unsigned int solStat;
	unsigned int posType;
	float baselineLength;
	float heading;
	float pitch;
	float reserved;
	float headingDeviation;
	float pitchDeviation;
	char roverStnID[4];
	char masterStnID[4];
	unsigned char SVs;
	unsigned char solnSVs;
	unsigned char obs;
	unsigned char multi;
	unsigned char solSource;
	unsigned char extSolStat;
	unsigned char GalileoBeidouSigmask;
	unsigned char GPSGlonassSigmask;
	unsigned int crc;
}log_heading2_t ;

#define MAX_NUM_PRN 0x20
typedef struct
{
	float GDOP;
	float PDOP;
	float HDOP;
	float HTDOP;
	float TDOP;
	float cutoff;
	unsigned int numOfPRN;
	unsigned int PRN[MAX_NUM_PRN];
}log_psrdop_t;

typedef struct
{
	unsigned int solStatus;
	unsigned int velType;
	float latency;
	float age;
	double horizonSpeed;
	double movingDirection;//trk gnd
	double verticalSpeed;
	float reserved;
	unsigned int crc;
}log_bestvel_t;

typedef struct
{
	unsigned int sol_stat;
	unsigned int pos_type;
	double lat;
	double lon;
	double hgt;
	float undulation;
	unsigned int datum_id;
	float lat_deviation;
	float lon_deviation;
	float hgt_deviation;
	char stn_id[4];
	float diff_age;
	float sol_age;
	unsigned char SVs;
	unsigned char solnSVs;
	unsigned char solnL1SVs;
	unsigned char solnMultiSVs;
	unsigned char reserved;
	unsigned char ext_sol_state;
	unsigned char GalileoBeidouSigmask;
	unsigned char GPSGlonassSigmask;
	unsigned int crc;
}log_bestpos_t;
#pragma pack(pop)
/* extern function */
unsigned int Decode_Raw_Data(unsigned char *bf, unsigned int len);
void Make_100HZ_Process(unsigned char *pack, unsigned int len);
int get_log_count(unsigned int type);
void reset_log_parse(void);
static void log_switch_process(unsigned char *data, unsigned int len);
#endif /*__718D_DECODE_H__*/