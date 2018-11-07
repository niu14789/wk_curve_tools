// suggestion.cpp : ʵ���ļ�
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

// suggestion �Ի���

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
	m_loader.SetWindowTextW(_T("�����ϴ�"));
	/*-------------------------------------------*/
}


BEGIN_MESSAGE_MAP(suggestion, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &suggestion::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &suggestion::OnBnClickedButton1)
END_MESSAGE_MAP()


// suggestion ��Ϣ�������


void suggestion::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		AfxMessageBox(_T("���ⳤ��̫��ӦС��32���ַ�"));
		return;
	}else if ( len == 0 )
	{
		AfxMessageBox(_T("����Ϊ�գ�Ӧ��д��������Ĺؼ��ʣ�����<��������><�������>��"));
		return;
	}
	/*----------------------------------------------------------------------------------*/
	len = get_string_edit(IDC_EDIT3,nick_name,sizeof(nick_name));
	/*----------------*/
	if( len >= sizeof(nick_name) )
	{
		AfxMessageBox(_T("�ǳƳ���̫��ӦС��32���ַ�"));
		return;
	}
	/*--------------------------------------------------------------------------------*/
	len = get_string_edit(IDC_EDIT1,data,sizeof(data));
	/*----------------*/
	if( len >= sizeof(data) )
	{
		AfxMessageBox(_T("��������̫��ӦС��1024���ַ�"));
		return;
	}else if ( len == 0 )
	{
		AfxMessageBox(_T("����Ϊ�գ�Ӧ��д����������������"));
		return;
	}
	/*-------------------------------------------*/
	m_topic.EnableWindow(0);
	m_nick_name.EnableWindow(0);
	m_describe.EnableWindow(0);
	m_loader.EnableWindow(0);
	m_loader.SetWindowTextW(_T("���ڷ���"));
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
	sprintf_s(send_buffer,"�������%s  �û�����%s\r\n�ǳƣ�%s\r\n����������%s",pc_name,user_name,nick_name,data);
	/* send email */
	if( send_email(&smtp_suggestion_config,"�������",topic,send_buffer) == 0 )
	{
		AfxMessageBox(_T("���ͳɹ�"));
		OnOK();
	}else
	{
		AfxMessageBox(_T("����ʧ�ܣ�������������"));
		/*-------------------------------------------*/
		m_topic.EnableWindow(1);
		m_nick_name.EnableWindow(1);
		m_describe.EnableWindow(1);
		m_loader.EnableWindow(1);
		m_loader.SetWindowTextW(_T("�����ϴ�"));
		/*-------------------------------------------*/
	}
}
/*---------------------------------*/
void suggestion::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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