#pragma once

#include "Resource.h"

// contribution �Ի���

class contribution : public CDialogEx
{
	DECLARE_DYNAMIC(contribution)

public:
	contribution(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~contribution();

// �Ի�������
	enum { IDD = IDD_DIALOG13 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
