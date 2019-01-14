
// TeeChart5_testDlg.cpp : ÊµÏÖÎÄ¼þ
//

#include "stdafx.h"
#include "TeeChart5_test.h"
#include "TeeChart5_testDlg.h"
#include "afxdialogex.h"
#include "CSeries.h"
#include "tchart1.h"
#include "C_open.h"
#include "CLegend0.h"
#include "CAxis.h"
#include "CAxes.h"
#include "math.h"
#include "C_PROCOTOL_CREATE.h"
#include <Dbt.h>
#include "m_com_num.h"
#include "limit_setting.h"
#include "map.h"
#include "motor.h"
#include "CAspect.h"
#include "CPoint3DSeries.h"
#include "Auto_set.h"
#include "CEnvironment.h"
#include "CAxisTitle.h"
#include "CAxisTitle0.h"
#include "CPen0.h"
#include "CTeeFont.h"
#include "CAxisLabels.h"
#include "CAxisLabels0.h"
#include "CMarks.h"
#include "smtp.h"
#include "server_manage.h"
#include "system_config.h"
#include "suggestion.h"
#include "rt27.h"
#include "del_point.h"
#include "data_review.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRect m_rect,m_orignal;
extern COM_MANAGE_DEF COM;
extern FILE_MAN_DEF file_man;
extern SYS_LIST_CHESE_DEF chese_list;
extern SYS_LIST_CHESE_DEF  limit_file;
extern LINE_MULIT_DEF line_mulit[128];
static CFG_FILE_DEF READ_CFS;
PARAM_LIST_DEF param_list_show;
static unsigned char read_buffer_block[10240];//10KB
static unsigned char * dynamic_area = NULL;
static unsigned char sempher = 0;
int list_click_seq = 0;
int start_stop_flags = 0;
unsigned char motor_close = 0;
/*--------------*/
static unsigned int last_postion = 0;
static unsigned int lane_last_postion = 0;
static unsigned int last_sd = 0;
static unsigned int lat_pos_point = 0;
/* multi thread */
CWinThread * handle_com_read;
//int create_thread(void);
UINT ThreadProc(LPVOID pParam);
void thread_suspend(unsigned int mode);
/*----------------------------*/
//static CSeries lineSeries_s[20];
static unsigned int avs[50];
static double last_dou[1024];
/*------------------------------------*/
SYSTEM_CONFIG_INF_DEF system_config_inf;
/*------------------------------------*/
static unsigned char auto_connect = 0;
/*------------------------------------*/
unsigned int pic_take_public_s = 0;
unsigned int pic_take_now = 0;
unsigned int pic_feedback_err = 0;
unsigned int pic_feedback_ok = 0;
/*------------------------------------*/
motor motor_dlg;
/*------------------------------------*/
static unsigned int color_index = 0;
/*------------------------------------*/
static unsigned int version_ctrl = 0;
/*------------------------------------*/
extern SYSTEM_DELETE_POINT_DEF system_delete_point;
/*------------------------------------*/
// ÓÃÓÚÓ¦ÓÃ³ÌÐò¡°¹ØÓÚ¡±²Ëµ¥ÏîµÄ CAboutDlg ¶Ô»°¿ò

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ¶Ô»°¿òÊý¾Ý
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö

// ÊµÏÖ
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTeeChart5_testDlg ¶Ô»°¿ò




CTeeChart5_testDlg::CTeeChart5_testDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTeeChart5_testDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON7);
}

void CTeeChart5_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART1, m_chart);
	DDX_Control(pDX, IDC_BUTTON2, m_ana);
	DDX_Control(pDX, IDC_BUTTON1, m_open);
	DDX_Control(pDX, IDC_LIST1, m_list_ctrl);
	DDX_Control(pDX, IDC_BUTTON9, m_show_test);
	DDX_Control(pDX, IDC_COMBO1, m_combox_param_show);
	DDX_Control(pDX, IDC_CHECK1, m_check_hold);
	DDX_Control(pDX, IDC_MSCOMM1, m_mscomm);
	DDX_Control(pDX, IDC_STATIC2, m_taps);
	DDX_Control(pDX, IDC_BUTTON29, m_manul_test);
	DDX_Control(pDX, IDC_COMBO2, m_pic_cnt);
	DDX_Control(pDX, IDC_BUTTON14, m_btn_pic);
	DDX_Control(pDX, IDC_EDIT1, m_edit_pic);
	DDX_Control(pDX, IDC_CHECK2, m_check_camera);
	DDX_Control(pDX, IDC_BUTTON31, m_btn_ex0);
	DDX_Control(pDX, IDC_BUTTON7, m_btn_ex2);
	DDX_Control(pDX, IDC_BUTTON32, m_btn_ex1);
	DDX_Control(pDX, IDC_BUTTON33, m_btn_ex3);
}

BEGIN_MESSAGE_MAP(CTeeChart5_testDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTeeChart5_testDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTeeChart5_testDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTeeChart5_testDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTeeChart5_testDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CTeeChart5_testDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON8, &CTeeChart5_testDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CTeeChart5_testDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON7, &CTeeChart5_testDlg::OnBnClickedButton7)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_32771, &CTeeChart5_testDlg::On32771)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_32781, &CTeeChart5_testDlg::On32781)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CTeeChart5_testDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON5, &CTeeChart5_testDlg::OnBnClickedButton5)
	ON_COMMAND(ID_32777, &CTeeChart5_testDlg::On32777)
	ON_WM_DEVICECHANGE()
	ON_BN_CLICKED(IDC_BUTTON10, &CTeeChart5_testDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CTeeChart5_testDlg::OnBnClickedButton11)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CTeeChart5_testDlg::OnLvnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CTeeChart5_testDlg::OnNMDblclkList2)
	ON_BN_CLICKED(IDC_BUTTON26, &CTeeChart5_testDlg::OnBnClickedButton26)
	ON_BN_CLICKED(IDC_BUTTON14, &CTeeChart5_testDlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON22, &CTeeChart5_testDlg::OnBnClickedButton22)
	ON_BN_CLICKED(IDC_BUTTON27, &CTeeChart5_testDlg::OnBnClickedButton27)
	ON_BN_CLICKED(IDC_BUTTON29, &CTeeChart5_testDlg::OnBnClickedButton29)
	ON_BN_CLICKED(IDC_BUTTON30, &CTeeChart5_testDlg::OnBnClickedButton30)
	ON_BN_CLICKED(IDC_BUTTON31, &CTeeChart5_testDlg::OnBnClickedButton31)
	ON_BN_CLICKED(IDC_BUTTON32, &CTeeChart5_testDlg::OnBnClickedButton32)
	ON_BN_CLICKED(IDC_BUTTON33, &CTeeChart5_testDlg::OnBnClickedButton33)
END_MESSAGE_MAP()


// CTeeChart5_testDlg ÏûÏ¢´¦Àí³ÌÐò

BOOL CTeeChart5_testDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// IDM_ABOUTBOX
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	//-----------------------
	SetIcon(m_hIcon, TRUE);		
	SetIcon(m_hIcon, FALSE);	
	/*-----------------------*/
	/*-----------------------*/
	GetClientRect(&m_rect);
	GetClientRect(&m_orignal);
	/*------------------------*/
	m_combox_param_show.SetCurSel(0);
#if !VERSION_CTRL
	 /* list contrl */
	 DWORD dwStyle = m_list_ctrl.GetExtendedStyle();     
     dwStyle |= LVS_EX_FULLROWSELECT;
     dwStyle |= LVS_EX_GRIDLINES;
	 dwStyle |= LVS_EX_CHECKBOXES;
	 /*--------*/
     m_list_ctrl.SetExtendedStyle(dwStyle);
	 /* capute */
     m_list_ctrl.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 40);
     m_list_ctrl.InsertColumn(1, _T("检查项"), LVCFMT_LEFT, 80);
     m_list_ctrl.InsertColumn(2, _T("结果"), LVCFMT_CENTER, 90);
	 m_list_ctrl.InsertColumn(3, _T("数据"), LVCFMT_LEFT, 1000);
#endif
	 /* line init */
	 chart_line_init();
	 /*-------------------*/
#if !VERSION_CTRL
	 Read_limit_file();
	 memset(&line_mulit,0xff,sizeof(line_mulit));
//	 create_thread();
	 /*-------------check system config files---------------*/
	 FILE * cfg_file;
	 /* exist ? */
	 fopen_s(&cfg_file,"config.inf","rb");
	/* create ok ?*/
	if( cfg_file == NULL )
	{
		system_config_fresh(0,0x12345678);
	}else
	{
		int len = fread(read_buffer_block,1,sizeof(read_buffer_block),cfg_file);
		/* --  */
		if( len != sizeof(system_config_inf) )
		{
			system_config_fresh(0,0x12345678);
		}else
		{
			memcpy(&system_config_inf,read_buffer_block,len);
		}
		/*--------------------*/
		fclose(cfg_file);
	}
	/*----------set tet-------------*/
	if( !system_config_inf.inf[1] )
	{ 
	    check_list_show(system_config_inf.inf[1]);
	}

	/* */

	/* open */
	if( system_config_inf.inf[2] )
	{
		/*-----------*/
		m_taps.SetWindowTextW(_T("提示信息：自动模式，请插上电台......"));
		Get_COM_STATUS(1);
	}else
	{
		/*-----------*/
		m_taps.SetWindowTextW(_T("提示信息：手动模式......"));
		Get_COM_STATUS(0);
	}

	/*----------------------*/
	m_pic_cnt.SetCurSel(0);
	/*----------------------*/
#endif
	CEnvironment evn = m_chart.get_Environment();
	evn.put_MouseWheelScroll(0);
	/*----------*/
	create_color_table();
	/* send a email to devolopers */
	exmail_initial();
	/*-----------------------------*/
#if !VERSION_CTRL
	/* hide some funstions first . because we won't need them */
	GetDlgItem(IDC_BUTTON7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON31)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON32)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON33)->ShowWindow(SW_HIDE);
#else
	check_list_show(0);
	/* hide some functions */
	GetDlgItem(IDC_BUTTON7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON31)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON32)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON33)->ShowWindow(SW_HIDE);
	/* ----------------------- */
	m_show_test.SetWindowTextW(_T("帮助文档"));
	char buffer[64];
	sprintf_s(buffer,"%s 点击<帮助文档>获取更多帮助。",SOFTVERSION);
	/* transform */
	USES_CONVERSION;
	/*---------------------------*/
	CString show = A2T(buffer);
	/*---------------------------*/
	m_taps.SetWindowTextW(show);
	/*--------------------------*/
#endif
	/* file end */
	/* return */
	return TRUE; 
}

void CTeeChart5_testDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
/*------------------------------*/
void CTeeChart5_testDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ÓÃÓÚ»æÖÆµÄÉè±¸ÉÏÏÂÎÄ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ê¹Í¼±êÔÚ¹¤×÷Çø¾ØÐÎÖÐ¾ÓÖÐ
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// »æÖÆÍ¼±ê
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		/*CPaintDC dc(this); 
		CRect rect;
		GetClientRect(&rect);

		dc.FillSolidRect(rect,RGB(177,225,224));
		*/
		CDialogEx::OnPaint();
	}
}
/*--------------------*/
HCURSOR CTeeChart5_testDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*------------code------------*/
void CTeeChart5_testDlg::OnBnClickedButton1()
{
	 CAxes chartaxis =(CAxes)m_chart.get_Axis();
	 /*---------------------------------------*/
	 CAxis axis,axis_bottom;
	 /*---------------------------------------*/
	 for( int i = 0 ; i < 19 ; i ++ )
	 {
		 if( avs[i] == 1 )
		 {
			if( i == 0 )
			{
				axis = (CAxis)chartaxis.get_Left();
			}else
			{
				axis = (CAxis)chartaxis.get_Custom(i-1);
			}
		 }
	 }
	/*----------------------------*/
	axis_bottom = (CAxis)chartaxis.get_Bottom();
	axis_bottom.put_Automatic(1);
	/*----------------------------*/
}


void CTeeChart5_testDlg::OnBnClickedButton2()
{
    /* get cline_num;*/
	int num = m_combox_param_show.GetCurSel();
	/* create line */
	CSeries line_cfs = (CSeries)m_chart.Series(param_list_show.param_list[num].line_num);
	/*-----------------------*/
	if( param_list_show.param_list[num].status != 0 )
	{
		/*---------------*/
		unsigned long cols = get_color(0);//(colorB<<16)|(colorG<<8)|(colorR);
		/*---------------*/
		param_list_show.param_list[num].color = cols;
		/*---------------*/
		line_cfs.put_Color(cols);
		/* axis color */
		axis_color(param_list_show.param_list[num].line_num,cols,0);
	}
}
/* axis reset */
void CTeeChart5_testDlg::axis_reset(void)
{
	CAxes chartaxis =(CAxes)m_chart.get_Axis();
	CAxis axis;
	/*--------------------------*/		
	axis = (CAxis)chartaxis.get_Left();
	/*------------------------------*/
	axis.put_Visible(1);
	/*------------------------------*/
	axis_color(0,0,1);
	/*------------------------------*/
	CAxisTitle Custom_title = axis.get_Title();
	Custom_title.put_Visible(0);
	/*--------------------*/
	axis.put_Automatic(1);
	/* others line */
	for( int i = 0 ; i < 20; i ++ )
	{
		axis = (CAxis)chartaxis.get_Custom(i);
		/*----------------------------------*/
		axis.put_Visible(0);
		/*----------------------------------*/
		axis.put_Automatic(1);
	}
	/*----------------*/
	color_index = 0;
}
/*----------------------------------------------------------*/
void CTeeChart5_testDlg::axis_color(unsigned int num,unsigned int color,unsigned int mode)
{
	if( mode == 0 )
	{
		return;
	}
	/* some defines */
	CAxes chartaxis =(CAxes)m_chart.get_Axis();
	CAxis Custom_axis;
	/* show axis */
	if( num >= 1 )
	{
		Custom_axis = (CAxis)chartaxis.get_Custom(num-1);
	}else
	{
		Custom_axis = (CAxis)chartaxis.get_Left();
	}
	/* get title */
	CAxisTitle Custom_title = Custom_axis.get_Title();
	/* font set */
	CTeeFont font = Custom_title.get_Font();
	font.put_Color(color);
	/*--------------------------------*/
	CPen0 pen = Custom_axis.get_AxisPen(); 
	/*--------------------------------*/
	pen.put_Color(color);
	/*-------------------------------*/
	CAxisLabels label = Custom_axis.get_Labels();
	font = label.get_Font();
	font.put_Color(color);
	/*---------------------------------*/
}
void CTeeChart5_testDlg::OnBnClickedButton3()
{
   suggestion s;
   s.DoModal();
}

void CTeeChart5_testDlg::OnBnClickedButton4()
{
   	CAxes chartaxis =(CAxes)m_chart.get_Axis();
	
	CAxis axis = (CAxis)chartaxis.get_Left();
	/*----------------------------------*/
	Auto_set dl;
	dl.DoModal();
	/*------set scale------*/
	//if( auto_scale_g.global_auto )
	//{
	//	axis.put_Automatic(1);
	//	/*----------------------*/
	//	for( int i = 0 ; i < 20 ; i ++ )
	//	{
	//		axis = (CAxis)chartaxis.get_Custom(i);
	//		/* put automatic */
	//		axis.put_Automatic(1);
	//	}
	//}else
	//{
	//	if( auto_scale_g.line_cfg[0].auto_scale )
	//	{
	//		axis.put_Automatic(1);
	//	}else
	//	{
	//		axis.put_Automatic(0);
	//		axis.put_Minimum(auto_scale_g.line_cfg[0].min);
	//		axis.put_Maximum(auto_scale_g.line_cfg[0].max);
	//	}
	//	/*--- others ---------------------------------*/
	//	for( int i = 0 ; i < 19 ; i ++ )
	//	{
	//		axis = (CAxis)chartaxis.get_Custom(i);
	//		/*---------------------------------*/
	//		if( auto_scale_g.line_cfg[i+1].auto_scale )
	//		{
	//			axis.put_Automatic(1);
	//		}else
	//		{
	//			axis.put_Automatic(0);
	//			axis.put_Minimum(auto_scale_g.line_cfg[i+1].min);
	//			axis.put_Maximum(auto_scale_g.line_cfg[i+1].max);
	//		}
	//	}
	//	/*---------------------------------------------*/
	//}
	/*-------------------------------------------------*/
	//if( axis_changed != auto_scale_g.mutiple_axis )
	//{
	//	reflush_chart();
	//}
	/*-------------------------------------------------*/
}
void CTeeChart5_testDlg::OnBnClickedButton6()
{
	/*------------------------------*/
	for( int i = 0 ; i < 20 ; i ++ )
	{
		if( avs[20-i-1] == 1 )
		{
			/* clear */
			for( unsigned int j = 0 ; j < param_list_show.param_list_num ; j++ )
			{
				if( param_list_show.param_list[j].status != 0 )
				{
					if( param_list_show.param_list[j].line_num == 20-i-1 )
					{
						release_current_line(j);
						/*--------------------------*/
						if( color_index > 0 )
						{
							color_index --;
						}
						/*--------------------------*/
					}
				}
			}
			/*----------------------*/
			break;
			/*---------------------*/
			
		}
	}
	/* clear */
}


void CTeeChart5_testDlg::OnBnClickedButton8()
{
	if( COM.radio_status == 1 )
	{
		clear_all_exist_lines(0);
	}else
	{
		clear_all_line(0);
		last_postion = 0;
		lane_last_postion = 0;
		last_sd = 0;
		lat_pos_point = 0;
		version_ctrl = 0;
	}
	/* recreative the color table */
	create_color_table();
	/*----------------------------*/
}

void CTeeChart5_testDlg::check_list_show(unsigned char flags)
{
	/* */
	if( flags == 0 )
	{
		/**/
		/*m_list_ctrl.*/
		 GetDlgItem(IDC_LIST1)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_GROUD)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_CHECK2)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_CHECK3)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_CHECK5)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_CHECK4)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_COMBO2)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_BUTTON14)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_BUTTON26)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_BUTTON30)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_BUTTON22)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_BUTTON27)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_BUTTON29)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_EDIT2)->ShowWindow(SW_HIDE);
		 //GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
		/* set chart size */
		 CWnd *pWnd,*pWnd_li;   
		 CRect rect1,rect2;

		 pWnd = GetDlgItem(IDC_TCHART1);
		 pWnd->GetWindowRect(&rect1);
		 ScreenToClient(&rect1);

		 pWnd_li = GetDlgItem(IDC_LIST1);
		 pWnd_li->GetWindowRect(&rect2);
		 ScreenToClient(&rect2);
		 /*--------------------------*/
		 //rect1.right += rect2.Width();
		 rect1.right = m_rect.right;
		 /*--------------------------*/
		 pWnd->MoveWindow(rect1);
		 /* ------------- */
		 m_show_test.SetWindowTextW(_T("显示测试"));
		 /*-----*/
		 flags = 1;
	}else
	{
		/**/
		/*m_list_ctrl.*/
		 GetDlgItem(IDC_LIST1)->ShowWindow(SW_SHOW);
		 GetDlgItem(IDC_GROUD)->ShowWindow(SW_SHOW);
		 GetDlgItem(IDC_CHECK2)->ShowWindow(SW_SHOW);
		 GetDlgItem(IDC_CHECK3)->ShowWindow(SW_SHOW);
		 GetDlgItem(IDC_CHECK5)->ShowWindow(SW_SHOW);
		 GetDlgItem(IDC_CHECK4)->ShowWindow(SW_SHOW);
		 GetDlgItem(IDC_COMBO2)->ShowWindow(SW_SHOW);
		 GetDlgItem(IDC_BUTTON14)->ShowWindow(SW_SHOW);
		 GetDlgItem(IDC_BUTTON26)->ShowWindow(SW_SHOW);
		 GetDlgItem(IDC_BUTTON30)->ShowWindow(SW_SHOW);
		 GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);
		 GetDlgItem(IDC_BUTTON22)->ShowWindow(SW_SHOW);
		 GetDlgItem(IDC_BUTTON27)->ShowWindow(SW_SHOW);
		 GetDlgItem(IDC_BUTTON29)->ShowWindow(SW_SHOW);
		 GetDlgItem(IDC_EDIT2)->ShowWindow(SW_SHOW);
		 //GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
		/* set chart size */
		 CWnd *pWnd,*pWnd_li;   
		 CRect rect1,rect2;

		 pWnd = GetDlgItem(IDC_TCHART1);
		 pWnd->GetWindowRect(&rect1);
		 ScreenToClient(&rect1);

		 pWnd_li = GetDlgItem(IDC_LIST1);
		 pWnd_li->GetWindowRect(&rect2);
		 ScreenToClient(&rect2);
		 /*--------------------------*/
		 rect1.right -= rect2.Width() + (m_rect.right - rect2.right)*2;
		 /*--------------------------*/
		 pWnd->MoveWindow(rect1);
		 /* ------------- */
		 m_show_test.SetWindowTextW(_T("隐藏测试"));
		 /*-----*/
		 flags = 0;
	}
}

void CTeeChart5_testDlg::OnBnClickedButton9()
{   
#if !VERSION_CTRL
	system_config_inf.inf[1] ^= 1;
	check_list_show(system_config_inf.inf[1]);
	///* fresh the data */
	system_config_fresh(1,system_config_inf.inf[1]);
#else
	AfxMessageBox(_T("程序目录：使用说明.pdf"));
#endif
}

void CTeeChart5_testDlg::OnBnClickedButton7()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	//m_chart
	//CAxis chartaxis=(CAxis)m_chart.get_Axis();
	//CAxis chartaxisbottom=(CAxis)chartaxis.get_Bottom();
	//chartaxisbottom.Scroll(1.0,TRUE);
}

void CTeeChart5_testDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
    if(nType==1) 
	{
		return;//最小化则什么都不做  
	}
    // TODO: Add your message handler code here  
    CWnd *pWnd;   

    pWnd = GetDlgItem(IDC_TCHART1);  
    ChangeSize(pWnd, cx, cy,0);  

	pWnd = GetDlgItem(IDC_GROUD);  
	ChangeSize(pWnd, cx, cy,1);

	pWnd = GetDlgItem(IDC_CHECK2);  
	ChangeSize(pWnd, cx, cy,1);

	pWnd = GetDlgItem(IDC_CHECK3);  
	ChangeSize(pWnd, cx, cy,1);

	pWnd = GetDlgItem(IDC_CHECK5);  
	ChangeSize(pWnd, cx, cy,1);

	pWnd = GetDlgItem(IDC_CHECK4);  
	ChangeSize(pWnd, cx, cy,1);

	pWnd = GetDlgItem(IDC_COMBO2);  
	ChangeSize(pWnd, cx, cy,1);

	pWnd = GetDlgItem(IDC_BUTTON14);  
	ChangeSize(pWnd, cx, cy,1);

	pWnd = GetDlgItem(IDC_BUTTON26);  
	ChangeSize(pWnd, cx, cy,1);

	pWnd = GetDlgItem(IDC_BUTTON30);  
	ChangeSize(pWnd, cx, cy,1);

	pWnd = GetDlgItem(IDC_EDIT1);  
	ChangeSize(pWnd, cx, cy,1);

	pWnd = GetDlgItem(IDC_BUTTON22);  
	ChangeSize(pWnd, cx, cy,1);

	pWnd = GetDlgItem(IDC_BUTTON27);  
	ChangeSize(pWnd, cx, cy,1);

	pWnd = GetDlgItem(IDC_BUTTON29);  
	ChangeSize(pWnd, cx, cy,1);

	pWnd = GetDlgItem(IDC_SLIDER1);  
	ChangeSize(pWnd, cx, cy,1);

	pWnd = GetDlgItem(IDC_STATIC);  
	ChangeSize(pWnd, cx, cy,1);

	pWnd = GetDlgItem(IDC_EDIT2);
	ChangeSize(pWnd, cx, cy,1); 

	pWnd = GetDlgItem(IDC_LIST1);
	ChangeSize(pWnd, cx, cy,0);  



    GetClientRect(&m_rect);// 将变化后的对话框大小设为旧大小   
}
void CTeeChart5_testDlg::ChangeSize(CWnd *pWnd, int cx, int cy,unsigned int mode)  
{  
    if(pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建   
    {  
        CRect rect;   //获取控件变化前的大小    
        pWnd->GetWindowRect(&rect);  
        ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标  

        rect.left=rect.left*cx/m_rect.Width();//调整控件大小  
        rect.right=rect.right*cx/m_rect.Width();  

		if( mode == 1 )
		{
			rect.top=rect.top*cy/m_rect.Height();  
		}else
		{
			rect.top=rect.top;//*cy/m_rect.Height();  
		}
        
        rect.bottom=rect.bottom*cy/m_rect.Height();  
        pWnd->MoveWindow(rect);//设置控件大小  
    }  
}  

void CTeeChart5_testDlg::On32771()//open file
{
	//C_open openDlg;
	//openDlg.DoModal();
	TCHAR szFilter[] = _T("所支持的文件|*.*|"); 

    CFileDialog fileDlg(TRUE, _T("open file"), NULL, 0, szFilter, this); 

	if( COM.radio_status == 1 )
	{
        MessageBox(_T("当前检测到电台已连接,请拔掉电台之后，重启软件，才可进入log模式"),_T("tips"),0);
		return;
	}

    if(IDOK == fileDlg.DoModal())   
    {   
        CString cFilePath = fileDlg.GetPathName();  
		/* transfer */
		USES_CONVERSION;
		/* transfer */
		char * path_c = T2A(cFilePath);
        /*----------*/
		/* get name */
		int file_sec = allocate_file_area(path_c, strlen(path_c));
		/* if */
		if( file_sec == (-1) )
		{
            MessageBox(_T("最多支持10个文件"),_T("tips"),0);
		    return;
		}
    }
}

/* extern the define form c_opon.cpp */
int CTeeChart5_testDlg::allocate_file_area( char * path , unsigned int len)
{
	/* loop for check */
	for( int j = 0 ; j < 10 ; j ++ )
	{
		if( file_man.file[j].file_enable == 0 )
		{
			memset(file_man.file[j].file_path,0,sizeof(file_man.file[j].file_path));
			memset(file_man.file[j].file_name,0,sizeof(file_man.file[j].file_name));
			memset(file_man.file[j].file_point,0,sizeof(file_man.file[j].file_point));
			memset(file_man.file[j].file_tmp,0,sizeof(file_man.file[j].file_tmp));
			/* copy */
			for( unsigned int i = 0 ; i < len ; i++)
			{
				file_man.file[j].file_path[i] = path[i];
			}
			/*-----------*/
			file_man.num++;
			/*------------*/
			file_man.file[j].file_enable = 1;
			/* do decode */
	        Read_Procotol_decode_waves(j);
			/* return ok */
			return j;
		}
	}
	/*-----------*/
	return (-1);
}
/* drop files */
void CTeeChart5_testDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT count;            
    WCHAR  filePath[256];       
    char path_c[512];
	unsigned int i;
	int file_sec;
	/* --- */
	if( COM.radio_status == 1 )
	{
        MessageBox(_T("当前检测到电台已连接,请拔掉电台之后，重启软件，才可进入log模式"),_T("tips"),0);
		return;
	}
    /*--------------------------------*/
	memset(filePath,0,sizeof(filePath));
	memset(path_c,0,sizeof(path_c));
    /*--------------------------------*/
    count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);   //系统的API函数   
    /* get count */      
    if(count)             
    {
        for( i = 0; i < count ; i++)                      
        {  
            int pathLen = DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));  //API函数  

	        USES_CONVERSION;

	        char * file_name = T2A(filePath);
			/*-----------------------------*/
			memcpy(path_c,file_name,strlen(file_name));
			/* get name */
		    file_sec = allocate_file_area(path_c, sizeof(path_c));
		    /* if */
		    if( file_sec == (-1) )
		    {
                MessageBox(_T("最多支持10个文件"),_T("tips"),0);
		    	return;
		    }
        }  
    }
    DragFinish(hDropInfo);   //API函数
	CDialogEx::OnDropFiles(hDropInfo);
}


void CTeeChart5_testDlg::On32781()
{
	// TODO: 在此添加命令处理程序代码
	C_PROCOTOL_CREATE procotol_create;
	procotol_create.DoModal();
}
/*---------read and decode area-----------*/
void CTeeChart5_testDlg::Read_Procotol_decode_waves(unsigned int index)
{
	char buffer[256];
	int len_str = strlen(file_man.file[index].file_path);
	int i,len_point = 0;
	unsigned int start_addr;
	unsigned int log_type = 0;
	CString show;
    USES_CONVERSION;
	/*---------------*/
	memset(buffer,0,sizeof(buffer));
	/* get file .xxx */
	for( i = 0 ; i < len_str ; i++ )
	{
		if( file_man.file[index].file_path[ len_str - 1 - i ] == '\\' )
		{
			if( ( i - len_point - 1 ) <= 32 )
			{
			    memcpy( file_man.file[index].file_name , &file_man.file[index].file_path[ len_str - i ],i - len_point - 1);
				/* point */
				memcpy( file_man.file[index].file_point,&file_man.file[index].file_path[ len_str - len_point ],len_point);
			}else
			{
				file_man.file[index].file_enable = 0;
				MessageBox(_T("文件名长度超过32"),_T("tips"),0);
				return;
			}
			break;
		}
		if( len_point == 0 && file_man.file[index].file_path[ len_str - 1 - i ] == '.' )
		{
			len_point = i;
		}
	}
	/* read cfs files */
	FILE * pf_cfs_file;
#if VERSION_CTRL
	if( strstr(file_man.file[index].file_point,"bin") != NULL )
	{
		/* open */
		fopen_s(&pf_cfs_file,"D200_log.CFS","rb");
		/* log type */
		log_type = 1;
		/* open ok ? */
		if( pf_cfs_file == NULL )
		{
			MessageBox(_T("找不到.bin文件对应的配置文件D200_log.CFG"),_T("tips"),0);
			return;
		}
	}
#if 0
	else if( strstr(file_man.file[index].file_point,"gsof") != NULL )
	{
		/* open */
		fopen_s(&pf_cfs_file,"D200_GSOF.CFS","rb");
		/* open ok ? */
		if( pf_cfs_file == NULL )
		{
			MessageBox(_T("找不到.gsof文件对应的配置文件D200_GSOF.CFG"),_T("tips"),0);
			return;
		}
	}else if( strstr(file_man.file[index].file_point,"gim") != NULL )
	{
		/* open */
		fopen_s(&pf_cfs_file,"D200_GIM.CFS","rb");
		/* open ok ? */
		if( pf_cfs_file == NULL )
		{
			MessageBox(_T("找不到.gim文件对应的配置文件D200_GIM.CFG"),_T("tips"),0);
			return;
		}
	}else if( strstr(file_man.file[index].file_point,"imr") != NULL )
    {
		/* open */
		fopen_s(&pf_cfs_file,"D200_IMR.CFS","rb");
		/* open ok ? */
		if( pf_cfs_file == NULL )
		{
			MessageBox(_T("找不到.imr文件对应的配置文件D200_IMR.CFG"),_T("tips"),0);
			return;
		}
	}
#endif
	else
#endif
	{
		if( strcmp(file_man.file[index].file_point,"bin") == 0 )
		{
			/* log type */
			log_type = 1;
		}
		char buffer_other[256];
		/* procotol */
		if( file_man.file[index].default_procotol_type == 0 )
		{
			sprintf_s(buffer_other,"%s.CFS",file_man.file[index].file_point);
		}
		else
		{
			strcpy_s(buffer_other,file_man.file[index].procotol_file);
		}
		/* open */
		fopen_s(&pf_cfs_file,buffer_other,"rb");
		/* open ok ? */
		if( pf_cfs_file == NULL )
		{
			m_taps.SetWindowTextW(_T("未找到配置文件，重新选择"));
			Open_cfg();//select one file
			if( file_man.file[index].default_procotol_type == 0 )
			{
				sprintf_s(buffer_other,"%s.CFS",file_man.file[index].file_point);
			}
			else
			{
				strcpy_s(buffer_other,file_man.file[index].procotol_file);
			}
	        /* open */
		    fopen_s(&pf_cfs_file,buffer_other,"rb");
		    /* open ok ? */
		    if( pf_cfs_file == NULL )
		    {
				char buffer_tmp[1024];

				sprintf_s(buffer_tmp,"找不到.%s文件对应的配置文件 %s",file_man.file[index].file_point,buffer_other);

				CString d = A2T(buffer_tmp);

				MessageBox(d,_T("tips"),0);
				/* clear some msg */
				file_man.file[index].file_enable = 0;
				file_man.num --;
				/*-------------------------------------*/
				return;
			}
		}
	}
	/* found */
	m_taps.SetWindowTextW(_T("找到配置文件,解析中..."));
	/*--------read data to CFS FILE-----------*/
	int len = fread(&READ_CFS,1,sizeof(READ_CFS),pf_cfs_file);
	/*  lose some data */
	if( len == sizeof(READ_CFS) )
	{
		MessageBox(_T("有一些数据可能会丢失"),_T("tips"),0);
	}
	/*  deal with some data */
	if( READ_CFS.cfs_global_msg.procotol_type == 2 )//persional
	{
		if( READ_CFS.cfs_global_msg.procotol_select == 0 )
		{
			MessageBox(_T("未选择自定义协议，需要重新生成配置文件"),_T("tips"),0);
			/* clear some msg */
			file_man.file[index].file_enable = 0;
			file_man.num --;
			/*-------------------------------------*/
			return;
		}
		/*---------------system supply-----------------*/
		if( READ_CFS.cfs_global_msg.procotol_select == 2 )
		{
			//RT27
			if( rt27_decode(file_man.file[index].file_path,file_man.file[index].file_tmp,sizeof(file_man.file[index].file_tmp)) != 0 )
			{
			   return;
			}
		}else if( READ_CFS.cfs_global_msg.procotol_select == 3 )
		{
			//FPOS
			if( fpos_process(file_man.file[index].file_path,file_man.file[index].file_tmp,0) != 0 )
			{
				return;
			}
			/*---------------------------------------*/
		}else if( READ_CFS.cfs_global_msg.procotol_select == 4 )
		{
			//POS
			if( fpos_process(file_man.file[index].file_path,file_man.file[index].file_tmp,1) != 0 )
			{
				return;
			}
			/*-----------------------------------------------*/
		}else if( READ_CFS.cfs_global_msg.procotol_select == 1 )
		{
			MessageBox(_T("暂不支持外部协议"),_T("tips"),0);
			return;
		}else
		{
			MessageBox(_T("未知协议"),_T("tips"),0);
			/* clear some msg */
			file_man.file[index].file_enable = 0;
			file_man.num --;
			/*-------------------------------------*/
			return;
		}
	}
	/*-------------------------------------*/
	if( param_list_show.param_list_num == 0 )
	{
		m_combox_param_show.ResetContent();
	}
	/*---- show position clear ----*/
	param_list_show.now_num = 0;
	start_addr = param_list_show.param_list_num;
	/* get file name */
	char file_name_buffer_tmp[64];
	char file_point_buffer_tmp[32];
	unsigned int len_fnb = 0;
	unsigned int len_point_tmp = 0;
	/*-------------------*/
	memset(file_name_buffer_tmp,0,sizeof(file_name_buffer_tmp));
	memset(file_point_buffer_tmp,0,sizeof(file_point_buffer_tmp));
	/* get file name len */
	len_fnb = strlen(file_man.file[index].file_name);
	len_point_tmp = strlen(file_man.file[index].file_point);
	/* length than 8 ? */
	if(  len_fnb > 8 )
	{
       len_fnb = 8;
	}
	/* length than 32 */
	if( len_point_tmp > 8 )
	{
		len_point_tmp = 8;
	}
	/* copy data */
	memcpy(file_name_buffer_tmp,file_man.file[index].file_name,len_fnb);
	memcpy(file_point_buffer_tmp,file_man.file[index].file_point,len_point_tmp);
	/* get file size */
	FILE * pf_read_log;
	/* open */
	if( READ_CFS.cfs_global_msg.procotol_select == 0 )
	{
	    fopen_s(&pf_read_log,file_man.file[index].file_path,"rb");
	}else
	{
		fopen_s(&pf_read_log,file_man.file[index].file_tmp,"rb");
	}
	/* log file read fail */
	if( pf_read_log == NULL )
	{
		MessageBox(_T("日志文件打开失败"),_T("tips"),0);
		/* clear some msg */
		file_man.file[index].file_enable = 0;
		file_man.num --;
		/*-------------------------------------*/
		return;
	}
	/*----------------------*/
	/* get file size */
	fseek(pf_read_log,0,SEEK_END);
	unsigned int file_size = ftell(pf_read_log);
	fseek(pf_read_log,0,SEEK_SET);
	/* macclos the file areas */
	if( file_size == 0 )
	{
		MessageBox(_T("文件为空"),_T("tips"),0);
		/*------------------------------------*/
		/* clear some msg */
		file_man.file[index].file_enable = 0;
		file_man.num --;
		/*-------------------------------------*/
		return;
	}
	/* flag */
	int allocate_flag = 0;
	unsigned char * memory_t = 0;
	/*---------------param_list------------*/
	for( unsigned int i = 0 ; i < READ_CFS.cfs_global_msg.sample_num ; i ++ )
	{
		sprintf_s(param_list_show.param_list[param_list_show.param_list_num].name,"%s:%s->%s",
		file_man.file[index].file_point,file_name_buffer_tmp,READ_CFS.pmd[i].name);
		/* add to combox */
		show = A2T(param_list_show.param_list[param_list_show.param_list_num].name);
		/*--------------*/
		m_combox_param_show.AddString(show);
		/* allocate the line */
		param_list_show.param_list[param_list_show.param_list_num].point_num = file_size / READ_CFS.cfs_global_msg.block_size;
		/* allocate memory */
		/*----malloc the data*/
		param_list_show.param_list[param_list_show.param_list_num].data_y = 
		(unsigned char *)malloc(param_list_show.param_list[param_list_show.param_list_num].point_num*8);//double
		/*---------------------*/
		if( param_list_show.param_list[param_list_show.param_list_num].data_y == NULL )
		{
			MessageBox(_T("内存分配失败"),_T("tips"),0);
			/* clear some msg */
			file_man.file[index].file_enable = 0;
			file_man.num --;
			/*-------------------------------------*/
			return;
		}
		/* allocate first */
		if( allocate_flag == 0 )
		{
			allocate_flag = 1;
			param_list_show.param_list[param_list_show.param_list_num].data_x = 
			(unsigned char *)malloc(param_list_show.param_list[param_list_show.param_list_num].point_num*8);//double
			/*---------------------*/
			if( param_list_show.param_list[param_list_show.param_list_num].data_x == NULL )
			{
				MessageBox(_T("内存分配失败"),_T("tips"),0);
				/* clear some msg */
				file_man.file[index].file_enable = 0;
				file_man.num --;
				/*-------------------------------------*/
				return;
			}
			/* save memory addr */
			memory_t = param_list_show.param_list[param_list_show.param_list_num].data_x;
		}
		else
		{
			param_list_show.param_list[param_list_show.param_list_num].data_x = memory_t;
		}
        /*------------------------------*/
		param_list_show.param_list[ param_list_show.param_list_num ].mark = READ_CFS.pmd[i].mark;
		/*---------------------------------*/
		strcpy_s(param_list_show.param_list[ param_list_show.param_list_num ].from_file,file_man.file[index].file_path);
		/*---------------------------------*/
		param_list_show.param_list[ param_list_show.param_list_num ].time_us = READ_CFS.cfs_global_msg.time_mark;
		/*---------------------------------*/
		param_list_show.param_list[ param_list_show.param_list_num ].line_type = READ_CFS.pmd[i].line_type;
		/* hot key */
		memcpy(param_list_show.param_list[ param_list_show.param_list_num ].hot_key,READ_CFS.pmd[i].hot_key,16);
		/* function about */
		if( READ_CFS.pmd[i].math_type == 0xff )
		{
			param_list_show.param_list[ param_list_show.param_list_num ].function_enable = 1;
			param_list_show.param_list[ param_list_show.param_list_num ].func = READ_CFS.pmd[i].width;
			param_list_show.param_list[ param_list_show.param_list_num ].func_param_count = READ_CFS.pmd[i].if_num;
			memcpy(param_list_show.param_list[ param_list_show.param_list_num ].func_param,&READ_CFS.pmd[i].buffer[150],5*16);
		}
		/*----------------*/
		param_list_show.param_list_num++;	
		/* over */
		if( param_list_show.param_list_num >= 512 )
		{
			MessageBox(_T("文件中包含的曲线数量大于512"),_T("tips"),0);
			/* clear some msg */
			file_man.file[index].file_enable = 0;
			file_man.num --;
			/*-------------------------------------*/
			return;			
		}
	}
	/* show the first */
	m_combox_param_show.SetCurSel(param_list_show.param_list_num-1);
	/* offset */
	if( READ_CFS.cfs_global_msg.offset != 0 )
	{
		fseek(pf_read_log,READ_CFS.cfs_global_msg.offset,SEEK_SET);
	}
	/* get file size */
	while(1)
	{
		unsigned int len = fread(read_buffer_block,1,READ_CFS.cfs_global_msg.block_size,pf_read_log);
		/* seek */
		if( len < READ_CFS.cfs_global_msg.block_size )
		{
			break;
		}
		/*--------decode the data----------*/
		//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*(&(*&(&(*&(*^*(&^(*%^&$%*^&%(&%(*^(&*&*^(
		math_and_line(read_buffer_block,len,start_addr);
	}
	/* deal with create and functions */
	function_thread(start_addr);
	/* taps */
	m_taps.SetWindowTextW(_T("解析完成"));
	/* close file */
	if( pf_read_log != NULL )
	{
		fclose(pf_read_log);
	}
	/* check opened */
	if( pf_cfs_file != NULL )
	{
		fclose(pf_cfs_file);
	}
	/* -- ok -- */
	// start check the some tools
	Arm_Distortion(file_man.file[index].file_path,log_type);
}
int CTeeChart5_testDlg::math_and_line(unsigned char *data,unsigned int len,unsigned int start)
{
	int real_len = 0;
	double * line_data;
	unsigned char buffer[8];
	/*--------------------*/
	for( unsigned int i = 0 ; i < READ_CFS.cfs_global_msg.sample_num ; i ++ )
	{
		line_data = (double *)param_list_show.param_list[ i + start ].data_y;
		/* create or normal */
	   if( READ_CFS.pmd[i].math_type != 0xFF )
	   {
			/* cle */
			memcpy(buffer,&data[ READ_CFS.pmd[i].offset ] , READ_CFS.pmd[i].width );
			/* math */
			//------dsf----
			double tmp;
			/* loop value */
			if( math_transfer(data,buffer,i,&tmp) == 0 )
			{
			   line_data[param_list_show.now_num] = tmp;
			   last_dou[i] = tmp;
			}else
			{
				line_data[param_list_show.now_num] = last_dou[i];
			}
	    }
	    else
		{
			line_data[param_list_show.now_num] = 0xff;
	    }
		/*------------*/
		if( i == 0 )
		{
			/* set x axis */
			line_data = (double *)param_list_show.param_list[ i + start ].data_x;
			/* set data */
			if( READ_CFS.cfs_global_msg.time_mark != 0 )
			{
				line_data[param_list_show.now_num] = 
					(double)param_list_show.now_num * (double)READ_CFS.cfs_global_msg.time_mark / (double)1000000;
			}else
			{
				line_data[param_list_show.now_num] = param_list_show.now_num;
			}
		}
	}
	param_list_show.now_num++;
	return 0;
}
/*--------*/
double CTeeChart5_testDlg::Short2Float(double a, double b) 
{
	double c = 0.0f;
	c = a / 32768.0f * b;
	return c;
}
/* little */
void CTeeChart5_testDlg::big2little_len(unsigned char * src,unsigned char * dst,unsigned char len)
{
	for(int i=0;i<len;i++)
	{
		dst[i] = src[ len - 1 - i ];
	}
}
/* decode */
int CTeeChart5_testDlg::math_transfer(unsigned char * base,unsigned char * buffer,unsigned int index ,double * value)
{
	double ret;
	unsigned char tmp_buffer[8];
	/* get  if */
	if( READ_CFS.pmd[index].if_param_value[0] != 0 || READ_CFS.pmd[index].if_param_value[1] != 0 )
	{
		unsigned int i;
        /* It has a if param */
        for( i = 0 ; i < READ_CFS.cfs_global_msg.sample_num ; i ++ )
        {
			if( strcmp(READ_CFS.pmd[i].name ,"count") == 0 )//READ_CFS.pmd[index].if_param_string ) this version only supply COUNT
			{
                memcpy( tmp_buffer,&base[ READ_CFS.pmd[i].offset ] , READ_CFS.pmd[i].width);
                /* type */
                if( READ_CFS.pmd[i].type == 1 || READ_CFS.pmd[i].type == 2 )
                {
                   unsigned char * tmp = (unsigned char*)tmp_buffer;
                   /* force */
                   unsigned char dt = (unsigned char)READ_CFS.pmd[index].if_param_value[0];
                   /*--------------------------------------------------------------------*/
                   if( !(( (*tmp) & dt ) == (unsigned char)READ_CFS.pmd[index].if_param_value[1] ))
                   {
                   	  // if no math 
                      return (-1);
                   }else
                   {
                   	  break;
                   }
                   /*--------------------------------------------------------------------*/
                }else if( READ_CFS.pmd[i].type == 3 || READ_CFS.pmd[i].type == 4 )
                {
                   unsigned short * tmp = (unsigned short*)tmp_buffer;
                   /* force */
                   unsigned short dt = (unsigned short)READ_CFS.pmd[index].if_param_value[0];
				   /* math */
				   dt &= (*tmp);
				   /* get equal */
				   unsigned short acx = (unsigned short)READ_CFS.pmd[index].if_param_value[1];
                   /*--------------------------------------------------------------------*/
                   if( ! ( acx == dt ) )
                   {
                   	  // if no math 
                      return (-1);
                   }else
                   {
                   	  break;
                   }
                }else if( READ_CFS.pmd[i].type == 5 || READ_CFS.pmd[i].type == 6 )
                {
                   unsigned int * tmp = (unsigned int*)tmp_buffer;
                   /* force */
                   unsigned int dt = (unsigned int)READ_CFS.pmd[index].if_param_value[0];
                   /*--------------------------------------------------------------------*/
                   if( !(( (*tmp) & dt ) == (unsigned int)READ_CFS.pmd[index].if_param_value[1] ))
                   {
                   	  // if no math 
                      return (-1);
                   }else
                   {
                   	  break;
                   }
                }else
                {
					MessageBox(_T("错误的参数"),_T("tips"),0);
					return (-1);                	
			    }
			}
        }
	}
	/* S2F ? */
	if( READ_CFS.pmd[index].math_type == 1 )
	{
		if( READ_CFS.pmd[index].little == 0 )
		{
			memcpy(tmp_buffer,buffer,8); // little ending
		}else
		{
			big2little_len(buffer,tmp_buffer,READ_CFS.pmd[index].width); // big ending
		}
		/* type */
		if( READ_CFS.pmd[index].type == 1 )
		{
			unsigned char * tmp = (unsigned char*)tmp_buffer;
		    ret = Short2Float((double)(*tmp),(double)READ_CFS.pmd[index].f_math[0]);
		}else if( READ_CFS.pmd[index].type == 2 )
		{
			char * tmp = (char*)tmp_buffer;
		    ret = Short2Float((double)(*tmp),(double)READ_CFS.pmd[index].f_math[0]);
		}else if( READ_CFS.pmd[index].type == 3 )
		{
			unsigned short * tmp = ( unsigned short * )tmp_buffer;
		    ret = Short2Float((double)(*tmp),(double)READ_CFS.pmd[index].f_math[0]);
		}else if( READ_CFS.pmd[index].type == 4 )
		{
			short * tmp = (short *)tmp_buffer;
		    ret = Short2Float((double)(*tmp),(double)READ_CFS.pmd[index].f_math[0]);
		}else if( READ_CFS.pmd[index].type == 5 )
		{
			unsigned int * tmp = ( unsigned int * )tmp_buffer;
		    ret = Short2Float((double)(*tmp),(double)READ_CFS.pmd[index].f_math[0]);
		}else if( READ_CFS.pmd[index].type == 6 )
		{
			int * tmp = ( int * )tmp_buffer;
		    ret = Short2Float((double)(*tmp),(double)READ_CFS.pmd[index].f_math[0]);
		}else if( READ_CFS.pmd[index].type == 7 )
		{
			float * tmp = ( float * )tmp_buffer;
		    ret = Short2Float((double)(*tmp),(double)READ_CFS.pmd[index].f_math[0]);
		}else if( READ_CFS.pmd[index].type == 8 )
		{
			double * tmp = ( double * )tmp_buffer;
		    ret = Short2Float((double)(*tmp),(double)READ_CFS.pmd[index].f_math[0]);
		}else if( READ_CFS.pmd[index].type == 9 )//should not appear
		{
			char * tmp = (char*)tmp_buffer;
		    ret = Short2Float((double)(*tmp),(double)READ_CFS.pmd[index].f_math[0]);
		}else if( READ_CFS.pmd[index].type == 10 )//should not appear
		{
			char * tmp = (char*)tmp_buffer;
		    ret = Short2Float((double)(*tmp),(double)READ_CFS.pmd[index].f_math[0]);
		}else
		{
			/* should not appear */
			//MessageBox(_T("参数错误"),_T("tips"),0);
			short * tmp = ( short * )tmp_buffer;
		    ret = Short2Float((double)(*tmp),(double)READ_CFS.pmd[index].f_math[0]);
		}
		/* run */
		if( READ_CFS.pmd[index].math_number >= 3 )
		{
			for( unsigned int i = 1 ; i < READ_CFS.pmd[index].math_number ; i +=2 )
			{
				if( READ_CFS.pmd[index].asmf_type[(i-1)/2] == '*' )
				{
					ret *= READ_CFS.pmd[index].f_math[(i-1)/2 +1 ];
				}else if( READ_CFS.pmd[index].asmf_type[(i-1)/2] == '/' )
				{
					ret /= READ_CFS.pmd[index].f_math[(i-1)/2 +1 ];
				}else if( READ_CFS.pmd[index].asmf_type[(i-1)/2] == '+' )
				{
					ret += READ_CFS.pmd[index].f_math[(i-1)/2 +1 ];
				}else if( READ_CFS.pmd[index].asmf_type[(i-1)/2] == '-' )
				{
					ret -= READ_CFS.pmd[index].f_math[(i-1)/2 +1 ];
				}else
				{
					MessageBox(_T("运算符错误"),_T("tips"),0);
					return (-1);
				}
			}
		}
		/* set value */
		*value = ret;
		return 0;
	}else if( READ_CFS.pmd[index].math_type == 2 )
	{
		/* change big ending */
		if( READ_CFS.pmd[index].little == 0 )
		{
			memcpy(tmp_buffer,buffer,8); // little ending
		}else
		{
			big2little_len(buffer,tmp_buffer,READ_CFS.pmd[index].width); // big ending
		}
        /* type */
		if( READ_CFS.pmd[index].type == 1 )
		{
			unsigned char * tmp = (unsigned char*)tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 2 )
		{
			char * tmp = (char*)tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 3 )
		{
			unsigned short * tmp = ( unsigned short * )tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 4 )
		{
			short * tmp = (short *)tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 5 )
		{
			unsigned int * tmp = ( unsigned int * )tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 6 )
		{
			int * tmp = ( int * )tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 7 )
		{
			float * tmp = ( float * )tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 8 )
		{
			double * tmp = ( double * )tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 9 )//should not appear
		{
			char * tmp = (char*)tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 10 )// bit should not appear
		{
			if( READ_CFS.pmd[index].asmf_type[0] == '&' )
			{
				unsigned char red = ( unsigned char )READ_CFS.pmd[index].f_math[0];
				/* */
				unsigned char red_tmp = red & tmp_buffer[0];
				/* right */
				for( int i = 0 ; i < 8 ; i ++ )
				{
					if( red & 0x1 )
					{
						break;
					}else
					{
						red_tmp >>= 1;
					}
					/*----------------*/
					red >>= 1;
				}
				/*-------------------------*/
			    ret = (double)(red_tmp);
				/*--------*/
				*value = ret;
				/*--------*/
				return 0;
			}
		}else
		{
			/* should not appear */
			//MessageBox(_T("参数错误"),_T("tips"),0);
			short * tmp = ( short * )tmp_buffer;
		    ret = (double)(*tmp);
		}
		/*-------------------------------------*/
		if( READ_CFS.pmd[index].math_number >= 2 )
		{
			for( unsigned int i = 0 ; i < READ_CFS.pmd[index].math_number ; i +=2 )
			{
				if( READ_CFS.pmd[index].asmf_type[i/2] == '*' )
				{
					ret *= READ_CFS.pmd[index].f_math[i/2];
				}else if( READ_CFS.pmd[index].asmf_type[i/2] == '/' )
				{
					ret /= READ_CFS.pmd[index].f_math[i/2];
				}else if( READ_CFS.pmd[index].asmf_type[i/2] == '+' )
				{
					ret += READ_CFS.pmd[index].f_math[i/2];
				}else if( READ_CFS.pmd[index].asmf_type[i/2] == '-' )
				{
					ret -= READ_CFS.pmd[index].f_math[i/2];
				}else
				{
					MessageBox(_T("运算符错误"),_T("tips"),0);
					return (-1);
				}	
			}				 	
		}
		/* set value */
		*value = ret;
		return 0;
	}
    else // NONE
	{
	    /* change big ending */
		if( READ_CFS.pmd[index].little == 0 )
		{
			memcpy(tmp_buffer,buffer,8); // little ending
		}else
		{
			big2little_len(buffer,tmp_buffer,READ_CFS.pmd[index].width); // big ending
		}
        /* type */
		if( READ_CFS.pmd[index].type == 1 )
		{
			unsigned char * tmp = (unsigned char*)tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 2 )
		{
			char * tmp = (char*)tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 3 )
		{
			unsigned short * tmp = ( unsigned short * )tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 4 )
		{
			short * tmp = (short *)tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 5 )
		{
			unsigned int * tmp = ( unsigned int * )tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 6 )
		{
			int * tmp = ( int * )tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 7 )
		{
			float * tmp = ( float * )tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 8 )
		{
			double * tmp = ( double * )tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 9 )//should not appear
		{
			char * tmp = (char*)tmp_buffer;
		    ret = (double)(*tmp);
		}else if( READ_CFS.pmd[index].type == 10 )// bit should not appear
		{
			short * tmp = ( short * )tmp_buffer;
		    ret = (double)(*tmp);
		}else
		{
			/* should not appear */
			//MessageBox(_T("参数错误"),_T("tips"),0);
			short * tmp = ( short * )tmp_buffer;
		    ret = (double)(*tmp);
		}
		/* set value */
		*value = ret;
		return 0;
	}
}
/* lines chart init */
void CTeeChart5_testDlg::chart_line_init(void)
{
    /* get */
	for( int i = 0 ; i < 50 ; i ++ )
	{
		/* create lines */
		CSeries line_cfs = (CSeries)m_chart.Series(i);
		/*-------------*/
		line_cfs = (CSeries)m_chart.Series(i);
		/*-------------*/
		line_cfs.put_ShowInLegend(0);
		/*-------------*/
		avs[i] = 0;
	}
}
void * CTeeChart5_testDlg::Get_line_source(unsigned int *num)
{
	/*----------------------*/
	for( int i = 0 ; i < 20 ; i ++ )
	{
		if( avs[i] == 0 )
		{
			avs[i] = 1;
			*num = i;
			return ( void * )0x12345678;
		}
	}
	/*   */
	return 0;
}
void * CTeeChart5_testDlg::Get_2axis_source(unsigned int *num)
{
	/*----------------------*/
	for( int i = 20 ; i < 30 ; i ++ )
	{
		if( avs[i] == 0 )
		{
			avs[i] = 1;
			*num = i;
			return ( void * )0x12345678;
		}
	}
	/*   */
	return 0;
}
void * CTeeChart5_testDlg::Get_point_source(unsigned int *num)
{
	/*----------------------*/
	for( int i = 30 ; i < 40 ; i ++ )
	{
		if( avs[i] == 0 )
		{
			avs[i] = 1;
			*num = i;
			return ( void * )0x12345678;
		}
	}
	/*   */
	return 0;
}
void * CTeeChart5_testDlg::Get_star_source(unsigned int *num)
{
	/*----------------------*/
	for( int i = 40 ; i < 50 ; i ++ )
	{
		if( avs[i] == 0 )
		{
			avs[i] = 1;
			*num = i;
			return ( void * )0x12345678;
		}
	}
	/*   */
	return 0;
}
/*-----------------------------*/
void CTeeChart5_testDlg::release_current_line(unsigned int num)
{
	/* has drawed . then clear */
	if( param_list_show.param_list[num].status == 0xff )
	{
		CSeries line_cfs = (CSeries)m_chart.Series(param_list_show.param_list[num].line_num);
		line_cfs.Clear();
		line_cfs.put_ShowInLegend(0);
		avs[param_list_show.param_list[num].line_num] = 0;
		param_list_show.param_list[num].status = 0;
	}
}
/*----------------------------*/
void CTeeChart5_testDlg::Line_flash(void)
{

}
void CTeeChart5_testDlg::Legend_handle( unsigned int mode )
{
	CLegend0 legend = (CLegend0)m_chart.get_Legend();
	/*----------------*/
	if( mode == 0 )
	{
		legend.put_Visible(FALSE);
	}else
	{
		legend.put_Visible(TRUE);
	}
}
/*---------------------------*/
void CTeeChart5_testDlg::clear_all_line(unsigned int mode)
{
	//for( unsigned int i = 0 ; i < 50 ; i ++ )
	//{
	//	CSeries line_cfs = (CSeries)m_chart.Series(i);
	//	/* setting */
	//	line_cfs.Clear();
	//	line_cfs.put_ShowInLegend(0);
	//	avs[i] = 0;
	//}
	for( unsigned int i = 0 ; i < param_list_show.param_list_num ; i ++ )
	{
		if( param_list_show.param_list[i].status == 0xff )
		{
			int seq = param_list_show.param_list[i].line_num;
			/*-------------*/
			CSeries line_cfs = (CSeries)m_chart.Series(seq);
			/* setting */
			line_cfs.Clear();
			line_cfs.put_ShowInLegend(0);
			avs[seq] = 0;
		}
	}
	/*----------------*/
	if( mode == 0 )
	{
		for( unsigned int i = 0 ; i < param_list_show.param_list_num ; i ++ )
		{
		   param_list_show.param_list[i].status = 0;
		}
	}
	else
	{
		for( unsigned int i = 0 ; i < param_list_show.param_list_num ; i ++ )
		{
			if( param_list_show.param_list[i].status == 0xff )
			{
				param_list_show.param_list[i].status = 0xFE;
			}
		}
	}
	/*--------------*/
    Legend_handle(0);//hide
	/*-----------*/
	axis_reset();
	/*------------------*/
	color_index = 0;
	/* recreative the color table */
	create_color_table();
	/*----------------------------*/
}
/*--------------------------*/
void CTeeChart5_testDlg::draw_axis(unsigned int num_index)
{
	CAxes chartaxis =(CAxes)m_chart.get_Axis();
	/* set */
	int t_lnk = param_list_show.param_list[num_index].line_num;
	/* create line data */
	CSeries line_now = (CSeries)m_chart.Series(t_lnk);
	/* mutiple axis supple flags */
	if( param_list_show.param_list[num_index].axis_multiple == 0 )
	{
		/* return */
		return;
	}
	/* multiple */
	CAxis Custom_axis;
	CAxisTitle Custom_title;
	CTeeFont font;
	CPen0 pen;
	CAxisLabels label;
	/*-----------------------------*/
	if( t_lnk >= 1 )
	{
	   Custom_axis = (CAxis)chartaxis.get_Custom(t_lnk-1);
	}
	else
	{
		Custom_axis = (CAxis)chartaxis.get_Left();
	}
	/* set position */
	Custom_axis.put_PositionPercent(t_lnk*5);
	/* get title */
	Custom_title = Custom_axis.get_Title();
	/* put vision */
	Custom_axis.put_Visible(1);
	/* auto or not */
	if( param_list_show.param_list[num_index].axis_auto == 1 )
	{
		//manul
		Custom_axis.put_Automatic(0);
		/* min and max */
		Custom_axis.put_Minimum(param_list_show.param_list[num_index].axis_min);
		Custom_axis.put_Maximum(param_list_show.param_list[num_index].axis_max);
	}
	else
	{
		Custom_axis.put_Automatic(1);
	}
	/* transfer */
	USES_CONVERSION;
	/*----------------------*/
	CString show = A2T(param_list_show.param_list[num_index].name);
	/*-------------------*/
	Custom_title.put_Caption(show);
	Custom_title.put_Angle(90);
	Custom_title.put_Visible(1);
	/* font set */
	unsigned int color = param_list_show.param_list[num_index].color;
	font = Custom_title.get_Font();
	font.put_Color(color);
	font.put_Size(13);
	font.put_Bold(TRUE);
	/*--------------------------------*/
	pen = Custom_axis.get_AxisPen(); 
	/*--------------------------------*/
	pen.put_Color(color);
	pen.put_Width(2);
	/*-------------------------------*/
	label = Custom_axis.get_Labels();
	font = label.get_Font();
	font.put_Color(color);
	font.put_Bold(TRUE);
	/*-------------------------------*/
	if( t_lnk >= 1 )
	{
	    line_now.put_VerticalAxisCustom(t_lnk-1);
	}else
	{
		line_now.put_VerticalAxis(0);		
	}
	/*-------------------------------*/
}
/*--------------------------*/
void CTeeChart5_testDlg::draw_single(unsigned int num)
{
	unsigned int mode = m_check_hold.GetCheck()?1:0;
    /* number */
    unsigned int line_num;
	/* safearray */
    COleSafeArray XValue,YValue,ZValue;
    SAFEARRAYBOUND rgsabound;
	/* temple */
	double dval;
	/* have no data */
	if( param_list_show.param_list[num].point_num == 0 )
	{
		AfxMessageBox(_T("没有数据,无法绘制"));
		return;
	}
	if( param_list_show.param_list[num].status == 0xff )
	{
		/* has been draw */
		return;
	}
	/* clear all line */
	if( mode == 0 )
	{
	   clear_all_line(0);
	}
	/* get line */
	CSeries * line_p;
	/* line type */
	if( param_list_show.param_list[num].line_type == 0 )
	{
	    line_p = (CSeries *)Get_line_source(&line_num);
	}
	else
	{
	    /* set */
		if( param_list_show.param_list[num].line_type == 1 )
		{
			line_p = (CSeries *)Get_point_source(&line_num);
		}
		else if( param_list_show.param_list[num].line_type == 7 )
		{
			line_p = (CSeries *)Get_star_source(&line_num);
		}
		else
		{
			MessageBox(_T("目前不支持的形状"),_T("tips"),0);
			return;			
		}
	}
    /* max line number */
	if( line_p == NULL )
	{
		MessageBox(_T("最多支持20条曲线"),_T("tips"),0);
		return;
	}
	/* det */
	CSeries line_cfs;
	CPoint3DSeries line_3d;
	/* 2D or 3D */
	if( param_list_show.param_list[num].line_type == 0 )
	{
		line_cfs = (CSeries)m_chart.Series(line_num);
	}
	else
	{
		line_cfs = m_chart.Series(line_num);
		line_3d = (CPoint3DSeries)line_cfs.get_asPoint3D();
	}
	/* get axis */
	/*--------------------------*/
	double *line_data_y = (double *)param_list_show.param_list[num].data_y;
	double *line_data_x = (double *)param_list_show.param_list[num].data_x;
	/*------init------*/
	rgsabound.cElements = param_list_show.param_list[num].point_num - param_list_show.param_list[num].offset;
    rgsabound.lLbound=0;
    XValue.Create(VT_R8,1,&rgsabound);
    YValue.Create(VT_R8,1,&rgsabound);
	ZValue.Create(VT_R8,1,&rgsabound);
	/* draw all */
	for( long i = param_list_show.param_list[num].offset ; i < (long)param_list_show.param_list[num].point_num ; i++ )
	{
		dval = line_data_x[i];
		/* init */
        XValue.PutElement(&i, &dval);

        dval = line_data_y[i];

        YValue.PutElement(&i, &dval);
		/* Z */
        dval = 0;

        ZValue.PutElement(&i, &dval);
	}
	/* 2D or 3D */
	if( param_list_show.param_list[num].line_type == 0 )
	{
		/* show the line */
		line_cfs.AddArray(param_list_show.param_list[num].point_num,YValue,XValue);
	}
	else
	{
		line_3d.AddArrayXYZ(XValue,YValue,ZValue);
	}
	/*-----------*/
	unsigned long cols = get_color(0);//(colorB<<16)|(colorG<<8)|(colorR);
	/*---------------*/
	param_list_show.param_list[num].color = cols;
	param_list_show.param_list[num].status = 0xff;//has been drawed
	param_list_show.param_list[num].line_num = line_num;
	/*-------------*/
    /* show legend */
	Legend_handle(1);
	/* transfer */
	USES_CONVERSION;
	/*----------------------*/
	CString show = A2T(param_list_show.param_list[num].name);
	/*-----------------------------*/
	line_cfs.put_Title(show);
	/* show legend */
	line_cfs.put_ShowInLegend(1);
	/* put color */
	line_cfs.put_Color(cols);
	/*---------------------*/
	draw_axis(num);
	/*------------------------------------------------------------------------------------*/
	CMarks line_mark = line_cfs.get_Marks();
	/* mark show */
	if( param_list_show.param_list[num].mark != 0 )
	{
		unsigned char type = param_list_show.param_list[num].mark >> 24;
		unsigned int every = param_list_show.param_list[num].mark & 0xffffff;
		/* enable */
		line_mark.put_Visible(1);
		line_mark.put_Angle(0);
		line_mark.put_Style(0);
		line_mark.put_ArrowLength(-20);
		/* type */
		if( type == 0xF2 && every )
		{
			/*------------------*/
			line_mark.put_DrawEvery((param_list_show.param_list[num].point_num-1) / every);
			/*------------------*/
		}else if( type == 0xF1 && every )
		{
			line_mark.put_DrawEvery(every);
		}else
		{
			/* fail */
			line_mark.put_Visible(0);
		}
	}else
	{
		line_mark.put_Visible(0);
	}
	/*-----------*/
}
/*---------------------------------------------*/
void CTeeChart5_testDlg::OnCbnSelchangeCombo1()
{
	/* get cline_num;*/
	int seq = m_combox_param_show.GetCurSel();
	/* has not show */
	if( param_list_show.param_list_num )
	{
	  /*---------------------------------*/
	  draw_single(seq);
	}else if( COM.radio_status == 1 )
	{
		//============================
		Get_line_group_index(seq);
	}
}
/* close files */
void CTeeChart5_testDlg::OnBnClickedButton5()
{
	clear_all_line(0);
	/* release the memory */
	release_memory();
	memset(&param_list_show,0,sizeof(param_list_show));
	memset(&file_man,0,sizeof(file_man));
	m_combox_param_show.ResetContent();
	/*-----------------------------*/
	memset(last_dou,0,sizeof(last_dou));
	/*-----------------------------*/
	CString show = _T("<可绘制曲线的参数列表>");
	m_combox_param_show.AddString(show);
	m_combox_param_show.SetCurSel(0);
	/*-----------------------------*/
	OnBnClickedButton8();
}

void CTeeChart5_testDlg::On32777()
{
	m_com_num com;
	com.DoModal();
}
BEGIN_EVENTSINK_MAP(CTeeChart5_testDlg, CDialogEx)
	ON_EVENT(CTeeChart5_testDlg, IDC_MSCOMM1, 1, CTeeChart5_testDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()


void CTeeChart5_testDlg::OnCommMscomm1()
{
	VARIANT InputData;   
	int leng;
	static unsigned char cData[2048];   
	long k;

    COleSafeArray fs;

	if( m_mscomm.get_CommEvent() == 2 )   
	{
		leng = m_mscomm.get_InBufferCount(); 
		
		InputData = m_mscomm.get_Input();

		fs = InputData; //V

        for( k = 0 ; k < (long)fs.GetOneDimSize() ; k++ ) 
		{
			fs.GetElement(&k, cData + k); 
		}
	    /* get and decode data */
		fm_link_handle(cData,leng);
	}else
	{
		k = 0;
	}
}
BOOL CTeeChart5_testDlg::OnDeviceChange(UINT nEventType,DWORD dwData)  
{  
    //DEV_BROADCAST_DEVICEINTERFACE* dbd = (DEV_BROADCAST_DEVICEINTERFACE*) dwData; 
    switch (nEventType)  
    {  
		case DBT_DEVICEREMOVECOMPLETE://移除设备 
		case DBT_DEVICEARRIVAL://添加设备  
			if( system_config_inf.inf[2] )
			{
	           Get_COM_STATUS(1);//change
			}
			break;  
		default:  
			break;  
    }  
    /*--------*/
    return TRUE;  
}
/*---------------------------------------*/
void CTeeChart5_testDlg::OnBnClickedButton10()
{

}
/* -----------------------------------about COMS -------------------------------------*/
HANDLE hCom;
DCB  DCB_U,dcb;
/*------------------------------------------------------------------------------------*/
/*---------------------------*/
int CTeeChart5_testDlg::open_mscomm(unsigned int num)
{
//	//	BYTE bSet;
//	if( num == 0 )//has not opened
//	{
//		return (-1);
//	}
//	DCB dcb ;
//	BOOL fRetVal ;
//	COMMTIMEOUTS CommTimeOuts;
//	CString szCom; 
//	szCom.Format(_T("\\\\.\\COM%d"), num);
//	hCom = CreateFile(szCom.GetBuffer(50), GENERIC_READ | GENERIC_WRITE,//可读、可写
//							FILE_SHARE_READ | FILE_SHARE_WRITE, // 不共享FILE_SHARE_READ,FILE_SHARE_WRITE,FILE_SHARE_DELETE
//							NULL, // 无安全描
//							OPEN_EXISTING, //打开已存在文件
//							0,///*FILE_ATTRIBUTE_NORMAL | */FILE_FLAG_OVERLAPPED, // 文件属性
//							NULL//一个有效的句柄，已存在的设备句柄A valid handle to a template file with the GENERIC_READ access right
//							);
//
//	int nError = GetLastError();
//	if (INVALID_HANDLE_VALUE == hCom )
//	{
//		char chError[256];memset(chError, 0, 256);
//		int nBuffLen = 256;
//		return ( FALSE ) ;
//	}
//	// 设置缓冲区,输入/输出大小(字节数)
//	SetupComm(hCom,4096,4096) ;
//	// 指定监视事件_收到字符放入缓冲区
//	SetCommMask( hCom, EV_RXCHAR ) ;
//
//	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
//	CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
//	CommTimeOuts.ReadTotalTimeoutConstant = 1000 ;
//	CommTimeOuts.WriteTotalTimeoutMultiplier = 2*CBR_9600/9600 ;
//	CommTimeOuts.WriteTotalTimeoutConstant = 0 ;
//	//给定串口读与操作限时
//	SetCommTimeouts(/*COMFileTemp*/hCom, &CommTimeOuts ) ;
//
//	//设置串口参数:波特率=9600;停止位 1个;无校验;8位
//	dcb.DCBlength = sizeof( DCB ) ;
//	GetCommState( hCom, &dcb ) ;
//	dcb.BaudRate =CBR_9600;
//	dcb.StopBits =ONESTOPBIT;
//	dcb.Parity = NOPARITY;
//	dcb.ByteSize=8;
//	dcb.fBinary=TRUE;//二进制通信, 非字符通信
//	dcb.fOutxDsrFlow = 0 ;
//	dcb.fDtrControl = DTR_CONTROL_ENABLE ;
//	dcb.fOutxCtsFlow = 0 ;
//	dcb.fRtsControl = RTS_CONTROL_ENABLE ;
//	dcb.fInX = dcb.fOutX = 1 ;
//	dcb.XonChar = 0X11 ;
//	dcb.XoffChar = 0X13 ;
//	dcb.XonLim = 100 ;
//	dcb.XoffLim = 100 ;
//	dcb.fParity = TRUE ;
//
//	//根据设备控制块配置通信设备
//	fRetVal = SetCommState(hCom, &dcb ) ;
//
//	if(!fRetVal) return FALSE;
//
//	//刷清缓冲区
//	PurgeComm( hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
//
//	//指定串口执行扩展功能
//	EscapeCommFunction( hCom, SETDTR ) ;
//
//
//
//// 	//设置串口 “收到字符放入缓冲区”事件
//// 	if (!SetCommMask(COMFile, EV_RXCHAR )) return ( FALSE );
//
//	return TRUE ;
		//unsigned char com_num;
		//static char open_flag = 0;
		//char buffer[100];
		//CString dis;

		//com_num = num;
		//if( num != 0 )//has not opened
		//{
		//	sprintf_s(buffer,"COM%d",com_num);
  //      
		//	USES_CONVERSION;

		//	dis = A2T(buffer);

		//	hCom = CreateFile(dis,//
		//					  GENERIC_READ|GENERIC_WRITE, //
		//					  0, //
		//					  NULL,  
		//					  OPEN_EXISTING, //
		//					  0,
		//					  NULL);   

		//	if(hCom==(HANDLE)-1)   
		//	{  
		//	   MessageBox(_T(" open COM fail "),_T("tips"),0);
		//	   return (-1);   
		//	}  
		//	/* set timeout */
		//	COMMTIMEOUTS TimeOuts;  
		//	/* equse */
		//	TimeOuts.ReadIntervalTimeout = MAXDWORD;//两个字节之间的间隔时间  
		//	TimeOuts.ReadTotalTimeoutMultiplier = 0;//读时间系数  
		//	TimeOuts.ReadTotalTimeoutConstant = 0;//读时间常数  
		//	TimeOuts.WriteTotalTimeoutMultiplier = 100;//设定写超时  
		//	TimeOuts.WriteTotalTimeoutConstant = 500;  
		//	SetCommTimeouts(hCom,&TimeOuts);  
		//	/*-------------*/
		//	GetCommState(hCom,&dcb);
		//	dcb.BaudRate = 9600;
		//	dcb.ByteSize = 8; 
		//	dcb.Parity = NOPARITY; 
		//	dcb.StopBits = ONESTOPBIT; 
		//	dcb.fBinary = TRUE;
		//	dcb.fParity = TRUE;
	 //       /* set */
		//	if (!SetCommState(hCom, &dcb))
		//	{
		//		MessageBox(_T("config err"),_T("tips"),0);
		//		return (-1);
		//	}
		//	/* setting */
		//	SetupComm(hCom, 1024, 1024);
		//	PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		//	/* get state */
		//	GetCommState(hCom,&DCB_U);
		//}
	/* some params */
	unsigned int baudrate = COM.com_bd;
	/*-------------------*/
    if( num != 0 )//has not opened
	{
		/*----------------------*/
		if(m_mscomm.get_PortOpen())
        {
           m_mscomm.put_PortOpen(FALSE);
        }
		/* init */
        m_mscomm.put_CommPort(num); 
        m_mscomm.put_InBufferSize(1024);
        m_mscomm.put_OutBufferSize(1024);
        m_mscomm.put_InputLen(0);
        m_mscomm.put_InputMode(1);
        m_mscomm.put_RThreshold(1);
		/* set bandrate */
		if( baudrate == 0 )
		{
           m_mscomm.put_Settings(_T("9600,n,8,1"));
		}else if( baudrate == 1 )
		{
           m_mscomm.put_Settings(_T("57600,n,8,1"));
		}else if( baudrate == 2 )
		{
           m_mscomm.put_Settings(_T("115200,n,8,1"));
		}else if( baudrate == 3 )
		{
           m_mscomm.put_Settings(_T("38400,n,8,1"));
		}else if( baudrate == 4 )
		{
           m_mscomm.put_Settings(_T("961200,n,8,1"));
		}else
		{
			return (-1);
		}
        /* get status */
        if (!m_mscomm.get_PortOpen())
        {
			/* ok */
            m_mscomm.put_PortOpen(TRUE);
			/* open ok */
			return 0;
        }
        else
        {
			/* fail */
            m_mscomm.put_OutBufferCount(0);
			/*---*/
			return (-1);
        }
	}else
	{
		////if(m_mscomm.get_PortOpen())
		////{
		////  m_mscomm.put_OutBufferCount(0);
	 ////     m_mscomm.put_PortOpen(FALSE);
		////}
		m_mscomm.CloseWindow();
	}
	return 0;
}
void CTeeChart5_testDlg::OnBnClickedButton11()
{

}
/* int get com_status */
int CTeeChart5_testDlg::Get_COM_STATUS(unsigned int mode)
{
//	unsigned int i;
	/*------------*/
	Refresh_com_list();
	/*------------*/
	if( mode == 0 )
	{
		return (-1);
	}
	/* if manul mode */
	if( mode != 0 && mode != 1 )
	{
		/* we got the correct radio */
		COM.com_num_using = mode;
		/*--------------------------*/
		m_taps.SetWindowTextW(_T("提示信息：正在连接电台......"));
		/*-------start time-------*/
		reflesh_global_status(1);
	}
	/*---------------*/
	//for( i = 0 ; i < COM.Available_com_num ; i ++ )
	//{
	//	if( strstr(COM.com_detail[i].name,"Silabser") != NULL )
	//	{
	//		break;
	//	}
	//}
	///* if */
	//if( i == COM.Available_com_num )
	//{
	//	if( COM.opened = 1 )
	//	{
	//		//reflesh the com status
	//		COM.opened = 0;
	//		/*----------*/
	//		m_taps.SetWindowTextW(_T("提示信息：电台已断开连接......"));
	//		/*----------*/
	//		reflesh_global_status(0);
	//	}
	//	/* return */
	//	return (-1);
	//}
	/* opened or not */
	if( COM.opened == 0 )
	{
		/* we got the correct radio */
		//COM.com_num_using = COM.com_detail[i].com_det;
		///*--------------------------*/
		//m_taps.SetWindowTextW(_T("提示信息：正在连接电台......"));
		///*-------start time-------*/
		//reflesh_global_status(1);
//		COM.opened = 1;//opened parse
		/*------------------------*/
	}
	/* return */
	return 0;
}
/* refresh the com list */
void CTeeChart5_testDlg::Refresh_com_list(void)
{
    HKEY hkey;  
    int result;
    int i = 0;
	/* get list */
    result = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                            _T( "Hardware\\DeviceMap\\SerialComm" ),
                            NULL,
                            KEY_READ,
                            &hkey );
	/* clear odd date */
	com_msg_clear();
	/*----------------*/
    if( ERROR_SUCCESS == result )   //   打开串口注册表   
    {   
        TCHAR portName[100], commName[100];
        DWORD dwLong, dwSize;
        /* do list */
        do
        {   
            dwSize = sizeof( portName ) / sizeof( TCHAR );
            dwLong = dwSize;
            result = RegEnumValue( hkey, i, portName, &dwLong, NULL, NULL, ( LPBYTE )commName, &dwSize );
			/* over ? */
            if( ERROR_NO_MORE_ITEMS == result )
            {
                break;
            }
			/* transfer */
			transfer_com_msg(commName,portName);
			/*---------*/
			i++;
			/*---------*/
        } while ( 1 );
		/* unregrister */
        RegCloseKey( hkey );   
    }
}
/*------------COM-------------*/
void CTeeChart5_testDlg::com_msg_clear(void)
{
	COM.Available_com_num = 0;
}
void CTeeChart5_testDlg::transfer_com_msg(TCHAR * comname , TCHAR * portname)
{
	char com_name_s[100];
	char port_name[100];
    /* memset */
	memset(com_name_s,0,sizeof(com_name_s));
	memset(port_name,0,sizeof(port_name));
	/*----------------*/
	for( int i = 0 ; i < sizeof(com_name_s) ; i++)
	{
		if( comname[i] != 0 )
		{
			com_name_s[i] = (char)comname[i];
		}else
		{
			/*----------*/
			break;//error
		}
	}
	/*----------------*/
	for( int i = 0 ; i < sizeof(port_name) ; i++)
	{
		if( portname[i] != 0 )
		{
			port_name[i] = (char)portname[i];
		}else
		{
			/*----------*/
			break;//error
		}
	}
	/* length */
	int port_len,name_len;
	/*
	*/
	port_len = strlen(com_name_s);
	/*-----------*/
	name_len = strlen(port_name);
	/*-------get COM num and transfer----------*/
	for( int i = 0 ; i < name_len ; i ++ )
	{
		if( port_name[ name_len - i - 1 ] == '\\' )//0x5c
		{
			memcpy(COM.com_detail[COM.Available_com_num].name,&port_name[ name_len - i],i+1);
			break;
		}
	}
	/* get portnum */
	if( sscanf_s(com_name_s,"COM%d",&COM.com_detail[COM.Available_com_num].com_det) != 1 )
	{
		MessageBox(_T("串口信息错误"),_T("tips"),0);
		return;
	}
    /* */
	COM.Available_com_num++;
}
/* time tick */
void CTeeChart5_testDlg::OnTimer(UINT_PTR nIDEvent) 
{
  switch(nIDEvent)
  {
    case 1://radio manager
		radio_link();
		link_autopilot_thread();
		 break;
	case 2:
		link_autopilot_thread();
		break;
	case 3:
		unit_data_thread();
		break;
	case 4:
		Set_line_output_thread();
		break;
	case 5:
		pic_num_show();
		break;
    default:break;
  }
  /* overrade */
  CDialog::OnTimer(nIDEvent);
}
/* send com data */
int CTeeChart5_testDlg::send(unsigned char * data,unsigned int len)
{
   CByteArray hexdata; 
   /*-----------*/
   if( COM.opened != 1 )
   {
	   return (-1);
   }
   /* copy data */
   for( unsigned int i = 0 ; i < len ; i++ )
   {
     hexdata.Add(data[i]);
   }
   /* send */
   m_mscomm.put_Output(COleVariant(hexdata));
   /*---------*/
   return len;
 //   unsigned long leny = 0;
	///* write file */
	//WriteFile(hCom,data,(DWORD)len,&leny,0);
	///* return */
 //   return leny;
}
/* radio link COM.opened = 1; */
void CTeeChart5_testDlg::radio_link(void)
{
	unsigned char radio_link_data[8] = { 0x46 ,0x4D ,0x55 ,0x00 ,0x00 ,0x11 ,0xAA ,0x55 };
	/* opened */
	if( COM.open_flag == 0 && COM.opened == 1 )
	{
		/* reset system */
		if( COM.reset_flags == 1 )
		{
			m_taps.SetWindowTextW(_T("提示信息：需要重新重启软件......"));
			return;
		}
		int ret = open_mscomm(COM.com_num_using);
		/* open ok ? */
		if( ret != 0 )
		{
			/* fail */
			MessageBox(_T("电台不可用"),_T("tips"),0);
			/* reset */
			KillTimer(1);
			COM.opened = 0;
			/*----------*/
			return;
		}
		/* ok */
		COM.open_flag = 1;
		COM.com_status = 1;//opened
		COM.reset_flags = 1;
	}
	/* send data */
	send(radio_link_data,sizeof(radio_link_data));
}
/* void SetTimer(1,10,NULL) */
void CTeeChart5_testDlg::reflesh_global_status(unsigned int mode)
{
	if( mode == 1 )
	{
		/* create */
		SetTimer(1,1000,NULL);
		COM.opened = 1;
	}else if( mode == 0 )
	{
		/* kill data */
		COM.open_flag = 0;
		COM.opened = 0;
		if( COM.com_status == 1 )
		{
			COM.com_status = 0;
		    open_mscomm(0);
		}
	}
}
/* create thread */
int create_thread(void)
{
	//handle_com_read = AfxBeginThread(ThreadProc, NULL);
	///* ok */
	//if( handle_com_read != NULL )
	//{
	//	return 0;
	//}else
	{
		return (-1);
	}
}
/* suspendThread */
void thread_suspend(unsigned int mode)
{
	//if( handle_com_read == NULL )
	//{
	//	return;
	//}
	///* mode */
	//if( mode == 0 )
	//{
	//	handle_com_read->SuspendThread();
	//}else
	//{
	//	handle_com_read->ResumeThread();
	//}
}
/*-----------------*/
//static unsigned int cnt_thead = 0;
//UINT ThreadProc(LPVOID pParam)
//{
//	/* read data */
//	static unsigned char buffer[100];
//	static unsigned long len = 0;
//
//	unsigned int read_len = 100;
//
//	while(1)
//	{
//		/* start */
//		ReadFile(hCom,&buffer,(DWORD)1,&len,0);
//		/*----------*/
//		if( len != 0 )
//		{
//		  len = 0;
//		}
//        /*----*/
//	}
//  return 0;
//}

void CTeeChart5_testDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	thread_suspend(0);
}
/* changed */
void CTeeChart5_testDlg::Set_sempher(unsigned char mode)
{
	sempher = mode;
}
/* ------- */
void CTeeChart5_testDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	/* return */
	if( sempher == 1 )
	{
		return;
	}
	/* get leng item */
	int nItem,i;  
    nItem = m_list_ctrl.GetItemCount();
	/* for */
	for( i = 0 ; i < nItem ; i ++ )
	{
		int ret = m_list_ctrl.GetCheck(i)?1:0;
	    /*-----------------------------------*/
		if( chese_list.chese[i].check_if != ret )
		{
			sempher = 1;
			/*--------------------*/
			chese_list.chese[i].check_if = ret;
			/* check data refresh */
			refresh_result_all();
			/*-------*/
			sempher = 0;
		}
	}
}
/* double */
void CTeeChart5_testDlg::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	/* list */
	*pResult = 0;

	POSITION ps;
	ps = m_list_ctrl.GetFirstSelectedItemPosition();
	int nIndex = m_list_ctrl.GetNextSelectedItem(ps);
	/*--------------*/
	if( nIndex != (-1) )
	{
		list_click_seq = nIndex;
	    limit_setting limit_dlg;
	    limit_dlg.DoModal();
		refresh_result_all();
		Save_limit_file();
	}
	/* get */
	
}
/*-------------------------------------------*/
void CTeeChart5_testDlg::Read_limit_file(void)
{
	FILE * pf_file_limit;
	/*-----------------*/
	fopen_s(&pf_file_limit,"D200_limit.cfg","rb");
	/* exist */
	if( pf_file_limit == NULL )
	{
		fopen_s(&pf_file_limit,"D200_limit.cfg","wb+");
		/*------------------------------------------*/
		if( pf_file_limit == NULL )
		{
			MessageBox(_T("配置文件创建失败"),_T("tips"),0);
			/* ----- */
			return;
		}
		/* return */
		return;
	}
	/*-------*/
	int len = fread(&chese_list,1,sizeof(chese_list),pf_file_limit);
	/*-------*/
	if( len == 0 )
	{
		MessageBox(_T("配置文件可能有问题，需要重新保存"),_T("tips"),0);
		/*---------------*/
		memset(&chese_list,0,sizeof(chese_list));	
	}
	/*-------*/
	for( int i = 0 ; i < 64 ; i ++ )
	{
		chese_list.chese[i].mean_rms_num = 0;
		memset(chese_list.chese[i].mean_now,0,sizeof(chese_list.chese[i].mean_now));
		memset(chese_list.chese[i].rms_now,0,sizeof(chese_list.chese[i].rms_now));
		memset(chese_list.chese[i].raw_data,0,sizeof(chese_list.chese[i].raw_data));
	}
	/*-------*/
	fclose(pf_file_limit);
}
void CTeeChart5_testDlg::Save_limit_file(void)
{
    FILE * pf_file_limit;
	/*-----------------*/
	fopen_s(&pf_file_limit,"D200_limit.cfg","wb+");
	/*------------------------------------------*/
	if( pf_file_limit == NULL )
	{
		MessageBox(_T("配置文件创建失败"),_T("tips"),0);
		/* ----- */
		return;
	}
	/*----------------*/
	fwrite(&chese_list,1,sizeof(chese_list),pf_file_limit);
	/*-------*/
	fclose(pf_file_limit);
}
/*-------------------------------------------*/
void CTeeChart5_testDlg::OnBnClickedButton123()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTeeChart5_testDlg::OnBnClickedButton26()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDD_TEECHART5_TEST_DIALOG,_T("211"));
}

void CTeeChart5_testDlg::reset(void)
{
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	/*---------------*/
	*pd = 246;
	/*-------------------------*/
	if( fm_link_send(76,package,33) == (-1) )
	{
		MessageBox(_T("电台串口未打开"),_T("tips"),0);
		return;
	}
}
void CTeeChart5_testDlg::take_pic_btn_event(unsigned int otk)
{
	static unsigned char flag = 0;
	// TODO: 在此添加控件通知处理程序代码
	if( flag == 0 && otk )
	{
		reset();
		reset();
		m_taps.SetWindowTextW(_T("提示信息：拍照测试初始化中，请稍等"));
		Sleep(15000);
		m_taps.SetWindowTextW(_T("提示信息：拍照测试中..."));
		/* take */
		pic_take_public_s = m_pic_cnt.GetCurSel();
		/* ------------------- */
		if( pic_take_public_s == 0 )
		{
			MessageBox(_T("请选择一个拍照数量"),_T("tips"),0);
			return;
		}
		/*------------------*/
		KillTimer(1);
		KillTimer(2);
		KillTimer(3);
		KillTimer(4);
		/*-----------------*/
		pic_take_now = pic_take_public_s * 5;
		/*--------------------------*/
		char buffer[100];
		/*--------------------------*/
		sprintf_s(buffer,"停止(%d）",pic_take_now);
		/*--------------------------*/
		CString show;
		USES_CONVERSION;
		/*--------------*/
		show = A2T(buffer);
		/*----------------*/
		m_btn_pic.SetWindowTextW(show);
		/* set timer */
		SetTimer(5,5000,NULL);
		/*-----------*/
		flag = 1;
	}else
	{
		KillTimer(5);
		m_btn_pic.SetWindowTextW(_T("开始拍照"));
		/*-----------------------------------*/
		pic_feedback_ok = 0;
		pic_feedback_err = 0;
		/*-----------*/
		flag = 0;
	}
}

void CTeeChart5_testDlg::take_a_photo(void)
{
	static unsigned char pic_s = 1;
	/*take_pic_btn_event(1);*/
	unsigned char package[13];
	/* clear */
	memset(package,0,sizeof(package));
	/*---------*/
	package[4]  = 0x4E;
	package[5]  = 0x7E;
	package[6]  = 0xFE;
	package[10] = 0x01;
	/* command id */
	package[11] = pic_s++;
    /* send */
	for( int i = 0 ; i < 5 ; i++ )
	{
		if( fm_link_send( 155 , package , 13 ) == (-1) )
		{
			MessageBox(_T("电台串口未打开"),_T("tips"),0);
			return;
		}
	}
}

void CTeeChart5_testDlg::OnBnClickedButton14()
{
	take_a_photo();
}
/* show num */
void CTeeChart5_testDlg::pic_num_show(void)
{
	//static unsigned char pic_s = 1;
	///* buffer */
	//unsigned char package[13];
	///* clear */
	//memset(package,0,sizeof(package));
	///*---------*/
	//package[4]  = 0x4E;
	//package[5]  = 0x7E;
	//package[6]  = 0xFF;
	//package[10] = 0x01;
	///* command id */
	//package[11] = pic_s++;
 //   /* send */
	//for( int i = 0 ; i < 5 ; i++ )
	//{
	//	if( fm_link_send( 155 , package , 13 ) == (-1) )
	//	{
	//		MessageBox(_T("电台串口未打开"),_T("tips"),0);
	//		return;
	//	}
	//}
	///*-----------*/
	//pic_take_now--;
	///*-----------*/
 //   char buffer[100];
	///*--------------------------*/
	//sprintf_s(buffer,"停止(%d）",pic_take_now);
	///*--------------------------*/
	//CString show;
	//USES_CONVERSION;
	///*--------------*/
	//show = A2T(buffer);
	///*----------------*/
	//m_btn_pic.SetWindowTextW(show);
	///*-------------------*/
	//if( pic_take_now == 0 )
	//{
	//	take_pic_btn_event(0);
	//}
}
/*----------------------------*/
void CTeeChart5_testDlg::pic_feedback_show(void)
{
	pic_feedback_ok ++;
	/* show */
	char buffer[100];
	/*--------------------------*/
	sprintf_s(buffer,"%d",pic_feedback_ok);
	/*--------------------------*/
	CString show;
	USES_CONVERSION;
	/*--------------*/
	show = A2T(buffer);
	/*----------------*/
	m_edit_pic.SetWindowTextW(show);
}
/*----------------------------*/
void CTeeChart5_testDlg::OnBnClickedButton22()
{
	if( MessageBox(_T("电机测试模式会暂停其它测试，飞行器会复位重启，是否进入？"),_T("tips"),1) == 1 )
	{
		disable_all_timer();
		set_test_mode(0);
		m_taps.SetWindowTextW(_T("提示信息：电机测试中"));
		motor_close = 1;
		motor_dlg.DoModal();
		motor_close = 0;
		m_manul_test.EnableWindow(1);
		m_taps.SetWindowTextW(_T("提示信息：已退出电机测试"));
	}
}


void CTeeChart5_testDlg::OnBnClickedButton27()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTeeChart5_testDlg::set_test_mode(unsigned int flag)
{
	if( start_stop_flags == 0 )
	{
		m_taps.SetWindowTextW(_T("提示信息：暂停测试"));
		m_manul_test.SetWindowTextW(_T("开始测试"));
		m_manul_test.EnableWindow(1);
		start_stop_flags = 1;
	}
}

void CTeeChart5_testDlg::OnBnClickedButton29()
{
	if( start_stop_flags == 0 )
	{
		m_taps.SetWindowTextW(_T("提示信息：暂停测试"));
		m_manul_test.SetWindowTextW(_T("开始测试"));
		m_manul_test.EnableWindow(0);
		start_stop_flags = 1;
	}else
	{
		m_taps.SetWindowTextW(_T("提示信息：恢复测试"));
		SetTimer(1,1000,NULL);
		m_manul_test.SetWindowTextW(_T("暂停测试"));
		start_stop_flags = 0;
	}

}
/*--------------------*/
void CTeeChart5_testDlg::OnBnClickedButton30()
{
	list_click_seq = 0;
	limit_setting limit_dlg;
	limit_dlg.DoModal();
	/* update */
	refresh_result_all();
	Save_limit_file();
	//-----------------------
}
/* ---------------- */
void CTeeChart5_testDlg::Arm_Distortion(char * path,unsigned int if_check)
{
	/*--------------------*/
	FILE * fp;
	int Arm_DsTn_cnt = 0; 
    double  ctrlsum = 0.0f;
	unsigned short TakeoffOk,AttChanged,charYawChanged;
	double dWBY,attctrl2;
	short arm_buffer[256];
	char show_buffer[200];
	char result_buffer[50];
	/* define */
	if( if_check != 1 )
	{
		/* can not supply this format */
		return;
	}
	/*----------------------*/
	fopen_s(&fp,path,"rb");
	/*---------------------*/
	if( fp == NULL )
	{
		MessageBox(_T("计算机臂参数时log文件打开失败"),_T("tips"),0);
		return;
	}
    /* ok */
	while(1)
	{
		int len = fread(arm_buffer,1,sizeof(arm_buffer),fp);
		/* over ? */
		if( len < sizeof(arm_buffer) )
		{
			if( Arm_DsTn_cnt != 0 )
			{
			   ctrlsum /= (double)Arm_DsTn_cnt;
			}
			/* close file */
			fclose(fp);
			/* exit */
			break;
		}
		/* check and sum */
		TakeoffOk  = ( arm_buffer[1] & ( 1 << 0 ) ) ? 1 : 0;
		AttChanged = ( arm_buffer[1] & ( 1 << 6 ) ) ? 1 : 0;
		charYawChanged = ( arm_buffer[1] & ( 1 << 7 ) ) ? 1 : 0;
		/* calibration */
		dWBY = Short2Float( (double)arm_buffer[4],0.3f ) / 0.012f * 57.3f;
		/*-----------------------*/
		if( ( arm_buffer[0] & 0x1 ) == 1 )
		{
			attctrl2 = Short2Float( (double)arm_buffer[32] , 50.0f );
			/* check */
			if( ( abs(dWBY) < 1.0f ) && ( TakeoffOk == 1 ) && ( AttChanged == 0 ) && ( charYawChanged == 0 ) )
			{   
				ctrlsum += attctrl2;
				/* */
				Arm_DsTn_cnt++ ;
			}
		}
	}
	/* Arm_DsTn_cnt */
	if( Arm_DsTn_cnt < 20 )
	{
      sprintf_s(result_buffer,"起飞时间不够");
	}else
	{
		/*---------------*/
		if( (float)ctrlsum < 0.5f )
		{
			sprintf_s(result_buffer,"优秀");
		}else if( (float)ctrlsum < 1.0f )
		{ 
			sprintf_s(result_buffer,"及格");
		}else
		{
			sprintf_s(result_buffer,"已变形");
		}
	}
	/* show the msg */
	sprintf_s(show_buffer,"%f , 自动判断：%s", (float)ctrlsum,result_buffer);
    CString show;
    USES_CONVERSION;
	/*--------------*/
	show = A2T(show_buffer);
	/* show */
	//MessageBox(show,_T("tips"),0);
	m_taps.SetWindowTextW(show);
}

void CTeeChart5_testDlg::open_map(void)
{
	// TODO: 在此添加命令处理程序代码
	map map_g;
	map_g.DoModal();
}
/*----------------------------------------------------------------------------*/
void CTeeChart5_testDlg::system_config_fresh(unsigned int offset , unsigned int data)
{
	FILE * sys_cfg;
	/*------------*/
	if( offset >= 256 )
	{
		MessageBox(_T("系统配置文件参数错误！"),_T("tips"),0);
		return;
	}
	/**/
	system_config_inf.inf[offset] = data;
	/*------------------------------*/
	fopen_s(&sys_cfg,"config.inf","wb+");
	/* create ok ?*/
	if( sys_cfg == NULL )
	{
		MessageBox(_T("系统配置文件创建失败"),_T("tips"),0);
		return;
	}
	/*------------------------*/
	fwrite(&system_config_inf,1,sizeof(system_config_inf),sys_cfg);
	/* close */
	fclose(sys_cfg);
}
/* get system config name */
unsigned int CTeeChart5_testDlg::system_config_msg(unsigned int offset)
{
	return system_config_inf.inf[offset];
}
/* disable all timer */
void CTeeChart5_testDlg::disable_all_timer(void)
{
	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
	KillTimer(4);
	KillTimer(5);
}
/* Key process */
BOOL CTeeChart5_testDlg::PreTranslateMessage(MSG* pMsg)
{
   //判断是否为键盘消息
   if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
   {
      //判断是否按下键盘Enter键
      if(pMsg->message==WM_KEYDOWN)
      {
		  switch(pMsg->wParam)
		  {
			  case VK_F2:
				  On32771();
				  break;
#if !VERSION_CTRL
			  case VK_F3:
				  On32777();
				  break;
#endif
			  case VK_F4:
				  On32781();
				  break;
			  case VK_F5:
				  reflush_chart();
				  break;
#if !VERSION_CTRL
			  case VK_F6:
                  Open_cfg();
				  break;
			  case VK_F7:
				  open_data_review();
				  break;
			  case VK_F8:
				 open_map();
				  break;
			  case VK_F9:
				  
				  break;
			  case 0x56:
				  /* show version . one by one */
				  create_version_line(0);
				  /*---------------------------*/
				  break;
#else
			  case VK_F6:
				  //-------------------------------
				  delete_point();
				  break;
#endif
#if !VERSION_CTRL
			  case 90:
				  fpos_analysis(0);
#endif
				  break;
			  default:
				  HOTKEY_THREAD(pMsg->wParam);
				  break;
		  }
		  /*------------------*/
          return TRUE;
      }
   }
 return CDialog::PreTranslateMessage(pMsg);
}
/*----------------------------------------------*/
void CTeeChart5_testDlg::release_memory(void)
{
	void *p_used[200];
	unsigned int num_p = 0;
	void * backup0 = 0,*backup1 = 0;

	memset(p_used,0,sizeof(p_used));

    for( unsigned int i = 0 ; i < param_list_show.param_list_num ; i ++ )
	{

		if( param_list_show.param_list[i].data_y != NULL )
		{
			if( backup0 != param_list_show.param_list[i].data_y )
			{
				backup0 = param_list_show.param_list[i].data_y;
				/*------------------------------------*/
				free(param_list_show.param_list[i].data_y);
				/* release */
				param_list_show.param_list[i].data_y = NULL;
			}
		}
		/*-----------------*/
		if( param_list_show.param_list[i].data_x != NULL )
		{
			if( backup1 != param_list_show.param_list[i].data_x )
			{
				backup1 = param_list_show.param_list[i].data_x;
				/*--------------------------*/
				unsigned int j = 0;
				/* set num */
				for( j = 0 ; j < num_p ; j ++ )
				{
					if( p_used[j] == backup1 )
					{
						break;
					}
				}
				if( j != num_p )
				{
					continue;
				}
				/* free */
				free(param_list_show.param_list[i].data_x);
				/* release */
				param_list_show.param_list[i].data_x = NULL;
				/*------------------------------*/
				p_used[num_p++] = backup1;
			}
		}
	}
}
/*----------------------------------------------*/
void CTeeChart5_testDlg::reflush_chart(void)
{
	/* clear without clear line msg */
	clear_all_line(1);
	/*-----------------------------*/
	last_postion = 0;
	lane_last_postion = 0;
	last_sd = 0;
	lat_pos_point = 0;
	/* release the memeries */
	release_memory();
	/*-----------------------------*/
	param_list_show.param_list_num = 0;
	/*-----------------------------*/
	memset(last_dou,0,sizeof(last_dou));
	/*-----------------------------*/
	version_ctrl = 0;
	/*-----------------------------*/
	for( unsigned int j = 0 ; j < file_man.num ; j ++ )
	{
		if( file_man.file[j].file_enable == 1 )
		{
			/* do decode */
	        Read_Procotol_decode_waves(j);
			/* return ok */
		}
	}
	/* redraw */
	for( unsigned int i = 0 ; i < param_list_show.param_list_num ; i ++ )
	{
		if( param_list_show.param_list[i].status == 0xFE )
		{
			/* has not show */
			if( param_list_show.param_list_num )
			{
			  /*---------------------------------*/
			  draw_single(i);
			}else if( COM.radio_status == 1 )
			{
				//============================
				Get_line_group_index(i);
			}
		}
		/*----------------------------*/
		/*----------------------------*/
	}
}
/*------------------------------*/
static unsigned int color_tabal[7];
const unsigned int color_tabal_tmp[7] = {0x1BC700,0xFEB610,0xFF0000,0x0,0xBA398E,0x1044F3,0x2EA8ED};
/* get color */
unsigned int CTeeChart5_testDlg::get_color(unsigned int mode)
{
	/*------------*/
	unsigned int color_tmp;
	/*--------------*/
	if( color_index< 7 )
	{
		/*-----------------------------*/
		color_tmp = color_tabal[color_index];
	}else
	{
		srand((unsigned int)time(NULL));
		/*-----------*/
		unsigned int colorR = (unsigned char)rand();
		unsigned int colorG = (unsigned char)rand();
		unsigned int colorB = (unsigned char)rand();
		unsigned long cols = (colorB<<16)|(colorG<<8)|(colorR);
		/*----------------*/
		color_tmp = cols;
	}
	/*-------------------------*/
	color_index++;
	/*------------------------*/
	return color_tmp;
}
/* create the color table */
void CTeeChart5_testDlg::create_color_table(void)
{
	srand((unsigned int)time(NULL));
	unsigned int tmp;
	/*-----------*/
	for( int i = 0 ; i < 7 ; i++ )
	{
		tmp = rand() % 7;
	}
	/*---------------*/
	for( int i = 0 ; i < 7 ; i++ )
	{
		color_tabal[i] = color_tabal_tmp[(i+tmp)%7];
	}
}
/* VERSION_TABLE */
const char *version_table[19] = {"VERSION_FC_NEW","VERSION_GPS","VERSION_BOTTOM","VERSION_MAG1","VERSION_MAG2","VERSION_SONAR",
                                 "VERSION_GIMBAL1","VERSION_GIMBAL5","VERSION_ESC1","VERSION_ESC2","VERSION_ESC3",
                                 "VERSION_ESC4","VERSION_VISION","VERSION_CPLD","VERSION_FPGA","VERSION_BATTERY",
                                 "VERSION_LIDAR_CTRL","VERSION_LIDAR_DEV","VERSION_OP3000"};
/*-----------------------------------------------*/
void CTeeChart5_testDlg::create_version_line(unsigned int mode)
{
	/* uplock mode */
	m_check_hold.SetCheck(0);//
	/*-----------------------*/
	unsigned int i = 0;
	/*-----------------------*/
	for( i = 0 ; i < param_list_show.param_list_num ; i ++ )
	{
		if( strstr(param_list_show.param_list[i].name,version_table[version_ctrl]) != NULL )
		{
			/* draw the line */
			draw_single(i);
			/*------------------------*/
			break;
		}
	}
	/*-------------------------------*/
	if( i >= param_list_show.param_list_num )
	{
		char buffer[100];
		/*-------------------------------*/
		sprintf_s(buffer,"Can not find %s Skip it",version_table[version_ctrl]);
		/*-------------------------------*/
		USES_CONVERSION;
		/*-------------------------------*/
		CString show = A2T(buffer);
		/*-------------------------------*/
		AfxMessageBox(show);
	}
	/*-------------------------------*/
	version_ctrl++;
	//------------------
	if( version_ctrl >= 19 )
	{
		version_ctrl = 0;
	}
	/*------------------------*/
}
/*-----------------------------------------------*/
int CTeeChart5_testDlg::fpos_analysis(unsigned int mode)
{
    int event_num_pos = 0xffff;
	int enent_time_pos = 0xffff;
	int enent_point_num = 0;
	/*---------------------*/
	int fpos_num_pos = 0xffff;
	int fpos_point_num = 0;
	int CT1_pos = 0xffff;
	int CT2_pos = 0xffff;
	int CT3_pos = 0xffff;
	int CT4_pos = 0xffff;
	int CT5_pos = 0xffff;
	int GPS_LON_FPOS = 0xffff;
	int GPS_LAT_FPOS = 0xffff;
	int HEIGHT_FPOS = 0xffff;
	/* create a file */
	FILE * txt_wb;
	char path_data[512];
	memset(path_data,0,sizeof(path_data));
	/*---------------------*/
	unsigned int TIME_PERIOD = 10*1000;//5ms
	/*---------------------*/
	/* find the lat and lon in param list */
	for( unsigned int i = 0 ; i < param_list_show.param_list_num ; i ++ )
	{
		if( event_num_pos == 0xffff && strstr(param_list_show.param_list[i].name,"EVENTNUMBER") != NULL )
		{
			event_num_pos = i;
			enent_point_num = param_list_show.param_list[i].point_num;
		}

		if( enent_time_pos == 0xffff && strstr(param_list_show.param_list[i].name,"EVENTTIME") != NULL )
		{
			enent_time_pos = i;
		}
		/*--------fpos----------*/
		if( fpos_num_pos == 0xffff && strstr(param_list_show.param_list[i].name,"FPOS_INDEX") != NULL )
		{
			fpos_num_pos = i;
			fpos_point_num = param_list_show.param_list[i].point_num;
			strcpy_s(path_data,param_list_show.param_list[i].from_file);
		}
		/* fpos CT1 */
		if( CT1_pos == 0xffff && strstr(param_list_show.param_list[i].name,"CT1") != NULL )
		{
			CT1_pos = i;
		}
		/* fpos CT2 */
		if( CT2_pos == 0xffff && strstr(param_list_show.param_list[i].name,"CT2") != NULL )
		{
			CT2_pos = i;
		}
		/* fpos CT3 */
		if( CT3_pos == 0xffff && strstr(param_list_show.param_list[i].name,"CT3") != NULL )
		{
			CT3_pos = i;
		}
		/* fpos CT4 */
		if( CT4_pos == 0xffff && strstr(param_list_show.param_list[i].name,"CT4") != NULL )
		{
			CT4_pos = i;
		}
		/* fpos CT5 */
		if( CT5_pos == 0xffff && strstr(param_list_show.param_list[i].name,"CT5") != NULL )
		{
			CT5_pos = i;
		}
		/* postion */
		if( GPS_LON_FPOS == 0xffff && strstr(param_list_show.param_list[i].name,"GPS_LON_FPOS") != NULL )
		{
			GPS_LON_FPOS = i;
		}
		/* postion */
		if( GPS_LAT_FPOS == 0xffff && strstr(param_list_show.param_list[i].name,"GPS_LAT_FPOS") != NULL )
		{
			GPS_LAT_FPOS = i;
		}
		/* postion */
		if( HEIGHT_FPOS == 0xffff && strstr(param_list_show.param_list[i].name,"HEIGHT_FPOS") != NULL )
		{
			HEIGHT_FPOS = i;
		}
		/*--*/
		if( event_num_pos != 0xffff && enent_time_pos != 0xffff && fpos_num_pos != 0xffff &&
			CT1_pos != 0xffff && CT2_pos != 0xffff && CT3_pos != 0xffff && CT4_pos != 0xffff && CT5_pos != 0xffff && 
			GPS_LON_FPOS != 0xffff && GPS_LAT_FPOS != 0xffff && HEIGHT_FPOS != 0xffff)
		{
			break;
		}
	}
	/*-------------------------------*/
	if( event_num_pos == 0xffff || enent_time_pos == 0xffff || fpos_num_pos == 0xffff ||
			CT1_pos == 0xffff || CT2_pos == 0xffff || CT3_pos == 0xffff || CT4_pos == 0xffff || CT5_pos == 0xffff || 
			GPS_LON_FPOS == 0xffff || GPS_LAT_FPOS == 0xffff || HEIGHT_FPOS == 0xffff)
	{
		AfxMessageBox(_T("一些文件没有打开"));
		return (-1);
	}
	/*------------------------------*/
	char create_buffer[512];
	memset(create_buffer,0,sizeof(create_buffer));
	//-------------
	sprintf_s(create_buffer,"%s.txt",path_data);
	// open file 
	fopen_s(&txt_wb,create_buffer,"wb+");
	/*-------------------------------*/
	if( txt_wb == NULL )
	{
		AfxMessageBox(_T("输出文件创建失败，不能分析"));
		return (-1);
	}
	/*------------------------------*/
	char output[1024];
	unsigned int error_flag = 0;
	/* clear buffer */
	memset(output,0,sizeof(output));
	/* check rt27 event */
	double * tmp = (double *)param_list_show.param_list[event_num_pos].data_y;
	/*------------------*/
	int firt_num_event = 0;
	/*------------------*/
	if( (int)tmp[0] < 50 )
	{
		firt_num_event = (int)tmp[0];
	}
	/* check num */
	if( enent_point_num != ( (int)tmp[enent_point_num - 1 ] - (int)tmp[0] + 1 ) )
	{
		int last = (int)tmp[0];
		/* check what */
		for( int i = 0 ; i < enent_point_num ; i ++ )
		{
			if( last != (int)tmp[i] )
			{
				/* position */
				memset(output,0,sizeof(output));
				sprintf_s(output,"1.RT27中打标点在%d处发生丢失,异常\r\n",i);
				fwrite(output,strlen(output),1,txt_wb);
				/*-------------------------*/
				last = (int)tmp[i]; 
				error_flag ++;
			}
			/*--------------------*/
            last ++;
		}
	}else
	{
		memset(output,0,sizeof(output));
		sprintf_s(output,"1.RT27中打标点数量与解析出来的数量相同，正确\r\n");
		fwrite(output,strlen(output),1,txt_wb);
	}
	/*----check fpos num-------*/
	tmp = (double *)param_list_show.param_list[fpos_num_pos].data_y;
	/* check num */
	if( fpos_point_num != ( (int)tmp[fpos_point_num - 1 ] - (int)tmp[0] + 1 ) )
	{
		int last = (int)tmp[0];
		/* check what */
		for( int i = 0 ; i < fpos_point_num ; i ++ )
		{
			if( last != (int)tmp[i] )
			{
				/* position */
				memset(output,0,sizeof(output));
				sprintf_s(output,"2.FPOS中记录条数在%d处发生丢失,异常\r\n",i);
				fwrite(output,strlen(output),1,txt_wb);
				/*-------------------------*/
				last = (int)tmp[i]; 
				error_flag++;
			}
			/*--------------------*/
            last ++;
		}
	}else
	{
		memset(output,0,sizeof(output));
		sprintf_s(output,"2.FPOS中记录条数与解析出来的数量相同，正确\r\n");
		fwrite(output,strlen(output),1,txt_wb);
	}
	/* check event and event */
	if( error_flag == 0 )
	{
		if( fpos_point_num <= enent_point_num )
		{
			/* calibrate and create line */
			double *e_time;
			/*----------------------------*/
			const double *ct1_const = (double *)param_list_show.param_list[CT1_pos].data_y;
			const double *ct2_const = (double *)param_list_show.param_list[CT2_pos].data_y;
			const double *ct3_const = (double *)param_list_show.param_list[CT3_pos].data_y;
			const double *ct4_const = (double *)param_list_show.param_list[CT4_pos].data_y;
			const double *ct5_const = (double *)param_list_show.param_list[CT5_pos].data_y;
			/*----------------------------*/
			/* set x axis */
			unsigned char *x_axis = (unsigned char *)malloc(fpos_point_num*8);
			/*----------------------------*/
			e_time = (double *)param_list_show.param_list[enent_time_pos].data_y;
			/*----------------------------*/
			strcpy_s(param_list_show.param_list[param_list_show.param_list_num].name,"EVENT-CT1(us)");
			/* malloc memorizes */
			param_list_show.param_list[param_list_show.param_list_num].data_y = (unsigned char *)malloc(fpos_point_num*8);
			param_list_show.param_list[param_list_show.param_list_num].data_x = x_axis;
			/* dest */
			double *ct1_data = (double *)param_list_show.param_list[param_list_show.param_list_num].data_y;
			param_list_show.param_list[param_list_show.param_list_num].point_num = fpos_point_num;
			/*------------------*/
			param_list_show.param_list_num++;
			/*-------------------------------------------------------------------------------*/
			strcpy_s(param_list_show.param_list[param_list_show.param_list_num].name,"EVENT-CT2(us)");
			/* malloc memorizes */
			param_list_show.param_list[param_list_show.param_list_num].data_y = (unsigned char *)malloc(fpos_point_num*8);
			param_list_show.param_list[param_list_show.param_list_num].data_x = x_axis;
			/* dest */
			double *ct2_data = (double *)param_list_show.param_list[param_list_show.param_list_num].data_y;
			param_list_show.param_list[param_list_show.param_list_num].point_num = fpos_point_num;
			/*------------------*/
			param_list_show.param_list_num++;
			/*-------------------------------------------------------------------------------*/
			strcpy_s(param_list_show.param_list[param_list_show.param_list_num].name,"EVENT-CT3(us)");
			/* malloc memorizes */
			param_list_show.param_list[param_list_show.param_list_num].data_y = (unsigned char *)malloc(fpos_point_num*8);
			param_list_show.param_list[param_list_show.param_list_num].data_x = x_axis;
			/* dest */
			double *ct3_data = (double *)param_list_show.param_list[param_list_show.param_list_num].data_y;
			param_list_show.param_list[param_list_show.param_list_num].point_num = fpos_point_num;
			/*------------------*/
			param_list_show.param_list_num++;
			/*-------------------------------------------------------------------------------*/
			strcpy_s(param_list_show.param_list[param_list_show.param_list_num].name,"EVENT-CT4(us)");
			/* malloc memorizes */
			param_list_show.param_list[param_list_show.param_list_num].data_y = (unsigned char *)malloc(fpos_point_num*8);
			param_list_show.param_list[param_list_show.param_list_num].data_x = x_axis;
			/* dest */
			double *ct4_data = (double *)param_list_show.param_list[param_list_show.param_list_num].data_y;
			param_list_show.param_list[param_list_show.param_list_num].point_num = fpos_point_num;
			/*------------------*/
			param_list_show.param_list_num++;
			/*-------------------------------------------------------------------------------*/
			strcpy_s(param_list_show.param_list[param_list_show.param_list_num].name,"EVENT-CT5(us)");
			/* malloc memorizes */
			param_list_show.param_list[param_list_show.param_list_num].data_y = (unsigned char *)malloc(fpos_point_num*8);
			param_list_show.param_list[param_list_show.param_list_num].data_x = x_axis;
			/* dest */
			double *ct5_data = (double *)param_list_show.param_list[param_list_show.param_list_num].data_y;
			param_list_show.param_list[param_list_show.param_list_num].point_num = fpos_point_num;
			/*------------------*/
			param_list_show.param_list_num++;
			/*-------------------------------------------------------------------------------*/
			int diff_basic = enent_point_num - fpos_point_num;
			/* temp */
			double * x_axis_d = (double *)x_axis;
			// open source
			for( int i = 0 ; i < fpos_point_num ; i ++ )
			{   
				double diff = e_time[diff_basic+i] - ct1_const[i];
				/* judge */
				if( abs(diff) > TIME_PERIOD )//50 ms
				{
					memset(output,0,sizeof(output));
					sprintf_s(output,"3.时间间隔EVENT-CT1异常 RT27:%d,%lf FPOS:%d,%lf diff:%lf\r\n",i+diff_basic+firt_num_event,e_time[diff_basic+i],i+1,ct1_const[i],diff);
					fwrite(output,strlen(output),1,txt_wb);
					error_flag++;
				}
				// save data 
                ct1_data[i] = diff;
				/*---------------------------------------------------------------------*/
				diff = e_time[diff_basic+i] - ct2_const[i];
				/* judge */
				if( abs(diff) > TIME_PERIOD )//50 ms
				{
					memset(output,0,sizeof(output));
					sprintf_s(output,"3.时间间隔EVENT-CT2异常 RT27:%d,%lf FPOS:%d,%lf diff:%lf\r\n",i+diff_basic+firt_num_event,e_time[diff_basic+i],i+1,ct2_const[i],diff);
					fwrite(output,strlen(output),1,txt_wb);
					error_flag++;
				}
				// save data 
                ct2_data[i] = diff;
				/*---------------------------------------------------------------------*/
				diff = e_time[diff_basic+i] - ct3_const[i];
				/* judge */
				if( abs(diff) > TIME_PERIOD )//50 ms
				{
					memset(output,0,sizeof(output));
					sprintf_s(output,"3.时间间隔EVENT-CT3异常 RT27:%d,%lf FPOS:%d,%lf diff:%lf\r\n",i+diff_basic+firt_num_event,e_time[diff_basic+i],i+1,ct3_const[i],diff);
					fwrite(output,strlen(output),1,txt_wb);
					error_flag++;
				}
				// save data 
                ct3_data[i] = diff;
				/*---------------------------------------------------------------------*/
				diff = e_time[diff_basic+i] - ct4_const[i];
				/* judge */
				if( abs(diff) > TIME_PERIOD )//50 ms
				{
					memset(output,0,sizeof(output));
					sprintf_s(output,"3.时间间隔EVENT-CT4异常 RT27:%d,%lf FPOS:%d,%lf diff:%lf\r\n",i+diff_basic+firt_num_event,e_time[diff_basic+i],i+1,ct4_const[i],diff);
					fwrite(output,strlen(output),1,txt_wb);
					error_flag++;
				}
				// save data 
                ct4_data[i] = diff;
				/*---------------------------------------------------------------------*/
				diff = e_time[diff_basic+i] - ct5_const[i];
				/* judge */
				if( abs(diff) > TIME_PERIOD )//50 ms
				{
					memset(output,0,sizeof(output));
					sprintf_s(output,"3.时间间隔EVENT-CT5异常 RT27:%d,%lf FPOS:%d,%lf diff:%lf\r\n",i+diff_basic+firt_num_event,e_time[diff_basic+i],i+1,ct5_const[i],diff);
					fwrite(output,strlen(output),1,txt_wb);
					error_flag++;
				}
				// save data 
                ct5_data[i] = diff;
				/*---------------------------------------------------------------------*/
				x_axis_d[i] = i;
				/*---------------------------------------------------------------------*/
			}
			/*-------------------------------------------------------------------------------*/
			if( error_flag == 0 )
			{
				memset(output,0,sizeof(output));
				sprintf_s(output,"3.时间间隔-----------------正确\r\n");
				fwrite(output,strlen(output),1,txt_wb);
			}
			/*-------------------------------------------------------------------------------*/
		}else
		{
			memset(output,0,sizeof(output));
			sprintf_s(output,"3.FPOS中记录数量与RT27记录打标点数量不匹配，相差%d，异常\r\n",fpos_point_num - enent_point_num);
			fwrite(output,strlen(output),1,txt_wb);
			error_flag++;
		}
	}
	/* check pos */
	const double *lon_const = (double *)param_list_show.param_list[GPS_LON_FPOS].data_y;
	const double *lat_const = (double *)param_list_show.param_list[GPS_LAT_FPOS].data_y;
	/* i */
	double lon_last = lon_const[0];
	double lat_last = lat_const[0];
	/*------------------------------------------------------------------------------*/
	int error_fpos_lon_lat = 0;
	/*------------------------------------------------------------------------------*/
	for(unsigned int i = 1 ; i < param_list_show.param_list[GPS_LON_FPOS].point_num ; i ++ )
	{
		if( lon_const[i] == lon_last && lat_const[i] == lat_last )
		{
			memset(output,0,sizeof(output));
			sprintf_s(output,"4.FPOS中记录数量存在相同的位置点，异常，fpos行:%d\r\n",i);
			fwrite(output,strlen(output),1,txt_wb);
			error_flag++;
			error_fpos_lon_lat++;
		}
		/*-----------------------------------*/
		lon_last = lon_const[i];
		lat_last = lat_const[i];
		/*-----------------------------------*/
	}
	/*---------------------------------------*/
	if( error_fpos_lon_lat == 0 )
	{
		memset(output,0,sizeof(output));
		sprintf_s(output,"4.FPOS中记录数量位置点，正确\r\n");
		fwrite(output,strlen(output),1,txt_wb);
	}
	/*--------------*/
    /* transform */
	USES_CONVERSION;
	/* fresh the combox */
	CString show;
	/* show */
	if( error_flag == 0 )
	{
		memset(output,0,sizeof(output));
		sprintf_s(output,"\r\n\r\n总结：未发现问题\r\n");
		fwrite(output,strlen(output),1,txt_wb);
		/*--------------------------------------*/
		//AfxMessageBox(_T("分析完成，未发现问题"));
		if( MessageBox(_T("分析完成，未发现问题，点<确定>查看分析报告"),_T("分析报告"),1) == 1 )
		{
			error_flag = 0;//open txt
			/*-----------------------------*/
			show = A2T(create_buffer);
			ShellExecute(NULL,_T("open"),show,NULL,NULL,SW_SHOW);
			/*-----------------------------*/
		}else
		{
			error_flag = 0;
		}
	}else
	{
		memset(output,0,sizeof(output));
		sprintf_s(output,"\r\n\r\n总结：发现问题!!!!!!!!!!!!!!!\r\n");
		fwrite(output,strlen(output),1,txt_wb);
		/*--------------------------------------*/
	    if( MessageBox(_T("分析完成，发现问题，点<确定>查看分析报告"),_T("分析报告"),1) == 1 )
		{
			error_flag = 0; // open txt
			/*-----------------------------*/
			show = A2T(create_buffer);
			ShellExecute(NULL,_T("open"),show,NULL,NULL,SW_SHOW);
			/*-----------------------------*/
		}else
		{
			error_flag = 0;
		}
	}
	//fresh the combox
	combox_list_fresh();
	/*-------------------------------------------*/
	fclose(txt_wb);
	/*--------------*/
	return 0;
}
/*-----------------------------------------------*/
void CTeeChart5_testDlg::combox_list_fresh(void)
{
    /* transform */
	USES_CONVERSION;
	/* fresh the combox */
	CString show;
	/*------------------*/
	m_combox_param_show.ResetContent();
    /*-------------------------------------------*/
	for( unsigned int i = 0 ; i < param_list_show.param_list_num ; i ++ )
	{
		show = A2T(param_list_show.param_list[i].name);
	    m_combox_param_show.AddString(show);
	}
	/*-------------------------------------------*/
	m_combox_param_show.SetCurSel(param_list_show.param_list_num - 1);
}
/*-----------------------------------------------*/
void CTeeChart5_testDlg::delete_point(void)
{
	del_point del;
	del.DoModal();
	/*-------------------------*/
	if( system_delete_point.enable )
	{
		system_delete_point.enable = 0;
		/*---------------------*/
		CSeries line = (CSeries)m_chart.Series(system_delete_point.line_num);
		/*---------------------*/
		for( unsigned int i = system_delete_point.from ; i < system_delete_point.to + 1 ; i ++ )
		{
			line.Delete(i);
		}
		/*---------------------*/
	}
	/*-------------------------*/
}
/*-----------------------------------------------*/
void CTeeChart5_testDlg::OnBnClickedButton31()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTeeChart5_testDlg::OnBnClickedButton32()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CTeeChart5_testDlg::OnBnClickedButton33()
{
	// TODO: 在此添加控件通知处理程序代码
}
/* */
int CTeeChart5_testDlg::function_thread(unsigned int start)
{
	int ret = 0;
	/* find func */
	for( unsigned int i = start ; i < param_list_show.param_list_num ; i ++ )
	{
		if( param_list_show.param_list[i].function_enable == 1 )
		{
			switch(param_list_show.param_list[i].func)
			{
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				ret = func_copy2_0(start,i);
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
			case 8:
				break;
			case 9:
				ret = func_ky(start,i);
				break;
			case 10:
				break;
			case 11:
				ret = func_PCG2(start,i);
				break;
			case 12:
				break;
			case 13:
				ret = func_kyx(start,i);
				break;
			case 14:
				break;
			case 15:
				break;
			default:
				break;
			}
		}
	}
	return ret;
}
/* function exe */
int CTeeChart5_testDlg::func_copy2_0(unsigned int start , int index)
{
	char buffer[2][128];
	/* copy2_0 need two params */
	if( param_list_show.param_list[index].func_param_count != 3 )
	{
		return (-1);//param error
	}
	/* template */
	char * p0 = param_list_show.param_list[index].func_param[0];
	char * p1 = param_list_show.param_list[index].func_param[1];
	/* create buffer */
	memset(buffer,0,sizeof(buffer));
	/* get name and set param */
	sprintf_s(buffer[0],"->%s",p0);
	sprintf_s(buffer[1],"->%s",p1);
	/* find flag */
	int pos_0 = 0xffff;
	int pos_1 = 0xffff;
	/* search */
	for( unsigned int i = start ; i < param_list_show.param_list_num ; i ++ )
	{
		char * c;
		/* param one */
		c = strstr(param_list_show.param_list[i].name,buffer[0]);
		/* ok */
		if( c != NULL && *(c+strlen(buffer[0])) == 0 )
		{
			pos_0 = i;
		}
		/* param two */
		c = strstr(param_list_show.param_list[i].name,buffer[1]);
		/* ok */
		if( c != NULL && *(c+strlen(buffer[1])) == 0 )
		{
			pos_1 = i;
		}
		/* over */
		if( pos_0 != 0xffff && pos_1 != 0xffff )
		{
			break;// get first package
		}
	}
	/* ok ? */
	if( pos_0 == 0xffff || pos_1 == 0xffff )
	{
		return (-2);
	}
	/*--------------------------------*/
	int point_num = param_list_show.param_list[pos_0].point_num > param_list_show.param_list[pos_1].point_num ?
	                param_list_show.param_list[pos_1].point_num :param_list_show.param_list[pos_0].point_num; //max num
	/* macro memory */
	param_list_show.param_list[index].data_x = 
		(unsigned char *)malloc(point_num*8);
	/*--------------------------------*/
	if( param_list_show.param_list[index].data_x == NULL )
	{
		return (-3);//macro fail
	}
	/* get data and decode */
	double  *line_src_y0 = (double *)param_list_show.param_list[pos_0].data_y;
	double  *line_src_y1 = (double *)param_list_show.param_list[pos_1].data_y;
	double  *line_dst_y = (double *)param_list_show.param_list[index].data_y;
	double  *line_dst_x = (double *)param_list_show.param_list[index].data_x;
	/* clear point num */
	param_list_show.param_list[index].point_num = 0;
	int filter = 0;
	/*---------------------*/
	for( int i = 0 ; i < point_num ; i ++ )
	{
		if( filter == 0 )
		{
			if( line_src_y0[i] != 0 && line_src_y1[i] != 0 )
			{
				filter = 1;
			}
		}else
		{
			line_dst_y[param_list_show.param_list[index].point_num] = line_src_y0[i];
			line_dst_x[param_list_show.param_list[index].point_num] = line_src_y1[i];
			/* set param ok */
			param_list_show.param_list[index].point_num++;
		}
	}
	/*---------------------------------------------------*/
	return 0;
}
/* function exe */
int CTeeChart5_testDlg::func_PCG2(unsigned int start , int index)
{
	char buffer[3][128];
	/* copy2_0 need two params */
	if( param_list_show.param_list[index].func_param_count != 4 )
	{
		return (-1);//param error
	}
	/* template */
	char * p0 = param_list_show.param_list[index].func_param[0];
	char * p1 = param_list_show.param_list[index].func_param[1];
	char * p2 = param_list_show.param_list[index].func_param[2];
	/* create buffer */
	memset(buffer,0,sizeof(buffer));
	/* get name and set param */
	sprintf_s(buffer[0],"->%s",p0);
	sprintf_s(buffer[1],"->%s",p1);
	sprintf_s(buffer[2],"->%s",p2);
	/* find flag */
	int pos_0 = 0xffff;
	int pos_1 = 0xffff;
	int pos_2 = 0xffff;
	/* search */
	for( unsigned int i = start ; i < param_list_show.param_list_num ; i ++ )
	{
		char * c;
		/* param one */
		c = strstr(param_list_show.param_list[i].name,buffer[0]);
		/* ok */
		if( c != NULL && *(c+strlen(buffer[0])) == 0 )
		{
			pos_0 = i;
		}
		/* param two */
		c = strstr(param_list_show.param_list[i].name,buffer[1]);
		/* ok */
		if( c != NULL && *(c+strlen(buffer[1])) == 0 )
		{
			pos_1 = i;
		}
		/* param three */
		c = strstr(param_list_show.param_list[i].name,buffer[2]);
		/* ok */
		if( c != NULL && *(c+strlen(buffer[2])) == 0 )
		{
			pos_2 = i;
		}
		/* over */
		if( pos_0 != 0xffff && pos_1 != 0xffff && pos_2 != 0xffff)
		{
			break;// get first package
		}
	}
	/* ok ? */
	if( pos_0 == 0xffff || pos_1 == 0xffff || pos_2 == 0xffff )
	{
		return (-2);
	}
	/*--------------------------------*/
	int point_num = param_list_show.param_list[pos_0].point_num;
	/* macro memory */
	param_list_show.param_list[index].data_x = 
		(unsigned char *)malloc(point_num*8);
	/*--------------------------------*/
	if( param_list_show.param_list[index].data_x == NULL )
	{
		return (-3);//macro fail
	}
	/* get data and decode */
	double  *line_src_p = (double *)param_list_show.param_list[pos_0].data_y;
	double  *line_src_y0 = (double *)param_list_show.param_list[pos_1].data_y;
	double  *line_src_y1 = (double *)param_list_show.param_list[pos_2].data_y;
	double  *line_dst_y = (double *)param_list_show.param_list[index].data_y;
	double  *line_dst_x = (double *)param_list_show.param_list[index].data_x;
	/* clear point num */
	param_list_show.param_list[index].point_num = 0;
	double last = 0;
	/*---------------------*/
	for( int i = 0 ; i < point_num ; i ++ )
	{
		if( last != line_src_p[i] )
		{
			last = line_src_p[i];
			line_dst_y[param_list_show.param_list[index].point_num] = line_src_y0[i];
			line_dst_x[param_list_show.param_list[index].point_num] = line_src_y1[i];
			/* set param ok */
			param_list_show.param_list[index].point_num++;
		}
	}
	/*---------------------------------------------------*/
	return 0;
}
/* function exe */
int CTeeChart5_testDlg::func_ky(unsigned int start , int index)
{
	char buffer[1][128];
	/* copy2_0 need two params */
	if( param_list_show.param_list[index].func_param_count != 2 )
	{
		return (-1);//param error
	}
	/* template */
	char * p0 = param_list_show.param_list[index].func_param[0];
	/* create buffer */
	memset(buffer,0,sizeof(buffer));
	/* get name and set param */
	sprintf_s(buffer[0],"->%s",p0);
	/* find flag */
	int pos_0 = 0xffff;
	/* search */
	for( unsigned int i = start ; i < param_list_show.param_list_num ; i ++ )
	{
		char * c;
		/* param one */
		c = strstr(param_list_show.param_list[i].name,buffer[0]);
		/* ok */
		if( c != NULL && *(c+strlen(buffer[0])) == 0 )
		{
			pos_0 = i;
		}
		/* over */
		if( pos_0 != 0xffff )
		{
			break;// get first package
		}
	}
	/* ok ? */
	if( pos_0 == 0xffff )
	{
		return (-2);
	}
	/*--------------------------------*/
	int point_num = param_list_show.param_list[pos_0].point_num;
	/* get data and decode */
	double  *line_src_y0 = (double *)param_list_show.param_list[pos_0].data_y;
	double  *line_dst_y  = (double *)param_list_show.param_list[index].data_y;
	/* clear point num */
	param_list_show.param_list[index].point_num = 0;
	int filter = 0;
	/*---------------------*/
	for( int i = 1 ; i < point_num ; i ++ )
	{
		line_dst_y[param_list_show.param_list[index].point_num] = line_src_y0[i] - line_src_y0[i-1];
		/* set param ok */
		param_list_show.param_list[index].point_num++;
	}
	/*---------------------------------------------------*/
	return 0;
}
/* function exe */
int CTeeChart5_testDlg::func_kyx(unsigned int start , int index)
{
	char buffer[1][128];
	/* copy2_0 need two params */
	if( param_list_show.param_list[index].func_param_count != 2 )
	{
		return (-1);//param error
	}
	/* template */
	char * p0 = param_list_show.param_list[index].func_param[0];
	/* create buffer */
	memset(buffer,0,sizeof(buffer));
	/* get name and set param */
	sprintf_s(buffer[0],"->%s",p0);
	/* find flag */
	int pos_0 = 0xffff;
	/* search */
	for( unsigned int i = start ; i < param_list_show.param_list_num ; i ++ )
	{
		char * c;
		/* param one */
		c = strstr(param_list_show.param_list[i].name,buffer[0]);
		/* ok */
		if( c != NULL && *(c+strlen(buffer[0])) == 0 )
		{
			pos_0 = i;
		}
		/* over */
		if( pos_0 != 0xffff )
		{
			break;// get first package
		}
	}
	/* ok ? */
	if( pos_0 == 0xffff )
	{
		return (-2);
	}
	/*--------------------------------*/
	int point_num = param_list_show.param_list[pos_0].point_num;
	/* macro memory */
	param_list_show.param_list[index].data_x = 
		(unsigned char *)malloc(point_num*8);
	/*--------------------------------*/
	if( param_list_show.param_list[index].data_x == NULL )
	{
		return (-3);//macro fail
	}
	/* get data and decode */
	double  *line_src_y0 = (double *)param_list_show.param_list[pos_0].data_y;
	double  *line_dst_y  = (double *)param_list_show.param_list[index].data_y;
	double  *line_dst_x  = (double *)param_list_show.param_list[index].data_x;
	/* clear point num */
	param_list_show.param_list[index].point_num = 0;
	int filter = 0;
	double last = 123456;
	int last_i = 0;
	/*---------------------*/
	for( int i = 0 ; i < point_num ; i ++ )
	{
		if( last != line_src_y0[i] )
		{
			last = line_src_y0[i];
			/* create */
			int t = i - last_i;
			double td;
			/* -------------- */
			if( param_list_show.param_list[index].time_us != 0 )
			{
				td = (double)t * (double)READ_CFS.cfs_global_msg.time_mark / (double)1000000;
			}
			/*----------------*/
			line_dst_y[param_list_show.param_list[index].point_num] = td;
			line_dst_x[param_list_show.param_list[index].point_num] = param_list_show.param_list[index].point_num;
			/* incremeter */
		    param_list_show.param_list[index].point_num++;
			last_i = i;
		}
	}
	/*---------------------------------------------------*/
	return 0;
}
/* hot key ideal */
void CTeeChart5_testDlg::HOTKEY_THREAD(char key)
{
	int num = param_list_show.param_list_num;
	/*-----------------------------------*/
	for( int i = 0 ; i < num ; i ++ )
	{
		for( int j = 0 ; j < 16 ; j ++ )
		{
			if( param_list_show.param_list[i].hot_key[j] == key )
			{
				draw_single(i);
				break;
			}
			/* break */
			if( param_list_show.param_list[i].hot_key[j] == 0 )
			{
				break;
			}
		}
	}
}
/*-----------------------------*/
void CTeeChart5_testDlg::Open_cfg(void)
{
	C_open openDlg;
	openDlg.DoModal();
}
void CTeeChart5_testDlg::open_data_review(void)
{
	data_review re;
	re.DoModal();
}