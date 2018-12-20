// limit_setting.cpp : 实现文件
//

#include "stdafx.h"
#include "limit_setting.h"
#include "afxdialogex.h"
#include "TeeChart5_testDlg.h"
#include "Data_detail.h"
// limit_setting 对话框
extern SYS_LIST_CHESE_DEF chese_list;
extern const char * string_chese_table[20][32];
extern int list_click_seq;
int now_seq = 0xffff;
/*-------------------*/
CEdit * low[16];
CEdit * now[16];
CEdit * upd[16];
CStatic * param_name[16];
CButton * check_type_mode[4];
/*-------------------*/
IMPLEMENT_DYNAMIC(limit_setting, CDialogEx)

limit_setting::limit_setting(CWnd* pParent /*=NULL*/)
	: CDialogEx(limit_setting::IDD, pParent)
	, m_check_type(false)
{

}

limit_setting::~limit_setting()
{
}

void limit_setting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_limit_combox);
	DDX_Control(pDX, IDC_EDIT1, m_limit_path);
	DDX_Control(pDX, IDC_CHECK1, m_check_if);
	DDX_Control(pDX, IDC_EDIT2, m_limit_num);
	DDX_Control(pDX, IDC_EDIT3, m_edit_l1);
	DDX_Control(pDX, IDC_EDIT9, m_edit_l2);
	DDX_Control(pDX, IDC_EDIT10, m_edit_l3);
	DDX_Control(pDX, IDC_EDIT11, m_edit_l4);
	DDX_Control(pDX, IDC_EDIT12, m_edit_l5);
	DDX_Control(pDX, IDC_EDIT13, m_edit_l6);
	DDX_Control(pDX, IDC_EDIT15, m_edit_l7);
	DDX_Control(pDX, IDC_EDIT25, m_edit_l8);
	DDX_Control(pDX, IDC_EDIT4, m_edit_c1);
	DDX_Control(pDX, IDC_EDIT14, m_edit_c2);
	DDX_Control(pDX, IDC_EDIT21, m_edit_c3);
	DDX_Control(pDX, IDC_EDIT22, m_edit_c4);
	DDX_Control(pDX, IDC_EDIT23, m_edit_c5);
	DDX_Control(pDX, IDC_EDIT17, m_edit_c6);
	DDX_Control(pDX, IDC_EDIT24, m_edit_c7);
	DDX_Control(pDX, IDC_EDIT26, m_edit_c8);
	DDX_Control(pDX, IDC_EDIT5, m_edit_u1);
	DDX_Control(pDX, IDC_EDIT27, m_edit_u2);
	DDX_Control(pDX, IDC_EDIT28, m_edit_u3);
	DDX_Control(pDX, IDC_EDIT29, m_edit_u4);
	DDX_Control(pDX, IDC_EDIT30, m_edit_u5);
	DDX_Control(pDX, IDC_EDIT20, m_edit_u6);
	DDX_Control(pDX, IDC_EDIT31, m_edit_u7);
	DDX_Control(pDX, IDC_EDIT32, m_edit_u8);
	DDX_Control(pDX, IDC_STATIC1, m_limit_static1);
	DDX_Control(pDX, IDC_STATIC3, m_limit_static2);
	DDX_Control(pDX, IDC_STATIC6, m_limit_static3);
	DDX_Control(pDX, IDC_STATIC4, m_limit_static4);
	DDX_Control(pDX, IDC_STATIC5, m_limit_static6);
	DDX_Control(pDX, IDC_STATIC7, m_limit_static5);
	DDX_Control(pDX, IDC_STATIC8, m_limit_static7);
	DDX_Control(pDX, IDC_STATIC9, m_limit_static8);
	DDX_Control(pDX, IDC_STATIC13, m_limit_static9);
	DDX_Control(pDX, IDC_STATIC14, m_limit_static10);
	DDX_Control(pDX, IDC_STATIC16, m_limit_static11);
	DDX_Control(pDX, IDC_STATIC15, m_limit_static12);
	DDX_Control(pDX, IDC_STATIC21, m_limit_static13);
	DDX_Control(pDX, IDC_STATIC20, m_limit_static14);
	DDX_Control(pDX, IDC_STATIC22, m_limit_static15);
	DDX_Control(pDX, IDC_STATIC23, m_limit_static16);

	DDX_Control(pDX, IDC_EDIT6, m_edit_l9);
	DDX_Control(pDX, IDC_EDIT33, m_edit_l10);
	DDX_Control(pDX, IDC_EDIT34, m_edit_l11);
	DDX_Control(pDX, IDC_EDIT35, m_edit_l12);
	DDX_Control(pDX, IDC_EDIT36, m_edit_l13);
	DDX_Control(pDX, IDC_EDIT37, m_edit_l14);
	DDX_Control(pDX, IDC_EDIT38, m_edit_l15);
	DDX_Control(pDX, IDC_EDIT45, m_edit_l16);
	DDX_Control(pDX, IDC_EDIT7, m_edit_c9);
	DDX_Control(pDX, IDC_EDIT39, m_edit_c10);
	DDX_Control(pDX, IDC_EDIT40, m_edit_c11);
	DDX_Control(pDX, IDC_EDIT41, m_edit_c12);
	DDX_Control(pDX, IDC_EDIT42, m_edit_c13);
	DDX_Control(pDX, IDC_EDIT43, m_edit_c14);
	DDX_Control(pDX, IDC_EDIT44, m_edit_c15);
	DDX_Control(pDX, IDC_EDIT46, m_edit_c16);
	DDX_Control(pDX, IDC_EDIT8, m_edit_u9);
	DDX_Control(pDX, IDC_EDIT47, m_edit_u10);
	DDX_Control(pDX, IDC_EDIT48, m_edit_u11);
	DDX_Control(pDX, IDC_EDIT49, m_edit_u12);
	DDX_Control(pDX, IDC_EDIT50, m_edit_u13);
	DDX_Control(pDX, IDC_EDIT51, m_edit_u14);
	DDX_Control(pDX, IDC_EDIT52, m_edit_u15);
	DDX_Control(pDX, IDC_EDIT53, m_edit_u16);
	DDX_Control(pDX, IDC_EDIT54, m_result);

	DDX_Control(pDX, IDC_STATIC10, m_dowm_text1);
	DDX_Control(pDX, IDC_STATIC17, m_down_text);
	DDX_Control(pDX, IDC_STATIC12, m_up_text1);
	DDX_Control(pDX, IDC_STATIC19, m_up_text);
	/* init */
	init_oncreate();
}


BEGIN_MESSAGE_MAP(limit_setting, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &limit_setting::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &limit_setting::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO1, &limit_setting::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CHECK1, &limit_setting::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK3, &limit_setting::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_BUTTON4, &limit_setting::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_RADIO5, &limit_setting::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &limit_setting::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &limit_setting::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &limit_setting::OnBnClickedRadio8)
	ON_STN_CLICKED(IDC_STATIC1, &limit_setting::OnStnClickedStatic1)
END_MESSAGE_MAP()

void limit_setting::init_oncreate(void)
{
	CString show;
	/*------------*/
	USES_CONVERSION;
	/* reset */
	m_limit_combox.ResetContent();
	/*--------------------------*/
	if( chese_list.total_line > 0 )
	{
		for( unsigned int i = 0 ; i < chese_list.total_line ; i++ )
		{
			show = A2T(chese_list.chese[i].title_chese);
			/* add to combox */
			m_limit_combox.AddString(show);
		}
	}
	m_limit_combox.SetCurSel(0);
	/* equse */
	low[0] = &m_edit_l1;
	low[1] = &m_edit_l2;
	low[2] = &m_edit_l3;
	low[3] = &m_edit_l4;
	low[4] = &m_edit_l5;
	low[5] = &m_edit_l6;
	low[6] = &m_edit_l7;
	low[7] = &m_edit_l8;
	low[8] = &m_edit_l9;
	low[9] = &m_edit_l10;
	low[10] = &m_edit_l11;
	low[11] = &m_edit_l12;
	low[12] = &m_edit_l13;
	low[13] = &m_edit_l14;
	low[14] = &m_edit_l15;
	low[15] = &m_edit_l16;
	/*------------*/
	upd[0] = &m_edit_u1;
	upd[1] = &m_edit_u2;
	upd[2] = &m_edit_u3;
	upd[3] = &m_edit_u4;
	upd[4] = &m_edit_u5;
	upd[5] = &m_edit_u6;
	upd[6] = &m_edit_u7;
	upd[7] = &m_edit_u8;
	upd[8] = &m_edit_u9;
	upd[9] = &m_edit_u10;
	upd[10] = &m_edit_u11;
	upd[11] = &m_edit_u12;
	upd[12] = &m_edit_u13;
	upd[13] = &m_edit_u14;
	upd[14] = &m_edit_u15;
	upd[15] = &m_edit_u16;
	/*-------------*/
	now[0] = &m_edit_c1;
	now[1] = &m_edit_c2;
	now[2] = &m_edit_c3;
	now[3] = &m_edit_c4;
	now[4] = &m_edit_c5;
	now[5] = &m_edit_c6;
	now[6] = &m_edit_c7;
	now[7] = &m_edit_c8;
	now[8] = &m_edit_c9;
	now[9] = &m_edit_c10;
	now[10] = &m_edit_c11;
	now[11] = &m_edit_c12;
	now[12] = &m_edit_c13;
	now[13] = &m_edit_c14;
	now[14] = &m_edit_c15;
	now[15] = &m_edit_c16;
	/*------------*/
	param_name[0] = &m_limit_static1;
	param_name[1] = &m_limit_static2;
	param_name[2] = &m_limit_static3;
	param_name[3] = &m_limit_static4;
	param_name[4] = &m_limit_static5;
	param_name[5] = &m_limit_static6;
	param_name[6] = &m_limit_static7;
	param_name[7] = &m_limit_static8;
	param_name[8] = &m_limit_static9;
	param_name[9] = &m_limit_static10;
	param_name[10] = &m_limit_static11;
	param_name[11] = &m_limit_static12;
	param_name[12] = &m_limit_static13;
	param_name[13] = &m_limit_static14;
	param_name[14] = &m_limit_static15;
	param_name[15] = &m_limit_static16;
	/*-------------------*/
	check_type_mode[0] = ((CButton *)GetDlgItem(IDC_RADIO5));
	check_type_mode[1] = ((CButton *)GetDlgItem(IDC_RADIO6));
	check_type_mode[2] = ((CButton *)GetDlgItem(IDC_RADIO7));
	check_type_mode[3] = ((CButton *)GetDlgItem(IDC_RADIO8));
	/*-------------------------*/
	click_one_line(list_click_seq);
}

void limit_setting::disable_mode_select(unsigned int mode)
{
	if( mode == 0 )
	{
		for( int i = 0 ; i < 4 ;i++)
		{
			check_type_mode[i]->EnableWindow(0);
		}
	}else
	{
		for( int i = 0 ; i < 4 ;i++)
		{
			check_type_mode[i]->EnableWindow(1);
		}
	}
}

void limit_setting::disable_check_none0(unsigned int mode)
{
	if( mode == 0 )
	{
		for( int i = 0 ; i < 16 ;i++)
		{
			low[i]->EnableWindow(0);
			upd[i]->EnableWindow(0);
			now[i]->EnableWindow(0);
		}
		/*==========================*/
		//m_check_beside0.EnableWindow(0);
		/*----------*/
	}else
	{
        for( int i = 0 ; i < 16 ; i++ )
		{
			low[i]->EnableWindow(1);
			upd[i]->EnableWindow(1);
			now[i]->EnableWindow(1);
		}
		/*-=========================*/
		//m_check_beside0.EnableWindow(1);
	}
}

int limit_setting::search_chese_name(char * name)
{
	for( int i = 0 ; i < 20 ; i ++ )
	{
		if( name == NULL || string_chese_table[i][0] == NULL )
		{
			return (-1);
		}
		if( strstr(name,string_chese_table[i][0]) != NULL )
		{
			return i;
		}
	}
	/* return */
	return (-1);
}

void limit_setting::click_one_line(unsigned int index)
{
	unsigned char *show_uint8,*show_uint8_l,*show_uint8_u,*show_uint8_mean,*show_uint8_mean_error,*show_uint8_rms,*show_uint8_mean_now,*show_uint8_rms_now;;
	char *show_int8,*show_int8_l,*show_int8_u,*show_int8_mean,*show_int8_mean_error,*show_int8_rms,*show_int8_mean_now,*show_int8_rms_now;;
	unsigned short *show_uint16,*show_uint16_l,*show_uint16_u,*show_uint16_mean,*show_uint16_mean_error,*show_uint16_rms,*show_uint16_mean_now,*show_uint16_rms_now;;
	short *show_int16, *show_int16_l, *show_int16_u,*show_int16_mean,*show_int16_mean_error,*show_int16_rms,*show_int16_mean_now,*show_int16_rms_now;;
	unsigned int *show_uint32,*show_uint32_l,*show_uint32_u,*show_uint32_mean,*show_uint32_mean_error,*show_uint32_rms,*show_uint32_mean_now,*show_uint32_rms_now;;
	int *show_int32,*show_int32_l,*show_int32_u,*show_int32_mean,*show_int32_mean_error,*show_int32_rms,*show_int32_mean_now,*show_int32_rms_now;;
	float *show_float,*show_float_l,*show_float_u,*show_flaot_mean,*show_flaot_mean_error,*show_flaot_rms,*show_flaot_mean_now,*show_flaot_rms_now;
	char buffer[16];
	int tags;
	unsigned int i;
	/*---------*/
    CString show;
	/*---------*/
	USES_CONVERSION;
	/*-----------*/
	now_seq = index;
	/* set cursor */
	m_limit_combox.SetCurSel(index);
	/*---------*/
	if( chese_list.chese[index].check_if == 0 )
	{
		disable_mode_select(0);
		disable_check_none0(0);
		m_check_if.SetCheck(0);
	}else
	{
		m_check_if.SetCheck(1);
	}
	/* loop */
	for( int i = 0 ; i < 16 ; i ++ )
	{
		param_name[i]->SetWindowTextW(_T(""));
		now[i]->SetWindowTextW(_T(""));
		upd[i]->SetWindowTextW(_T(""));
		low[i]->SetWindowTextW(_T(""));
	}
	/*-------------------*/
    set_check_type();
	/* set num */
	sprintf_s(buffer,"%d",chese_list.chese[index].num);
	/* set */
	show = A2T(buffer);
	m_limit_num.SetWindowTextW(show);
	/*----------*/
	sprintf_s(buffer,"%s",chese_list.chese[index].result);
	/* set */
	show = A2T(buffer);
	m_result.SetWindowTextW(show);
	/* get tags */
	tags = search_chese_name(chese_list.chese[index].title);
	/*--------------------------*/
	if( chese_list.chese[index].num >= 16 )
	{
		chese_list.chese[index].num = 16;
	}
	/*--------------------------*/
	switch(chese_list.chese[index].type)
	{
		case 0x81:
			show_int8 = (char *)chese_list.chese[index].raw_data;
			show_int8_l = (char *)chese_list.chese[index].limit_low;
			show_int8_u = (char *)chese_list.chese[index].limit_high;
			/* show */
			for( i = 0 ; i < chese_list.chese[index].num ; i ++ )
			{
				if( chese_list.chese[index].check_type == 3 )
				{
					show_int8_mean = (char *)chese_list.chese[index].limit_mean;
					show_int8_mean_error = (char *)chese_list.chese[index].limit_mean_error;
					show_int8_rms = (char *)chese_list.chese[index].limit_rms;
					show_int8_mean_now = (char *)&chese_list.chese[index].mean_now;
					show_int8_rms_now = (char *)&chese_list.chese[index].rms_now;
					/*-------------------------------*/
					sprintf_s(buffer,"%d,%d",show_int8_mean_now[i],show_int8_rms_now[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					now[i]->SetWindowTextW(show);
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d,%d",show_int8_mean[i],show_int8_mean_error[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					low[i]->SetWindowTextW(show);	
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_int8_rms[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					upd[i]->SetWindowTextW(show);	
				}else
				{
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_int8[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					now[i]->SetWindowTextW(show);
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_int8_l[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					low[i]->SetWindowTextW(show);	
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_int8_u[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					upd[i]->SetWindowTextW(show);	
				}
				/*-------show tags------------*/
				if( tags != (-1) )
				{
					show = A2T(string_chese_table[tags][i+2]);
					param_name[i]->SetWindowTextW(show);
				}
			}
			break;
		case 0x01:
			show_uint8 = (unsigned char *)chese_list.chese[index].raw_data;
			show_uint8_l = (unsigned char *)chese_list.chese[index].limit_low;
			show_uint8_u = (unsigned char *)chese_list.chese[index].limit_high;
			/* show */
			for( i = 0 ; i < chese_list.chese[index].num ; i ++ )
			{
				if( chese_list.chese[index].check_type == 3 )
				{
					show_uint8_mean = (unsigned char *)chese_list.chese[index].limit_mean;
					show_uint8_mean_error = (unsigned char *)chese_list.chese[index].limit_mean_error;
					show_uint8_rms = (unsigned char *)chese_list.chese[index].limit_rms;
					show_uint8_mean_now = (unsigned char *)&chese_list.chese[index].mean_now;
					show_uint8_rms_now = (unsigned char *)&chese_list.chese[index].rms_now;
					/*-------------------------------*/
					sprintf_s(buffer,"%d,%d",show_uint8_mean_now[i],show_uint8_rms_now[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					now[i]->SetWindowTextW(show);
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d,%d",show_uint8_mean[i],show_uint8_mean_error[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					low[i]->SetWindowTextW(show);	
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_uint8_rms[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					upd[i]->SetWindowTextW(show);	
				}else
				{
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_uint8[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					now[i]->SetWindowTextW(show);
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_uint8_l[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					low[i]->SetWindowTextW(show);	
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_uint8_u[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					upd[i]->SetWindowTextW(show);	
				}
				/*-------show tags------------*/
				if( tags != (-1) )
				{
					show = A2T(string_chese_table[tags][i+2]);
					param_name[i]->SetWindowTextW(show);
				}
			}
			break;
		case 0x82:
			show_int16  = (short *)chese_list.chese[index].raw_data;
			show_int16_l = (short *)chese_list.chese[index].limit_low;
			show_int16_u = (short *)chese_list.chese[index].limit_high;
			/* show */
			for( i = 0 ; i < chese_list.chese[index].num ; i ++ )
			{
				if( chese_list.chese[index].check_type == 3 )
				{
					show_int16_mean = (short *)chese_list.chese[index].limit_mean;
					show_int16_mean_error = ( short *)chese_list.chese[index].limit_mean_error;
					show_int16_rms = (short *)chese_list.chese[index].limit_rms;
					show_int16_mean_now = (short *)&chese_list.chese[index].mean_now;
					show_int16_rms_now = (short *)&chese_list.chese[index].rms_now;
					/*-------------------------------*/
					sprintf_s(buffer,"%d,%d",show_int16_mean_now[i],show_int16_rms_now[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					now[i]->SetWindowTextW(show);
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d,%d",show_int16_mean[i],show_int16_mean_error[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					low[i]->SetWindowTextW(show);	
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_int16_rms[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					upd[i]->SetWindowTextW(show);	
				}else
				{
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_int16[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					now[i]->SetWindowTextW(show);
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_int16_l[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					low[i]->SetWindowTextW(show);	
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_int16_u[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					upd[i]->SetWindowTextW(show);	
				}
				/*-------show tags------------*/
				if( tags != (-1) )
				{
					show = A2T(string_chese_table[tags][i+2]);
					param_name[i]->SetWindowTextW(show);
				}
			}
			break;
		case 0x02:
			show_uint16  = (unsigned short *)chese_list.chese[index].raw_data;
			show_uint16_l = (unsigned short *)chese_list.chese[index].limit_low;
			show_uint16_u = (unsigned short *)chese_list.chese[index].limit_high;
			/* show */
			for( i = 0 ; i < chese_list.chese[index].num ; i ++ )
			{
				if( chese_list.chese[index].check_type == 3 )
				{
					show_uint16_mean = (unsigned short *)chese_list.chese[index].limit_mean;
					show_uint16_mean_error = (unsigned short *)chese_list.chese[index].limit_mean_error;
					show_uint16_rms = (unsigned short *)chese_list.chese[index].limit_rms;
					show_uint16_mean_now = (unsigned short *)&chese_list.chese[index].mean_now;
					show_uint16_rms_now = (unsigned short *)&chese_list.chese[index].rms_now;
					/*-------------------------------*/
					sprintf_s(buffer,"%d,%d",show_uint16_mean_now[i],show_uint16_rms_now[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					now[i]->SetWindowTextW(show);
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d,%d",show_uint16_mean[i],show_uint16_mean_error[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					low[i]->SetWindowTextW(show);	
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_uint16_rms[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					upd[i]->SetWindowTextW(show);	
				}else
				{
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_uint16[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					now[i]->SetWindowTextW(show);
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_uint16_l[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					low[i]->SetWindowTextW(show);	
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_uint16_u[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					upd[i]->SetWindowTextW(show);	
				}
				/*-------show tags------------*/
				if( tags != (-1) )
				{
					show = A2T(string_chese_table[tags][i+2]);
					param_name[i]->SetWindowTextW(show);
				}
			}
			break;
		case 0x84:
			show_int32  = (int *)chese_list.chese[index].raw_data;
			show_int32_l = (int *)chese_list.chese[index].limit_low;
			show_int32_u = (int *)chese_list.chese[index].limit_high;
			/* show */
			for( i = 0 ; i < chese_list.chese[index].num ; i ++ )
			{
				if( chese_list.chese[index].check_type == 3 )
				{
					show_int32_mean = ( int *)chese_list.chese[index].limit_mean;
					show_int32_mean_error = ( int *)chese_list.chese[index].limit_mean_error;
					show_int32_rms = ( int *)chese_list.chese[index].limit_rms;
					show_int32_mean_now = ( int *)&chese_list.chese[index].mean_now;
					show_int32_rms_now = ( int *)&chese_list.chese[index].rms_now;
					/*-------------------------------*/
					sprintf_s(buffer,"%d,%d",show_int32_mean_now[i],show_int32_rms_now[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					now[i]->SetWindowTextW(show);
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d,%d",show_int32_mean[i],show_int32_mean_error[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					low[i]->SetWindowTextW(show);	
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_int32_rms[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					upd[i]->SetWindowTextW(show);	
				}else
				{
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_int32[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					now[i]->SetWindowTextW(show);
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_int32_l[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					low[i]->SetWindowTextW(show);	
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_int32_u[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					upd[i]->SetWindowTextW(show);	
				}
				/*-------show tags------------*/
				if( tags != (-1) )
				{
					show = A2T(string_chese_table[tags][i+2]);
					param_name[i]->SetWindowTextW(show);
				}
			}
			break;
		case 0x04:
			show_uint32  = (unsigned int *)chese_list.chese[index].raw_data;
			show_uint32_l = (unsigned int *)chese_list.chese[index].limit_low;
			show_uint32_u = (unsigned int *)chese_list.chese[index].limit_high;
			/* show */
			for( i = 0 ; i < chese_list.chese[index].num ; i ++ )
			{
				if( chese_list.chese[index].check_type == 3 )
				{
					show_uint32_mean = (unsigned int *)chese_list.chese[index].limit_mean;
					show_uint32_mean_error = (unsigned int *)chese_list.chese[index].limit_mean_error;
					show_uint32_rms = (unsigned int *)chese_list.chese[index].limit_rms;
					show_uint32_mean_now = (unsigned int *)&chese_list.chese[index].mean_now;
					show_uint32_rms_now = (unsigned int *)&chese_list.chese[index].rms_now;
					/*-------------------------------*/
					sprintf_s(buffer,"%d,%d",show_uint32_mean_now[i],show_uint32_rms_now[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					now[i]->SetWindowTextW(show);
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d,%d",show_uint32_mean[i],show_uint32_mean_error[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					low[i]->SetWindowTextW(show);	
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_uint32_rms[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					upd[i]->SetWindowTextW(show);	
				}else
				{
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_uint32[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					now[i]->SetWindowTextW(show);
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_uint32_l[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					low[i]->SetWindowTextW(show);	
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%d",show_uint32_u[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					upd[i]->SetWindowTextW(show);	
					/*-------show tags------------*/
				}
				if( tags != (-1) )
				{
					show = A2T(string_chese_table[tags][i+2]);
					param_name[i]->SetWindowTextW(show);
				}
			}
			break;
		case 0x05:
			show_float  = (float *)chese_list.chese[index].raw_data;
			show_float_l = (float *)chese_list.chese[index].limit_low;
			show_float_u = (float *)chese_list.chese[index].limit_high;
			/* show */
			for( i = 0 ; i < chese_list.chese[index].num ; i ++ )
			{
				if( chese_list.chese[index].check_type == 3 )
				{
					show_flaot_mean = (float *)chese_list.chese[index].limit_mean;
					show_flaot_mean_error = (float *)chese_list.chese[index].limit_mean_error;
					show_flaot_rms = (float *)chese_list.chese[index].limit_rms;
					show_flaot_mean_now = (float *)&chese_list.chese[index].mean_now;
					show_flaot_rms_now = (float *)&chese_list.chese[index].rms_now;
					/*-------------------------------*/
					sprintf_s(buffer,"%5.2f,%5.2f",show_flaot_mean_now[i],show_flaot_rms_now[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					now[i]->SetWindowTextW(show);
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%5.2f,%5.2f",show_flaot_mean[i],show_flaot_mean_error[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					low[i]->SetWindowTextW(show);	
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%5.2f",show_flaot_rms[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					upd[i]->SetWindowTextW(show);	
				}else
				{
					/*---------------------------------------*/
					sprintf_s(buffer,"%5.2f",show_float[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					now[i]->SetWindowTextW(show);
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%5.2f",show_float_l[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					low[i]->SetWindowTextW(show);	
					/*==================================================*/
					/*---------------------------------------*/
					sprintf_s(buffer,"%5.2f",show_float_u[i]);
					/* set */
					show = A2T(buffer);
					/* show now */
					upd[i]->SetWindowTextW(show);	
				}
				/*-------show tags------------*/
				if( tags != (-1) )
				{
					show = A2T(string_chese_table[tags][i+2]);
					param_name[i]->SetWindowTextW(show);
				}
			}
			break;
		default :
		break;
	}
}

// limit_setting 消息处理程序


void limit_setting::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void limit_setting::OnBnClickedButton2()
{
	unsigned char *show_uint8_l,*show_uint8_u;
	char *show_int8_l,*show_int8_u;
	unsigned short *show_uint16_l,*show_uint16_u;
	short *show_int16_l, *show_int16_u;
	unsigned int *show_uint32_l,*show_uint32_u;
	int *show_int32_l,*show_int32_u;
	float *show_float_l,*show_float_u,*show_float_m,*show_float_e,*show_float_r;
	unsigned int i;
	float tmp,tmp2;
	char buffer[16];
	/*---------*/
    CString show;
	/*---------*/
	USES_CONVERSION;
	/*-----------------------------*/
	if( chese_list.chese[now_seq].check_if == 1 )
	{
		if( chese_list.chese[now_seq].check_type != 0 )
		{
			switch(chese_list.chese[now_seq].type)
			{
				case 0x81:
					show_int8_l = (char *)chese_list.chese[now_seq].limit_low;
					show_int8_u = (char *)chese_list.chese[now_seq].limit_high;
					/* show */
					for( i = 0 ; i < chese_list.chese[now_seq].num ; i ++ )
					{
						if( get_float_edit(low[i],&tmp) != 0 )
						{
							sprintf_s(buffer,"下限参数%d错误",i);
							show = A2T(buffer);
							MessageBox(show,_T("tips"),0);
							return;
						}
						/* ok */
						show_int8_l[i] = (char)tmp;
						/*====================================*/
						if( get_float_edit(upd[i],&tmp) != 0 )
						{
							sprintf_s(buffer,"上限参数%d错误",i);
							show = A2T(buffer);
							MessageBox(show,_T("tips"),0);
							return;
						}
						/* ok */
						show_int8_u[i] = (char)tmp;
					}
					break;
				case 0x01:
					show_uint8_l = (unsigned char *)chese_list.chese[now_seq].limit_low;
					show_uint8_u = (unsigned char *)chese_list.chese[now_seq].limit_high;
					/* show */
					for( i = 0 ; i < chese_list.chese[now_seq].num ; i ++ )
					{
						if( get_float_edit(low[i],&tmp) != 0 )
						{
							sprintf_s(buffer,"下限参数%d错误",i);
							show = A2T(buffer);
							MessageBox(show,_T("tips"),0);
							return;
						}
						/* ok */
						show_uint8_l[i] = (unsigned char)tmp;
						/*====================================*/
						if( get_float_edit(upd[i],&tmp) != 0 )
						{
							sprintf_s(buffer,"上限参数%d错误",i);
							show = A2T(buffer);
							MessageBox(show,_T("tips"),0);
							return;
						}
						/* ok */
						show_uint8_u[i] = (unsigned char)tmp;
					}
					break;
				case 0x82:
					show_int16_l = (short *)chese_list.chese[now_seq].limit_low;
					show_int16_u = (short *)chese_list.chese[now_seq].limit_high;
					/* show */
					for( i = 0 ; i < chese_list.chese[now_seq].num ; i ++ )
					{
						if( get_float_edit(low[i],&tmp) != 0 )
						{
							sprintf_s(buffer,"下限参数%d错误",i);
							show = A2T(buffer);
							MessageBox(show,_T("tips"),0);
							return;
						}
						/* ok */
						show_int16_l[i] = (short)tmp;
						/*====================================*/
						if( get_float_edit(upd[i],&tmp) != 0 )
						{
							sprintf_s(buffer,"上限参数%d错误",i);
							show = A2T(buffer);
							MessageBox(show,_T("tips"),0);
							return;
						}
						/* ok */
						show_int16_u[i] = (short)tmp;
					}
					break;
				case 0x02:
					show_uint16_l = (unsigned short *)chese_list.chese[now_seq].limit_low;
					show_uint16_u = (unsigned short *)chese_list.chese[now_seq].limit_high;
					/* show */
					for( i = 0 ; i < chese_list.chese[now_seq].num ; i ++ )
					{
						if( get_float_edit(low[i],&tmp) != 0 )
						{
							sprintf_s(buffer,"下限参数%d错误",i);
							show = A2T(buffer);
							MessageBox(show,_T("tips"),0);
							return;
						}
						/* ok */
						show_uint16_l[i] = (unsigned short)tmp;
						/*====================================*/
						if( get_float_edit(upd[i],&tmp) != 0 )
						{
							sprintf_s(buffer,"上限参数%d错误",i);
							show = A2T(buffer);
							MessageBox(show,_T("tips"),0);
							return;
						}
						/* ok */
						show_uint16_u[i] = (unsigned short)tmp;
					}
					break;
				case 0x84:
					show_int32_l = (int *)chese_list.chese[now_seq].limit_low;
					show_int32_u = (int *)chese_list.chese[now_seq].limit_high;
					/* show */
					for( i = 0 ; i < chese_list.chese[now_seq].num ; i ++ )
					{
						if( get_float_edit(low[i],&tmp) != 0 )
						{
							sprintf_s(buffer,"下限参数%d错误",i);
							show = A2T(buffer);
							MessageBox(show,_T("tips"),0);
							return;
						}
						/* ok */
						show_int32_l[i] = (int)tmp;
						/*====================================*/
						if( get_float_edit(upd[i],&tmp) != 0 )
						{
							sprintf_s(buffer,"上限参数%d错误",i);
							show = A2T(buffer);
							MessageBox(show,_T("tips"),0);
							return;
						}
						/* ok */
						show_int32_u[i] = (int)tmp;
					}
					break;
				case 0x04:
					show_uint32_l = (unsigned int *)chese_list.chese[now_seq].limit_low;
					show_uint32_u = (unsigned int *)chese_list.chese[now_seq].limit_high;
					/* show */
					for( i = 0 ; i < chese_list.chese[now_seq].num ; i ++ )
					{
						if( get_float_edit(low[i],&tmp) != 0 )
						{
							sprintf_s(buffer,"下限参数%d错误",i);
							show = A2T(buffer);
							MessageBox(show,_T("tips"),0);
							return;
						}
						/* ok */
						show_uint32_l[i] = (unsigned int)tmp;
						/*====================================*/
						if( get_float_edit(upd[i],&tmp) != 0 )
						{
							sprintf_s(buffer,"上限参数%d错误",i);
							show = A2T(buffer);
							MessageBox(show,_T("tips"),0);
							return;
						}
						/* ok */
						show_uint32_u[i] = (unsigned int)tmp;
					}
					break;;
				case 0x05:
					show_float_l = (float *)chese_list.chese[now_seq].limit_low;
					show_float_u = (float *)chese_list.chese[now_seq].limit_high;
					show_float_m = (float *)chese_list.chese[now_seq].limit_mean;
					show_float_r = (float *)chese_list.chese[now_seq].limit_rms;
					show_float_e = (float *)chese_list.chese[now_seq].limit_mean_error;
					/* get data */
					if( chese_list.chese[now_seq].check_type == 3 )
					{
	                    /* show */
						for( i = 0 ; i < chese_list.chese[now_seq].num ; i ++ )
						{
							if( get2_float_edit(low[i],&tmp,&tmp2) != 0 )
							{
								sprintf_s(buffer,"均值参数%d错误",i);
								show = A2T(buffer);
								MessageBox(show,_T("tips"),0);
								return;
							}
							/* ok */
							show_float_m[i] = (float)tmp;
							show_float_e[i] = (float)tmp2;
							/*====================================*/
							if( get_float_edit(upd[i],&tmp) != 0 )
							{
								sprintf_s(buffer,"方差参数%d错误",i);
								show = A2T(buffer);
								MessageBox(show,_T("tips"),0);
								return;
							}
							/* ok */
							show_float_r[i] = (float)tmp;
						}
					}else
					{
						/* show */
						for( i = 0 ; i < chese_list.chese[now_seq].num ; i ++ )
						{
							if( get_float_edit(low[i],&tmp) != 0 )
							{
								sprintf_s(buffer,"下限参数%d错误",i);
								show = A2T(buffer);
								MessageBox(show,_T("tips"),0);
								return;
							}
							/* ok */
							show_float_l[i] = (float)tmp;
							/*====================================*/
							if( get_float_edit(upd[i],&tmp) != 0 )
							{
								sprintf_s(buffer,"上限参数%d错误",i);
								show = A2T(buffer);
								MessageBox(show,_T("tips"),0);
								return;
							}
							/* ok */
							show_float_u[i] = (float)tmp;
						}
					}
					break;
				default:
					break;
		   }	
	    }
	}
	/*-----------------------------*/
	cfg_save_read(0);
}


void limit_setting::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int seq = m_limit_combox.GetCurSel();
	now_seq = seq;
	/*---------------------------*/
	click_one_line(seq);
}

void limit_setting::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_check_if.GetCheck() )
	{
		if( chese_list.chese[now_seq].check_type != 0 )
		{
			disable_check_none0(1);
		}else
		{
			disable_check_none0(0);
		}
		/*---------------*/
		disable_mode_select(1);
		chese_list.chese[now_seq].check_if = 1;
	}else
	{
		disable_mode_select(0);
		disable_check_none0(0);
		chese_list.chese[now_seq].check_if = 0;
	}
}
void limit_setting::cfg_save_read(unsigned int mode)
{
    FILE * pf_file_limit;
	/*------------------------------*/
	if( chese_list.total_line == 0 )
	{
		if( chese_list.total_line == 0 )
		{
			MessageBox(_T("请先连上飞机，进行数据采集"),_T("tips"),0);
			return;
		}else
		{
			fopen_s(&pf_file_limit,"D200_limit.cfg","wb+");
			/* */
			if( pf_file_limit == NULL )
			{
				/* create fail */
				MessageBox(_T("文件创建失败，可能是由于加密系统或权限不够"),_T("tips"),0);
				return;
			}
			/*------*/
			fwrite(&chese_list,1,sizeof(chese_list),pf_file_limit);
			fclose(pf_file_limit);
			/*------*/
			fopen_s(&pf_file_limit,"D200_limit.cfg","rb");
			/*--------------*/
			if( pf_file_limit == NULL )
			{
				/* create fail */
				MessageBox(_T("文件打开失败，可能是由于加密系统或权限不够"),_T("tips"),0);
				return;
			}
			/* ok */
            if( fread_s(&chese_list,sizeof(chese_list),1,sizeof(chese_list),pf_file_limit) != sizeof(chese_list) )
			{
				memset(&chese_list,0,sizeof(chese_list));
			   /* create fail */
				MessageBox(_T("文件打开失败，可能是由于加密系统或权限不够"),_T("tips"),0);
			}
			/*-----------------*/
			fclose(pf_file_limit);
			/*-----------------*/
			return;
 		}
	}
	/*-----------------*/
	fopen_s(&pf_file_limit,"D200_limit.cfg","wb+");
	/* exist */
	if( pf_file_limit == NULL )
	{
		/* create fail */
		MessageBox(_T("文件创建失败，可能是由于加密系统或权限不够"),_T("tips"),0);
		/*---------------*/
		memset(&chese_list,0,sizeof(chese_list));
		/* return */
		return;
	}
	/*-------*/
	fwrite(&chese_list,1,sizeof(chese_list),pf_file_limit);
	/*-------*/
	fclose(pf_file_limit);
	/*-------------*/
	MessageBox(_T("保存成功"),_T("tips"),0);
}

void limit_setting::set_check_type()
{
   if( chese_list.chese[now_seq].check_type == 0 )
   {
	   disable_check_none0(0);
	   mean_mode_enter(0);
	   check_type_mode[0]->SetCheck(1);
   }else if( chese_list.chese[now_seq].check_type == 1 )
   {
	   mean_mode_enter(0);
	   check_type_mode[1]->SetCheck(1);
   }else if( chese_list.chese[now_seq].check_type == 2 )
   {
	   mean_mode_enter(0);
	   check_type_mode[2]->SetCheck(1);
   }else if( chese_list.chese[now_seq].check_type == 3 )
   {
	   mean_mode_enter(1);
	   check_type_mode[3]->SetCheck(1);
   }else
   {

   }
}
/*--------------------------------------*/
void limit_setting::OnBnClickedCheck3()
{

}
/*---------------*/
int limit_setting::get_float_edit( CEdit * edit , float * data )
{
	wchar_t get_cs[512];
	char c_string[256];
    int len_t;

	len_t = edit->GetWindowTextW(get_cs,512);

	for( int i = 0 ; i < len_t ; i ++  )
	{
		c_string[i] = (char)get_cs[i];
	}
	/*----------------------*/
	int ret = sscanf_s(c_string,"%f",data);

	if( ret == 1 )
	{
		return 0;
	}else
	{
		return (-1);
	}
}
/*  two param */
int limit_setting::get2_float_edit( CEdit * edit , float * data ,float * data2)
{
	wchar_t get_cs[512];
	char c_string[256];
    int len_t;

	len_t = edit->GetWindowTextW(get_cs,512);

	for( int i = 0 ; i < len_t ; i ++  )
	{
		c_string[i] = (char)get_cs[i];
	}
	/*----------------------*/
	int ret = sscanf_s(c_string,"%f,%f",data,data2);

	if( ret == 2 )
	{
		return 0;
	}else
	{
		return (-1);
	}
}
/*-------------------------------------*/
void limit_setting::OnBnClickedCheck4()
{
	//   chese_list.chese[now_seq].check_type = 3;
	//// TODO: 在此添加控件通知处理程序代码
	//if( m_mean_rms.GetCheck() )
	//{
	//	m_check_beside0.EnableWindow(0);
	//	m_check_none0.EnableWindow(0);
	//	chese_list.chese[now_seq].check_type = 3;
	//}else
	//{

	//}
}


void limit_setting::OnBnClickedButton4()
{
	Data_detail Data_detail_d;
	Data_detail_d.DoModal();
}


void limit_setting::OnBnClickedRadio5()//not 0
{
	// TODO: 在此添加控件通知处理程序代码
	disable_check_none0(0);
	mean_mode_enter(0);
	chese_list.chese[now_seq].check_type = 0;
	click_one_line(now_seq);
}


void limit_setting::OnBnClickedRadio6()
{
	// TODO: 在此添加控件通知处理程序代码
	disable_check_none0(1);
	mean_mode_enter(0);
	chese_list.chese[now_seq].check_type = 1;
	click_one_line(now_seq);
}


void limit_setting::OnBnClickedRadio7()
{
	// TODO: 在此添加控件通知处理程序代码
	disable_check_none0(1);
	mean_mode_enter(0);
	chese_list.chese[now_seq].check_type = 2;
	click_one_line(now_seq);
}


void limit_setting::OnBnClickedRadio8()
{
	// TODO: 在此添加控件通知处理程序代码
	disable_check_none0(1);
	chese_list.chese[now_seq].check_type = 3;
	/* transfer the data */
	mean_mode_enter(1);
	click_one_line(now_seq);
}

void limit_setting::mean_mode_enter(unsigned int mode)
{
	if( mode == 0 )
	{
		m_dowm_text1.SetWindowTextW(_T("下限"));
		m_down_text.SetWindowTextW(_T("下限"));
		m_up_text1.SetWindowTextW(_T("上限"));
		m_up_text.SetWindowTextW(_T("上限"));
	}else
	{
		m_dowm_text1.SetWindowTextW(_T("均值"));
		m_down_text.SetWindowTextW(_T("均值"));
		m_up_text1.SetWindowTextW(_T("方差"));
		m_up_text.SetWindowTextW(_T("方差"));
	}
}

void limit_setting::OnStnClickedStatic1()
{
	// TODO: 在此添加控件通知处理程序代码
}
