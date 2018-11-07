// del_point.cpp : 实现文件
//

#include "stdafx.h"
#include "del_point.h"
#include "afxdialogex.h"
#include "TeeChart5_testDlg.h"

extern SYSTEM_AUTO_SCALE_DEF auto_scale_g;

SYSTEM_DELETE_POINT_DEF system_delete_point;

// del_point 对话框

IMPLEMENT_DYNAMIC(del_point, CDialogEx)

del_point::del_point(CWnd* pParent /*=NULL*/)
	: CDialogEx(del_point::IDD, pParent)
{

}

del_point::~del_point()
{
}

void del_point::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_del_point_list);
	DDX_Control(pDX, IDC_EDIT1, m_point_start);
	DDX_Control(pDX, IDC_EDIT55, m_point_stop);

	line_list_init();

}


BEGIN_MESSAGE_MAP(del_point, CDialogEx)
	ON_BN_CLICKED(IDOK, &del_point::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &del_point::OnBnClickedButton1)
END_MESSAGE_MAP()


// del_point 消息处理程序


void del_point::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
/*-------------------------------*/
void del_point::line_list_init(void)
{
    /*-----------------------*/
	USES_CONVERSION;

	CString show;
    /*-----------------------*/
	m_del_point_list.ResetContent();
	/*-----------------------*/
	for( int i = 0 ; i < 20 ; i ++ )
	{
		if( auto_scale_g.line_cfg[i].opened )
		{

			/*----------------------*/
			show = A2T(auto_scale_g.line_cfg[i].title);
			/*----------------------*/
			m_del_point_list.AddString(show);
		}
	}
	/*----------------------------*/
	m_del_point_list.SetCurSel(0);
	/*----------------------------*/
	char buffer[32];
	sprintf_s(buffer,"%d",system_delete_point.from);
	show = A2T(buffer);
	m_point_start.SetWindowTextW(show);
	/*-----------------------------*/
	sprintf_s(buffer,"%d",system_delete_point.to);
	show = A2T(buffer);
	m_point_stop.SetWindowTextW(show);
	/*----------------------------*/
}
/*----------------------------*/
int del_point::get_dex_edit( int id,unsigned int * data , unsigned int len )
{
	wchar_t get_cs[512];
	char c_string[256];
    int len_t;

	len_t = GetDlgItem(id)->GetWindowTextW(get_cs,512);

	memset(c_string,0,sizeof(c_string));

	for( int i = 0 ; i < len_t ; i ++  )
	{
		c_string[i] = (char)get_cs[i];
	}
	/*----------------------*/
	int ret = sscanf_s(c_string,"%d",data);

	if( ret == 1 )
	{
		return 0;
	}else
	{
		return (-1);
	}
}
/*----------------------------*/
void del_point::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if( get_dex_edit(IDC_EDIT1,&system_delete_point.from,sizeof(system_delete_point.from)) != 0 )
	{
		AfxMessageBox(_T("起始点错误"));
		return;
	}
	/*----------------------------------*/
	if( get_dex_edit(IDC_EDIT55,&system_delete_point.to,sizeof(system_delete_point.to)) != 0 )
	{
		AfxMessageBox(_T("终止点错误"));
		return;
	}
	/*---------------------------------*/
	if( system_delete_point.from > system_delete_point.to )
	{
		AfxMessageBox(_T("起始点 > 终止点"));
		return;
	}
	/*--------------------------------*/
	system_delete_point.line_num = m_del_point_list.GetCurSel();
	system_delete_point.enable = 1;
	/*--------------------------------*/
	OnOK();
	/*--------------------------------*/
}
