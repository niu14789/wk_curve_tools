#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Resource.h"

// data_review 对话框

class data_review : public CDialogEx
{
	DECLARE_DYNAMIC(data_review)

public:
	void data_review::page_msg(unsigned int index , unsigned int pos);
	void data_review::data_anypostion(unsigned int index , unsigned int pos,unsigned int count);
	void data_review::Init(void);
	data_review(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~data_review();

// 对话框数据
	enum { IDD = IDD_DIALOG11 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_data_review_list;
	CComboBox m_line_com;
	afx_msg void OnCbnSelchangeCombo1();
	CEdit m_skip_edit;
};
