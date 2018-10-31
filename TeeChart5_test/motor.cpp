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
	DDX_Control(pDX, IDC_SLIDER1, m_silder_pwm);
	/*----------------------*/
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
	m_silder_pwm.SetRange(1000,2000);
	m_silder_pwm.SetPos(1500);
	m_edit_pwm.SetWindowTextW(_T("1500"));
	/* disable the windows */
	ct->set_test_mode(0);
	/*----reset-----*/
	OnBnClickedButton4();
	OnBnClickedButton4();

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
	if( sscanf(c_string,"%d",&time) != 1 )
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
	param[1] = 1500.0f;
	param[2] = time;
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
			m_silder_pwm.SetPos(1500);
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
	unsigned int pos = m_silder_pwm.GetPos();

	char buffer[100];

	sprintf_s(buffer,"%d",pos);

	USES_CONVERSION;

	CString show = A2T(buffer);

	m_edit_pwm.SetWindowTextW(show);

	unsigned char package[33];

	unsigned short * pd = (unsigned short *)&package[28];
	float * param = ( float * )package;
	/*---------------------*/
	*pd = 253;
	param[0] = 3;
	param[1] = pos;
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
	if( sscanf(c_string,"%d",&time) != 1 )
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
    unsigned short * fac = (unsigned short *)data;
	/*------------------------------------------*/
	USES_CONVERSION;
	/*------------------------------------------*/
	if( *fac == 251 )
	{
		factorty_ok = 1;
		if( data[2] == 0 )
		{
		    MessageBox(_T("烤机模式进入成功"),_T("tips"),0);
		}else
		{
			MessageBox(_T("烤机模式进入失败，已经在烤机模式中，或烤机已经结束，若想重新进入，请复位后再次点击"),_T("tips"),0);
		}
	}else if( *fac == 252 )
	{
		factorty_ok = 0;
		/*if( data[2] == 0 )
		{
		    MessageBox(_T("烤机模式退出成功"),_T("tips"),0);
		}else
		{
			MessageBox(_T("烤机模式退出失败"),_T("tips"),0);
		}*/
	}else if( *fac == 253 )
	{
		if( data[2] == 0 )
		{
		    //MessageBox(_T("油门下发成功"),_T("tips"),0);
		}else
		{
			MessageBox(_T("油门下发失败"),_T("tips"),0);
		}
	}
	else if( *fac == 254 )
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
	}else if( *fac == 255 )
	{
		if( data[2] == 0xff )
		{
		    MessageBox(_T("磁力计校准下发成功"),_T("tips"),0);
		}else
		{
			if(data[2] == 1)
			{
				MessageBox(_T("磁力计校准失败"),_T("tips"),0);
			}else
			{
				MessageBox(_T("磁力计校准成功"),_T("tips"),0);
			}
		}
	}else if( *fac == 256 )
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
	}else if( *fac == 257 )
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
	}else if( *fac == 258 )
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
	//for( unsigned int i = 0 ; i < chese_list.total_line ; i ++ )
	//{
	//	if( strstr( chese_list.chese[i].title , "g_mrs") != NULL )
	//	{
	//		chese_list.chese[i].check_type = 5;//check and err
	//		MessageBox(_T("设置成功") , _T("tips") , 0 ); 
	//		/*------------------------------*/
	//		ct->refresh_result_all();
	//		/*------------------------------*/
	//		return;
	//	}
	//}
	motor_check_flags = 5;
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
