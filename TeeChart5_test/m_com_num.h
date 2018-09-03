#pragma once
#include "afxwin.h"
#include "Resource.h"

typedef struct
{
   unsigned int com_det;
   char name[100];
}COM_DETAIL_DEF;
// m_com_num 对话框
typedef struct
{
  unsigned int com_num_using;
  unsigned int seq;
  unsigned int auto_link;
  unsigned int opened;
  unsigned int open_flag;
  unsigned int com_status;
  unsigned int radio_status;
  unsigned int reset_flags;
  unsigned int show_flags;
  unsigned int num_node;
  unsigned int num_shell;
  unsigned int now_num;
  unsigned int now_sw;
  unsigned int Available_com_num;
  COM_DETAIL_DEF com_detail[256];
}COM_MANAGE_DEF;

class m_com_num : public CDialogEx
{
	DECLARE_DYNAMIC(m_com_num)

public:
	void m_com_num::com_list_refresh(void);
	m_com_num(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~m_com_num();

// 对话框数据SetupDiGetDeviceRegistryProperty
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_baurate;
	CComboBox m_com;
	CButton m_open_button;
	CButton m_auto_link;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton25();
	afx_msg void OnBnClickedButton14();
	CButton m_single_borad;
	afx_msg void OnBnClickedCheck1();
};
