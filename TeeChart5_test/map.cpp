// map.cpp : 实现文件
//

#include "stdafx.h"
#include "map.h"
#include "afxdialogex.h"
#include "TeeChart5_testDlg.h"
#include "MsHTML.h"

extern PARAM_LIST_DEF param_list_show;
// map 对话框

unsigned int skip_num = 0;
double *lon,*lat,*height,*psi,*gama,*teta;
unsigned int review_point_num = 0; 
unsigned int curren_review_position = 0;
unsigned char U3D_enable = 0;
unsigned char pause_flag = 0;

unsigned char ok_flag = 0;

IMPLEMENT_DYNAMIC(map, CDialogEx)

map::map(CWnd* pParent /*=NULL*/)
	: CDialogEx(map::IDD, pParent)
{

}

map::~map()
{

}

void map::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_web);

	DDX_Control(pDX, IDC_COMBO3, m_unity_select);
	DDX_Control(pDX, IDC_COMBO2, m_rate);
	DDX_Control(pDX, IDC_COMBO1, m_list);

	DDX_Control(pDX, IDC_BUTTON2, m_start);
	DDX_Control(pDX, IDC_PROGRESS1, m_process);
	/*-------------*/
	init_map();
}


BEGIN_MESSAGE_MAP(map, CDialogEx)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO3, &map::OnCbnSelchangeCombo3)
	ON_BN_CLICKED(IDC_BUTTON4, &map::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &map::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON28, &map::OnBnClickedButton28)
	ON_CBN_SELCHANGE(IDC_COMBO2, &map::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// map 消息处理程序
void map::init_map(void)
{
	/* transform */
	USES_CONVERSION;
	/*---------------------------*/
	char * p;
	/* path */
	wchar_t pFileName[MAX_PATH];     
	/* get current dir */
	int nPos = GetCurrentDirectory( MAX_PATH, pFileName);
	/* transfer and create */
	p = T2A(pFileName);
	/* create */
	char buffer[128];
	/*---------------------*/
	sprintf_s(buffer,sizeof(buffer),"%s/map.html",p);
	/*---------------------*/
	CString path = A2T(buffer);
	/* loader */
	m_web.Navigate(path,NULL,NULL,NULL,NULL);
	/* init ohers */
	m_unity_select.ResetContent();
	/* add data */
	m_unity_select.AddString(_T("不导入3D场景"));
	m_unity_select.AddString(_T("导入3D场景01"));
	m_unity_select.AddString(_T("导入3D场景02"));
	m_unity_select.AddString(_T("导入3D场景03"));
	m_unity_select.AddString(_T("导入3D场景04"));
	m_unity_select.AddString(_T("导入3D场景05"));
	/*---------*/
	m_unity_select.SetCurSel(0);
	/* set rate */
	m_rate.ResetContent();
	/* add data */
	m_rate.AddString(_T("回放速率"));
	m_rate.AddString(_T("x2"));
	m_rate.AddString(_T("x4"));
	m_rate.AddString(_T("x6"));
	m_rate.AddString(_T("x8"));
	m_rate.AddString(_T("x10"));
	m_rate.AddString(_T("x12"));
	m_rate.AddString(_T("x14"));
	m_rate.AddString(_T("x16"));
	/*---------*/
	m_rate.SetCurSel(0);
	/*-------------------*/
	m_list.ResetContent();
	CString show;
	/* add data */
	for( unsigned int i = 0 ; i < param_list_show.param_list_num ; i ++ )
	{
		show = A2T(param_list_show.param_list[i].name);
		/* add data */
		m_list.AddString(show);
	}
	/* focus on */
	m_list.SetCurSel(0);
	/* open data */
	unsigned char f = 0;
	ok_flag = 0;
	/* read data */
	for(unsigned int i = 0 ; i < param_list_show.param_list_num ; i ++ )
	{
		if( strstr(param_list_show.param_list[i].name,"GPS航线轨迹") != NULL )
		{
			lon = (double *)param_list_show.param_list[i].data_x;
			lat = (double *)param_list_show.param_list[i].data_y;
			/* get num */
			review_point_num = param_list_show.param_list[i].point_num;
			/* get flag */
			f |= 0x2;
		}
		/* ok */
		char *c = strstr(param_list_show.param_list[i].name,"->PSI");
		/* tail */
		if( c != NULL && *(c+strlen("->PSI")) == 0 )
		{
			psi = (double *)param_list_show.param_list[i].data_y;
			/* get flag */
			f |= 0x1;
		}
		/* get ry */
		c = strstr(param_list_show.param_list[i].name,"->RY");
		/*--------*/
		if( c != NULL && *(c+strlen("->RY")) == 0)
		{
			height = (double *)param_list_show.param_list[i].data_y;
			/* get flag */
			f |= 0x4;
		}
		/* gamma */
		c = strstr(param_list_show.param_list[i].name,"->GAMMA");
		/*-------*/
		if( c != NULL && *(c+strlen("->GAMMA")) == 0)
		{
			gama = (double *)param_list_show.param_list[i].data_y;
			/* get flag */
			f |= 0x8;
		}
		/* teta */
		c = strstr(param_list_show.param_list[i].name,"->TETA");
		if( c != NULL && *(c+strlen("->TETA")) == 0)
		{
			teta = (double *)param_list_show.param_list[i].data_y;
			/* get flag */
			f |= 0x10;
		}
		/* return */
		if( f == 0x1F )
		{
			break;
		}
	}
	/* return */
	if( f != 0x1F )
	{
		if( f & 0x02 )
		{
			AfxMessageBox(_T("一些变量没有定义，飞放可能不正常！！！"));
		}
		else
		{
			AfxMessageBox(_T("未打开日志文件或“GPS航线轨迹”数据没有定义,无法回放"));
			return;
		}
	}
	/* ok flag */
	ok_flag = 1;
	curren_review_position = 0;
}

void map::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	int seq = m_unity_select.GetCurSel();
	/*----------------*/
	if( seq == 1 )
	{
		if( Auti_open() != (-1) )
		{
			U3D_enable = 1;
			spy_send_return_home();
		}
	}
}
/*--------------------------*/
void map::OnBnClickedButton4()
{
	if( !ok_flag )
	{
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	fucus_on_map((float)lon[0],(float)lat[0]);
	//lon[curren_review_position],(float)lat[curren_review_position],
}


void map::OnBnClickedButton2()
{
	if( !ok_flag )
	{
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	if( pause_flag == 0 )
	{
		int rate = m_rate.GetCurSel();
		/* skip num */
		skip_num = (rate+1)*20;
        /* to head */
		/*-----------*/
		fucus_on_map((float)lon[0],(float)lat[0]);
		/* set time */
		curren_review_position = 0;
		/*----------*/
		m_start.SetWindowTextW(_T("暂停"));
		/*----------*/
		pause_flag = 1;
		/*----------*/
		m_process.SetRange32(0,review_point_num);
		/*----------*/
		SetTimer(1,100,NULL);
	}
	else if( pause_flag == 1 )
	{
		m_start.SetWindowTextW(_T("继续"));
		pause_flag = 2;
		KillTimer(1);
	}
	else if( pause_flag == 2 )
	{
		m_start.SetWindowTextW(_T("暂停"));
		/*----------*/
		pause_flag = 1;
		/*----------*/
		SetTimer(1,10,NULL);
	}
}
/* */

#define PR (100000)
#define RR (1)

void map::aircraft_move_thread(void)
{
	/* unity move */
	if( U3D_enable == 1 )
	{
		spy_send_data_to_unity(0,0,0,
			(float)(lon[curren_review_position]-lon[0]) * PR,(float)height[curren_review_position] ,(float)(lat[curren_review_position]-lat[0]) * PR);
	}
	/* judging */
	if( psi != NULL )
	{
		/*------------*/
		move_aircraft((float)lon[curren_review_position],(float)lat[curren_review_position],(int)psi[curren_review_position],2);
		/*--------------------*/
	}
	else
	{
		/*------------*/
		move_aircraft((float)lon[curren_review_position],(float)lat[curren_review_position],0,2);
		/*--------------------*/
	}
	curren_review_position += skip_num;
	/*--------------------*/
	if( curren_review_position >= review_point_num )
	{
		curren_review_position = 0;
		/* kill */
		KillTimer(1);
		pause_flag = 0;
		m_start.SetWindowTextW(_T("开始回放"));
	}
	/*- set process */
	m_process.SetPos(curren_review_position);
	/*--------------*/
}
/*------------------------*/
void map::show_home()
{
   	CComQIPtr<IHTMLDocument2> spDoc = m_web.get_Document();
	CComDispatchDriver spScript;
	spDoc->get_Script(&spScript);
   //113.913834,22.55618
	CComVariant var[3],varRet;
	spScript.Invoke0(L"show_home",&varRet);	// TODO: 在此添加控件通知处理程序代码
}
void map::move_aircraft(float lon,float lat,int psi,int mode)
{
		// TODO: 在此添加控件通知处理程序代码
   	CComQIPtr<IHTMLDocument2> spDoc = m_web.get_Document();
	CComDispatchDriver spScript;
	spDoc->get_Script(&spScript);
//113.913834,22.55618
	CComVariant var[4] = {lon,lat,psi},varRet;
	spScript.Invoke2(L"show_aircraft_position",&var[0],&var[1],&varRet);
	var[3] = mode;
    spScript.Invoke2(L"show_aircraft_psi",&var[2],&var[3],&varRet);
}
void map::fucus_on_map(float lon,float lat)
{
	// TODO: 在此添加控件通知处理程序代码

	CComQIPtr<IHTMLDocument2> spDoc = m_web.get_Document();
	CComDispatchDriver spScript;
	spDoc->get_Script(&spScript);
//113.913834,22.55618
	CComVariant var1 = lon,var2 = lat,varRet;
	spScript.Invoke2(L"show_aircraft",&var1,&var2,&varRet);
}
/* time tick */
void map::OnTimer(UINT_PTR nIDEvent) 
{
  switch(nIDEvent)
  {
    case 1://radio manager
	   aircraft_move_thread();
	case 2:
		
	break;
    default:break;
  }
  /* overrade */
  CDialog::OnTimer(nIDEvent);
}

void map::OnBnClickedButton28()
{
	// TODO: 在此添加控件通知处理程序代码
//	spy_send_data_to_unity(0,0,0,0,curren_review_position++,0);
}


void map::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	int rate = m_rate.GetCurSel();
	/* skip num */
	skip_num = (rate+1)*20;
}
