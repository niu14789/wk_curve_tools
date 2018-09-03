#pragma once

#include "Resource.h"
#include "explorer1.h"

// map 对话框

class map : public CDialogEx
{
	DECLARE_DYNAMIC(map)

public:
	map(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~map();

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 m_web;
};
