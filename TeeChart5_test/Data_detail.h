#pragma once

#include "Resource.h"
#include "afxcmn.h"
#include "afxwin.h"
// Data_detail 对话框

class Data_detail : public CDialogEx
{
	DECLARE_DYNAMIC(Data_detail)

public:
	void Data_detail::Data_update_source(int seq,int now_select);
	Data_detail(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Data_detail();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_data_detail_list;
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_param_select;
};
