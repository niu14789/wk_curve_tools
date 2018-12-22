// data_review.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "data_review.h"
#include "afxdialogex.h"
#include "TeeChart5_testDlg.h"
#include "stdio.h"

#define MULTIPLE_PER    (1000)

extern PARAM_LIST_DEF param_list_show;

// data_review �Ի���

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
	DDX_Control(pDX, IDC_EDIT5, m_skip_edit);
}


BEGIN_MESSAGE_MAP(data_review, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &data_review::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// data_review ��Ϣ�������
void data_review::Init(void)
{
	/* list contrl */
	DWORD dwStyle = m_data_review_list.GetExtendedStyle();     
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	/*--------*/
	m_data_review_list.SetExtendedStyle(dwStyle);
	/* capute */
	m_data_review_list.InsertColumn(0, _T("���"), LVCFMT_CENTER, 60);
	m_data_review_list.InsertColumn(1, _T("��ֵx��"), LVCFMT_CENTER, 210);
	m_data_review_list.InsertColumn(2, _T("��ֵy��"), LVCFMT_CENTER, 210);
	m_data_review_list.InsertColumn(3, _T("��ֵ"), LVCFMT_CENTER, 70);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int seq = m_line_com.GetCurSel();
	/* set name */
	if( param_list_show.param_list_num == 0 )
	{
		return;
	}
	/* default . show max */
	data_anypostion(seq,0,MULTIPLE_PER);
}
/* show anypostion data */
void data_review::data_anypostion(unsigned int index , unsigned int pos,unsigned int count)
{
   /*------------------------*/
	m_data_review_list.DeleteAllItems();
	/*-----------------------*/
	USES_CONVERSION;
	CString d0,d1,d2,c_index;
	char buffer[64];
	double * x,*y;
	/* filte the over ride */
	if( pos >= param_list_show.param_list[index].point_num )
	{
		if( param_list_show.param_list[index].point_num == 0 )
		{
			AfxMessageBox(_T("û������"));
		}else
		{
			AfxMessageBox(_T("�������������"));
		}
		/* return */
		return;
	}
	/* per page more than large */
	if( count > MULTIPLE_PER )
	{
		AfxMessageBox(_T("ÿ����������"));
		return;
	}
	/* calibrate the real leng of point num */
	unsigned real_len = 0;
	/* judging */
	if( ( pos + count ) > param_list_show.param_list[index].point_num )
	{
		real_len = param_list_show.param_list[index].point_num - pos;
	}
	else
	{
		real_len = count;
	}
	/*----------------------*/
	x = (double *)param_list_show.param_list[index].data_x;
	y = (double *)param_list_show.param_list[index].data_y;
	/* seek */
	x += pos;
	y += pos;
	/*----------------------*/
	for( unsigned int i = 0 ; i < real_len ; i ++ )
	{
		sprintf_s(buffer,"%d",i+pos);
		/*-----------------*/
		c_index = A2T(buffer);
		/*------------------*/
		sprintf_s(buffer,"%lf",x[i]);
		/*-----------------*/
		d0 = A2T(buffer);
		/*------------------*/
		sprintf_s(buffer,"%lf",y[i]);
		/*-----------------*/
		d1 = A2T(buffer);
		/*------------------*/
		sprintf_s(buffer,"%d",i+pos);
		/*-----------------*/
		d2 = A2T(buffer);
		/*------------------*/
		m_data_review_list.InsertItem(i, c_index);
		m_data_review_list.SetItemText(i, 1, d0);
		m_data_review_list.SetItemText(i, 2, d1);
		m_data_review_list.SetItemText(i, 3, d2);
	}
	/* show */
	page_msg(index,pos);
}
/* show page msg */
void data_review::page_msg(unsigned int index , unsigned int pos)
{
    char buffer[32];
	/*------------*/
	sprintf_s(buffer,sizeof(buffer),"%d/%d",pos,param_list_show.param_list[index].point_num);
	/* show */
	USES_CONVERSION;
	CString d0;
	/* send to data */
	d0 = A2T(buffer);
	/*--------------*/
	m_skip_edit.SetWindowTextW(d0);
}
/*-----------------------------------*/