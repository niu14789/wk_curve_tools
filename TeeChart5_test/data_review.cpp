// data_review.cpp : 实现文件
//

#include "stdafx.h"
#include "data_review.h"
#include "afxdialogex.h"
#include "TeeChart5_testDlg.h"
#include "stdio.h"

extern PARAM_LIST_DEF param_list_show;

// data_review 对话框

IMPLEMENT_DYNAMIC(data_review, CDialogEx)

data_review::data_review(CWnd* pParent /*=NULL*/)
	: CDialogEx(data_review::IDD, pParent)
{

}

data_review::~data_review()
{
}

void data_review::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_data_review_list);
	DDX_Control(pDX, IDC_COMBO1, m_line_com);

	Init();
}


BEGIN_MESSAGE_MAP(data_review, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &data_review::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// data_review 消息处理程序
void data_review::Init(void)
{
	/* list contrl */
	DWORD dwStyle = m_data_review_list.GetExtendedStyle();     
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	/*--------*/
	m_data_review_list.SetExtendedStyle(dwStyle);
	/* capute */
	m_data_review_list.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 60);
	m_data_review_list.InsertColumn(1, _T("数值x轴"), LVCFMT_CENTER, 210);
	m_data_review_list.InsertColumn(2, _T("数值y轴"), LVCFMT_CENTER, 210);
	m_data_review_list.InsertColumn(3, _T("均值"), LVCFMT_CENTER, 70);
	/*-------------------*/
	USES_CONVERSION;
	m_line_com.ResetContent();
	CString show;
	/* add data */
	for( unsigned int i = 0 ; i < param_list_show.param_list_num ; i ++ )
	{
		show = A2T(param_list_show.param_list[i].name);

		m_line_com.AddString(show);
	}
	/*-------------------*/
	m_line_com.SetCurSel(0);
}

void data_review::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int seq = m_line_com.GetCurSel();
	/* set name */
	if( param_list_show.param_list_num == 0 )
	{
		return;
	}
	/*------------------------*/
	m_data_review_list.DeleteAllItems();
	/*-----------------------*/
	USES_CONVERSION;
	CString d0,d1,d2,c_index;
	char buffer[64];
	double * x,*y;
	/*----------------------*/
	x = (double *)param_list_show.param_list[seq].data_x;
	y = (double *)param_list_show.param_list[seq].data_y;
	/*----------------------*/
	for( unsigned int i = 0 ; i < param_list_show.param_list[seq].point_num; i ++ )
	{
		memset(buffer,0,sizeof(buffer));
		sprintf_s(buffer,"%d",i);
		/*-----------------*/
		c_index = A2T(buffer);
		/*------------------*/
		memset(buffer,0,sizeof(buffer));
		sprintf_s(buffer,"%lf",x[i]);
		/*-----------------*/
		d0 = A2T(buffer);
		/*------------------*/
		memset(buffer,0,sizeof(buffer));
		sprintf_s(buffer,"%lf",y[i]);
		/*-----------------*/
		d1 = A2T(buffer);
		/*------------------*/
		memset(buffer,0,sizeof(buffer));
		sprintf_s(buffer,"%d",i);
		/*-----------------*/
		d2 = A2T(buffer);
		/*------------------*/
		m_data_review_list.InsertItem(i, c_index);
		m_data_review_list.SetItemText(i, 1, d0);
		m_data_review_list.SetItemText(i, 2, d1);
		m_data_review_list.SetItemText(i, 3, d2);
	}
}
