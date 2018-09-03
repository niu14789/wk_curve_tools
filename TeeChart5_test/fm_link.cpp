/*

includes 

*/
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
#include "motor.h"
/*----------------------*/
extern motor motor_dlg;
/*----------------------*/
extern COM_MANAGE_DEF COM;
extern LINE_CONFIG_DEF line_cmd;
/*----------------------*/
SYS_LIST_DEF sys_list;
SYS_LIST_CHESE_DEF chese_list;
unsigned char show_taps_flags = 1;
unsigned char timer4_flags = 0;
/*----------------------*/
extern int start_stop_flags;
/*----------------------*/
unsigned int combox_clear_flags = 1;
/* ---------------------*/
extern unsigned char motor_close;
/* create */
int CTeeChart5_testDlg::fm_data_decode(unsigned char ID , unsigned char *data,unsigned int len_p)
{
	node_def *node;
	LINE_CONFIG_DEF * line;
	/* factory mode */
	//if( ID != 0xff && ID != 0xfe )
	//{
	//	return (-1);
	//}
	/*-------------get cmd ID---------------*/
	if( ID == 0xff )
	{
		switch(data[0])
		{
			case 0xff:
				if( len_p == 3 )
				{
					m_taps.SetWindowTextW(_T("提示信息：正在采集数据"));
					m_combox_param_show.EnableWindow(0);
					/*-----------------*/
					COM.num_node  = data[1];
					COM.num_shell = data[2];
					sys_list.list_num = COM.num_shell;
					/*-----------------*/
					KillTimer(2);
					KillTimer(1);
					/* start from 0,0*/
					set_unit_num(1,0);
					/* start unit the date thread */
					SetTimer(3,200,NULL);
					/* -  */			
					List_delete_all();
					/*------------*/
				}
				break;
			case 0xfd:
				line = ( LINE_CONFIG_DEF * )data;
				/* ----------------- */
				if( line->cmd == line_cmd.cmd && line->line_num == line_cmd.line_num )
				{
					for( int i = 0 ; i < line->line_num ; i++ )
					{
						if( ! ( line->line_gid[i][0] == line_cmd.line_gid[i][0] && line->line_gid[i][1] == line_cmd.line_gid[i][1] ) )
						{
							/*-------------------------*/
							return (-1);
							/*-------------------------*/
						}
					}
					/* show wave taps*/
					if( show_taps_flags == 1 )
					{
						m_taps.SetWindowTextW(_T("提示信息：设置成功，正在绘制......"));
						show_taps_flags = 0;
						line_cmd.cmd = 0xfc;//start
						timer4_flags = 1;
					}
				}
				break;
			case 0xfb:
				/* add line data */
				if( timer4_flags == 1 )
				{
					timer4_flags = 0;
					KillTimer(4);
				}
				/* add lines */
			    Force_cache_wave(data,len_p);
				/*-----------*/
				break;
			default:
				break;
		}
	}else if( ID == 0xfe )
	{
		m_taps.SetWindowTextW(_T("提示信息：正在采集数据"));
		node = ( node_def * )data;
		/* get data */
		if( node->line_num == COM.now_num )
		{
			memcpy(sys_list.list[node->line_num].title,node->name,sizeof(node->name));
			/* get type and len */
			sys_list.list[node->line_num].data_type = node->size >> 16;
			sys_list.list[node->line_num].data_num = node->size & 0xffff;
			/* copy data */
			memcpy(sys_list.list[node->line_num].raw_data,node->data,node->size_pl);
			/* set list index */
			sys_list.list[node->line_num].index = node->line_num;
			/* add to the combox */
			if( combox_clear_flags == 1 )
			{
				push_data_to_dram_lines(node,1);
				combox_clear_flags = 2;
			}
			/*------------------*/
			push_data_to_dram_lines(node,combox_clear_flags);
			/* add lists */
			List_refresh_line(&sys_list.list[node->line_num]);
			/*----------*/
			COM.now_num ++;
			/* over */
			if( COM.now_num >= COM.num_shell )
			{
				COM.now_sw = 1;
				COM.now_num = 0;
				/* unit ok */
				KillTimer(1);
                KillTimer(2);
				KillTimer(3);
				/*----------*/
				m_taps.SetWindowTextW(_T("提示信息：采集数据完毕"));
				/*--------*/
				m_manul_test.EnableWindow(1);
				/* close the add combox */
				combox_clear_flags = 0;
				/* again */
			    if( start_stop_flags == 0 )
				{
					m_taps.SetWindowTextW(_T("提示信息：正在重新测试"));
					SetTimer(1,1000,NULL);
					//m_manul_test.EnableWindow(0);
				}else
				{
					m_taps.SetWindowTextW(_T("提示信息：已手动暂停测试"));
					m_manul_test.EnableWindow(1);
					m_combox_param_show.EnableWindow(1);
				}
			}
		}
	}else if( ID == 77 )
	{
		if( motor_close )
		{
		   motor_dlg.show_factory(data,len_p);
		}
	}else if( ID == 252 )
	{
		if( motor_close )
		{
		   motor_dlg.show_motor_msg(data,len_p);
		}
	}else if( ID == 180 )
	{
		unsigned char tmp = data[44];
		/*--------------*/
		if( tmp == 0x01 )
		{
			pic_feedback_show();
		}
	}
    return 0;
}

