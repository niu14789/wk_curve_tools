#pragma once

#include "Resource.h"
#include "afxwin.h"
#include "afxcmn.h"

// export_tool 对话框

class export_tool : public CDialogEx
{
	DECLARE_DYNAMIC(export_tool)

public:
	void export_tool::remove_zero(char * d , unsigned int len);
	int export_tool::get_string_edit( int id,void * data , unsigned int len );
	void export_tool::export_list_flush(void);
	void export_tool::init(void);
	export_tool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~export_tool();

// 对话框数据
	enum { IDD = IDD_DIALOG12 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_export_file;
	CComboBox m_format;
	CButton m_output_title;
	CButton m_btn_export;
	CProgressCtrl m_export_process;
	CListCtrl m_list_p;
	CListCtrl m_out_list;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnCbnSelchangeCombo1();
	CButton two_axis;
	afx_msg void OnBnClickedCheck3();
	CButton m_default_path;
	afx_msg void OnBnClickedButton1();
};
