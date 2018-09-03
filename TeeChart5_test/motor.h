#pragma once

#include "Resource.h"
#include "afxcmn.h"
#include "afxwin.h"

// motor 对话框

class motor : public CDialogEx
{
	DECLARE_DYNAMIC(motor)

public:
	void motor::show_motor_msg(unsigned char * data ,unsigned int len);
	void motor::show_factory(unsigned char * data ,unsigned int len);
	unsigned int motor::get_pwm_value(void);
	void motor::motor_init(void);
	motor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~motor();

// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_process_1r;
	CProgressCtrl m_process_1c;
	CProgressCtrl m_process_2r;
	CProgressCtrl m_process_2c;
	CProgressCtrl m_process_3r;
	CProgressCtrl m_process_3c;
	CProgressCtrl m_process_4r;
	CProgressCtrl m_process_4c;
	afx_msg void OnBnClickedButton1();
	CSliderCtrl m_pwm;
	CEdit m_time;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_edit_pwm;
	CSliderCtrl m_silder_pwm;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton28();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton17();
};
