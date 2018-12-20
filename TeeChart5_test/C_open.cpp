// C_open.cpp : 实现文件
//

#include "stdafx.h"
#include "C_open.h"
#include "afxdialogex.h"

// C_open 对话框

IMPLEMENT_DYNAMIC(C_open, CDialogEx)

C_open::C_open(CWnd* pParent /*=NULL*/)
	: CDialogEx(C_open::IDD, pParent)
{

}

C_open::~C_open()
{
}

void C_open::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_co1);


	widget_init();

}


BEGIN_MESSAGE_MAP(C_open, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &C_open::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &C_open::OnBnClickedButton2)
	ON_WM_DROPFILES()
	ON_CBN_SELCHANGE(IDC_COMBO1, &C_open::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// C_open 消息处理程序

/* start -----------------------------------****************************************/

FILE_MAN_DEF file_man;

unsigned int file_index = 0;

/* data initial */
void C_open::widget_init(void)
{
	USES_CONVERSION;
	/*---------------------------*/
	CString show;
	/*---------------------------*/
	m_co1.ResetContent();
	/*---------------*/
	for( int i = 0 ; i < file_man.num ; i ++ )
	{
		if( file_man.file[i].file_enable )
		{
			char buffer[128];
			/*------------*/
			sprintf(buffer,"%s.%s",file_man.file[i].file_name,file_man.file[i].file_point);
			show = A2T(buffer);
			/* add to combox */
			m_co1.AddString(show);
		}
	}
	/*--------------------------*/
	//focus on one map
	focus_on(file_man.num - 1);
}
/*------------------------------*/
void C_open::focus_on(int index)
{
	USES_CONVERSION;
	char buffer[128];
	/* open */
	if( !file_man.num )
	{
		return;//
	}
	/* open */
	file_index = index;
	/* set */
	m_co1.SetCurSel(index);
	/*-----------------------*/
	if( file_man.file[index].default_procotol_type == 0 )
	{
		sprintf(buffer,"%s.CFS",file_man.file[index].file_point);
		CString show = A2T(buffer);
		/*-----------------------*/
		SetDlgItemText(IDC_EDIT1,show);
	}
	else
	{
		CString show = A2T(file_man.file[index].procotol_file);
		/*-----------------------*/
		SetDlgItemText(IDC_EDIT1,show);
	}
}
/*------------------------------*/
CString strFilePath;

int C_open::open_file(unsigned int index)
{
    TCHAR szFilter[] = _T("解析文件|*.CFS|"); 

    CFileDialog fileDlg(TRUE, _T("open file"), NULL, 0, szFilter, this); 

    if(IDOK == fileDlg.DoModal())   
    {   
        strFilePath = fileDlg.GetPathName();  
        SetDlgItemText(IDC_EDIT1, strFilePath);

		return 0;
    }

	return (-1);
}

void C_open::OnBnClickedButton1()
{
	/* personal procotol */
	file_man.file[file_index].default_procotol_type = 0;
	/*-------------------*/
	OnOK();
}

void C_open::Set_procotol(char * path)
{
	/* open file */
	memset(file_man.file[file_index].procotol_file,0,sizeof(file_man.file[file_index].procotol_file));
	/* set lis */
	memcpy(file_man.file[file_index].procotol_file,path,strlen(path));
	/* personal procotol */
	file_man.file[file_index].default_procotol_type = 1;
}

void C_open::OnBnClickedButton2()
{
    if( !file_man.num )
	{
		return;//
	}
	/* ok or fail */
	if( open_file(1) == 0 )
	{
		/*-----------------------*/
		USES_CONVERSION;
		/*-----------------------*/
		char * tc = T2A(strFilePath);
		/* set */
		Set_procotol(tc);
		/* exit */
		OnOK();
	}
}

void C_open::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	 if( !file_man.num )
	{
		return;//
	}
	UINT count;            
    WCHAR  filePath[200];          
    count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);   //系统的API函数         
    if(count)             
    {  
        for(UINT i=0; i<count; i++)                      
        {  
            int pathLen = DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));  //API函数   
			USES_CONVERSION;
			/*-----------------------*/
			char * tc = T2A(filePath);
			/* set */
			Set_procotol(tc);
			/*---------------------------------*/
			SetDlgItemText(IDC_EDIT1, filePath);
			/* exit */
			OnOK();
        }  
    }  
    DragFinish(hDropInfo);   //API函数
	CDialogEx::OnDropFiles(hDropInfo);
}


void C_open::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
    if( !file_man.num )
	{
		return;//
	}
	/*------------------------*/
	file_index = m_co1.GetCurSel();
	/* set */
	focus_on(file_index);
}
