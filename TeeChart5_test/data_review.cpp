// data_review.cpp : 实现文件
//

#include "stdafx.h"
#include "data_review.h"
#include "afxdialogex.h"
#include "TeeChart5_testDlg.h"
#include "stdio.h"
#include "export_tool.h"

#define MULTIPLE_PER    (1000)

static unsigned int index_g = 0xFFFFFFFF;
static unsigned int method_g = 0;
static unsigned int current_pos = 0;

extern PARAM_LIST_DEF param_list_show;

static unsigned char single_time = 0;
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
	DDX_Control(pDX, IDC_COMBO3, m_mothed);

	DDX_Control(pDX, IDC_EDIT3, m_search_edit0);
	DDX_Control(pDX, IDC_BUTTON4, m_single);

	Init();
}


BEGIN_MESSAGE_MAP(data_review, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &data_review::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO3, &data_review::OnCbnSelchangeCombo3)
	ON_BN_CLICKED(IDC_BUTTON34, &data_review::OnBnClickedButton34)
	ON_BN_CLICKED(IDC_BUTTON2, &data_review::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &data_review::OnBnClickedButton4)
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
	m_data_review_list.InsertColumn(1, _T("数值X轴"), LVCFMT_CENTER, 210);
	m_data_review_list.InsertColumn(2, _T("数值Y轴"), LVCFMT_CENTER, 210);
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
	/* set method */
	m_mothed.ResetContent();
	/* addstring */
	m_mothed.AddString(_T("<选择搜索方法>"));//0
	m_mothed.AddString(_T("跳转到序号"));//1
	m_mothed.AddString(_T("X轴范围"));//2
	m_mothed.AddString(_T("Y轴范围"));//3
	m_mothed.AddString(_T("X轴变化率"));//4
	m_mothed.AddString(_T("Y轴变化率"));//5
	/* focus on one */
	m_mothed.SetCurSel(0);
	/* disable the edit windows */
	m_search_edit0.EnableWindow(0);
	/* single mode */
	single_time = 0;
}
/* skip */
void data_review::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
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
			AfxMessageBox(_T("没有数据"));
		}else
		{
			AfxMessageBox(_T("超过最大数据量"));
		}
		/* return */
		return;
	}
	/* per page more than large */
	if( count > MULTIPLE_PER )
	{
		AfxMessageBox(_T("每次数量超限"));
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
		sprintf_s(buffer,sizeof(buffer),"%d",i+pos);
		/*-----------------*/
		c_index = A2T(buffer);
		/*------------------*/
		sprintf_s(buffer,sizeof(buffer),"%.10lf",(double)x[i]);
		/*-----------------*/
		remove_zero(buffer,strlen(buffer));
		/*-----------------*/
		d0 = A2T(buffer);
		/*------------------*/
		sprintf_s(buffer,sizeof(buffer),"%.10lf",(double)y[i]);
		/*-----------------*/
		remove_zero(buffer,strlen(buffer));
		/*-----------------*/
		d1 = A2T(buffer);
		/*------------------*/
		sprintf_s(buffer,sizeof(buffer),"%d",i+pos);
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
	// TODO: 在此添加控件通知处理程序代码
	if( index_g == 0xFFFFFFFF )
	{
		AfxMessageBox(_T("未选择曲线"));
		return;
	}
	/*--------------------*/
	if( method_g == 0 )
	{
		AfxMessageBox(_T("未选择搜索方法"));
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
		AfxMessageBox(_T("请入数值格式不正确"));
		return;
	}
	/*-------------------------*/
	switch(method_g)
	{
		case 1://jump to the speciply position
			jump_to_postion(cur_num,num,index_g);
			break;
		case 2://x axis rang
			search_x_rang(cur_num,num,index_g);
			break;
		case 3:
			search_y_rang(cur_num,num,index_g);
			break;
		case 4:
			search_kx_rang(cur_num,num,index_g);
			break;
		case 5:
			search_ky_rang(cur_num,num,index_g);
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
			AfxMessageBox(_T("输入第二个参数为0"));
			return;
		}
		/* get count */
		count = array1;
	}
	/*--------------*/
	if( array0 >= param_list_show.param_list[in].point_num )
	{
		AfxMessageBox(_T("超过最大数据量"));
		return;
	}
	/*-----------------*/
	data_anypostion(in,array0,count);
}

void data_review::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	export_tool d;
	d.DoModal();
}
/* this current line */
static unsigned int line_zero = 0;
/* add one line */
void data_review::add_one_line(unsigned int in,unsigned int pos)
{
	if( line_zero >= MULTIPLE_PER )
	{
		//AfxMessageBox(_T("超过最大数据量"));
		return;
	}
	/*-----------------*/
	USES_CONVERSION;
	CString d0,d1,d2,c_index;
	char buffer[64];
	double *x = (double *)param_list_show.param_list[in].data_x;
	double *y = (double *)param_list_show.param_list[in].data_y;
	/*-----------------*/
	sprintf_s(buffer,"%d",pos);
	/*-----------------*/
	c_index = A2T(buffer);
	/*------------------*/
	sprintf_s(buffer,"%.10lf",x[pos]);
	/*-----------------*/
	remove_zero(buffer,strlen(buffer));
	/*-----------------*/
	d0 = A2T(buffer);
	/*------------------*/
	sprintf_s(buffer,"%.10lf",y[pos]);
	/*-----------------*/
	remove_zero(buffer,strlen(buffer));
	/*-----------------*/
	d1 = A2T(buffer);
	/*------------------*/
	sprintf_s(buffer,"%d",pos);
	/*-----------------*/
	d2 = A2T(buffer);
	/*------------------*/
	m_data_review_list.InsertItem(line_zero, c_index);
	m_data_review_list.SetItemText(line_zero, 1, d0);
	m_data_review_list.SetItemText(line_zero, 2, d1);
	m_data_review_list.SetItemText(line_zero, 3, d2);

	/*------------*/
    line_zero++;
	/*------------*/
}
/*---------------*/
void data_review::search_x_rang(double *num_darray,unsigned int num,unsigned int in)
{
	if( num != 2 )
	{
		AfxMessageBox(_T("参数错误，支持两个参数xxx-xxx"));
		return;
	}
	/*-----------*/
	if( num_darray[0] > num_darray[1] )
	{
		AfxMessageBox(_T("范围错误，左值应该小于等于右值"));
		return;
	}
	/* single */
	if( single_time == 0 )
	{
		/* clear */
		line_zero = 0;
		m_data_review_list.DeleteAllItems();
	}
	/* search and add to list */
	double *x = (double *)param_list_show.param_list[in].data_x;
	unsigned int pm = param_list_show.param_list[in].point_num;
	/* */
	for( unsigned int i = 0 ; i < pm ; i ++ )
	{
		if( x[i] >= num_darray[0] && x[i] <= num_darray[1] )
		{
			add_one_line(in,i);
		}
	}
}
void data_review::search_y_rang(double *num_darray,unsigned int num,unsigned int in)
{
	if( num != 2 )
	{
		AfxMessageBox(_T("参数错误，支持两个参数xxx-xxx"));
		return;
	}
	/*-----------*/
	if( num_darray[0] > num_darray[1] )
	{
		AfxMessageBox(_T("范围错误，左值应该小于等于右值"));
		return;
	}
	/* single */
	if( single_time == 0 )
	{
		/* clear */
		line_zero = 0;
		m_data_review_list.DeleteAllItems();
	}
	/* search and add to list */
	double *y = (double *)param_list_show.param_list[in].data_y;
	unsigned int pm = param_list_show.param_list[in].point_num;
	/* */
	for( unsigned int i = 0 ; i < pm ; i ++ )
	{
		if( y[i] >= num_darray[0] && y[i] <= num_darray[1] )
		{
			add_one_line(in,i);
		}
	}
}
void data_review::search_kx_rang(double *num_darray,unsigned int num,unsigned int in)
{
	if( num != 2 )
	{
		AfxMessageBox(_T("参数错误，支持两个参数xxx-xxx"));
		return;
	}
	/*-----------*/
	if( num_darray[0] > num_darray[1] )
	{
		AfxMessageBox(_T("范围错误，左值应该小于等于右值"));
		return;
	}
	/* single */
	if( single_time == 0 )
	{
		/* clear */
		line_zero = 0;
		m_data_review_list.DeleteAllItems();
	}
	/* search and add to list */
	double *x = (double *)param_list_show.param_list[in].data_x;
	unsigned int pm = param_list_show.param_list[in].point_num;
	/* */
	for( unsigned int i = 1 ; i < pm ; i ++ )
	{
		if( (x[i] - x[i-1]) >= num_darray[0] && (x[i] - x[i-1]) <= num_darray[1] )
		{
			add_one_line(in,i);
		}
	}
}
void data_review::search_ky_rang(double *num_darray,unsigned int num,unsigned int in)
{
	if( num != 2 )
	{
		AfxMessageBox(_T("参数错误，支持两个参数xxx-xxx"));
		return;
	}
	/*-----------*/
	if( num_darray[0] > num_darray[1] )
	{
		AfxMessageBox(_T("范围错误，左值应该小于等于右值"));
		return;
	}
	/* single */
	if( single_time == 0 )
	{
		/* clear */
		line_zero = 0;
		m_data_review_list.DeleteAllItems();
	}
	/* search and add to list */
	double *y = (double *)param_list_show.param_list[in].data_y;
	unsigned int pm = param_list_show.param_list[in].point_num;
	/* */
	for( unsigned int i = 1 ; i < pm ; i ++ )
	{
		if( (y[i] - y[i-1]) >= num_darray[0] && (y[i] - y[i-1]) <= num_darray[1] )
		{
			add_one_line(in,i);
		}
	}
}

void data_review::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	if( single_time == 0 )
	{
		m_single.SetWindowTextW(_T("多次"));
		single_time = 1;
	}
	else
	{
		m_single.SetWindowTextW(_T("单次"));
		single_time = 0;
	}
}
/* remove 0 */
void data_review::remove_zero(char * d , unsigned int len)
{
	for( unsigned int i = 0 ; i < len ; i ++ )
	{
		if( d[ len - 1 - i ] == '0' )
		{
			d[ len - 1 - i ] = 0;
		}
		else if( d[ len - 1 - i ] == '.' )
		{
			d[ len - 1 - i ] = 0;
			break;
		}
		else
		{
			break;
		}
	}
}