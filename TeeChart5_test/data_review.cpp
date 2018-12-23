// data_review.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "data_review.h"
#include "afxdialogex.h"
#include "TeeChart5_testDlg.h"
#include "stdio.h"

#define MULTIPLE_PER    (1000)

static unsigned int index_g = 0xFFFFFFFF;
static unsigned int method_g = 0;
static unsigned int current_pos = 0;

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
	DDX_Control(pDX, IDC_COMBO3, m_mothed);

	DDX_Control(pDX, IDC_EDIT3, m_search_edit0);
	
	Init();
}


BEGIN_MESSAGE_MAP(data_review, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &data_review::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO3, &data_review::OnCbnSelchangeCombo3)
	ON_BN_CLICKED(IDC_BUTTON34, &data_review::OnBnClickedButton34)
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
	m_data_review_list.InsertColumn(1, _T("��ֵX��"), LVCFMT_CENTER, 210);
	m_data_review_list.InsertColumn(2, _T("��ֵY��"), LVCFMT_CENTER, 210);
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
	/* set method */
	m_mothed.ResetContent();
	/* addstring */
	m_mothed.AddString(_T("<ѡ����������>"));//0
	m_mothed.AddString(_T("��ת�����"));//1
	m_mothed.AddString(_T("X�᷶Χ"));//2
	m_mothed.AddString(_T("Y�᷶Χ"));//3
	m_mothed.AddString(_T("X��仯��"));//4
	m_mothed.AddString(_T("X��仯��"));//5
	/* focus on one */
	m_mothed.SetCurSel(0);
	/* disable the edit windows */
	m_search_edit0.EnableWindow(0);
}
/* skip */
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
	/* get */
	current_pos = 0;
	index_g = seq;
	/* focus on one */
	m_mothed.SetCurSel(0);
	/* disable the edit windows */
	m_search_edit0.EnableWindow(0);
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
	m_search_edit0.SetWindowTextW(d0);
}
/*-----------------------------------*/
void data_review::OnCbnSelchangeCombo3()
{
	/* select nothing */
	if( index_g == 0xFFFFFFFF )
	{
		return;
	}
	/* do */
	int seq = m_mothed.GetCurSel();
	/*---------------------------*/
	if( seq == 0 )
	{
		m_search_edit0.EnableWindow(0);
		page_msg(index_g,current_pos);
	}
	else
	{
		m_search_edit0.EnableWindow(1);
		m_search_edit0.SetWindowTextW(_T(""));
	}
	/* get method */
	method_g = seq;
	/* some function */
}
/* set */
void data_review::OnBnClickedButton34()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( index_g == 0xFFFFFFFF )
	{
		AfxMessageBox(_T("δѡ������"));
		return;
	}
	/*--------------------*/
	if( method_g == 0 )
	{
		AfxMessageBox(_T("δѡ����������"));
		return;
	}
	/* get num and */
	double cur_num[5];
	int num;
	/*-------------------*/
	num = get_dex_edit(IDC_EDIT3,cur_num);
	/* ok or not */
    if( num == (-1) )
	{
		AfxMessageBox(_T("������ֵ��ʽ����ȷ"));
		return;
	}
	/*-------------------------*/
	switch(method_g)
	{
		case 1://jump to the speciply position
			jump_to_postion(cur_num,num,index_g);
			break;
		case 2:
			break;
		case 3:
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
			break;
		default :
			break;
	}
}
/* get dex num */
int data_review::get_dex_edit( int id,double * data)
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
	int ret = sscanf_s(c_string,"%lf-%lf-%lf-%lf-%lf",&data[0],&data[1],&data[2],&data[3],&data[4]);
	/* ok or not */
	if( ret != 0 )
	{
		return ret;
	}else
	{
		return (-1);
	}
}
/* jump to postion */
void data_review::jump_to_postion(double *num_darray,unsigned int num,unsigned int in)
{
	unsigned int count = MULTIPLE_PER;
	/*-----*/
	unsigned int array0 = (unsigned int)num_darray[0];
	unsigned int array1 = (unsigned int)num_darray[1];
	/* num */
	if(  num >= 2 )
	{
		if( array1 == 0 )
	    {
			AfxMessageBox(_T("����ڶ�������Ϊ0"));
			return;
		}
		/* get count */
		count = array1;
	}
	/*--------------*/
	if( array0 >= param_list_show.param_list[in].point_num )
	{
		AfxMessageBox(_T("�������������"));
		return;
	}
	/*-----------------*/
	data_anypostion(in,array0,count);
}