#pragma once

#include "Resource.h"

// help �Ի���

class help : public CDialogEx
{
	DECLARE_DYNAMIC(help)

public:
	help(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~help();

// �Ի�������
	enum { IDD = IDD_DIALOG14 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
