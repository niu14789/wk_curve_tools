// contribution.cpp : 实现文件
//

#include "stdafx.h"
#include "contribution.h"
#include "afxdialogex.h"


// contribution 对话框

IMPLEMENT_DYNAMIC(contribution, CDialogEx)

contribution::contribution(CWnd* pParent /*=NULL*/)
	: CDialogEx(contribution::IDD, pParent)
{

}

contribution::~contribution()
{
}

void contribution::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(contribution, CDialogEx)
END_MESSAGE_MAP()


// contribution 消息处理程序
