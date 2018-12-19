#pragma once

#include "Resource.h"
#include "afxwin.h"
// C_PROCOTOL_CREATE 对话框
/* pri define */

typedef struct 
{
	unsigned int offset;
	unsigned int num;
	unsigned int fix[16];
}PRI_HEAD_DEF;

typedef struct 
{
	unsigned int offset;
	unsigned int width;
	unsigned int little;
	unsigned int math;
	unsigned int value;
}PLEN_MSG_DEF;

typedef struct 
{
	unsigned int type;
	unsigned int from;
	unsigned int to;
	unsigned int offset;
}CHECK_MSG_DEF;

typedef struct 
{
	unsigned int offset;
	unsigned int num;
	unsigned int fix[16];	
}PRI_TAIL_DEF;
/*-------------------------*/

typedef struct 
{
	char name[32];
	unsigned int ok;
	unsigned int time_mark;
	unsigned int procotol_type;
	unsigned int procotol_select;
	unsigned int block_size;
	unsigned int offset;
	unsigned int rev;
	unsigned int param_num;
	unsigned int sample_num;
	unsigned int math_num;
}CFS_GLOBAL_MSG_DEF;

typedef struct 
{
	char name[32];
	unsigned int head_fix_num;
    PRI_HEAD_DEF head_fix[16];
    PLEN_MSG_DEF PLEN_MSG;
    unsigned int data_offset;
    CHECK_MSG_DEF check_msg;
    unsigned int tail_fix_num;
    PRI_TAIL_DEF tail_fix[16];
}private_protocol_def;

typedef struct
{
	unsigned int cmd;// 1 is data , 2 is param , 3 is math ,0xff is not available
	unsigned int index;
	char name[32];
	unsigned int offset;
	unsigned int width;
	unsigned int type;//UINT8 UINT16 UINT32 FLOAT
	unsigned int little;//0 is little , 1 is big
	unsigned int if_num;
	//char if_table[12][32];
	unsigned short func_param[14];
	unsigned int line_type;
	unsigned int if_param_value[4];
	unsigned int math_type;//0 is none , 1 S2F , 2 is THIS , 0xff is creater
	unsigned int math_number;
	float f_math[10];
	char asmf_type[12];
	char buffer[236];//236 - 40 = 196
	unsigned int mark;
	char hot_key[16];
	/* some other struct */
	/* some other struct */
    /* some other struct */
	/* some other struct */
}COMBOX_PARAM_DATA_DEF;

typedef struct
{
	CFS_GLOBAL_MSG_DEF   cfs_global_msg;
	private_protocol_def private_protocol;
	COMBOX_PARAM_DATA_DEF pmd[512];//1mb
}CFG_FILE_DEF;

typedef struct
{
    unsigned int num;
	COMBOX_PARAM_DATA_DEF param_data[256];
}COMBOX_DEF;

class C_PROCOTOL_CREATE : public CDialogEx
{
	DECLARE_DYNAMIC(C_PROCOTOL_CREATE)

public:
	int C_PROCOTOL_CREATE::Get_hotkey(char * src , unsigned int src_len , unsigned int num,const char * cmd,char * param);
	int C_PROCOTOL_CREATE::Get_line_type(char * src , unsigned int src_len , unsigned int num,const char * cmd,unsigned int * param);
	int C_PROCOTOL_CREATE::Get_function(char * src , unsigned int src_len , unsigned int num ,	char * func,char * param1,char * param2,char * param3,char * param4,char * param5);
	int C_PROCOTOL_CREATE::Get_param_MARKS(char * src , unsigned int src_len , unsigned int num,const char * cmd,unsigned int * param);
	void C_PROCOTOL_CREATE::flush_cfs(unsigned int msg);
	char * C_PROCOTOL_CREATE::Get_math_type(char * src , unsigned int src_len , unsigned int num, unsigned int *param1);
	int C_PROCOTOL_CREATE::Get_check_if(char * src , unsigned int src_len , unsigned int num, unsigned int *param1,unsigned int *param2);
	int C_PROCOTOL_CREATE::Get_param_type(char * src , unsigned int src_len , unsigned int num,const char * cmd,unsigned int * param);
	int C_PROCOTOL_CREATE::Get_param_OLAY(char * src , unsigned int src_len , unsigned int num,const char * cmd,unsigned int * param);
	int C_PROCOTOL_CREATE::Get_param_OLAY(const char * src , unsigned int src_len , const char * cmd,unsigned int * param);
	int C_PROCOTOL_CREATE::gets_one_line(char *buffer,char *dst,unsigned int dst_len,unsigned int len);
	int C_PROCOTOL_CREATE::decode_data_math(unsigned int index);
	void C_PROCOTOL_CREATE::read_file_to_current(char *path);
	void C_PROCOTOL_CREATE::global_num_update(int param,int sample,int math);
	int C_PROCOTOL_CREATE::setting_data(int index,int mode);
	void  C_PROCOTOL_CREATE::combox_update(void);
	int C_PROCOTOL_CREATE::combox_data_add(int index , char * string ,int mode);
	int C_PROCOTOL_CREATE::msg_out(char * msg);
	int C_PROCOTOL_CREATE::get_dex_edit( int id,unsigned int * data , unsigned int len );
	int C_PROCOTOL_CREATE::assert_msg(unsigned int type , unsigned int data ,char * msg);
	int C_PROCOTOL_CREATE::get_string_edit( int id,void * data , unsigned int len );
	void C_PROCOTOL_CREATE::widget_initation(void);
	C_PROCOTOL_CREATE(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~C_PROCOTOL_CREATE();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combox_protocol_type;
	CEdit m_CFS_PATH;
	CComboBox m_combox_data;
	CComboBox m_combox_param;
	CEdit m_global_protocol_name;
	CComboBox m_global_select;
	CEdit m_block_size;
	CEdit m_width;
	CComboBox m_head_msg;
	CEdit m_param_num;
	CEdit m_sample_point;
	CEdit m_math_num;
	CEdit d_edit;
	CEdit p_name;
	CEdit p_block_size;
	CComboBox p_type;
	CEdit p_math;
	CEdit p_if;
	CEdit a_name;
	CEdit a_block_size;
	CComboBox a_type;
	CComboBox a_math_type;
	CEdit a_if;
	CEdit t_name;
	CEdit t_edit;
	CEdit t_test;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnEnChangeEdit9();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton24();
	afx_msg void OnBnClickedButton23();
	CComboBox m_combox_little;
	afx_msg void OnBnClickedButton3();
	CEdit m_timemark;
	afx_msg void OnCbnSelchangeCombo2();
};
