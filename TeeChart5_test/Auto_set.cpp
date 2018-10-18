// Auto_set.cpp : 实现文件
//

#include "stdafx.h"
#include "Auto_set.h"
#include "afxdialogex.h"
#include "TeeChart5_testDlg.h"

extern SYSTEM_AUTO_SCALE_DEF auto_scale_g;
// Auto_set 对话框

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
	for( int i = 0 ; i < 20 ; i ++ )
	{
		if( auto_scale_g.line_cfg[i].opened )
		{
			USES_CONVERSION;
			/*----------------------*/
			CString show = A2T(auto_scale_g.line_cfg[i].title);
			/*----------------------*/
			m_line_list.AddString(show);
		}
	}
	/*----------------------------*/
	m_line_list.SetCurSel(0);
	/*----------------------------*/
}
// Auto_set 消息处理程序
void Auto_set::settings_auto_axis(void)
{
	/*------------------------------------*/
	if( auto_scale_g.global_auto )
	{
		/* disable the scale */
		m_auto_scale.EnableWindow(0);
		/* disable the scale */
		m_min_edit.EnableWindow(0);
		m_max_edit.EnableWindow(0);
		m_button_auto_set.EnableWindow(0);
		m_line_list.EnableWindow(0);
		/*----------------------*/
		m_auto_all.SetCheck(1);
		/*----------------------*/
	}else
	{
		/* disable the scale */
		m_auto_scale.EnableWindow(1);
		/* disable the scale */
		m_min_edit.EnableWindow(1);
		m_max_edit.EnableWindow(1);
		m_button_auto_set.EnableWindow(1);
		m_line_list.EnableWindow(1);
		/*----------------------*/
		m_auto_all.SetCheck(0);
		/*----------------------*/
	}
	/* mutple */
	if( auto_scale_g.mutiple_axis )
	{
		m_checkbox_mutiple.SetCheck(1);
	}else
	{
		m_checkbox_mutiple.SetCheck(0);
	}
}
void Auto_set::settings_show(unsigned int index)
{
	/* set old */
	char buffer[100];
	sprintf(buffer,"%lf",auto_scale_g.line_cfg[index].min);
	USES_CONVERSION;
	/*----------------------*/
	CString show = A2T(buffer);
	m_min_edit.SetWindowText(show);
	/*------------------------------------*/
	sprintf(buffer,"%lf",auto_scale_g.line_cfg[index].max);
	show = A2T(buffer);
	m_max_edit.SetWindowText(show);
	/*------------------------------------*/
	if( auto_scale_g.line_cfg[index].auto_scale == 1 )
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
	}else
	{
		/* disable the scale */
		m_min_edit.EnableWindow(1);
		m_max_edit.EnableWindow(1);
		m_button_auto_set.EnableWindow(1);
		/*------------------------------------*/
	}
	/*-------------------------------*/
	auto_scale_g.line_cfg[m_line_list.GetCurSel()].auto_scale = auto_set;
	/*-------------------------------*/
	for( int i = 0 ; i < 20 ; i ++ )
	{
		if( auto_scale_g.line_cfg[i].auto_scale == 0 )
		{
			auto_scale_g.mutiple_axis = 1;
			m_checkbox_mutiple.SetCheck(1);
		}
	}
	/*-------------------------------*/
}


void Auto_set::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	char buffer[512];
	/*-------------------------*/
	memset(buffer,0,sizeof(buffer));
	/*-------------------------*/
	get_mm_edit(IDC_EDIT1,buffer,sizeof(buffer));
	/*-----------------------------------------*/
	int seq = m_line_list.GetCurSel();
	/* get min */
	if( sscanf(buffer,"%lf",&auto_scale_g.line_cfg[seq].min) != 1 )
	{
		AfxMessageBox(_T("最小值错误"));
		return;
	}
	/*-------------------------*/
	memset(buffer,0,sizeof(buffer));
	/*-------------------------*/
	get_mm_edit(IDC_EDIT55,buffer,sizeof(buffer));
	/* get max */
	if( sscanf(buffer,"%lf",&auto_scale_g.line_cfg[seq].max) != 1 )
	{
		AfxMessageBox(_T("最大值错误"));
		return;
	}
	/*---------------*/
	if( auto_scale_g.line_cfg[seq].max <= auto_scale_g.line_cfg[seq].min )
	{
		AfxMessageBox(_T("最小值>=最大值"));
		return;
	}
}

void Auto_set::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	auto_scale_g.global_auto = m_auto_all.GetCheck();
	/* fresh */
	settings_auto_axis();
	/*--------------*/
	settings_show(m_line_list.GetCurSel());
	/*--------------*/
	if( auto_scale_g.global_auto )
	{
		OnOK();
	}else
	{
		for( int i = 0 ; i < 20 ; i ++ )
		{
			if( auto_scale_g.line_cfg[i].auto_scale == 0 )
			{
				auto_scale_g.mutiple_axis = 1;
				m_checkbox_mutiple.SetCheck(1);
			}
		}
	}
}

void Auto_set::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	auto_scale_g.mutiple_axis = m_checkbox_mutiple.GetCheck();
	/*--------------------------------*/
	if( auto_scale_g.mutiple_axis == 0 )
	{
		m_auto_all.SetCheck(1);
		auto_scale_g.global_auto = 1;
		/*------------*/
		/* fresh */
		settings_auto_axis();
	}
	/*--------------------------------*/
}


void Auto_set::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int seq = m_line_list.GetCurSel();
	/*------------------------------*/
	settings_show(seq);
	/*------------------------------*/
}
