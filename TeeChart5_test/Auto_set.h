#pragma once
#include "Resource.h"
#include "afxwin.h"

// Auto_set �Ի���

class Auto_set : public CDialogEx
{
	DECLARE_DYNAMIC(Auto_set)

public:
	Auto_set(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Auto_set();

// �Ի�������
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	int Auto_set::get_mm_edit( int id , void * data , unsigned int len );
	DECLARE_MESSAGE_MAP()
public:
	CButton m_auto_scale;
	CButton m_smart_scale;
	CEdit m_min_edit;
	CEdit m_max_edit;
	CButton m_button_auto_set;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton1();
};
