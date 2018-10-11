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
	DDX_Control(pDX, IDC_CHECK2, m_smart_scale);
	DDX_Control(pDX, IDC_EDIT1, m_min_edit);
	DDX_Control(pDX, IDC_EDIT55, m_max_edit);
	DDX_Control(pDX, IDC_BUTTON1, m_button_auto_set);
	/*------------------------------------*/
	m_smart_scale.EnableWindow(0);
	/* set old */
	char buffer[100];
	sprintf(buffer,"%lf",auto_scale_g.min);
	USES_CONVERSION;
	/*----------------------*/
	CString show = A2T(buffer);
	m_min_edit.SetWindowText(show);
	/*------------------------------------*/
	sprintf(buffer,"%lf",auto_scale_g.min);
	show = A2T(buffer);
	m_max_edit.SetWindowText(show);
	/*------------------------------------*/
	if( auto_scale_g.auto_scale == 1 )
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
	/**/
}


BEGIN_MESSAGE_MAP(Auto_set, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &Auto_set::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, &Auto_set::OnBnClickedButton1)
END_MESSAGE_MAP()


// Auto_set 消息处理程序

int Auto_set::get_mm_edit( int id , void * data , unsigned int len )
{
	wchar_t get_cs[512];
	char c_string[256];
    int len_t;

	memset(get_cs,0,sizeof(get_cs));
	memset(c_string,0,sizeof(c_string));

	len_t = GetDlgItem(id)->GetWindowTextW(get_cs,512);

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
	auto_scale_g.auto_scale = auto_set;
	/*-------------------------------*/
	if( auto_set == 1 )
	{
		OnOK();
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
	/* get min */
	if( sscanf(buffer,"%lf",&auto_scale_g.min) != 1 )
	{
		AfxMessageBox(_T("最小值错误"));
		return;
	}
	/*-------------------------*/
	memset(buffer,0,sizeof(buffer));
	/*-------------------------*/
	get_mm_edit(IDC_EDIT55,buffer,sizeof(buffer));
	/* get max */
	if( sscanf(buffer,"%lf",&auto_scale_g.max) != 1 )
	{
		AfxMessageBox(_T("最大值错误"));
		return;
	}
	/*---------------*/
	if( auto_scale_g.max <= auto_scale_g.min )
	{
		AfxMessageBox(_T("最小值>=最大值"));
		return;
	}
	/*---------------*/
	auto_scale_g.enable = 1;
	/*---------------*/
	OnOK();
	/*---------------*/
}
