// help.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "help.h"
#include "afxdialogex.h"


// help �Ի���

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


// help ��Ϣ�������
