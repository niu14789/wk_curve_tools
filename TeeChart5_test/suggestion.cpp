// suggestion.cpp : 实现文件
//

#include "stdafx.h"
#include "suggestion.h"
#include "afxdialogex.h"
#include "smtp.h"
#include "server_manage.h"
/*-------------------*/
static email_config smtp_suggestion_config = 
{
	"smtp.exmail.qq.com",//host
	"bml1emxAZmVpbWFyb2JvdGljcy5jb20=\r\n",//account
	"MzM2MTEwMG5pdQ==\r\n",//password
	"niuzl@feimarobotics.com",//from
	"niuzl@feimarobotics.com",//t0
};

// suggestion 对话框

IMPLEMENT_DYNAMIC(suggestion, CDialogEx)

suggestion::suggestion(CWnd* pParent /*=NULL*/)
	: CDialogEx(suggestion::IDD, pParent)
{

}

suggestion::~suggestion()
{
}

void suggestion::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_topic);
	DDX_Control(pDX, IDC_EDIT3, m_nick_name);
	DDX_Control(pDX, IDC_EDIT1, m_describe);
	DDX_Control(pDX, IDC_BUTTON2, m_loader);
	/*-------------------------------------------*/
	m_topic.EnableWindow(1);
	m_nick_name.EnableWindow(1);
	m_describe.EnableWindow(1);
	m_loader.EnableWindow(1);
	m_loader.SetWindowTextW(_T("问题上传"));
	/*-------------------------------------------*/
}


BEGIN_MESSAGE_MAP(suggestion, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &suggestion::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &suggestion::OnBnClickedButton1)
END_MESSAGE_MAP()


// suggestion 消息处理程序


void suggestion::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	char topic[64];
	char nick_name[64];
	char data[1024];
	char send_buffer[2048];
	/* clear */
	memset(topic,0,sizeof(topic));
	memset(nick_name,0,sizeof(nick_name));
	memset(data,0,sizeof(data));
	/*----------------*/
	int len = get_string_edit(IDC_EDIT2,topic,sizeof(topic));
	/*----------------*/
	if( len >= sizeof(topic) ) 
	{
		AfxMessageBox(_T("主题长度太大，应小于32个字符"));
		return;
	}else if ( len == 0 )
	{
		AfxMessageBox(_T("主题为空，应添写描述问题的关键词，例如<运行死机><软件建议>等"));
		return;
	}
	/*----------------------------------------------------------------------------------*/
	len = get_string_edit(IDC_EDIT3,nick_name,sizeof(nick_name));
	/*----------------*/
	if( len >= sizeof(nick_name) )
	{
		AfxMessageBox(_T("昵称长度太大，应小于32个字符"));
		return;
	}
	/*--------------------------------------------------------------------------------*/
	len = get_string_edit(IDC_EDIT1,data,sizeof(data));
	/*----------------*/
	if( len >= sizeof(data) )
	{
		AfxMessageBox(_T("描述长度太大，应小于1024个字符"));
		return;
	}else if ( len == 0 )
	{
		AfxMessageBox(_T("描述为空，应添写问题或建议的描述内容"));
		return;
	}
	/*-------------------------------------------*/
	m_topic.EnableWindow(0);
	m_nick_name.EnableWindow(0);
	m_describe.EnableWindow(0);
	m_loader.EnableWindow(0);
	m_loader.SetWindowTextW(_T("正在发送"));
	/*-------------------------------------------*/
	USES_CONVERSION;
	/*-------------------------------------------*/
	wchar_t name_buffer[64];
	char * tmp;
	char pc_name[64],user_name[64];
	DWORD dwnamelen = 64;
	/*----------------*/
	memset(pc_name,0,sizeof(pc_name));
	memset(user_name,0,sizeof(user_name));
	/*----------------*/
	// get pc name & user name
	GetComputerName(name_buffer,&dwnamelen);
	tmp = T2A(name_buffer);
	memcpy(pc_name,tmp,strlen(tmp));
	/*-----------------------------------*/
	dwnamelen = 64;
	GetUserName(name_buffer,&dwnamelen);
	tmp = T2A(name_buffer);
	memcpy(user_name,tmp,strlen(tmp));
	/* create the queue */
	sprintf_s(send_buffer,"计算机：%s  用户名：%s\r\n昵称：%s\r\n问题描述：%s",pc_name,user_name,nick_name,data);
	/* send email */
	if( send_email(&smtp_suggestion_config,"软件反馈",topic,send_buffer) == 0 )
	{
		AfxMessageBox(_T("发送成功"));
		OnOK();
	}else
	{
		AfxMessageBox(_T("发送失败，请检查网络设置"));
		/*-------------------------------------------*/
		m_topic.EnableWindow(1);
		m_nick_name.EnableWindow(1);
		m_describe.EnableWindow(1);
		m_loader.EnableWindow(1);
		m_loader.SetWindowTextW(_T("问题上传"));
		/*-------------------------------------------*/
	}
}
/*---------------------------------*/
void suggestion::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(_T("164100086@qq.com"));
}
/*---------------------------------------*/
int suggestion::get_string_edit( int id,void * data , unsigned int len )
{
	wchar_t get_cs[2048];
	/*--------------------*/
	memset(get_cs,0,sizeof(get_cs));
	/*--------------------*/
	GetDlgItem(id)->GetWindowTextW(get_cs,2048);
    /*--------------------*/
	/* transfer */
	USES_CONVERSION;
	/* transfer */
	char * path_c = T2A(get_cs);
	/*----------------------*/
	int tmp = ( len > strlen(path_c) ) ? strlen(path_c) : len;
	//----
	memcpy(data,path_c,tmp);
	/*----------------------*/
	return tmp;
}