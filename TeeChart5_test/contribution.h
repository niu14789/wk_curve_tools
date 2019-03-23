#pragma once

#include "Resource.h"

// contribution 对话框

class contribution : public CDialogEx
{
	DECLARE_DYNAMIC(contribution)

public:
	contribution(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~contribution();

// 对话框数据
	enum { IDD = IDD_DIALOG13 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
