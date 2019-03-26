#pragma once

#include "Resource.h"
#include "afxcmn.h"
#include "afxwin.h"


#define __D_SERIES__   (0)
#define __V_SERIES__   (1)

// motor 对话框

class motor : public CDialogEx
{
	DECLARE_DYNAMIC(motor)

public:
	void motor::get_calibration_value(unsigned short * data,unsigned int len);
	int motor::get_dex_edit(int id, unsigned int * data);
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
	CComboBox m_test_item;
	CEdit m_edit_step;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton30();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton35();
	CComboBox m_rotation;
	CComboBox m_active;
	afx_msg void OnBnClickedButton37();
	afx_msg void OnBnClickedButton36();
	CButton m_min;
	afx_msg void OnBnClickedButton40();
	CButton m_osd;
	CButton m_max;
	afx_msg void OnBnClickedButton38();
	afx_msg void OnBnClickedButton39();
	afx_msg void OnBnClickedButton41();
	afx_msg void OnBnClickedButton11();
};
