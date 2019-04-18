#ifndef __718D_ALGO_H__
#define __718D_ALGO_H__
#include "decode.h"
/* Calc Gps Item */
#define DIFF_ITEM_FACTORY(n, p, f)	(((n)-(p))/(f))
#define DIFF_ITEM_RAW_FACTORY(n, p, f)	((int)((n)-(p))/(f))
/* Get Calc Gps Item */
#define GET_ITEM_RESULT(p, d)		((p) + (d))

/* extern function */
int Algo_100HZ_Process(satellite_info_st *satellite_item_info, unsigned char factory);

#endif /*__718D_ALGO_H__*/