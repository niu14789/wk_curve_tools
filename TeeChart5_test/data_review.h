#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Resource.h"

// data_review 对话框

class data_review : public CDialogEx
{
	DECLARE_DYNAMIC(data_review)

public:
	void data_review::jump_to_postion(double *num_array,unsigned int num,unsigned int in);
	int data_review::get_dex_edit( int id,double* data);
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
	CComboBox m_mothed;
	CEdit m_search_edit0;
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnBnClickedButton34();
	afx_msg void OnBnClickedButton2();
};
