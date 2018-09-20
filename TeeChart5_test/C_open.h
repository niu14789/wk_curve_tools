#pragma once

#include "Resource.h"
#include "afxwin.h"
// C_open 对话框11

typedef struct 
{
	unsigned int file_enable;//0 is enaable
	char file_path[512];
	char file_tmp[512];
	unsigned int default_procotol_type;
	char file_name[32];
	char file_point[32];
}FILE_OPEN_DEF;

typedef struct 
{
	unsigned int num;
	FILE_OPEN_DEF file[10];
}FILE_MAN_DEF;

class C_open : public CDialogEx
{
	DECLARE_DYNAMIC(C_open)

public:
	C_open(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~C_open();
	void C_open::open_file(unsigned int index);
// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton12();
	CComboBox m_co1;
	CComboBox m_co2;
	CComboBox m_co3;
	CComboBox m_co4;
	CComboBox m_co5;
	CComboBox m_co6;
	CComboBox m_co7;
	CComboBox m_co8;
	CComboBox m_co9;
	CComboBox m_co10;
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
