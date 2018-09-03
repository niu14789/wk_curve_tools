// m_com_num.cpp : 实现文件
//

#include "stdafx.h"
#include "m_com_num.h"
#include "afxdialogex.h"
#include "TeeChart5_testDlg.h"
/* define some msg */
COM_MANAGE_DEF COM;
// m_com_num 对话框
// motor 对话框
extern CWnd* m_pMainWnd_PUBLIC;
CTeeChart5_testDlg * ctm;
/* -------------------------------- */
static unsigned char open_flag = 0;
static unsigned char auto_link = 0;
/*----------------------------------*/
IMPLEMENT_DYNAMIC(m_com_num, CDialogEx)

m_com_num::m_com_num(CWnd* pParent /*=NULL*/)
	: CDialogEx(m_com_num::IDD, pParent)
{

}

m_com_num::~m_com_num()
{
}

void m_com_num::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_baurate);
	DDX_Control(pDX, IDC_COMBO2, m_com);
	DDX_Control(pDX, IDC_BUTTON1, m_open_button);
	DDX_Control(pDX, IDC_CHECK1, m_auto_link);
	DDX_Control(pDX, IDC_CHECK2, m_single_borad);
	/* init */
	com_list_refresh();
	m_baurate.SetCurSel(0);
	/* get main frame handle */
	ctm = (CTeeChart5_testDlg *)m_pMainWnd_PUBLIC;
	/* get link status */
	auto_link = ctm->system_config_msg(2);
	/* get checked */
	if( auto_link )
	{
		m_auto_link.SetCheck(1);
	}else
	{
	    m_auto_link.SetCheck(0);
	}
    /*------------------------*/
	if( open_flag )
	{
		m_open_button.EnableWindow(0);
		m_open_button.SetWindowTextW(_T("已打开"));
	}
}


BEGIN_MESSAGE_MAP(m_com_num, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &m_com_num::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON25, &m_com_num::OnBnClickedButton25)
	ON_BN_CLICKED(IDC_BUTTON14, &m_com_num::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_CHECK1, &m_com_num::OnBnClickedCheck1)
END_MESSAGE_MAP()


// m_com_num 消息处理程序

void m_com_num::com_list_refresh(void)
{
	CString show;
	char buffer[100];
	/* fixed */
	USES_CONVERSION;
	/* reset */
	m_com.ResetContent();
	/* set */
	for( unsigned int i = 0 ; i < COM.Available_com_num ; i ++ )
	{
		sprintf_s(buffer,"%s(COM%d)",COM.com_detail[i].name,COM.com_detail[i].com_det);
		/* show */
		show = A2T(buffer);
		/*------*/
		m_com.AddString(show);
	}
	/* set cursel */
	m_com.SetCurSel(COM.Available_com_num-1);
}

void m_com_num::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int seq = m_com.GetCurSel();
	/* open the gate */
	ctm->Get_COM_STATUS(COM.com_detail[seq].com_det);
	/*---------*/
	open_flag = 1;
	/*---------------*/
	unsigned char flag = m_auto_link.GetCheck();
	/*---------------*/
	if( flag )
	{
		ctm->system_config_fresh(2,1);
	}else
	{
		ctm->system_config_fresh(2,0);
	}
	/* destronys */
	OnOK();
}


void m_com_num::OnBnClickedButton25()
{
	// TODO: 在此添加控件通知处理程序代码
	com_list_refresh();
}


void m_com_num::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
}


void m_com_num::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
    unsigned char flag = m_auto_link.GetCheck();
	/*---------------*/
	if( flag )
	{
		ctm->system_config_fresh(2,1);
	}else
	{
		ctm->system_config_fresh(2,0);
	}
}
