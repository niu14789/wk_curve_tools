#pragma once
#include "Resource.h"
#include "afxwin.h"

// del_point 对话框

class del_point : public CDialogEx
{
	DECLARE_DYNAMIC(del_point)

public:
	int del_point::get_dex_edit( int id,unsigned int * data , unsigned int len );
	void del_point::line_list_init(void);
	del_point(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~del_point();

// 对话框数据
	enum { IDD = IDD_DIALOG10 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_del_point_list;
	CEdit m_point_start;
	CEdit m_point_stop;
	afx_msg void OnBnClickedButton1();
};
