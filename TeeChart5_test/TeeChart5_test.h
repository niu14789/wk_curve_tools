
// TeeChart5_test.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTeeChart5_testApp:
// �йش����ʵ�֣������ TeeChart5_test.cpp
//

class CTeeChart5_testApp : public CWinApp
{
public:
	CTeeChart5_testApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTeeChart5_testApp theApp;