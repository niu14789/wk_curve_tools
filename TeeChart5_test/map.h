#pragma once

#include "Resource.h"
#include "explorer1.h"
#include "afxwin.h"
#include "afxcmn.h"


typedef struct {
unsigned int addr_base;
unsigned int addr_camera;
}base_addr;

// map 对话框

class map : public CDialogEx
{
	DECLARE_DYNAMIC(map)

public:
	void map::aircraft_move_thread(void);
	void map::OnTimer(UINT_PTR nIDEvent) ;
	void map::show_home();
	void map::move_aircraft(float lon,float lat,int psi,int mode);
	void map::fucus_on_map(float lon,float lat);
	base_addr * map::compare(float * read,unsigned int len);
	unsigned int map::Auti_open(void);
	void map::spy_send_return_home(void);
	void map::spy_send_data_to_unity(float roll,float pitch,float yaw,float pos_x,float pox_y,float pos_z);
	int map::open_unity_thread(unsigned int addr);
	void map::init_map(void);
	map(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~map();

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 m_web;
	afx_msg void OnCbnSelchangeCombo3();
	CComboBox m_unity_select;
	CComboBox m_rate;
	CComboBox m_list;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	CButton m_start;
	afx_msg void OnBnClickedButton28();
	afx_msg void OnCbnSelchangeCombo2();
	CProgressCtrl m_process;
};
