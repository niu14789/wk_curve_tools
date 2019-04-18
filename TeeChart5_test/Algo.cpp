#include "stdafx.h"
#include "Algo.h"

int Algo_100HZ_Process(satellite_info_st *satellite_item_info, unsigned char factory)
{
	double diff_psr = 0,diff_adr = 0,diff_dopper = 0;
	int diff_raw_psr = 0,diff_raw_adr = 0,diff_raw_dopper = 0;
	
	GPS_Item_Info_st *gps_item_head = &satellite_item_info[0].item;
	GPS_Item_Info_st *gps_item_tail = &satellite_item_info[factory].item;
	GPS_Item_Info_st *gps_item_info, *gps_item_info_pre;

	diff_psr = (gps_item_tail->psr - gps_item_head->psr) / factory;
	/* get diff psr */
	diff_psr = DIFF_ITEM_FACTORY( gps_item_tail->psr, gps_item_head->psr, factory);
	/* get diff adr */
	diff_adr = DIFF_ITEM_FACTORY( gps_item_tail->adr, gps_item_head->adr, factory);
	/* get diff dopper */
	diff_dopper = DIFF_ITEM_FACTORY( gps_item_tail->dopper, gps_item_head->dopper, factory);
	/*raw diff psr*/
	diff_raw_psr = DIFF_ITEM_RAW_FACTORY( gps_item_tail->raw_psr, gps_item_head->raw_psr, factory);
	/*raw diff adr*/
	diff_raw_adr = DIFF_ITEM_RAW_FACTORY(gps_item_tail->raw_adr, gps_item_head->raw_adr, factory);
	/*raw diff dopper*/
	diff_raw_dopper = DIFF_ITEM_RAW_FACTORY(gps_item_tail->raw_dopper, gps_item_head->raw_dopper, factory);

	gps_item_info_pre = gps_item_head;
	for(unsigned char item = 1; item < factory;item ++)
	{
		gps_item_info = &satellite_item_info[item].item;

		/* Get Item Reslut */
		gps_item_info -> psr = GET_ITEM_RESULT(gps_item_info_pre->psr, diff_psr);

		gps_item_info -> adr = GET_ITEM_RESULT(gps_item_info_pre->adr, diff_adr);

		gps_item_info -> dopper = GET_ITEM_RESULT(gps_item_info_pre->dopper, diff_dopper);

		gps_item_info -> raw_psr = GET_ITEM_RESULT(gps_item_info_pre->raw_psr, diff_raw_psr);

		gps_item_info -> raw_adr = GET_ITEM_RESULT(gps_item_info_pre->raw_adr, diff_raw_adr);

		gps_item_info -> raw_dopper = GET_ITEM_RESULT(gps_item_info_pre->raw_dopper, diff_raw_dopper);

		gps_item_info_pre = gps_item_info;
	}

	return 0;
}