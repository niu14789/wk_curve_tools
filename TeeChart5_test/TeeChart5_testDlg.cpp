
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
SYSTEM_AUTO_SCALE_DEF auto_scale_g;
/*------------------------------------*/
motor motor_dlg;
/*------------------------------------*/
static unsigned int color_index = 0;
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
	ON_COMMAND(ID_32786, &CTeeChart5_testDlg::On32786)
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
	 /* line init */
	 chart_line_init();
	 /*-------------------*/
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
	CEnvironment evn = m_chart.get_Environment();
	evn.put_MouseWheelScroll(0);
	/*----------------------*/
	// should be read from files
	auto_scale_g.mutiple_axis = 0;
	auto_scale_g.global_auto = 0;
	/*-------------------------*/
	for(int i = 0 ; i < 20; i ++ )
	{
	  auto_scale_g.line_cfg[i].auto_scale = 1;
	}
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
			/*------set scale------*/
			if( auto_scale_g.global_auto )
			{
				axis.put_Automatic(1);
			}else
			{
				/*--- others ---------------------------------*/
				if( auto_scale_g.line_cfg[i].auto_scale )
				{
					axis.put_Automatic(1);
				}else
				{
					axis.put_Automatic(0);
					axis.put_Minimum(auto_scale_g.line_cfg[i].min);
					axis.put_Maximum(auto_scale_g.line_cfg[i].max);
				}
				/*---------------------------------------------*/
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
		//srand(time(NULL));
		///*------*/
		//unsigned int colorR = rand()%0xff;
		//unsigned int colorG = rand()%0xff;
		//unsigned int colorB = rand()%0xff;
		///*-----------*/
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
	if( auto_scale_g.mutiple_axis == 0 && mode == 0 )
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

void CTeeChart5_testDlg::page_show(unsigned int now,unsigned int total)
{
	
}
void CTeeChart5_testDlg::move_test(void)
{
	 CSeries lineSeries = (CSeries)m_chart.Series(2);
	CSeries lineSeries1 = (CSeries)m_chart.Series(1);
   //lineSeries0.Clear();

	static int i =  0 , t = 0;

	static double bfd[200000];
	static int get = 1;

	double doub = 0;

	if( get == 1 )
	{
		for(int i = 0 ; i < 200000 ; i ++ )
		{
			bfd[i] = sin(doub);
			doub += 0.2;
		}
	}

	static double bcd[200000];

    doub = 0;

	if( get == 1 )
	{
		get = 0;
		for(int i = 0 ; i < 200000 ; i ++ )
		{
			bcd[i] = cos(doub);
			doub += 0.2;
		}
	}

	lineSeries.Clear();
	lineSeries1.Clear();

    for ( i = t ; i < 200 + t; i++)
    {
        lineSeries.AddXY((double)i, bfd[i],NULL,NULL);
		lineSeries1.AddXY((double)i, bcd[i],NULL,NULL);
    }
	t+=1;
}
void CTeeChart5_testDlg::OnBnClickedButton3()
{
	AfxMessageBox(_T("功能开发中"));
}

void CTeeChart5_testDlg::OnBnClickedButton4()
{
   	CAxes chartaxis =(CAxes)m_chart.get_Axis();
	
	CAxis axis = (CAxis)chartaxis.get_Left();
	/*----------------------------------*/
	int axis_changed = auto_scale_g.mutiple_axis;
	/*----------------------------------*/
	Auto_set dl;
	dl.DoModal();
	/*------set scale------*/
	if( auto_scale_g.global_auto )
	{
		axis.put_Automatic(1);
		/*----------------------*/
		for( int i = 0 ; i < 20 ; i ++ )
		{
			axis = (CAxis)chartaxis.get_Custom(i);
			/* put automatic */
			axis.put_Automatic(1);
		}
	}else
	{
		if( auto_scale_g.line_cfg[0].auto_scale )
		{
			axis.put_Automatic(1);
		}else
		{
			axis.put_Automatic(0);
			axis.put_Minimum(auto_scale_g.line_cfg[0].min);
			axis.put_Maximum(auto_scale_g.line_cfg[0].max);
		}
		/*--- others ---------------------------------*/
		for( int i = 0 ; i < 19 ; i ++ )
		{
			axis = (CAxis)chartaxis.get_Custom(i);
			/*---------------------------------*/
			if( auto_scale_g.line_cfg[i+1].auto_scale )
			{
				axis.put_Automatic(1);
			}else
			{
				axis.put_Automatic(0);
				axis.put_Minimum(auto_scale_g.line_cfg[i+1].min);
				axis.put_Maximum(auto_scale_g.line_cfg[i+1].max);
			}
		}
		/*---------------------------------------------*/
	}
	/*-------------------------------------------------*/
	if( axis_changed != auto_scale_g.mutiple_axis )
	{
		reflush_chart();
	}
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
			for( int j = 0 ; j < param_list_show.param_list_num ; j++ )
			{
				if( param_list_show.param_list[j].status == 0xff )
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
	}
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
		 m_show_test.SetWindowTextW(_T("显示测试表"));
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
		 m_show_test.SetWindowTextW(_T("隐藏测试表"));
		 /*-----*/
		 flags = 0;
	}
}

void CTeeChart5_testDlg::OnBnClickedButton9()
{   
	system_config_inf.inf[1] ^= 1;
	check_list_show(system_config_inf.inf[1]);
	///* fresh the data */
	system_config_fresh(1,system_config_inf.inf[1]);
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
	}else if( strstr(file_man.file[index].file_point,"gsof") != NULL )
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
	else
	{
		char buffer_other[1024];

		sprintf(buffer_other,"D200_%s.CFS",file_man.file[index].file_point);

		/* open */
		fopen_s(&pf_cfs_file,buffer_other,"rb");
		/* open ok ? */
		if( pf_cfs_file == NULL )
		{
			char buffer_tmp[1024];

			sprintf(buffer_tmp,"找不到.%s文件对应的配置文件%s",file_man.file[index].file_point,buffer_other);

			CString d = A2T(buffer_tmp);

			MessageBox(d,_T("tips"),0);

			return;
		}
	}
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
			return;
		}
		/*---------------system supply-----------------*/
		if( READ_CFS.cfs_global_msg.procotol_select == 2 )
		{
			//RT27
			MessageBox(_T("暂不支持RT27协议"),_T("tips"),0);
			return;
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
		return;
	}
	/*---------------param_list------------*/
	for( unsigned int i = 0 ; i < READ_CFS.cfs_global_msg.sample_num ; i ++ )
	{
		sprintf(param_list_show.param_list[param_list_show.param_list_num].name,"%s:%s->%s",
		file_man.file[index].file_point,file_name_buffer_tmp,READ_CFS.pmd[i].name);
		/* add to combox */
		show = A2T(param_list_show.param_list[param_list_show.param_list_num].name);
		/*--------------*/
		m_combox_param_show.AddString(show);
		/* allocate the line */
		param_list_show.param_list[param_list_show.param_list_num].point_num = file_size / READ_CFS.cfs_global_msg.block_size;
		/* allocate memory */
		/*----malloc the data*/
		param_list_show.param_list[param_list_show.param_list_num].data = 
		(unsigned char *)malloc(param_list_show.param_list[param_list_show.param_list_num].point_num*8);//double
		/*---------------------*/
		if( param_list_show.param_list[param_list_show.param_list_num].data == NULL )
		{
			MessageBox(_T("内存分配失败"),_T("tips"),0);
			return;
		}		
	    /*------------------------------*/
		param_list_show.param_list_num++;	
		/* over */
		if( param_list_show.param_list_num >= 512 )
		{
			MessageBox(_T("文件中包含的曲线数量大于512"),_T("tips"),0);
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
		line_data = (double *)param_list_show.param_list[ i + start ].data;
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
	}else // NONE
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
	if( param_list_show.param_list[num].status != 0 )
	{
		CSeries line_cfs = (CSeries)m_chart.Series(param_list_show.param_list[num].line_num);
		line_cfs.Clear();
		line_cfs.put_ShowInLegend(0);
		avs[param_list_show.param_list[num].line_num] = 0;
		param_list_show.param_list[num].status = 0;
		/*-----------------------*/
		if( auto_scale_g.mutiple_axis )
		{
			/*---------------------------*/
			CAxes chartaxis =(CAxes)m_chart.get_Axis();
			CAxis axis;
			/*---------------------------*/
			if( param_list_show.param_list[num].line_num >= 1 )
			{
				 /*---------------------------------------------------*/
				 axis = (CAxis)chartaxis.get_Custom( param_list_show.param_list[num].line_num - 1 );
				 /*---------------------------------------------------*/
				 axis.put_Visible(0);
			}else
			{
				axis_reset();
			}
		}
		/*--------------------------*/
		auto_scale_g.line_cfg[param_list_show.param_list[num].line_num].opened = 0;
		/*--------------------------*/
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
	for( unsigned int i = 0 ; i < 50 ; i ++ )
	{
		CSeries line_cfs = (CSeries)m_chart.Series(i);
		/* setting */
		line_cfs.Clear();
		line_cfs.put_ShowInLegend(0);
		avs[i] = 0;
		if( i < 20 )
		{
			auto_scale_g.line_cfg[i].opened = 0;
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
	/*--------------*/
    Legend_handle(0);//hide
	/* axis hide */
    if( auto_scale_g.mutiple_axis )
	{
		CAxes chartaxis =(CAxes)m_chart.get_Axis();
		/* for loop */
		for( unsigned int i = 0 ; i < 20 ; i ++ )
		{
			/*---------------------------------------------------*/
			CAxis axis = (CAxis)chartaxis.get_Custom( i );
			/*---------------------------------------------------*/
			axis.put_Visible(0);
		}
		/*---------------------------------------------------*/
		axis_reset();
		/*---------------------------------------------------*/
	}
	/*-----------*/
	if( mode == 1 )
	{
		axis_reset();
	}
	/*------------------*/
	color_index = 0;
	/*------------------*/
}
/*--------------------------*/
void CTeeChart5_testDlg::draw_axis(unsigned int num,void * line,CString * title , unsigned int color,unsigned int mode)
{
	/* mutiple axis supple flags */
	if( auto_scale_g.mutiple_axis == 0 && mode == 0 )
	{
		CSeries * line_now = (CSeries *)line;
		line_now->put_VerticalAxis(0);
		return;
	}
	/*-----------------------------*/
	CAxes chartaxis =(CAxes)m_chart.get_Axis();
	CAxis Custom_axis;
	CAxisTitle Custom_title;
	CTeeFont font;
	CPen0 pen;
	CAxisLabels label;
	/* define lines */
	CSeries * line_now = (CSeries *)line;
	/* show axis */
	if( num >= 1 )
	{
		/* get axis */
		Custom_axis = (CAxis)chartaxis.get_Custom(num-1);
	}else
	{
		Custom_axis = (CAxis)chartaxis.get_Left();
	}
	/* set position */
	Custom_axis.put_PositionPercent(num*3);
	/* get title */
	Custom_title = Custom_axis.get_Title();
	/* put vision */
	Custom_axis.put_Visible(1);
	/*-------------------*/
	Custom_title.put_Caption(*title);
	Custom_title.put_Angle(90);
	Custom_title.put_Visible(1);
	/* font set */
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
	line_now->put_VerticalAxisCustom(num-1);
	/*-------------------------------*/
}
/*--------------------------*/
void CTeeChart5_testDlg::draw_single(unsigned int num,unsigned int mode)
{
    /* number */
    unsigned int line_num;
	/* safearray */
    COleSafeArray XValue,YValue;
    SAFEARRAYBOUND rgsabound;
	/* temple */
	double dval;
	/* clear all line */
	if( mode == 0 )
	{
	   clear_all_line(0);
	}else
	{
		release_current_line(num);
	}
	/* get line */
	CSeries * line_p = (CSeries *)Get_line_source(&line_num);
    /* max line number */
	if( line_p == NULL )
	{
		MessageBox(_T("最多支持20条曲线"),_T("tips"),0);
		return;
	}
	/*----------*/
	CSeries line_cfs = (CSeries)m_chart.Series(line_num);
	/* has drawed . then clear */
	if( param_list_show.param_list[num].status != 0 )
	{
		line_cfs.Clear();//clear 
	}
	/* get axis */
	/*--------------------------*/
	double *line_data = (double *)param_list_show.param_list[num].data;
	/*------init------*/
	rgsabound.cElements = param_list_show.param_list[num].point_num - param_list_show.param_list[num].offset;
    rgsabound.lLbound=0;
    XValue.Create(VT_R8,1,&rgsabound);
    YValue.Create(VT_R8,1,&rgsabound);
	/* draw all */
	for( long i = param_list_show.param_list[num].offset ; i < param_list_show.param_list[num].point_num ; i++ )
	{
		if( param_list_show.param_list[num].time_ms == 0 )
		{
           dval = i;
		}else
		{
		   dval = i * param_list_show.param_list[num].time_ms;
		}
		/* init */
        XValue.PutElement(&i, &dval);

        dval = line_data[i];

        YValue.PutElement(&i, &dval);
	}
	/* show the line */
	line_cfs.AddArray(param_list_show.param_list[num].point_num,YValue,XValue);
	/*------------*/
	//srand(time(NULL));
	///*-----------*/
	//unsigned int colorR = (unsigned char)rand();
	//unsigned int colorG = (unsigned char)rand();
	//unsigned int colorB = (unsigned char)rand();
	/*-----------*/
	unsigned long cols = get_color(0);//(colorB<<16)|(colorG<<8)|(colorR);
	/*---------------*/
	param_list_show.param_list[num].color = cols;
	param_list_show.param_list[num].status = 0xff;//has not drawed
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
	draw_axis(line_num,&line_cfs,&show,cols,0);
	/*---------------------*/
	auto_scale_g.line_cfg[line_num].opened = 1;
	int len_name;
	if( strlen(param_list_show.param_list[num].name) > 64 )
	{
		len_name = 64;
	}else
	{
		len_name = strlen(param_list_show.param_list[num].name);
	}
	/*------------------------------------------------------------------------------------*/
	memcpy(auto_scale_g.line_cfg[line_num].title,param_list_show.param_list[num].name,len_name);
	/*---------------------*/
}
/*---------------------------------------------*/
void CTeeChart5_testDlg::OnCbnSelchangeCombo1()
{
	/* get cline_num;*/
	int seq = m_combox_param_show.GetCurSel();
	/*---------------------------------*/
	int hold = m_check_hold.GetCheck()?1:0;
	/* has not show */
	if( param_list_show.param_list_num )
	{
	  /*---------------------------------*/
	  draw_single(seq,hold);
	}else if( COM.radio_status == 1 )
	{
		//============================
		Get_line_group_index(seq);
	}
}


void CTeeChart5_testDlg::OnBnClickedButton5()
{
	memset(&param_list_show,0,sizeof(param_list_show));
	memset(&file_man,0,sizeof(file_man));
	m_combox_param_show.ResetContent();
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
		//	sprintf(buffer,"COM%d",com_num);
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
	if( sscanf(com_name_s,"COM%d",&COM.com_detail[COM.Available_com_num].com_det) != 1 )
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
		sprintf(buffer,"停止(%d）",pic_take_now);
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
	//sprintf(buffer,"停止(%d）",pic_take_now);
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
	sprintf(buffer,"%d",pic_feedback_ok);
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
      sprintf(result_buffer,"起飞时间不够");
	}else
	{
		/*---------------*/
		if( (float)ctrlsum < 0.5f )
		{
			sprintf(result_buffer,"优秀");
		}else if( (float)ctrlsum < 1.0f )
		{
			sprintf(result_buffer,"及格");
		}else
		{
			sprintf(result_buffer,"已变形");
		}
	}
	/* show the msg */
	sprintf(show_buffer,"%f , 自动判断：%s", (float)ctrlsum,result_buffer);
    CString show;
    USES_CONVERSION;
	/*--------------*/
	show = A2T(show_buffer);
	/* show */
	//MessageBox(show,_T("tips"),0);
	m_taps.SetWindowTextW(show);
}

void CTeeChart5_testDlg::On32786()
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
			  case VK_F3:
				  On32777();
				  break;
			  case VK_F4:
				  On32781();
				  break;
			  case VK_F5:
				  reflush_chart();
				  break;
			  case VK_F6:
				  Position_axis_bin(m_check_hold.GetCheck()?1:0,1);
				  break;
			  case VK_F7:
				  Position_axis_bin(m_check_hold.GetCheck()?1:0,0);
				  break;
			  case VK_F8:
				  Position_point_lane(m_check_hold.GetCheck()?1:0);
				  break;
			  case VK_F9:
				  standart_diviaton(m_check_hold.GetCheck()?1:0);
			  default:
				  break;
		  }
		  /*------------------*/
          return TRUE;
      }
   }
 return CDialog::PreTranslateMessage(pMsg);
}
/* show lat lon */
void CTeeChart5_testDlg::Position_axis_bin(unsigned int mode,unsigned int p_or_l)//mode == 0 is single . mode mode != 0 is hold mode ; point(1) or line(0) 
{
	int lat_pos = 0xffff;
	int lon_pos = 0xffff;
	/* find the lat and lon in param list */
	if( p_or_l == 0 )
	{
		for( int i = last_postion ; i < param_list_show.param_list_num ; i ++ )
		{
			if( lat_pos == 0xffff && strstr(param_list_show.param_list[i].name,"GPS_LAT") != NULL )
			{
				lat_pos = i;
			}

			if( lon_pos == 0xffff && strstr(param_list_show.param_list[i].name,"GPS_LON") != NULL )
			{
				lon_pos = i;
			}
			/*--*/
			if( lat_pos != 0xffff && lon_pos != 0xffff )
			{
				last_postion = i+1;
				break;
			}
		}
		/*-------------------------------*/
		if( lat_pos == 0xffff || lon_pos == 0xffff )
		{
			AfxMessageBox(_T("数据表中不存在GPS_LAT或GPS_LON或已经绘制"));
			return;
		}
	}else if( p_or_l == 1 )
	{
		for( int i = lat_pos_point ; i < param_list_show.param_list_num ; i ++ )
		{
			if( lat_pos == 0xffff && strstr(param_list_show.param_list[i].name,"GPS_LAT") != NULL )
			{
				lat_pos = i;
			}

			if( lon_pos == 0xffff && strstr(param_list_show.param_list[i].name,"GPS_LON") != NULL )
			{
				lon_pos = i;
			}
			/*--*/
			if( lat_pos != 0xffff && lon_pos != 0xffff )
			{
				lat_pos_point = i+1;
				break;
			}
		}
		/*-------------------------------*/
		if( lat_pos == 0xffff || lon_pos == 0xffff )
		{
			AfxMessageBox(_T("数据表中不存在GPS_LAT或GPS_LON或已经绘制"));
			return;
		}
	}else
	{
		AfxMessageBox(_T("不支持的操作"));
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	unsigned int num;
	/* point or line */
	if( p_or_l == 0 )//line
	{
		/* none data avild */
		if( Get_2axis_source(&num) == NULL )
		{
			AfxMessageBox(_T("曲线数量超过10条！"));
			return;
		}
	}else if( p_or_l == 1 )
	{
		/* none data avild */
		if( Get_point_source(&num) == NULL )
		{
			AfxMessageBox(_T("曲线数量超过10条！"));
			return;
		}
	}
    /* show */
	double * lat_line = (double *)param_list_show.param_list[lat_pos].data;
	double * lon_line = (double *)param_list_show.param_list[lon_pos].data;
	/* check 0 */
	int j;
	/*---------------------------------------*/
	for( j = 0 ; j < param_list_show.param_list[lat_pos].point_num ; j ++ )
	{
		if( lat_line[j] != 0 && lon_line[j] != 0 )
		{
			break;
		}
	}
	/*---------------------------------*/
	if( j == param_list_show.param_list[lat_pos].point_num )
	{
		AfxMessageBox(_T("无可用数据！"));
		return;
	}
	/*---------------------------------------*/
	if( mode == 0 )
	{
       clear_all_line(0);
	}
	/*---------------------------------------*/
	CSeries line = (CSeries)m_chart.Series(num);
    /*---------------------------------*/
	for( int i = j ; i < param_list_show.param_list[lat_pos].point_num ; i ++ )
	{
		line.AddXY(lon_line[i],lat_line[i],NULL,NULL);
	}
	/*-------------------------*/
    /* show legend */
	Legend_handle(1);
	/* transfer */
	USES_CONVERSION;
	/*----------------------*/
	char buffer[200];
	memset(buffer,0,sizeof(buffer));
	/*----------------------------*/
	if( p_or_l == 0 )//line
	{
		sprintf(buffer,"%s_axis",param_list_show.param_list[lat_pos].name);
	}else if(  p_or_l == 1 )//line
	{
		sprintf(buffer,"%s_point",param_list_show.param_list[lat_pos].name);
	}
	/*----------------------------*/
	CString show = A2T(buffer);
	/*-----------------------------*/
	line.put_Title(show);
	/* show legend */
	line.put_ShowInLegend(1);
	/* put color */
	//srand(time(NULL));
	///*-----------*/
	//unsigned int colorR = (unsigned char)rand();
	//unsigned int colorG = (unsigned char)rand();
	//unsigned int colorB = (unsigned char)rand();
	/*-----------*/
	unsigned long cols = get_color(0);//(colorB<<16)|(colorG<<8)|(colorR);
	/*-----------*/
	line.put_Color(cols);
}
# if 1
/* gsof */
void CTeeChart5_testDlg::Position_point_lane(unsigned int mode)//mode == 0 is single . mode mode != 0 is hold mode 
{
	int lat_pos = 0xffff;
	int lon_pos = 0xffff;
	int lane_pic_pos = 0xffff;
	/* find the lat and lon in param list */
	for( int i = lane_last_postion ; i < param_list_show.param_list_num ; i ++ )
	{
		if( lat_pos == 0xffff && strstr(param_list_show.param_list[i].name,"GPS_LAT_LANE") != NULL )
		{
			lat_pos = i;
		}

		if( lon_pos == 0xffff && strstr(param_list_show.param_list[i].name,"GPS_LON_LANE") != NULL )
		{
			lon_pos = i;
		}
		/*----------------------------*/
		if( lane_pic_pos == 0xffff && strstr(param_list_show.param_list[i].name,"LANE_PIC_FC") != NULL )
		{
			lane_pic_pos = i;
		}
		/*--*/
		if( lat_pos != 0xffff && lon_pos != 0xffff && lane_pic_pos != 0xffff )
		{
		    lane_last_postion = i+1;
			break;
		}
	}
	/*-------------------------------*/
	if( lat_pos == 0xffff || lon_pos == 0xffff || lane_pic_pos == 0xffff )
	{
		AfxMessageBox(_T("数据表中不存在GPS_LAT_LANE或GPS_LON_LANE或已经绘制"));
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	unsigned int num;
	/* none data avild */
	if( Get_point_source(&num) == NULL )
	{
		AfxMessageBox(_T("曲线数量超过10条！"));
		return;
	}
    /* show */
	double * lat_line = (double *)param_list_show.param_list[lat_pos].data;
	double * lon_line = (double *)param_list_show.param_list[lon_pos].data;
	double * lane_pic_fc = (double *)param_list_show.param_list[lane_pic_pos].data;
	/*---------------------------------------*/
	if( mode == 0 )
	{
       clear_all_line(0);
	}
	/*---------------------------------------*/
	CSeries line = (CSeries)m_chart.Series(num);
    /*---------------------------------*/
	double last_lane = 0;
	/*---------------------------------------------------------------------------*/
	for( int i = 0 ; i < param_list_show.param_list[lane_pic_pos].point_num ; i ++ )
	{
		if( last_lane != lane_pic_fc[i] )
		{
			last_lane = lane_pic_fc[i];

			line.AddXY(lon_line[i],lat_line[i],NULL,NULL);
		}
	}
	/*-------------------------*/
    /* show legend */
	Legend_handle(1);
	/* transfer */
	USES_CONVERSION;
	/*----------------------*/
	char buffer[200];
	memset(buffer,0,sizeof(buffer));
	/*----------------------------*/
	sprintf(buffer,"%s_pic_axis",param_list_show.param_list[lat_pos].name);
	/*----------------------------*/
	CString show = A2T(buffer);
	/*-----------------------------*/
	line.put_Title(show);
	/* show legend */
	line.put_ShowInLegend(1);
	/* put color */
	//srand(time(NULL));
	///*-----------*/
	//unsigned int colorR = (unsigned char)rand();
	//unsigned int colorG = (unsigned char)rand();
	//unsigned int colorB = (unsigned char)rand();
	/*-----------*/
	unsigned long cols = get_color(0);//(colorB<<16)|(colorG<<8)|(colorR);
	/*-----------*/
	line.put_Color(cols);
}
#endif
void CTeeChart5_testDlg::standart_diviaton(unsigned int mode)//mode == 0 is single . mode mode != 0 is hold mode 
{
	int sd_pos = 0xffff;
	/* find the lat and lon in param list */
	for( int i = last_sd ; i < param_list_show.param_list_num ; i ++ )
	{
		if( sd_pos == 0xffff && strstr(param_list_show.param_list[i].name,"DV_SD") != NULL )
		{
			sd_pos = i;
		}
		/*--*/
		if( sd_pos != 0xffff )
		{
		    last_sd = i+1;
			break;
		}
	}
	/*-------------------------------*/
	if( sd_pos == 0xffff )
	{
		AfxMessageBox(_T("数据表中不存在DV_SD或DV_SD或已经绘制"));
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	unsigned int num;
	/* none data avild */
	if( Get_star_source(&num) == NULL )
	{
		AfxMessageBox(_T("曲线数量超过10条！"));
		return;
	}
    /* show */
	double * sd_line = (double *)param_list_show.param_list[sd_pos].data;
	/*---------------------------------------*/
	if( mode == 0 )
	{
       clear_all_line(0);
	}
	/*---------------------------------------*/
	CSeries line = (CSeries)m_chart.Series(num);
    /*---------------------------------*/
	double sd_last = 0;
	/*---------------------------------------------------------------------------*/
	for( int i = 0 ; i < param_list_show.param_list[sd_pos].point_num - 1; i ++ )
	{
		sd_last = sd_line[i+1] - sd_line[i];

		line.AddXY(i,sd_last,NULL,NULL);
	}
	/*-------------------------*/
    /* show legend */
	Legend_handle(1);
	/* transfer */
	USES_CONVERSION;
	/*----------------------*/
	char buffer[200];
	memset(buffer,0,sizeof(buffer));
	/*----------------------------*/
	sprintf(buffer,"%s_dv",param_list_show.param_list[sd_pos].name);
	/*----------------------------*/
	CString show = A2T(buffer);
	/*-----------------------------*/
	line.put_Title(show);
	/* show legend */
	line.put_ShowInLegend(1);
	/* put color */
	/*-----------*/
	//srand(time(NULL));
	///*--------------------------------------*/
	//unsigned int colorR = (unsigned char)rand();
	//unsigned int colorG = (unsigned char)rand();
	//unsigned int colorB = (unsigned char)rand();
	/*-----------*/
	unsigned long cols = get_color(0);//(colorB<<16)|(colorG<<8)|(colorR);
	/*-----------*/
	line.put_Color(cols);
}
/*----------------------------------------------*/
void CTeeChart5_testDlg::reflush_chart(void)
{
	int hold = m_check_hold.GetCheck()?1:0;
	/* clear without clear line msg */
	clear_all_line(1);
	/*-----------------------------*/
	last_postion = 0;
	lane_last_postion = 0;
	last_sd = 0;
	lat_pos_point = 0;
	/*-----------------------------*/
	param_list_show.param_list_num = 0;
	/*-----------------------------*/
	for( int j = 0 ; j < file_man.num ; j ++ )
	{
		if( file_man.file[j].file_enable == 1 )
		{
			/* do decode */
	        Read_Procotol_decode_waves(j);
			/* return ok */
		}
	}
	/* redraw */
	for( int i = 0 ; i < param_list_show.param_list_num ; i ++ )
	{
		if( param_list_show.param_list[i].status == 0xff )
		{
			
			/* has not show */
			if( param_list_show.param_list_num )
			{
			  /*---------------------------------*/
			  draw_single(i,hold);
			}else if( COM.radio_status == 1 )
			{
				//============================
				Get_line_group_index(i);
			}
		}
	}
}
/* get color */
unsigned int CTeeChart5_testDlg::get_color(unsigned int mode)
{
	/*------------------------------*/
	unsigned int color_tabal[7] = {0x1BC700,0xFEB610,0xFF0000,0x0,0xBA398E,0x1044F3,0x2EA8ED};
	/*------------*/
	unsigned int color_tmp;
	//	srand(time(NULL));
	///*-----------*/
	//unsigned int colorR = (unsigned char)rand();
	//unsigned int colorG = (unsigned char)rand();
	//unsigned int colorB = (unsigned char)rand();
	/*--------------*/
	if( color_index< 7 )
	{
		srand(time(NULL));
		/*-----------*/
		unsigned int fucse = rand() % 10 ;
		/*-----------------------------*/
		color_tmp = color_tabal[color_index];
	}else
	{
		srand(time(NULL));
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