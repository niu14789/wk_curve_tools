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
	DDX_Control(pDX, IDC_COMBO2, m_co2);
	DDX_Control(pDX, IDC_COMBO3, m_co3);
	DDX_Control(pDX, IDC_COMBO4, m_co4);
	DDX_Control(pDX, IDC_COMBO5, m_co5);
	DDX_Control(pDX, IDC_COMBO6, m_co6);
	DDX_Control(pDX, IDC_COMBO7, m_co7);
	DDX_Control(pDX, IDC_COMBO8, m_co8);
	DDX_Control(pDX, IDC_COMBO9, m_co9);
	DDX_Control(pDX, IDC_COMBO10, m_co10);
}


BEGIN_MESSAGE_MAP(C_open, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &C_open::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &C_open::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &C_open::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &C_open::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &C_open::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &C_open::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &C_open::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &C_open::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &C_open::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &C_open::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON12, &C_open::OnBnClickedButton12)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// C_open 消息处理程序

/* start -----------------------------------****************************************/

FILE_MAN_DEF file_man;

CString strFilePath[20];
const unsigned int edit_unit[20] =
{ IDC_EDIT1,IDC_EDIT2,IDC_EDIT3,IDC_EDIT4,IDC_EDIT5,IDC_EDIT6,IDC_EDIT7,IDC_EDIT8,IDC_EDIT9,IDC_EDIT10 };

void C_open::open_file(unsigned int index)
{
		//// TODO: 在此添加命令处理程序代码
    TCHAR szFilter[] = _T("波形文件|*.*|"); 

    CFileDialog fileDlg(TRUE, _T("open file"), NULL, 0, szFilter, this); 

    if(IDOK == fileDlg.DoModal())   
    {   
        strFilePath[index] = fileDlg.GetPathName();  
        SetDlgItemText(edit_unit[index], strFilePath[index]);
    }
}

void C_open::OnBnClickedButton1()
{
	open_file(0);
}


void C_open::OnBnClickedButton2()
{
	open_file(1);
}


void C_open::OnBnClickedButton3()
{
	open_file(2);
}


void C_open::OnBnClickedButton4()
{
	open_file(3);
}


void C_open::OnBnClickedButton5()
{
	open_file(4);
}


void C_open::OnBnClickedButton6()
{
	open_file(5);
}


void C_open::OnBnClickedButton7()
{
	open_file(6);
}


void C_open::OnBnClickedButton8()
{
    open_file(7);
}


void C_open::OnBnClickedButton9()
{
	open_file(8);
}


void C_open::OnBnClickedButton10()
{
	open_file(9);
}


void C_open::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	m_co1.AddString(_T("123"));
}


void C_open::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT count;            
    WCHAR  filePath[200];          
    count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);   //系统的API函数         
    if(count)             
    {  
        for(UINT i=0; i<count; i++)                      
        {  
            int pathLen = DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));  //API函数                                
        }  
    }  
    DragFinish(hDropInfo);   //API函数
	CDialogEx::OnDropFiles(hDropInfo);
}
