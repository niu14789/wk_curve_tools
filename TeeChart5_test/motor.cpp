// motor.cpp : 实现文件
//

#include "stdafx.h"
#include "motor.h"
#include "afxdialogex.h"
#include "TeeChart5_testDlg.h"
#include "stdafx.h"
#include "TeeChart5_test.h"
#include "TeeChart5_testDlg.h"
// motor 对话框
extern CWnd* m_pMainWnd_PUBLIC;
CTeeChart5_testDlg * ct;
extern SYS_LIST_CHESE_DEF chese_list;
/*------------------------------*/
unsigned char factorty_ok = 0;
/*------------------------------*/
unsigned char motor_check_flags = 0xff;
/*------------------------------*/
unsigned char D_or_V = __D_SERIES__;
unsigned char D_V_flag = 0;
/* ID and cmd */

/* dir */

#define TM_CMD_HEAD1			0xA0
#define TM_CMD_HEAD2			0xE0
#define TM_CMD_TAIL				0x0C

#define CAN_R1_ID       (0x60<<2)
#define CAN_R2_ID       (0x70<<2)
#define CAN_R3_ID       (0x90<<2)

#define NORMAL       (0x0<<4)

#define TM_TO_FC_DIR	0x01
#define FC_TO_TM_DIR	0x02
/* can id */
#define FC_TO_TM1_CAN_ID		(CAN_R1_ID | NORMAL | FC_TO_TM_DIR)
#define FC_TO_TM2_CAN_ID		(CAN_R2_ID | NORMAL | FC_TO_TM_DIR)
#define FC_TO_TM3_CAN_ID		(CAN_R3_ID | NORMAL | FC_TO_TM_DIR)

static int can_id[3] = {FC_TO_TM1_CAN_ID,FC_TO_TM2_CAN_ID,FC_TO_TM3_CAN_ID};

static int can_cmd[8] = {0xE1,0xCC,0xE2,0xE3,0x00,0x01,0xE9,0xEA};
/*------------------------------*/
IMPLEMENT_DYNAMIC(motor, CDialogEx)

motor::motor(CWnd* pParent /*=NULL*/)
	: CDialogEx(motor::IDD, pParent)
{

}

motor::~motor()
{
}

void motor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_process_1r);
	DDX_Control(pDX, IDC_PROGRESS2, m_process_1c);
	DDX_Control(pDX, IDC_PROGRESS3, m_process_2r);
	DDX_Control(pDX, IDC_PROGRESS4, m_process_2c);
	DDX_Control(pDX, IDC_PROGRESS5, m_process_3r);
	DDX_Control(pDX, IDC_PROGRESS6, m_process_3c);
	DDX_Control(pDX, IDC_PROGRESS7, m_process_4r);
	DDX_Control(pDX, IDC_PROGRESS8, m_process_4c);
	/*---------------------*/
	DDX_Control(pDX, IDC_EDIT1, m_time);
	/*------------------------*/
	DDX_Control(pDX, IDC_EDIT2, m_edit_pwm);
	/*----------------------*/
	DDX_Control(pDX, IDC_COMBO1, m_test_item);
	DDX_Control(pDX, IDC_EDIT3, m_edit_step);

	DDX_Control(pDX, IDC_COMBO2, m_rotation);
	DDX_Control(pDX, IDC_COMBO13, m_active);

	DDX_Control(pDX, IDC_BUTTON38, m_min);
	DDX_Control(pDX, IDC_BUTTON40, m_osd);
	DDX_Control(pDX, IDC_BUTTON39, m_max);

	motor_init();
}


BEGIN_MESSAGE_MAP(motor, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &motor::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &motor::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &motor::OnBnClickedButton3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &motor::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDC_BUTTON4, &motor::OnBnClickedButton4)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON5, &motor::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON28, &motor::OnBnClickedButton28)
	ON_BN_CLICKED(IDC_BUTTON13, &motor::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON17, &motor::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON6, &motor::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &motor::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON22, &motor::OnBnClickedButton22)
	ON_BN_CLICKED(IDC_BUTTON30, &motor::OnBnClickedButton30)
	ON_BN_CLICKED(IDC_BUTTON9, &motor::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON8, &motor::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON35, &motor::OnBnClickedButton35)
	ON_BN_CLICKED(IDC_BUTTON37, &motor::OnBnClickedButton37)
	ON_BN_CLICKED(IDC_BUTTON36, &motor::OnBnClickedButton36)
	ON_BN_CLICKED(IDC_BUTTON40, &motor::OnBnClickedButton40)
	ON_BN_CLICKED(IDC_BUTTON38, &motor::OnBnClickedButton38)
	ON_BN_CLICKED(IDC_BUTTON39, &motor::OnBnClickedButton39)
END_MESSAGE_MAP()


// motor 消息处理程序

void motor::motor_init(void)
{
	m_process_1r.SetRange(0,6000);
	m_process_1c.SetRange(0,30000);
	m_process_2r.SetRange(0,6000);
	m_process_2c.SetRange(0,30000);
	m_process_3r.SetRange(0,6000);
	m_process_3c.SetRange(0,30000);
	m_process_4r.SetRange(0,6000);
	m_process_4c.SetRange(0,30000);
    /*-----------------------------*/
	SetDlgItemText(IDC_STATIC1,_T(""));
	SetDlgItemText(IDC_STATIC11,_T(""));
	SetDlgItemText(IDC_STATIC2,_T(""));
	SetDlgItemText(IDC_STATIC22,_T(""));
	SetDlgItemText(IDC_STATIC3,_T(""));
	SetDlgItemText(IDC_STATIC33,_T(""));
	SetDlgItemText(IDC_STATIC4,_T(""));
	SetDlgItemText(IDC_STATIC44,_T(""));
	/*-----------------------------*/
    ct = (CTeeChart5_testDlg *)m_pMainWnd_PUBLIC;
	/*-----------------------------*/
	m_time.SetWindowTextW(_T("5"));
	/*---------------*/
	m_edit_pwm.SetWindowTextW(_T("1000"));
	/* disable the windows */
	ct->set_test_mode(0);
	/*----reset-----*/
	OnBnClickedButton4();
	OnBnClickedButton4();
	/*-----------------------*/
	m_test_item.ResetContent();
	/* add data */
	m_test_item.AddString(_T("尾倾转"));
	m_test_item.AddString(_T("左倾转"));
	m_test_item.AddString(_T("右倾转"));
	m_test_item.AddString(_T("左副翼"));
	m_test_item.AddString(_T("右副翼"));
	m_test_item.AddString(_T("左V尾"));
	m_test_item.AddString(_T("右V尾"));

	m_test_item.SetCurSel(0);
	/*---------*/
	m_rotation.ResetContent();
	/* add data */
	m_rotation.AddString(_T("左倾转"));
	m_rotation.AddString(_T("右倾转"));
	m_rotation.AddString(_T("尾倾转"));

	m_rotation.SetCurSel(0);
	/*---------*/
	m_active.ResetContent();
	/* add data */
	m_active.AddString(_T("卸力"));//0xE1
	m_active.AddString(_T("进入"));//0xCC
	m_active.AddString(_T("10度3HZ"));//0xE2
	m_active.AddString(_T("0-90度1HZ"));//0xE3
	m_active.AddString(_T("转至水平"));//0x00
	m_active.AddString(_T("转至垂直"));//0x01
	m_active.AddString(_T("0-90慢转"));//0xE9
	m_active.AddString(_T("90-0慢转"));//0xEA

	m_active.SetCurSel(0);
	/* set step */
	m_edit_step.SetWindowTextW(_T("50"));
	/* disable the btn */
	//m_min.EnableWindow(0);
	m_osd.EnableWindow(0);
	//m_max.EnableWindow(0);
}

int motor::get_dex_edit(int id, unsigned int * data)
{
	wchar_t get_cs[512];
	char c_string[256];
	int len_t;

	len_t = GetDlgItem(id)->GetWindowTextW(get_cs, 512);

	memset(c_string, 0, sizeof(c_string));

	for (int i = 0; i < len_t; i++)
	{
		c_string[i] = (char)get_cs[i];
	}
	/*----------------------*/
	int ret = sscanf_s(c_string, "%d", data);

	if (ret == 1)
	{
		return 0;
	}
	else
	{
		return (-1);
	}
}

void motor::OnBnClickedButton1()
{
// TODO: 在此添加控件通知处理程序代码
	/*----------*/
    int time;
	static char get_cs[100],c_string[50];

	static unsigned char flags_tips = 0;

	USES_CONVERSION;
			/* offset */
	m_time.GetWindowTextW((LPTSTR)get_cs,100);

	for(int i=0;i<100;i+=2)
	{
		c_string[i/2] = get_cs[i];
	}  
	/*--------------------------*/
	if( sscanf_s(c_string,"%d",&time) != 1 )
	{
		MessageBox(_T("输入电机测试时间"),_T("tips"),0);
		return;
	}
	/*-*/
	// TODO: 在此添加控件通知处理程序代码
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 251;
	param[0] = 1;
	param[1] = 1000.0f;
	param[2] = (float)time;
	/*-------------------------*/   
	/*-------------------------*/   
	if( flags_tips || MessageBox(_T("进入电机测试模式之后，电机会转动，请确认环境是否安全！") , _T("tips") , 1 ) == 1 )
	{
		if( flags_tips || MessageBox(_T("再次确认是否进入电机测试模式"),_T("tips"),1) == 1 )
		{
			if( ct->fm_link_send( 76 , package , 33 ) == (-1) )
			{
				MessageBox(_T("电台串口未打开"),_T("tips"),0);
				return;
			}
			/*--------------*/
			m_edit_pwm.SetWindowTextW(_T("1000"));
			/*--------------*/
			flags_tips = 1;
		}
	}
}
/*---------------------*/
void motor::OnBnClickedButton2()
{
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 252;
	param[0] = 2;
	/*-------------------------*/   
	if( ct->fm_link_send(76,package,33) == (-1) )
	{
		MessageBox(_T("电台串口未打开"),_T("tips"),0);
		return;
	}
}
/*----------------------------*/
void motor::OnBnClickedButton3()
{
	//unsigned int pos = m_silder_pwm.GetPos();
    int pos;

	char get_cs[100],c_string[50];

	memset(get_cs,0,sizeof(get_cs));
	memset(c_string,0,sizeof(c_string));

	USES_CONVERSION;
			/* offset */
	m_edit_pwm.GetWindowTextW((LPTSTR)get_cs,100);

	for(int i=0;i<100;i+=2)
	{
		c_string[i/2] = get_cs[i];
	}  
	/*--------------------------*/
	if( sscanf_s(c_string,"%d",&pos) != 1 )
	{
		MessageBox(_T("请输入电机PWM值"),_T("tips"),0);
		return;
	}
	/* send */
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 253;
	param[0] = 3;
	param[1] = (float)pos;
	/*-------------------------*/   
	if( ct->fm_link_send(76,package,33) == (-1) )
	{
		MessageBox(_T("电台串口未打开"),_T("tips"),0);
		return;
	}
}


void motor::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

unsigned int motor::get_pwm_value(void)
{
		/*----------*/
    unsigned int time;
	static char get_cs[100],c_string[50];

	USES_CONVERSION;
			/* offset */
	m_edit_pwm.GetWindowTextW((LPTSTR)get_cs,100);

	for(int i=0;i<100;i+=2)
	{
		c_string[i/2] = get_cs[i];
	}  
	/*--------------------------*/
	if( sscanf_s(c_string,"%d",&time) != 1 )
	{
		MessageBox(_T("输入油门大小"),_T("tips"),0);
		return (-1);
	}
	/*----------*/
	if( time < 1000 || time > 2000 )
	{
		MessageBox(_T("油门大小异常"),_T("tips"),0);
		return (-1);
	}
	/*---------------*/
	return time;
}
/*--------------------------*/
void motor::show_factory(unsigned char * data ,unsigned int len)
{
    unsigned short * facT = (unsigned short *)data;
	/*------------------------------------------*/
	USES_CONVERSION;
	/*------------------------------------------*/
	unsigned short fac_tub;//
	/* set */
	if( (*facT) & 0x8000 )
	{
		//V
		D_or_V = __V_SERIES__;
	}else
	{
		D_or_V = __D_SERIES__;
	}
	/*------------------------------------------*/
	fac_tub = (*facT) & 0x7fff;
	/*------------------------------------------*/
	if( fac_tub == 251 )
	{
		factorty_ok = 1;
		if( data[2] == 0 )
		{
		    MessageBox(_T("烤机模式进入成功"),_T("tips"),0);
		}else
		{
			MessageBox(_T("烤机模式进入失败，已经在烤机模式中，或烤机已经结束，若想重新进入，请复位后再次点击"),_T("tips"),0);
		}
	}else if( fac_tub == 252 )
	{
		factorty_ok = 0;
		/*if( data[2] == 0 )
		{
		    MessageBox(_T("烤机模式退出成功"),_T("tips"),0);
		}else
		{
			MessageBox(_T("烤机模式退出失败"),_T("tips"),0);
		}*/
	}else if( fac_tub == 259 )
	{
		if( data[2] == 0 )
		{
		    //MessageBox(_T("油门下发成功"),_T("tips"),0);
		}else
		{
			MessageBox(_T("舵面控制量下发失败"),_T("tips"),0);
		}
	}else if( fac_tub == 260 )
	{
		if( data[2] == 0 )
		{
		    MessageBox(_T("写入校准值成功"),_T("tips"),0);
		}else
		{
			MessageBox(_T("写入校准值失败"),_T("tips"),0);
		}
	}else if( fac_tub == 261 )
	{
		if( data[2] == 0 )
		{
		    //MessageBox(_T("倾转校准下发成功"),_T("tips"),0);
		}else
		{
			MessageBox(_T("倾转校准下发失败"),_T("tips"),0);
		}
	}
	else if( fac_tub == 253 )
	{
		if( data[2] == 0 )
		{
		    //MessageBox(_T("油门下发成功"),_T("tips"),0);
		}else
		{
			MessageBox(_T("油门下发失败"),_T("tips"),0);
		}
	}
	else if( fac_tub == 254 )
	{
		if( data[2] == 0xff )
		{
		    MessageBox(_T("电机标定指令下发成功"),_T("tips"),0);
		}else
		{
			if(data[2] == 1)
			{
				MessageBox(_T("电机标定失败"),_T("tips"),0);
			}else
			{
				MessageBox(_T("电机标定成功"),_T("tips"),0);
			}
		}
	}else if( fac_tub == 255 )
	{
		if( data[2] == 0xff )
		{
		    MessageBox(_T("磁力计校准下发成功"),_T("tips"),0);
		}else
		{
			/*
			0： 校准成功
			1：校准超时
			2：飞机倾斜角度过大
			3：环境磁场干扰大
			4：读取磁力计参数不成功
			5：磁力计数据异常
			6：磁场偏差大
			7：航向转动过快
			8:俯仰转动过快
			9：滚转转动过快
			10：写入磁力计参数不成功
			15：正在校准中...			
			*/
			if(data[2] == 0)
			{
				MessageBox(_T("磁力计校准成功"),_T("tips"),0);
			}
			else if( data[2] == 1)
			{
				MessageBox(_T("校准超时"),_T("tips"),0);
			}
			else if( data[2] == 2)
			{
				MessageBox(_T("飞机倾斜角度过大"),_T("tips"),0);
			}
			else if( data[2] == 3)
			{
				MessageBox(_T("环境磁场干扰大"),_T("tips"),0);
			}
			else if( data[2] == 4)
			{
				MessageBox(_T("读取磁力计参数不成功"),_T("tips"),0);
			}
			else if( data[2] == 5)
			{
				MessageBox(_T("磁力计数据异常"),_T("tips"),0);
			}
			else if( data[2] == 6)
			{
				MessageBox(_T("磁场偏差大"),_T("tips"),0);
			}
			else if( data[2] == 7)
			{
				MessageBox(_T("航向转动过快"),_T("tips"),0);
			}
			else if( data[2] == 8)
			{
				MessageBox(_T("俯仰转动过快"),_T("tips"),0);
			}
			else if( data[2] == 9)
			{
				MessageBox(_T("滚转转动过快"),_T("tips"),0);
			}else if( data[2] == 10)
			{
				MessageBox(_T("写入磁力计参数不成功"),_T("tips"),0);
			}
			else if( data[2] == 15)
			{
				MessageBox(_T("正在校准中..."),_T("tips"),0);
			}
			else
			{

			}

		}
	}else if( fac_tub == 256 )
	{
		if( data[2] == 0xff )
		{
		    MessageBox(_T("水平标定下发成功"),_T("tips"),0);
		}else
		{
			if( data[2] == 1 )
			{
				MessageBox(_T("水平标定失败"),_T("tips"),0);
			}else
			{
				MessageBox(_T("水平标定成功"),_T("tips"),0);
			}
		}
	}else if( fac_tub == 257 )
	{
		if( data[2] == 0xff )
		{
		    MessageBox(_T("SNR标定下发成功"),_T("tips"),0);
		}else if( data[2] == 0xfe )
		{
			MessageBox(_T("已经在SNR标定模式或标定模式已退出，若想重新进入，请复位"),_T("tips"),0);
		}
		else
		{
			if( data[2] == 1 )
			{
				MessageBox(_T("SNR标定失败"),_T("tips"),0);
			}else
			{
				MessageBox(_T("SNR标定成功"),_T("tips"),0);
			}
		}
	}else if( fac_tub == 258 )
	{
		if( data[2] == 0xff )
		{
		    MessageBox(_T("双目高度标定下发成功"),_T("tips"),0);
		}else if( data[2] == 0xfe )
		{
			MessageBox(_T("已经在双目高度标定模式或标定模式已退出，若想重新进入，请复位"),_T("tips"),0);
		}
		else
		{
			if( data[2] == 1 )
			{
				MessageBox(_T("双目高度标定失败"),_T("tips"),0);
			}else
			{
				MessageBox(_T("双目高度标定成功"),_T("tips"),0);
			}
		}
	}
}

void motor::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	/*---------------*/
	*pd = 246;
	/*-------------------------*/
	if( ct->fm_link_send(76,package,33) == (-1) )
	{
		MessageBox(_T("电台串口未打开"),_T("tips"),0);
		return;
	}
}
void motor::show_motor_msg(unsigned char * data ,unsigned int len)
{
    unsigned short *rmp  = ( unsigned short *)data;
	CString show;
	char buffer[10];
	USES_CONVERSION;

	factorty_ok = 1;

	/*1*/
	sprintf_s(buffer,"%d",rmp[0]);

	show = A2T(buffer);

	SetDlgItemText(IDC_STATIC1,show);

	m_process_1r.SetPos(rmp[0]);
	/*-------------------------------------------------------------*/
	sprintf_s(buffer,"%2.3fA",(float)rmp[1] / 1000 );

	show = A2T(buffer);

	SetDlgItemText(IDC_STATIC11,show);

	m_process_1c.SetPos(rmp[1]);

	/*2*/
	sprintf_s(buffer,"%d",rmp[2]);

	show = A2T(buffer);

	SetDlgItemText(IDC_STATIC2,show);

	m_process_2r.SetPos(rmp[2]);
	/*------------------------------------------------------------*/
	sprintf_s(buffer,"%2.3f A",(float)rmp[3] / 1000 );

	show = A2T(buffer);

	SetDlgItemText(IDC_STATIC22,show);

	m_process_2c.SetPos(rmp[3]);
	/*3*/
	sprintf_s(buffer,"%d",rmp[4]);

	show = A2T(buffer);

	SetDlgItemText(IDC_STATIC3,show);

	m_process_3r.SetPos(rmp[4]);
	/*----------------------------------------------------------*/
	sprintf_s(buffer,"%2.3f A",(float)rmp[5] / 1000 );

	show = A2T(buffer);

	SetDlgItemText(IDC_STATIC33,show);

	m_process_3c.SetPos(rmp[5]);
	/*4*/
	sprintf_s(buffer,"%d",rmp[6]);

	show = A2T(buffer);

	SetDlgItemText(IDC_STATIC4,show);

	m_process_4r.SetPos(rmp[6]);
	/*--------------------------------------------------------*/
	sprintf_s(buffer,"%2.3f A",(float)rmp[7] / 1000 );

	show = A2T(buffer);

	SetDlgItemText(IDC_STATIC44,show);

	m_process_4c.SetPos(rmp[7]);
}

void motor::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if( factorty_ok == 0 )
	{

	}else
	{
		OnBnClickedButton2();
	    OnBnClickedButton2();
		///*------------*/
		//MessageBox(_T("正在退出烤机模式，请稍等..."),_T("tips"),0);
		/*----------------*/
		for( int i = 0 ; i < 5 ; i ++ )
		{
			if( factorty_ok == 0 )
			{
				return;
			}
			/* delay 500 ms */
			Sleep(500);
			/*-------------*/
			OnBnClickedButton2();
		}
	}

}
/*-------------------------------------*/
void motor::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	if( MessageBox(_T("请确认转向、转速及电流正常") , _T("tips") , 1 ) == 1 )
	{
		//for( unsigned int i = 0 ; i < chese_list.total_line ; i ++ )
	 //   {
		//	if( strstr( chese_list.chese[i].title , "g_mrs") != NULL )
		//	{
		//		chese_list.chese[i].check_type = 4;//check and ok
		//		MessageBox(_T("设置成功") , _T("tips") , 0 ); 
		//		/*------------------------------*/
		//		ct->refresh_result_all();
		//		/*------------------------------*/
		//		return;
		//	}
		//}
		motor_check_flags = 4;
	}
}
/*-------------------------------*/
void motor::OnBnClickedButton28()
{
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 255;
	param[0] = 5;
	/*-------------------------*/   
	ct->fm_link_send(76,package,33);
}

void motor::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 257;
	param[0] = 5;
	/*-------------------------*/   
	ct->fm_link_send(76,package,33);
}

void motor::OnBnClickedButton17()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 258;
	param[0] = 5;
	/*-------------------------*/   
	ct->fm_link_send(76,package,33);
}
/* static servo num */
static unsigned short servo_value[7] = { 500,500,500,500,500,500,500 };
static unsigned short servo_after[7];
/*------------------*/
void motor::OnBnClickedButton6()
{
	unsigned int step;
	/* get step */
	if (get_dex_edit(IDC_EDIT3, &step) != 0)
	{
		AfxMessageBox(_T("步进输入错误"));
		return;
	}
	/* less than 100 */
	if (step > 100)
	{
		AfxMessageBox(_T("步进应小于100"));
		return;
	}
	/* set */
	int seq = m_test_item.GetCurSel();
	/*------*/
	if (seq >= 7)
	{
		AfxMessageBox(_T("错误的测试项"));
		return;
	}
	/*------------------*/
	if (servo_value[seq] < step)
	{
		AfxMessageBox(_T("已调整到最大位置"));
		return;
	}
	/* set final */
	servo_value[seq] -= step;
	/* create buffer */
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 259;
	/*---------------------*/
	for( unsigned int i = 0 ; i < 7 ; i ++ )
	{
		param[i] = (float)servo_value[i];
	}
	/*-------------------------*/   
	ct->fm_link_send(76,package,33);
}


void motor::OnBnClickedButton7()
{
    unsigned int step;
	/* get step */
	if (get_dex_edit(IDC_EDIT3, &step) != 0)
	{
		AfxMessageBox(_T("步进输入错误"));
		return;
	}
	/* less than 100 */
	if (step > 100)
	{
		AfxMessageBox(_T("步进应小于100"));
		return;
	}
	/* set */
	int seq = m_test_item.GetCurSel();
	/*------*/
	if (seq >= 7)
	{
		AfxMessageBox(_T("错误的测试项"));
		return;
	}
	/*------------------*/
	if (servo_value[seq] + step > 1000 )
	{
		AfxMessageBox(_T("已调整到最大位置"));
		return;
	}
	/* set final */
	servo_value[seq] += step;
	/* create buffer */
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 259;
	/*---------------------*/
	for( unsigned int i = 0 ; i < 7 ; i ++ )
	{
		param[i] = (float)servo_value[i];
	}
	/*-------------------------*/   
	ct->fm_link_send(76,package,33);
}


void motor::OnBnClickedButton22()
{
    int seq = m_test_item.GetCurSel();
	/*------*/
	if (seq >= 7)
	{
		AfxMessageBox(_T("错误的测试项"));
		return;
	}
	/*------------------*/
	/* set final */
	servo_value[seq] = 500;
	/* create buffer */
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 259;
	/*---------------------*/
	for( unsigned int i = 0 ; i < 7 ; i ++ )
	{
		param[i] = (float)servo_value[i];
	}
	/*-------------------------*/   
	ct->fm_link_send(76,package,33);
}


void motor::OnBnClickedButton30()
{
	// TODO: 在此添加控件通知处理程序代码
	int mr = m_rotation.GetCurSel();
	int mactive = m_active.GetCurSel();
	/*-------------*/
	if( mr >= 3 || mactive >= 8 )
	{
		AfxMessageBox(_T("不支持的倾转机构或不支持的动作类型"));
		return;
	}
	/* create buffer */
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	unsigned short * param = ( unsigned short * )package;
	/*---------------------*/
	*pd = 261;
	/*---------------------*/
	param[0] = can_id[mr];
	param[1] = 0;//can 0
	param[2] = 4;
	/* create cmd data */
	param[3] =  TM_CMD_HEAD1;
	param[4] =  TM_CMD_HEAD2;
	param[5] =  can_cmd[mactive];
	param[6] =  TM_CMD_TAIL;
	/*-------------------------*/   
	ct->fm_link_send(76,package,33);
}


void motor::OnBnClickedButton9()
{
    for( unsigned int i = 0 ; i < 7 ; i ++ )
	{
		/* set final */
		servo_value[i] = 500;
	}
	/* create buffer */
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 259;
	/*---------------------*/
	for( unsigned int i = 0 ; i < 7 ; i ++ )
	{
		param[i] = (float)servo_value[i];
	}
	/*-------------------------*/   
	ct->fm_link_send(76,package,33);
}
/* set data */
void motor::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
    /* create buffer */
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 260;
	/*---------------------*/
	for( unsigned int i = 0 ; i < 7 ; i ++ )
	{
		param[i] = (float)servo_value[i];
		servo_after[i] = servo_value[i];
	}
	/*-------------------------*/   
	ct->fm_link_send(76,package,33);
	/*-------------------------*/
	m_min.EnableWindow(1);
	m_osd.EnableWindow(1);
	m_max.EnableWindow(1);
}


void motor::OnBnClickedButton35()
{
	// TODO: 在此添加控件通知处理程序代码
}


void motor::OnBnClickedButton37()
{
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 254;
	param[0] = 5;
	/*-------------------------*/   
	ct->fm_link_send(76,package,33);
}


void motor::OnBnClickedButton36()
{
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 256;
	param[0] = 5;
	/*-------------------------*/   
	ct->fm_link_send(76,package,33);
}


void motor::OnBnClickedButton40()
{
    int seq = m_test_item.GetCurSel();
	/*------*/
	if (seq >= 7)
	{
		AfxMessageBox(_T("错误的测试项"));
		return;
	}
	/*------------------*/
	/* set final */
	servo_value[seq] = servo_after[seq];
	/* create buffer */
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 259;
	/*---------------------*/
	for( unsigned int i = 0 ; i < 7 ; i ++ )
	{
		param[i] = (float)servo_value[i];
	}
	/*-------------------------*/   
	ct->fm_link_send(76,package,33);
}


void motor::OnBnClickedButton38()
{
    int seq = m_test_item.GetCurSel();
	/*------*/
	if (seq >= 7)
	{
		AfxMessageBox(_T("错误的测试项"));
		return;
	}
	/*------------------*/
	/* set final */
	servo_value[seq] = 0;
	/* create buffer */
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 259;
	/*---------------------*/
	for( unsigned int i = 0 ; i < 7 ; i ++ )
	{
		param[i] = (float)servo_value[i];
	}
	/*-------------------------*/   
	ct->fm_link_send(76,package,33);
}


void motor::OnBnClickedButton39()
{
    int seq = m_test_item.GetCurSel();
	/*------*/
	if (seq >= 7)
	{
		AfxMessageBox(_T("错误的测试项"));
		return;
	}
	/*------------------*/
	/* set final */
	servo_value[seq] = 1000;
	/* create buffer */
	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 259;
	/*---------------------*/
	for( unsigned int i = 0 ; i < 7 ; i ++ )
	{
		param[i] = (float)servo_value[i];
	}
	/*-------------------------*/   
	ct->fm_link_send(76,package,33);
}
