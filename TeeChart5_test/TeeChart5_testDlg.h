
// TeeChart5_testDlg.h : 头文件
//

#pragma once
#include "tchart1.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "mscomm1.h"
/*-------------------*/
/* callback type */
#define _CB_IT_               (0x80000000)
#define _CB_TIMER_            (0x70000000)
#define _CB_EXE_              (0x60000000)
#define _CB_VAR_              (0x50000000)
#define _CB_ARRAY_            (0x40000000)
#define _CB_IDLE_             (0x30000000)
#define _CB_RT_               (0x20000000)
/* imr develop */
typedef struct{
  double time;
	long gx;
	long gy;
	long gz;
	long ax;
	long ay;
	long az;
}imr_def;
/*------------*/
typedef struct{
	double time;
	double vgx;
	double vgy;
	double vgz;
	double lat;
	double lon;
	double height;
	double time_sigma;
}gsof_show_def;

typedef struct {
	unsigned int total_size;
	unsigned int ok_package;
	unsigned int none_data;
	float per;
}gps_tes_def;

typedef struct
{
	char name[128];
	unsigned int point_num;
	unsigned int offset;
	unsigned int status;//draw status
	unsigned int line_num;
	unsigned int color;
	unsigned int time_ms;
	unsigned int scale;
	unsigned char * data;
}PARAM_LIST_SIGLE_DEF;

typedef struct
{
	unsigned int param_list_num;
	unsigned int now_num;
	PARAM_LIST_SIGLE_DEF param_list[512];
}PARAM_LIST_DEF;
/*-------------------------------*/
typedef struct
{
	unsigned int index;
	unsigned int data_num;
	unsigned int data_type;
	char title[32];
	unsigned char raw_data[32];
}SYS_LIST_DETAIL;
typedef struct
{
	unsigned int list_num;
	SYS_LIST_DETAIL list[256];
}SYS_LIST_DEF;

typedef struct
{
	unsigned int  index;
	unsigned int  check_if;
	unsigned int  check_type;
	unsigned int  type;
	unsigned int  num;
	unsigned int check_ok;
	unsigned char limit_high[64];
	unsigned char limit_low[64];
	unsigned char limit_mean[64];
	unsigned char limit_mean_error[64]; 
	unsigned char limit_rms[64];
	unsigned int mean_now[16];
	unsigned int rms_now[16];
	unsigned int  mean_rms_num;
	unsigned char data_mean_rms[16][40];
	char result[32];//result
	char title[32];
	char title_chese[64];
	char show_data[512];
	unsigned char raw_data[32];
}SYS_LIST_CHESE_DETAIL_DEF;
/*------*/
typedef struct
{
	unsigned int total_line;
	SYS_LIST_CHESE_DETAIL_DEF chese[64];
}SYS_LIST_CHESE_DEF;

typedef struct{
	unsigned short line_num;
	unsigned short i_flags;//status
	char name[24];//4*6
	int type;//node or shell
	int size;/* function or param or others and param numbers and returns or not */
	unsigned int size_pl;
	unsigned char data[200];
}node_def;

typedef struct{
	unsigned int index_raw;
	unsigned int param_num;
	unsigned int param_type;
	unsigned int line_data_start;
	unsigned int line_ok[32];
	unsigned int line_available_point[32];
	char name[32][32];
	unsigned int line_source[32];
	double line_data[32][1000];
}COMBOX_PARAM_STATIC_DEF;

typedef struct{
	unsigned int combox_item_num;
	unsigned int com_line_num;
	COMBOX_PARAM_STATIC_DEF rt_line[128];
}COMBOX_PARAM_RT_DEF;

typedef struct{
	unsigned short cmd;
	unsigned short line_num;
	unsigned char line_gid[20][2];
}LINE_CONFIG_DEF;

typedef struct{
	unsigned int li;
	unsigned int lj;
	unsigned int lin_srv;
}LINE_MULIT_DEF;

typedef struct{
	//unsigned int version_identify;
	//unsigned int auto_connect_radio;
	//unsigned int check_lish_show;
	unsigned int inf[256];
}SYSTEM_CONFIG_INF_DEF;

typedef struct
{
    char title[64];
	unsigned int index;
	unsigned int opened;
	unsigned int auto_scale;
	unsigned int enable;
	double min;
	double max;
}SYSTEM_LINE_CFS;

typedef struct
{
	unsigned int global_auto;
	unsigned int mutiple_axis;
	SYSTEM_LINE_CFS line_cfg[20];
}SYSTEM_AUTO_SCALE_DEF;

// CTeeChart5_testDlg 对话框
class CTeeChart5_testDlg : public CDialogEx
{
// 构造
public:
	void CTeeChart5_testDlg::create_color_table(void);
	unsigned int CTeeChart5_testDlg::get_color(unsigned int mode);
	void CTeeChart5_testDlg::axis_reset(void);
	void CTeeChart5_testDlg::axis_color(unsigned int num,unsigned int color,unsigned int mode);
	void CTeeChart5_testDlg::draw_axis(unsigned int num,void * line,CString * title , unsigned int color,unsigned int);
	int CTeeChart5_testDlg::pos_read_one_line(char * buffer,unsigned int len,char * dst,unsigned int dst_len);
	int CTeeChart5_testDlg::fpos_process(char *path,char *path_tmp,unsigned int mode);
	void * CTeeChart5_testDlg::Get_star_source(unsigned int *num);
	void CTeeChart5_testDlg::standart_diviaton(unsigned int mode);
	void CTeeChart5_testDlg::reflush_chart(void);
	void CTeeChart5_testDlg::Position_point_lane(unsigned int mode);
	void CTeeChart5_testDlg::Position_axis_bin(unsigned int mode,unsigned int p_or_l);
	void * CTeeChart5_testDlg::Get_2axis_source(unsigned int *num);
	void * CTeeChart5_testDlg::Get_point_source(unsigned int *num);
	void CTeeChart5_testDlg::m3d_display(void);
	BOOL CTeeChart5_testDlg::PreTranslateMessage(MSG* pMsg);
	void CTeeChart5_testDlg::take_a_photo(void);
	void CTeeChart5_testDlg::reset(void);
	void CTeeChart5_testDlg::pic_feedback_show(void);
	void CTeeChart5_testDlg::pic_num_show(void);
	void CTeeChart5_testDlg::take_pic_btn_event(unsigned int otk);
	void CTeeChart5_testDlg::disable_all_timer(void);
	void CTeeChart5_testDlg::set_test_mode(unsigned int flag);
	unsigned int CTeeChart5_testDlg::system_config_msg(unsigned int offset);
	void CTeeChart5_testDlg::check_list_show(unsigned char flags);
	void CTeeChart5_testDlg::system_config_fresh(unsigned int offset , unsigned int data);
	void CTeeChart5_testDlg::Arm_Distortion(char * path,unsigned int if_check);
	void CTeeChart5_testDlg::MeanCal(float * a , unsigned char b , float *m , float *r );
	int CTeeChart5_testDlg::Get_line10_source(unsigned int *num);
	void CTeeChart5_testDlg::clear_all_exist_lines(unsigned int mode);
	void CTeeChart5_testDlg::Draw_all_lines(void);
	void CTeeChart5_testDlg::Draw_simple_line(double * line_table , unsigned int len,int i,int j);
	void CTeeChart5_testDlg::Force_Q_wave(unsigned int line_g , unsigned int index , unsigned char * data );
	void CTeeChart5_testDlg::Force_cache_wave( unsigned char * line_raw , unsigned int len );
	void CTeeChart5_testDlg::Set_line_output_thread(void);
	int CTeeChart5_testDlg::Get_line_group_index(unsigned int seq);
	void CTeeChart5_testDlg::push_data_to_dram_lines( node_def * node ,unsigned int mode );
	void CTeeChart5_testDlg::move_test(void);
	void CTeeChart5_testDlg::Save_limit_file(void);
	void CTeeChart5_testDlg::Read_limit_file(void);
	void CTeeChart5_testDlg::Set_sempher(unsigned char mode);
	void CTeeChart5_testDlg::refresh_result_all(void);
	void CTeeChart5_testDlg::Check_list_one(SYS_LIST_CHESE_DETAIL_DEF * list);
	int CTeeChart5_testDlg::update_check_result( SYS_LIST_CHESE_DETAIL_DEF * check_ese,unsigned int mode);
	int CTeeChart5_testDlg::list_transfer(SYS_LIST_DETAIL * list);
	void CTeeChart5_testDlg::List_refresh_line(SYS_LIST_DETAIL * list);
	void CTeeChart5_testDlg::List_refresh_line(SYS_LIST_DEF * list);
	void CTeeChart5_testDlg::List_delete_all(void);
	void CTeeChart5_testDlg::List_refresh(unsigned int mode);
	void CTeeChart5_testDlg::set_unit_num(unsigned int now_sw,unsigned int now_num);
	void CTeeChart5_testDlg::unit_data_thread(void);
	int CTeeChart5_testDlg::fm_data_decode(unsigned char ID , unsigned char *data,unsigned int len_p);
	int CTeeChart5_testDlg::link_autopilot(unsigned char * data,unsigned len);
	int CTeeChart5_testDlg::gs_d_icr(unsigned char c);
	void CTeeChart5_testDlg::link_autopilot_thread(void);
	int CTeeChart5_testDlg::fm_link_send(unsigned char ID , unsigned char *data,unsigned int len_p);
	int CTeeChart5_testDlg::fmlink_package_create(unsigned char * buffer,unsigned int buffer_len,unsigned char ID,unsigned char * payload,unsigned int p_len);
	unsigned short CTeeChart5_testDlg::crc16_accumulate_buffer(unsigned short buffer_length,unsigned char* data_buffer);
	unsigned short CTeeChart5_testDlg::crc16_accumulate(unsigned char data,unsigned short crc_data);
	unsigned short CTeeChart5_testDlg::holder_check_crc(unsigned short crc_origin_one,unsigned char *data,int len);
	int CTeeChart5_testDlg::radio_sms(unsigned char data);
	int CTeeChart5_testDlg::check_radio(unsigned char * data,unsigned int len);
	void CTeeChart5_testDlg::fm_link_handle(unsigned char * data,unsigned int len);
//	int CTeeChart5_testDlg::create_thread(void);
//	UINT CTeeChart5_testDlg::ThreadProc(LPVOID pParam);
	int CTeeChart5_testDlg::send(unsigned char * data,unsigned int len);
	int CTeeChart5_testDlg::open_mscomm(unsigned int num);
	void CTeeChart5_testDlg::radio_link(void);
	void CTeeChart5_testDlg::reflesh_global_status(unsigned int mode);
	void CTeeChart5_testDlg::OnTimer(UINT_PTR nIDEvent);
	int CTeeChart5_testDlg::Get_COM_STATUS(unsigned int mode);
	void CTeeChart5_testDlg::com_msg_clear(void);
	void CTeeChart5_testDlg::transfer_com_msg(TCHAR * , TCHAR *);
	void CTeeChart5_testDlg::Refresh_com_list(void);
	BOOL CTeeChart5_testDlg::OnDeviceChange(UINT nEventType,DWORD dwData)  ;
	void CTeeChart5_testDlg::Legend_handle( unsigned int mode );
	int CTeeChart5_testDlg::allocate_file_area( char * path , unsigned int len);
	void CTeeChart5_testDlg::clear_all_line(unsigned int mode);
	void CTeeChart5_testDlg::chart_line_init(void);
	void CTeeChart5_testDlg::draw_single(unsigned int num,unsigned int mode);
	void CTeeChart5_testDlg::Line_flash(void);
	void CTeeChart5_testDlg::release_current_line(unsigned int num);
	void * CTeeChart5_testDlg::Get_line_source(unsigned int *mode);
	double CTeeChart5_testDlg::Short2Float(double a, double b);
    int CTeeChart5_testDlg::math_transfer(unsigned char * base,unsigned char * buffer,unsigned int index ,double * value);
	int CTeeChart5_testDlg::math_and_line(unsigned char *data,unsigned int len,unsigned int start);
	void CTeeChart5_testDlg::Read_Procotol_decode_waves(unsigned int index);
	CTeeChart5_testDlg(CWnd* pParent = NULL);	// 标准构造函数
	void CTeeChart5_testDlg::page_show(unsigned int now,unsigned total);
	void CTeeChart5_testDlg::show_line(int * line_availd ,unsigned int start , unsigned int end , unsigned int mode );
	void CTeeChart5_testDlg::On_show_single(unsigned int line,unsigned int mode);
	void CTeeChart5_testDlg::ChangeSize(CWnd *pWnd, int cx, int cy,unsigned int mode);
    gps_tes_def * CTeeChart5_testDlg::gsof_transfer( gsof_show_def * show , unsigned char * buffer , unsigned int len );
	void CTeeChart5_testDlg::big2little_len(unsigned char * src,unsigned char * dst,unsigned char len);
	void CTeeChart5_testDlg::gps_raw_detail(gsof_show_def * show,unsigned char type,unsigned char *data,unsigned char len );
    void CTeeChart5_testDlg::raw_gps_decode(gsof_show_def * show , unsigned char d);
	void CTeeChart5_testDlg::clear_cnt(void);
// 对话框数据
	enum { IDD = IDD_TEECHART5_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTchart1 m_chart;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CButton m_ana;
	CButton m_open;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void On32771();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CListCtrl m_list_ctrl;
	afx_msg void On32781();
	CButton m_show_test;
	CComboBox m_combox_param_show;
	CButton m_check_hold;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton5();
	afx_msg void On32777();
	CMscomm1 m_mscomm;
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	CComboBox m_com_reg;
	CStatic m_taps;
	afx_msg void OnDestroy();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton28();
	afx_msg void OnBnClickedButton123();
	afx_msg void OnBnClickedButton26();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton27();
	afx_msg void OnBnClickedButton29();
	afx_msg void OnBnClickedButton30();
	CButton m_manul_test;
	afx_msg void On32786();
	CComboBox m_pic_cnt;
	CButton m_btn_pic;
	CEdit m_edit_pic;
	CButton m_check_camera;
};
