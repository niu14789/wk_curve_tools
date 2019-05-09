// export_tool.cpp : 实现文件
//

#include "stdafx.h"
#include "export_tool.h"
#include "afxdialogex.h"
#include "TeeChart5_testDlg.h"

extern PARAM_LIST_DEF param_list_show;

static unsigned int export_count = 0;
static unsigned int exp_change[1000];

/* create */
char file_buffer_base[128];
char file_buffer_base_file[128];
// export_tool 对话框
char push_buffer[1024*1024*2];
/*--------------------*/
IMPLEMENT_DYNAMIC(export_tool, CDialogEx)

export_tool::export_tool(CWnd* pParent /*=NULL*/)
	: CDialogEx(export_tool::IDD, pParent)
{

}

export_tool::~export_tool()
{
}

void export_tool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_export_file);
	DDX_Control(pDX, IDC_COMBO1, m_format);
	DDX_Control(pDX, IDC_CHECK1, m_output_title);
	DDX_Control(pDX, IDC_BUTTON3, m_btn_export);
	DDX_Control(pDX, IDC_PROGRESS1, m_export_process);
	DDX_Control(pDX, IDC_LIST1, m_list_p);
	DDX_Control(pDX, IDC_LIST2, m_out_list);

	DDX_Control(pDX, IDC_CHECK2, two_axis);

	DDX_Control(pDX, IDC_CHECK3, m_default_path);

	init();
}


BEGIN_MESSAGE_MAP(export_tool, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &export_tool::OnNMDblclkList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &export_tool::OnNMDblclkList2)
	ON_BN_CLICKED(IDC_BUTTON4, &export_tool::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &export_tool::OnBnClickedButton3)
	ON_CBN_SELCHANGE(IDC_COMBO1, &export_tool::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CHECK3, &export_tool::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_BUTTON1, &export_tool::OnBnClickedButton1)
END_MESSAGE_MAP()


// export_tool 消息处理程序
void export_tool::init(void)
{
	char buffer[64];
	CString d0,c_index;
	/* transform */
	USES_CONVERSION;
	/* list contrl */
	DWORD dwStyle = m_list_p.GetExtendedStyle();     
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	/*--------*/
	m_list_p.SetExtendedStyle(dwStyle);
	/* capute */
	m_list_p.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40);
	m_list_p.InsertColumn(1, _T("曲线"), LVCFMT_LEFT, 160);
	/* list data */
	m_list_p.DeleteAllItems();
	/*---------------------*/
	m_out_list.SetExtendedStyle(dwStyle);
	/* capute */
	m_out_list.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40);
	m_out_list.InsertColumn(1, _T("导出曲线列表"), LVCFMT_LEFT, 160);
	/* list data */
	m_out_list.DeleteAllItems();
	/*---------------------------*/
	m_default_path.SetCheck(0);//default.h
	/*---------------------------*/
	for( unsigned int i = 0 ; i < param_list_show.param_list_num ; i ++ )
	{
		sprintf_s(buffer,"%d",i);
		/*-----------------*/
		c_index = A2T(buffer);
		/*-----------------*/
		char * p = strstr( param_list_show.param_list[i].name,"->");
		/*------------------*/
		if( p != NULL )
		{
		    d0 = A2T(p);
		}else
		{
			d0 = A2T(param_list_show.param_list[i].name);
		}
		/*------------------*/
		m_list_p.InsertItem(i, c_index);
		m_list_p.SetItemText(i, 1, d0);	
	}
	/* set format */
	m_format.ResetContent();
	m_format.AddString(_T("CSV"));
	m_format.AddString(_T("TXT"));
	/* focus */
	m_format.SetCurSel(0);//default
	/* default out put title */
	m_output_title.SetCheck(1);
	/* flush the out list */
	export_list_flush();
	/* create path and get time */
	CString fpath;
	char time_buffer[32];
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(time_buffer,sizeof(time_buffer),"%04d-%02d-%02d_%02d-%02d-%02d-%03d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);

	unsigned char dir_ok = 0;
	/*--------------------------*/
    if(!PathIsDirectory(_T("export")))
    {
	   if( !CreateDirectory(_T("export"), 0) )
	   {
		   //create_fail
		   dir_ok = 1;
	   }
    }
	/* set */
	char buff_b[128];
    /* create file */
	if( dir_ok == 0 )
	{
		/* transform */
		USES_CONVERSION;
		/*---------------------------*/
		char * p;
		/* path */
		wchar_t pFileName[MAX_PATH];     
		/* get current dir */
		int nPos = GetCurrentDirectory( MAX_PATH, pFileName);
		/* transfer and create */
		p = T2A(pFileName);
		/*---------------------*/
       int seq = m_format.GetCurSel();
	   /* create base name */
	   sprintf_s(file_buffer_base,sizeof(file_buffer_base),"%s\\export\\%s",p,time_buffer);
	   /* format */
	   if( seq == 0 )
	   {
		   sprintf_s(buff_b,sizeof(buff_b),"%s.csv",file_buffer_base);
	   }
	   else if( seq == 1 )
	   {
		   sprintf_s(buff_b,sizeof(buff_b),"%s.txt",file_buffer_base);
	   }
	   /*---------------------*/
	   fpath = A2T(buff_b);
	}
	else
	{
		fpath = A2T("<输出文件夹创建失败，需要手动输入文件路径>");
	}
	/* create file base */
    if( param_list_show.param_list_num > 0 )
	{
        sprintf_s(file_buffer_base_file,sizeof(file_buffer_base_file),"%s",param_list_show.param_list[0].from_file);
	}
	/*-----------------*/
	if( m_default_path.GetCheck() )
	{
		m_export_file.SetWindowTextW(fpath);
	}
	else
	{
        if( param_list_show.param_list_num > 0 )
		{
			/*---------------------------*/
			sprintf_s(buff_b,sizeof(buff_b),"%s.%s",file_buffer_base_file,(m_format.GetCurSel()==0)?"CSV":"TXT");
			/*--------------------------------*/
			fpath = A2T(buff_b);
			m_export_file.SetWindowTextW(fpath);
		}
	}
	
}
/* flush list */
void export_tool::export_list_flush(void)
{
	char buffer[64];
	CString d0,c_index;
	/* transform */
	USES_CONVERSION;
	m_out_list.DeleteAllItems();
	export_count = 0;
	/* add to list */
	for( unsigned int i = 0 ; i < param_list_show.param_list_num ; i ++ )
	{
		if( param_list_show.param_list[i].export_flag == 1 )
		{
			sprintf_s(buffer,"%d",export_count);
			/*-----------------*/
			c_index = A2T(buffer);
			/*-----------------*/
			char * p = strstr( param_list_show.param_list[i].name,"->");
			/*------------------*/
			if( p != NULL )
			{
				d0 = A2T(p);
			}else
			{
				d0 = A2T(param_list_show.param_list[i].name);
			}
			/*------------------*/
			m_out_list.InsertItem(export_count, c_index);
			m_out_list.SetItemText(export_count, 1, d0);	
			/* add to queue list */
			exp_change[export_count] = i;
			export_count++;
		}
	}
}
/* event */
void export_tool::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	/* set info */
	POSITION ps;
	ps = m_list_p.GetFirstSelectedItemPosition();
	int nIndex = m_list_p.GetNextSelectedItem(ps);
	/* add data to export list */
	param_list_show.param_list[nIndex].export_flag = 1;
	/* reflush list */
	export_list_flush();
	/*--------------*/
	export_list_flush();
}
/*-----------------------------*/
void export_tool::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	/* set info */
	POSITION ps;
	ps = m_out_list.GetFirstSelectedItemPosition();
	int nIndex = m_out_list.GetNextSelectedItem(ps);
	/* get list */
	int seq = exp_change[nIndex];
	/*----------*/
	param_list_show.param_list[seq].export_flag = 0;
	/* flush the lish */
	export_list_flush();
}
/*------------------*/
void export_tool::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
    memset(exp_change,0,sizeof(exp_change));
	m_out_list.DeleteAllItems();
	/* clear */
	for( unsigned int i = 0 ; i < param_list_show.param_list_num ; i ++ )
	{
		param_list_show.param_list[i].export_flag = 0;
	}
}
/* public function */
int export_tool::get_string_edit( int id,void * data , unsigned int len )
{
	wchar_t get_cs[512];
    int len_t;
	/*--------------------*/
	memset(data,0,len);
	memset(get_cs,0,sizeof(get_cs));
	/*--------------------*/
	len_t = GetDlgItem(id)->GetWindowTextW(get_cs,512);
    /*--------------------*/
	/* transfer */
	USES_CONVERSION;
	/* transfer */
	char * path_c = T2A(get_cs);
	/*----------------------*/
	memcpy(data,path_c,strlen(path_c));
	/*----------------------*/
	return len_t;
}
/* dec */
void export_tool::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if( export_count == 0 )
	{
		AfxMessageBox(_T("未选择曲线"));
		return;
	}
	/*----------*/
	char path[128];
	/* get path */
	get_string_edit(IDC_EDIT1,path,sizeof(path));
	/* set out */
	int title_enable = m_output_title.GetCheck();
	int two_axis_enable = two_axis.GetCheck();
	int format = m_format.GetCurSel();
	/* create file */
	FILE * fp;
	/* open */
	fopen_s(&fp,path,"wb+");
	/* ok ? */
	if( fp == NULL )
	{
		AfxMessageBox(_T("输出文件创建失败"));
		return;
	}
	/* get title and axis */
	memset(push_buffer,0,sizeof(push_buffer));
#if 1
	/* create */
	if( title_enable )
	{
		for( unsigned int i = 0 ; i < export_count ; i ++ )
		{
			int index = exp_change[i];
			/*------------------*/
			char * c  = strstr(param_list_show.param_list[index].name,"->");
			/*------------------*/
			if( c != NULL )
			{
				if( two_axis_enable == 0 )
				{
					sprintf_s(push_buffer+strlen(push_buffer),sizeof(push_buffer) - strlen(push_buffer),
						"%s%s",c+2,format?"   ":" , ");
				}
				else
				{
					sprintf_s(push_buffer+strlen(push_buffer),sizeof(push_buffer) - strlen(push_buffer),
						"%s_DX%s%s_DY%s",c+2,format?"   ":" , ",c+2,format?"   ":" , ");
				}
			}
		}
		/*----------------------*/
		fwrite(format?"index  ":"index ,",1,7,fp);
		fwrite(push_buffer,1,strlen(push_buffer),fp);
		fwrite("\r\n",1,2,fp);
	}
	/* get max point num */
	unsigned int max_point_num = 0;
	/* loop */
	for( unsigned int i = 0 ; i < export_count ; i ++ )
	{
		int index = exp_change[i];
		/* data log */
		if( param_list_show.param_list[index].point_num > max_point_num )
		{
			max_point_num = param_list_show.param_list[index].point_num;
		}
	}
	/* set process */
	m_export_process.SetRange32(0,max_point_num);
	/* set disable */
	m_export_file.EnableWindow(0);
	m_format.EnableWindow(0);
	m_output_title.EnableWindow(0);
	m_btn_export.EnableWindow(0);
	m_default_path.EnableWindow(0);
	two_axis.EnableWindow(0);
	m_btn_export.EnableWindow(0);
	m_default_path.EnableWindow(0);
	m_list_p.EnableWindow(0);
	m_out_list.EnableWindow(0);
	/* every one */
	for( unsigned int ti = 0 ; ti < max_point_num ; ti ++ )
	{
		memset(push_buffer,0,sizeof(push_buffer));
		/* create index */
		sprintf_s(push_buffer,sizeof(push_buffer),"%d%s",ti+1,format?"   ":" , ");
		/* write into */
		fwrite(push_buffer,1,strlen(push_buffer),fp);
		/* push one point */
		memset(push_buffer,0,sizeof(push_buffer));
		/* create data */
		for( unsigned int i = 0 ; i < export_count ; i ++ )
		{
			int index = exp_change[i];
			/* data log */
			double * dx = (double *)param_list_show.param_list[index].data_x;
			double * dy = (double *)param_list_show.param_list[index].data_y;
			/*----------*/
			if( ti < param_list_show.param_list[index].point_num )
			{
				if( two_axis_enable == 0 ) //one axis
				{
					sprintf_s(push_buffer+strlen(push_buffer),sizeof(push_buffer) - strlen(push_buffer),
						"%.10lf",dy[ti]);
					/*-----------------*/
					remove_zero(push_buffer,strlen(push_buffer));
					/*-----------------*/
					sprintf_s(push_buffer+strlen(push_buffer),sizeof(push_buffer) - strlen(push_buffer),
											"%s",format?"   ":" , ");
				}
				else
				{
					sprintf_s(push_buffer+strlen(push_buffer),sizeof(push_buffer) - strlen(push_buffer),
						"%.10lf",dx[ti]);
					/*-----------------*/
					remove_zero(push_buffer,strlen(push_buffer));
					/*-----------------*/
					sprintf_s(push_buffer+strlen(push_buffer),sizeof(push_buffer) - strlen(push_buffer),
						"%s%.10lf",format?"   ":" , ",dy[ti]);
					/*-----------------*/
					remove_zero(push_buffer,strlen(push_buffer));
					/*-----------------*/
					sprintf_s(push_buffer+strlen(push_buffer),sizeof(push_buffer) - strlen(push_buffer),
						"%s",format?"   ":" , ");
				}
			}
			else
			{
				if( two_axis_enable == 0 ) //one axis
				{
					sprintf_s(push_buffer+strlen(push_buffer),sizeof(push_buffer) - strlen(push_buffer),
						"%s%s%s%s"," ",format?"   ":" , "," ",format?"   ":" , ");
				}
				else
				{
					sprintf_s(push_buffer+strlen(push_buffer),sizeof(push_buffer) - strlen(push_buffer),
							"%s%s"," ",format?"   ":" , ");
				}
			}
		}
		/* write data to file */
		fwrite(push_buffer,1,strlen(push_buffer),fp);
		fwrite("\r\n",1,2,fp);
		/*--------------------*/
		m_export_process.SetPos(ti);
		/*--------------------*/
	}
	/* close files */
	fclose(fp);
	/* msg */
	AfxMessageBox(_T("导出成功"));
	/* set enable */
	m_export_file.EnableWindow(1);
	m_format.EnableWindow(1);
	m_output_title.EnableWindow(1);
	m_btn_export.EnableWindow(1);
	m_default_path.EnableWindow(1);
	two_axis.EnableWindow(1);
	m_btn_export.EnableWindow(1);
	m_default_path.EnableWindow(1);
	m_list_p.EnableWindow(1);
	m_out_list.EnableWindow(1);
#endif
}


void export_tool::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	char buf[128];
	int seq = m_format.GetCurSel();
	/* format */
    if( m_default_path.GetCheck() )
	{
		sprintf_s(buf,sizeof(buf),"%s.%s",file_buffer_base,seq?"txt":"csv");
	}
	else
	{
		sprintf_s(buf,sizeof(buf),"%s.%s",file_buffer_base_file,seq?"txt":"csv");
	}
	/* transfer */
	USES_CONVERSION;
	/* set data */
	CString ctr = A2T(buf);
	/* show */
	m_export_file.SetWindowTextW(ctr);
}


void export_tool::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	char buf[128];
	int seq = m_format.GetCurSel();
	/* format */
    if( m_default_path.GetCheck() )
	{
		sprintf_s(buf,sizeof(buf),"%s.%s",file_buffer_base,seq?"txt":"csv");
	}
	else
	{
		sprintf_s(buf,sizeof(buf),"%s.%s",file_buffer_base_file,seq?"txt":"csv");
	}
	/* transfer */
	USES_CONVERSION;
	/* set data */
	CString ctr = A2T(buf);
	/* show */
	m_export_file.SetWindowTextW(ctr);
}


void export_tool::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	for( unsigned int i = 0 ; i < param_list_show.param_list_num ; i ++ )
	{
		param_list_show.param_list[i].export_flag = 1;
	}
	/* flush the lish */
	export_list_flush();
}
void export_tool::remove_zero(char * d , unsigned int len)
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