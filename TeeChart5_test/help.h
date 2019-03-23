#pragma once

#include "Resource.h"

// help 对话框

class help : public CDialogEx
{
	DECLARE_DYNAMIC(help)

public:
	help(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~help();

// 对话框数据
	enum { IDD = IDD_DIALOG14 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
