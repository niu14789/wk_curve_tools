#pragma once

#include "Resource.h"
#include "explorer1.h"

// map �Ի���

class map : public CDialogEx
{
	DECLARE_DYNAMIC(map)

public:
	map(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~map();

// �Ի�������
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 m_web;
};
