#pragma once
#include "Resource.h"
#include "afxwin.h"

// suggestion �Ի���

class suggestion : public CDialogEx
{
	DECLARE_DYNAMIC(suggestion)

public:
	int suggestion::get_string_edit( int id,void * data , unsigned int len );
	suggestion(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~suggestion();

// �Ի�������
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	CEdit m_topic;
	CEdit m_nick_name;
	CEdit m_describe;
	CButton m_loader;
};
