#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Resource.h"

// data_review �Ի���

class data_review : public CDialogEx
{
	DECLARE_DYNAMIC(data_review)

public:
	void data_review::page_msg(unsigned int index , unsigned int pos);
	void data_review::data_anypostion(unsigned int index , unsigned int pos,unsigned int count);
	void data_review::Init(void);
	data_review(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~data_review();

// �Ի�������
	enum { IDD = IDD_DIALOG11 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_data_review_list;
	CComboBox m_line_com;
	afx_msg void OnCbnSelchangeCombo1();
	CEdit m_skip_edit;
};
