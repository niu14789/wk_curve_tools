// map.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "map.h"
#include "afxdialogex.h"


// map �Ի���

IMPLEMENT_DYNAMIC(map, CDialogEx)

map::map(CWnd* pParent /*=NULL*/)
	: CDialogEx(map::IDD, pParent)
{

}

map::~map()
{
}

void map::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_web);

	m_web.Navigate(_T("D:/map.html"),NULL,NULL,NULL,NULL);
}


BEGIN_MESSAGE_MAP(map, CDialogEx)
END_MESSAGE_MAP()


// map ��Ϣ�������
