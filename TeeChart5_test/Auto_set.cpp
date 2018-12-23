// Auto_set.cpp : 实现文件
//

#include "stdafx.h"
#include "Auto_set.h"
#include "afxdialogex.h"
#include "TeeChart5_testDlg.h"

// Auto_set 对话框

extern PARAM_LIST_DEF param_list_show;

static unsigned int seq_to_index[512];
static unsigned int seq_g = 0;

static unsigned int what_line = 0xffff;

IMPLEMENT_DYNAMIC(Auto_set, CDialogEx)

Auto_set::Auto_set(CWnd* pParent /*=NULL*/)
	: CDialogEx(Auto_set::IDD, pParent)
{

}

Auto_set::~Auto_set()
{
}

void Auto_set::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_auto_scale);
	DDX_Control(pDX, IDC_CHECK2, m_auto_all);
	DDX_Control(pDX, IDC_EDIT1, m_min_edit);
	DDX_Control(pDX, IDC_EDIT55, m_max_edit);
	DDX_Control(pDX, IDC_BUTTON1, m_button_auto_set);
	DDX_Control(pDX, IDC_CHECK4, m_checkbox_mutiple);
	DDX_Control(pDX, IDC_COMBO1, m_line_list);
	/*------------------------------------*/
	line_list_init();
   /*------------------------------------*/
	settings_auto_axis();
	/*------------------------------------*/
    settings_show(0);
}

BEGIN_MESSAGE_MAP(Auto_set, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &Auto_set::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, &Auto_set::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK2, &Auto_set::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK4, &Auto_set::OnBnClickedCheck4)
	ON_CBN_SELCHANGE(IDC_COMBO1, &Auto_set::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()
/*-------------------------*/
void Auto_set::line_list_init(void)
{
	m_line_list.ResetContent();
	/*-----------------------*/
	unsigned int count = param_list_show.param_list_num;
	/*-----------------------*/
	memset(seq_to_index,0,sizeof(seq_to_index));
	seq_g = 0;
	what_line = 0xffff;
	/*-----------------------*/
	for( unsigned int i = 0 ; i < count ; i ++ )
	{
		if( param_list_show.param_list[i].status == 0xff )
		{
			USES_CONVERSION;
			/*----------------------*/
			CString show = A2T(param_list_show.param_list[i].name);
			/*----------------------*/
			m_line_list.AddString(show);
			/*----------------------*/
			seq_to_index[seq_g++] = i;
		}
	}
	/*----------------------------*/
	m_line_list.SetCurSel(0);
	/*----------------------------*/
}
// Auto_set 消息处理程序
void Auto_set::settings_auto_axis(void)
{
	
}
void Auto_set::settings_show(unsigned int indexs)
{
	/*-------------*/
	if( seq_g == 0 )
	{
		return;
	}
	/*---------------*/
	what_line = indexs;
	/* set old */
	char buffer[100];
	unsigned int in = seq_to_index[indexs];
	sprintf_s(buffer,sizeof(buffer),"%lf",param_list_show.param_list[in].axis_min);
	USES_CONVERSION;
	/*----------------------*/
	CString show = A2T(buffer);
	m_min_edit.SetWindowText(show);
	/*------------------------------------*/
	sprintf_s(buffer,sizeof(buffer),"%lf",param_list_show.param_list[in].axis_max);
	show = A2T(buffer);
	m_max_edit.SetWindowText(show);
	/*------------------------------------*/
	if( param_list_show.param_list[in].axis_auto == 0 )
	{
		m_auto_scale.SetCheck(1);
		/* disable the scale */
		m_min_edit.EnableWindow(0);
		m_max_edit.EnableWindow(0);
		m_button_auto_set.EnableWindow(0);
		/*------------------------------------*/
	}else
	{
		m_auto_scale.SetCheck(0);
		/* disable the scale */
		m_min_edit.EnableWindow(1);
		m_max_edit.EnableWindow(1);
		m_button_auto_set.EnableWindow(1);
	}
}

int Auto_set::get_mm_edit( int id , void * data , unsigned int len )
{
	wchar_t get_cs[512];
	char c_string[256];
    int len_t;
	/*----------------------*/
	memset(data,0,len);
	memset(get_cs,0,sizeof(get_cs));
	memset(c_string,0,sizeof(c_string));
	/*----------------------*/
	len_t = GetDlgItem(id)->GetWindowTextW(get_cs,512);
	/*----------------------*/
	for( int i = 0 ; i < len_t ; i ++  )
	{
		c_string[i] = (char)get_cs[i];
	}
	/*----------------------*/
	memcpy(data,c_string,len_t);
	/*----------------------*/
	return len_t;
}
void Auto_set::OnBnClickedCheck1()
{
	if( what_line == 0xffff )
	{
		MessageBox(_T("未选择曲线"),_T("tips"),0);
		return;
	}
	/* set */
	unsigned int in = seq_to_index[what_line];
	// TODO: 在此添加控件通知处理程序代码
	int auto_set = m_auto_scale.GetCheck();
	/*----------------------------------*/
	if( auto_set )
	{
		/* disable the scale */
		m_min_edit.EnableWindow(0);
		m_max_edit.EnableWindow(0);
		m_button_auto_set.EnableWindow(0);
		/*------------------------------------*/
		param_list_show.param_list[in].axis_auto = 0;//auto
		/*------------------------------------*/
		param_list_show.param_list[in].axis_multiple = 0;
	}else
	{
		/* disable the scale */
		m_min_edit.EnableWindow(1);
		m_max_edit.EnableWindow(1);
		m_button_auto_set.EnableWindow(1);
	}
}
/*--------------------------------*/
void Auto_set::OnBnClickedButton1()
{
	if( what_line == 0xffff )
	{
		MessageBox(_T("未选择曲线"),_T("tips"),0);
		return;
	}
	//TODO: 在此添加控件通知处理程序代码
	char buffer[32];
	/*-------------------------*/
	get_mm_edit(IDC_EDIT1,buffer,sizeof(buffer));
	/*-----------------------------------------*/
	unsigned int in = seq_to_index[what_line];
	/* get min */
	if( sscanf_s(buffer,"%lf",&param_list_show.param_list[in].axis_min) != 1 )
	{
		AfxMessageBox(_T("最小值错误"));
		return;
	}
	/*-------------------------*/
	get_mm_edit(IDC_EDIT55,buffer,sizeof(buffer));
	/* get max */
	if( sscanf_s(buffer,"%lf",&param_list_show.param_list[in].axis_max) != 1 )
	{
		AfxMessageBox(_T("最大值错误"));
		return;
	}
	/*---------------*/
	if( param_list_show.param_list[in].axis_max <= param_list_show.param_list[in].axis_min )
	{
		AfxMessageBox(_T("最小值>=最大值"));
		return;
	}
	/*------------------------------------*/
	param_list_show.param_list[in].axis_auto = 1;//manul
	/*------------------------------------*/
	param_list_show.param_list[in].axis_multiple = 1;
	/*-----------------*/
	AfxMessageBox(_T("设置成功"));
}
/*------------------------------*/
void Auto_set::OnBnClickedCheck2()
{
	int check = m_auto_all.GetCheck() ? 0 : 1;
	/* check */
	unsigned int count = param_list_show.param_list_num;
	/* set all lines to auto */
	for( unsigned int i = 0 ; i < count ; i ++ )
	{
		param_list_show.param_list[i].axis_auto = check;//all auto
	}
}
/*-----------------------------*/
void Auto_set::OnBnClickedCheck4()
{
	int check = m_checkbox_mutiple.GetCheck() ? 1 : 0;
	/* check */
	/*------------------------------------------*/
	unsigned int count = param_list_show.param_list_num;
	/* set all lines to auto */
	for( unsigned int i = 0 ; i < count ; i ++ )
	{
		param_list_show.param_list[i].axis_multiple = check;//all auto
	}
}
/*---------------------------------*/
void Auto_set::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int seq = m_line_list.GetCurSel();
	/*------------------------------*/
	settings_show(seq);
	/*------------------------------*/
	what_line = seq;
	/*------------------------------*/
}
