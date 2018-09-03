#pragma once

#include "Resource.h"
#include "afxwin.h"
// limit_setting 对话框

class limit_setting : public CDialogEx
{
	DECLARE_DYNAMIC(limit_setting)

public:
	int limit_setting::get2_float_edit( CEdit * edit , float * data ,float * data2);
	void limit_setting::mean_mode_enter(unsigned int mode);
	void limit_setting::set_check_type();
	void limit_setting::disable_mode_select(unsigned int mode);
	int limit_setting::get_float_edit( CEdit * edit , float * data );
	void limit_setting::disable_check_none0(unsigned int mode);
	void limit_setting::cfg_save_read(unsigned int mode);
	int limit_setting::search_chese_name(char * name);
	void limit_setting::disable_check_if(unsigned int mode);
	void limit_setting::click_one_line(unsigned int index);
	void limit_setting::init_oncreate(void);
	limit_setting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~limit_setting();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CComboBox m_limit_combox;
	CEdit m_limit_path;
	CButton m_check_if;
	CEdit m_limit_num;
	CEdit m_edit_l1;
	CEdit m_edit_l2;
	CEdit m_edit_l3;
	CEdit m_edit_l4;
	CEdit m_edit_l5;
	CEdit m_edit_l6;
	CEdit m_edit_l7;
	CEdit m_edit_l8;
	CEdit m_edit_c1;
	CEdit m_edit_c2;
	CEdit m_edit_c3;
	CEdit m_edit_c4;
	CEdit m_edit_c5;
	CEdit m_edit_c6;
	CEdit m_edit_c7;
	CEdit m_edit_c8;
	CEdit m_edit_u1;
	CEdit m_edit_u2;
	CEdit m_edit_u3;
	CEdit m_edit_u4;
	CEdit m_edit_u5;
	CEdit m_edit_u6;
	CEdit m_edit_u7;
	CEdit m_edit_u8;
	CStatic m_limit_static1;
	CStatic m_limit_static2;
	CStatic m_limit_static3;
	CStatic m_limit_static4;
	CStatic m_limit_static6;
	CStatic m_limit_static5;
	CStatic m_limit_static7;
	CStatic m_limit_static8;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	CStatic m_limit_static9;
	CStatic m_limit_static10;
	CStatic m_limit_static11;
	CStatic m_limit_static12;
	CStatic m_limit_static13;
	CStatic m_limit_static14;
	CStatic m_limit_static15;
	CStatic m_limit_static16;
	CEdit m_edit_l9;
	CEdit m_edit_l10;
	CEdit m_edit_l11;
	CEdit m_edit_l12;
	CEdit m_edit_l13;
	CEdit m_edit_l14;
	CEdit m_edit_l15;
	CEdit m_edit_l16;
	CEdit m_edit_c9;
	CEdit m_edit_c10;
	CEdit m_edit_c11;
	CEdit m_edit_c12;
	CEdit m_edit_c13;
	CEdit m_edit_c14;
	CEdit m_edit_c15;
	CEdit m_edit_c16;
	CEdit m_edit_u9;
	CEdit m_edit_u10;
	CEdit m_edit_u11;
	CEdit m_edit_u12;
	CEdit m_edit_u13;
	CEdit m_edit_u14;
	CEdit m_edit_u15;
	CEdit m_edit_u16;
	CEdit m_result;
	afx_msg void OnBnClickedCheck4();
	bool m_check_type;
	afx_msg void OnBnClickedButton4();
	CButton m_check_typeds;
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	CStatic m_dowm_text1;
	CStatic m_down_text;
	CStatic m_up_text1;
	CStatic m_up_text;
	afx_msg void OnStnClickedStatic1();
};
