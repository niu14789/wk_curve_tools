// Data_detail.cpp : 实现文件
//

#include "stdafx.h"
#include "Data_detail.h"
#include "afxdialogex.h"
#include "TeeChart5_testDlg.h"

/*-----------------*/
extern SYS_LIST_CHESE_DEF chese_list;
extern int now_seq;
// Data_detail 对话框

IMPLEMENT_DYNAMIC(Data_detail, CDialogEx)

Data_detail::Data_detail(CWnd* pParent /*=NULL*/)
	: CDialogEx(Data_detail::IDD, pParent)
{

}

Data_detail::~Data_detail()
{
}

void Data_detail::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_data_detail_list);
		DDX_Control(pDX, IDC_COMBO1, m_param_select);
	/* init */
	/* list contrl */
	DWORD dwStyle = m_data_detail_list.GetExtendedStyle();     
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	/*--------*/
	m_data_detail_list.SetExtendedStyle(dwStyle);
	/* capute */
	m_data_detail_list.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 40);
	m_data_detail_list.InsertColumn(1, _T("数值"), LVCFMT_LEFT, 80);
	m_data_detail_list.InsertColumn(2, _T("均值"), LVCFMT_CENTER, 80);
	m_data_detail_list.InsertColumn(3, _T("方差"), LVCFMT_CENTER, 80);
	/*-------------------*/
	Data_update_source(now_seq,0);
	m_param_select.SetCurSel(0);
}


BEGIN_MESSAGE_MAP(Data_detail, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &Data_detail::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()



// Data_detail 消息处理程序
void Data_detail::Data_update_source(int seq,int now_select)
{
	char buffer[16];
	CString c_name,c_chese,c_index,c_data,c_result;
	float * show_float;
	/*---------*/
	USES_CONVERSION;
	/* delete */
	m_data_detail_list.DeleteAllItems();
	/*--------------------------------*/
	/*for( int j = 0 ; j < chese_list.chese[seq].num ; j ++ )
	{*/
		/*---*/
		for( int i = 0 ; i < chese_list.chese[seq].mean_rms_num ; i ++ )
		{
			/*-----------------*/
			sprintf_s(buffer,"%d",i);
			/*-----------------*/
			c_index = A2T(buffer);
			/*-----------------*/
			/*switch(chese_list.chese[seq].type)
			{
				case 0x05:*/
					show_float = (float *)&chese_list.chese[seq].data_mean_rms[now_select];
		/*			break;
				default:
					break;
			}*/
			/* get data */
			sprintf_s(buffer,"%f",show_float[i]);
			/*---------------*/
			c_data = A2T(buffer);
			/*--------*/
			m_data_detail_list.InsertItem(i, c_index);
			m_data_detail_list.SetItemText(i, 1, c_data);
		}
	//}
}

void Data_detail::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int select = m_param_select.GetCurSel();
	/* update */
	Data_update_source(now_seq,select);
}
