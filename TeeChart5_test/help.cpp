// help.cpp : 实现文件
//

#include "stdafx.h"
#include "help.h"
#include "afxdialogex.h"


// help 对话框

IMPLEMENT_DYNAMIC(help, CDialogEx)

help::help(CWnd* pParent /*=NULL*/)
	: CDialogEx(help::IDD, pParent)
{

}

help::~help()
{
}

void help::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(help, CDialogEx)
END_MESSAGE_MAP()


// help 消息处理程序
