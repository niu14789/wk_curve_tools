// C_PROCOTOL_CREATE.cpp : 实现文件
//

#include "stdafx.h"
#include "C_PROCOTOL_CREATE.h"
#include "afxdialogex.h"


// C_PROCOTOL_CREATE 对话框

IMPLEMENT_DYNAMIC(C_PROCOTOL_CREATE, CDialogEx)

C_PROCOTOL_CREATE::C_PROCOTOL_CREATE(CWnd* pParent /*=NULL*/)
	: CDialogEx(C_PROCOTOL_CREATE::IDD, pParent)
{

}

C_PROCOTOL_CREATE::~C_PROCOTOL_CREATE()
{
}

void C_PROCOTOL_CREATE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_combox_protocol_type);
	DDX_Control(pDX, IDC_EDIT1, m_CFS_PATH);
	DDX_Control(pDX, IDC_COMBO1, m_combox_data);
	//DDX_Control(pDX, IDC_COMBO14, m_combox_param);
	DDX_Control(pDX, IDC_EDIT2, m_global_protocol_name);
	DDX_Control(pDX, IDC_COMBO11, m_global_select);
	DDX_Control(pDX, IDC_EDIT4, m_block_size);
	DDX_Control(pDX, IDC_EDIT5, m_width);
	DDX_Control(pDX, IDC_COMBO12, m_head_msg);
	DDX_Control(pDX, IDC_EDIT7, m_param_num);
	DDX_Control(pDX, IDC_EDIT8, m_sample_point);
	DDX_Control(pDX, IDC_EDIT16, m_math_num);
	DDX_Control(pDX, IDC_EDIT14, a_name);
//	DDX_Control(pDX, IDC_COMBO9, a_type);
//	DDX_Control(pDX, IDC_COMBO13, a_math_type);
	DDX_Control(pDX, IDC_EDIT17, a_if);
	//DDX_Control(pDX, IDC_EDIT19, t_name);
	//DDX_Control(pDX, IDC_EDIT18, t_edit);
	//DDX_Control(pDX, IDC_EDIT20, t_test);
	/*----------*/
	widget_initation();
}


BEGIN_MESSAGE_MAP(C_PROCOTOL_CREATE, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &C_PROCOTOL_CREATE::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON22, &C_PROCOTOL_CREATE::OnBnClickedButton22)
	ON_BN_CLICKED(IDC_BUTTON4, &C_PROCOTOL_CREATE::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON16, &C_PROCOTOL_CREATE::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON15, &C_PROCOTOL_CREATE::OnBnClickedButton15)
	ON_EN_CHANGE(IDC_EDIT9, &C_PROCOTOL_CREATE::OnEnChangeEdit9)
	ON_BN_CLICKED(IDC_BUTTON13, &C_PROCOTOL_CREATE::OnBnClickedButton13)
	ON_CBN_SELCHANGE(IDC_COMBO1, &C_PROCOTOL_CREATE::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON24, &C_PROCOTOL_CREATE::OnBnClickedButton24)
	ON_BN_CLICKED(IDC_BUTTON23, &C_PROCOTOL_CREATE::OnBnClickedButton23)
	ON_BN_CLICKED(IDC_BUTTON3, &C_PROCOTOL_CREATE::OnBnClickedButton3)
END_MESSAGE_MAP()


// C_PROCOTOL_CREATE 消息处理程序
static CFG_FILE_DEF      CFG_CURRENT;
static COMBOX_DEF        DATA_COMBOX;
static CString CFS_File_Path;
static FILE * fp_cfs = NULL;
static char * file_open = NULL;
static char file_name_buffer[200];

void C_PROCOTOL_CREATE::widget_initation(void)
{
	m_combox_protocol_type.SetCurSel(0);
	m_global_select.SetCurSel(0);
	m_head_msg.SetCurSel(0);
	//m_combox_param.SetCurSel(0);
	m_combox_data.SetCurSel(0);
}
/* open file */
void C_PROCOTOL_CREATE::OnBnClickedButton1()
{
    TCHAR szFilter[] = _T("CFS文件|*.CFS|"); 

    CFileDialog fileDlg(TRUE, _T("open file"), NULL, 0, szFilter, this); 

    if(IDOK == fileDlg.DoModal())   
    {   
        CFS_File_Path = fileDlg.GetPathName();  
        SetDlgItemText(IDC_EDIT1, CFS_File_Path);
		/* read data */
		USES_CONVERSION;
		/* transfer */
		file_open = T2A(CFS_File_Path);
		/* */
		sprintf_s(file_name_buffer,"%s",file_open);
		// read_data_to_buffer
		read_file_to_current(file_open);
    }
}
/*------------create protocol-----------*/
void C_PROCOTOL_CREATE::OnBnClickedButton22()
{
	// TODO:
    memset(&CFG_CURRENT,0,sizeof(CFG_CURRENT));

	memset(&DATA_COMBOX,0,sizeof(DATA_COMBOX));

	m_global_protocol_name.SetWindowTextW(_T(""));

	fp_cfs = NULL;
}
/* public function */
int C_PROCOTOL_CREATE::get_string_edit( int id,void * data , unsigned int len )
{
	wchar_t get_cs[512];
    int len_t;
	/*--------------------*/
	memset(get_cs,0,sizeof(get_cs));
	/*--------------------*/
	len_t = GetDlgItem(id)->GetWindowTextW(get_cs,512);
    /*--------------------*/
	/* transfer */
	USES_CONVERSION;
	/* transfer */
	char * path_c = T2A(get_cs);
	/*----------------------*/
	memcpy(data,path_c,strlen(path_c));
	/*----------------------*/
	return len_t;
}
/*----------------*/
int C_PROCOTOL_CREATE::get_dex_edit( int id,unsigned int * data , unsigned int len )
{
	wchar_t get_cs[512];
	char c_string[256];
    int len_t;

	len_t = GetDlgItem(id)->GetWindowTextW(get_cs,512);

	memset(c_string,0,sizeof(c_string));

	for( int i = 0 ; i < len_t ; i ++  )
	{
		c_string[i] = (char)get_cs[i];
	}
	/*----------------------*/
	int ret = sscanf_s(c_string,"%d",data);

	if( ret == 1 )
	{
		return 0;
	}else
	{
		return (-1);
	}
}
int C_PROCOTOL_CREATE::assert_msg(unsigned int type , unsigned int data ,char * msg)
{
	CString show;

	USES_CONVERSION;

	show = A2T(msg);

	if( type == 0 )
	{
		if( data == 0 )
		{
			AfxMessageBox(show,0,0);
			return (-1);
		}
   }else
   {
		if( data > type )
		{
			AfxMessageBox(show,0,0);
			return (-1);
		}
   }
   return 0;
}
int C_PROCOTOL_CREATE::msg_out(char * msg)
{
	CString show;

	USES_CONVERSION;

	show = A2T(msg);

	AfxMessageBox(show,0,0);

    return 0;
}
/* save */
void C_PROCOTOL_CREATE::OnBnClickedButton4()
{
	char buffer[512];
	int len;
	/*---------*/
	memset(buffer,0,sizeof(buffer));
    /* get global name */
    len = get_string_edit(IDC_EDIT2,buffer,512);
    /* le */
    if( assert_msg(0,len,"请输入协议名") != 0 )
	{
		return;
	}
	if( assert_msg(32,len,"协议名应小于32个") != 0 )
	{
		return;
	}
	/* set to struct */
	memcpy(CFG_CURRENT.cfs_global_msg.name,buffer,len);
	/*-------------type-------------*/
	int type_def = m_combox_protocol_type.GetCurSel();
	/* assert */
    if( assert_msg(0,type_def,"请选择协议类型") != 0 )
	{
		return;
	}
	//1 is fix length , 2 is private_define
	CFG_CURRENT.cfs_global_msg.procotol_type = type_def;
	/*--------protocol select---------*/
	int protocol_select;
    /* get select num */
	protocol_select = m_global_select.GetCurSel();
    /*----------------*/
	if( type_def == 2 )
	{
		if( assert_msg(0,protocol_select,"请先生成并选择自定协议") != 0 )
		{
			return;
		}		
	}
	/*-----------*/
	CFG_CURRENT.cfs_global_msg.procotol_select = protocol_select;
	/*------------get block size-------------*/
	unsigned int tmp;
	if( get_dex_edit(IDC_EDIT4, &tmp,sizeof(tmp)) == 0 )
	{
		CFG_CURRENT.cfs_global_msg.block_size = tmp;
	}else
	{
		msg_out("读取长度参数错误");
		/*-----*/
		return;
	}
	/* read width */
	if( get_dex_edit(IDC_EDIT5, &tmp,sizeof(tmp)) == 0 )
	{
		CFG_CURRENT.cfs_global_msg.offset = tmp;
	}else
	{
		msg_out("偏移长度参数错误");
		/*-----*/
		return;
	}
	/* ok */
	CFG_CURRENT.cfs_global_msg.ok = 0xff;
	/* save file */
	flush_cfs(1);
}
/*-------------------*/
void C_PROCOTOL_CREATE::combox_update(void)
{
	CString show;

	USES_CONVERSION;
	/* reset the combox */
	m_combox_data.ResetContent();
	/*---------------*/
	for( unsigned int i = 0 ; i < DATA_COMBOX.num ; i ++ )
	{
		show = A2T(DATA_COMBOX.param_data[i].name);
		/*--------------*/
		m_combox_data.AddString(show);
	}
	/* set current pos */
	m_combox_data.SetCurSel(DATA_COMBOX.num-1);
}
/*--------------------------*/
int C_PROCOTOL_CREATE::combox_data_add(int index , char * string ,int mode)
{
	/* check */
	if( index >= 256 )
	{
		msg_out("数据块大小超限");
		/*--------------*/
		return (-1);
	}
	/* check same */
	if( mode == 0 )
	{
		for( int i = 0 ; i < index + 1 ; i ++ )
		{
			if( 0 == memcmp(DATA_COMBOX.param_data[i].name,string,32) )
			{
				msg_out("数据已存在");
				/*--------------*/
				return (-1);
			}
		}
	}
	/*---------------*/
	memcpy(DATA_COMBOX.param_data[index].name,string,32);
	/* incremeter */
	DATA_COMBOX.param_data[index].cmd = 1;
	DATA_COMBOX.param_data[index].index = index;
	/* -------*/
	if( mode == 0 )
	{
 	  DATA_COMBOX.num++;
	  /* set to struct */
	  global_num_update(0,1,0);
	  /* updata the global setting */
	}
    /* data combox update */
	combox_update();
	/* return */
	return 0;
}

void C_PROCOTOL_CREATE::global_num_update(int param,int sample,int math)
{
	char buffer[512];
	CString show;
	USES_CONVERSION;
	/* show param */
	if( param == 1 )
	{
		CFG_CURRENT.cfs_global_msg.param_num++;
	}else if( param == 2 )
	{
		CFG_CURRENT.cfs_global_msg.param_num--;
	}
	sprintf_s(buffer,"%d",CFG_CURRENT.cfs_global_msg.param_num);
	show = A2T(buffer);
	m_param_num.SetWindowTextW(show);
	/* show sample */
	if( sample == 1 )
	{
		CFG_CURRENT.cfs_global_msg.sample_num++;
	}else if( sample == 2 )
	{
		CFG_CURRENT.cfs_global_msg.sample_num--;
	}
	sprintf_s(buffer,"%d",CFG_CURRENT.cfs_global_msg.sample_num);
	show = A2T(buffer);
	m_sample_point.SetWindowTextW(show);
	/* show math */
	if( math == 1 )
	{
		CFG_CURRENT.cfs_global_msg.math_num++;
	}else if( math == 2 )
	{
		CFG_CURRENT.cfs_global_msg.math_num--;
	}
	sprintf_s(buffer,"%d",CFG_CURRENT.cfs_global_msg.math_num);
	show = A2T(buffer);
	m_math_num.SetWindowTextW(show);
	/*------------*/
}

int C_PROCOTOL_CREATE::gets_one_line(char *buffer,char *dst,unsigned int dst_len,unsigned int len)
{
	int dt = 0;

	char * tmp = dst;

	for( unsigned int i = 0 ; i < len ; i ++ )
	{
		if( buffer[i] != 0x0d && buffer[i] != 0x0A )
		{
			if( buffer[i] != ';' )
			{
			   *dst = buffer[i];
			   dst++;
			}else
			{
				tmp += dst_len;
				dst = tmp;
				dt++;
			}
		}
	}
	/* return */
	return dt;
}
/*----------------get param--------------------*/
int C_PROCOTOL_CREATE::Get_param_OLAY(char * src , unsigned int src_len , unsigned int num,const char * cmd,unsigned int * param)
{
	char * tmp;//buffer[64];

	unsigned int get = 0;

	for( unsigned int i = 0 ; i < num ; i++ )
	{
	    tmp = (char *)strstr(src,cmd);
		/* get */
		if( tmp != NULL )
		{
			int ret = sscanf(src+strlen(cmd),"[%d]",&get);
			/*---*/
			if( ret != 1 )
			{
				return (-1);
			}else
			{
				*param = get;
				return 0;
			}
		}
		/*-=*/
		src += src_len;
	}
	/* */
	return (-1);
}
/*----------------get param--------------------*/
int C_PROCOTOL_CREATE::Get_param_type(char * src , unsigned int src_len , unsigned int num,const char * cmd,unsigned int * param)
{
	char * tmp;//buffer[64];

	char get[16];

	for( unsigned int i = 0 ; i < num ; i++ )
	{
	    tmp = (char *)strstr(src,cmd);
		/* get */
		if( tmp != NULL )
		{
			int ret = sscanf(src+strlen(cmd),"[%s]",get);
			/*---*/
			if( ret != 1 )
			{
				return (-1);
			}else
			{
				if( strstr(get,"UINT8") != NULL )
				{
					*param = 1;
					return 0;
				}
				if( strstr(get,"INT8") != NULL )
				{
					*param = 2;
					return 0;
				}
				if( strstr(get,"UINT16") != NULL )
				{
					*param = 3;
					return 0;
				}
				if( strstr(get,"INT16") != NULL )
				{
					*param = 4;
					return 0;
				}
				if( strstr(get,"UINT32") != NULL )
				{
					*param = 5;
					return 0;
				}
				if( strstr(get,"INT32") != NULL )
				{
					*param = 6;
					return 0;
				}
				if( strstr(get,"FLOAT") != NULL )
				{
					*param = 7;
					return 0;
				}
				if( strstr(get,"DOUBLE") != NULL )
				{
					*param = 8;
					return 0;
				}
				if( strstr(get,"CHAR") != NULL )
				{
					*param = 9;
					return 0;
				}
				if( strstr(get,"BIT") != NULL )
				{
					*param = 10;
					return 0;
				}
				return (-1);
			}
		}
		/*-=*/
		src += src_len;
	}
	/* */
	return (-1);
}
/*----------------get param--------------------*/
int C_PROCOTOL_CREATE::Get_check_if(char * src , unsigned int src_len , unsigned int num,char *string , unsigned int *param1,unsigned int *param2)
{
	char * tmp;//buffer[64];

	for( unsigned int i = 0 ; i < num ; i++ )
	{
	    tmp = (char *)strstr(src,"IF");
		/* get */
		if( tmp != NULL )
		{
			int ret = sscanf(src,"IF[0X%x][0X%x]",param1,param2);
			/*---*/
			if( ret != 2 )
			{
				return (-1);
			}else
			{
				return 0;
			}
		}
		/*-=*/
		src += src_len;
	}
	/* */
	return (0);
}
char * C_PROCOTOL_CREATE::Get_math_type(char * src , unsigned int src_len , unsigned int num, unsigned int *param1)
{
	char * tmp;//buffer[64];

	for( unsigned int i = 0 ; i < num ; i++ )
	{
	    tmp = (char *)strstr(src,"S2F");
		/* get */
		if( tmp != NULL )
		{
			*param1 = 1;//short 2 float
			return tmp;
		}
		/*-------*/
		tmp = (char *)strstr(src,"THIS");
		/* get */
		if( tmp != NULL )
		{
			*param1 = 2;//short 2 float
			return tmp;
		}
		/*-=*/
		src += src_len;
	}
	/* */
	return NULL;
}
int C_PROCOTOL_CREATE::decode_data_math(unsigned int index)
{
	/* set math type and math value */
	char math_buffer[64][64];
	/* clear the buffer */
	memset(DATA_COMBOX.param_data[index].buffer,0,sizeof(DATA_COMBOX.param_data[index].buffer));
	/*------------------*/
	int math_cnt = get_string_edit(IDC_EDIT17,DATA_COMBOX.param_data[index].buffer,sizeof(DATA_COMBOX.param_data[index].buffer));
	/* memset */
	memset(math_buffer,0,sizeof(math_buffer));
	/* math type */
	int ret = gets_one_line(DATA_COMBOX.param_data[index].buffer,(char *)math_buffer,64, math_cnt);
	/* ok */
	if(ret == 0 )
	{
		msg_out("参数错误");
		/*---------------------*/
		return (-1);
	}
	/*--------check offset--------*/
	if( Get_param_OLAY((char *)&math_buffer,64,ret,"OFFSET",&DATA_COMBOX.param_data[index].offset) != 0 )
	{
		msg_out("参数错误");
		/*---------------------*/
		return (-1);
	}
	/*--------check len--------*/
	if( Get_param_OLAY((char *)&math_buffer,64,ret,"LEN",&DATA_COMBOX.param_data[index].width) != 0 )
	{
		msg_out("参数错误");
		/*---------------------*/
		return (-1);
	}
	/*--------check len--------*/
	if( Get_param_OLAY((char *)&math_buffer,64,ret,"ALIGN",&DATA_COMBOX.param_data[index].little) != 0 )
	{
		msg_out("参数错误");
		/*---------------------*/
		return (-1);
	}
	/*----------check type-----------*/
 	if( Get_param_type((char *)&math_buffer,64,ret,"TYPE",&DATA_COMBOX.param_data[index].type) != 0 )
	{
		msg_out("参数错误");
		/*---------------------*/
		return (-1);
	}
	/* check if */
	if( Get_check_if((char *)&math_buffer,64,ret,DATA_COMBOX.param_data[index].if_param_string,
		&DATA_COMBOX.param_data[index].if_param_value[0],&DATA_COMBOX.param_data[index].if_param_value[1]) != 0 )
	{
		msg_out("参数错误");
		/*---------------------*/
		return (-1);
	}
	/* get MATH type */
	char * tmp = Get_math_type((char *)&math_buffer,64,ret,&DATA_COMBOX.param_data[index].math_type);
	/* check */
	if( tmp == NULL )
	{
		DATA_COMBOX.param_data[index].math_type = 0;
		DATA_COMBOX.param_data[index].math_number = 0;
		/* return ok */
		return 0;
	}
    if( DATA_COMBOX.param_data[index].math_type == 1 )
	{
		DATA_COMBOX.param_data[index].math_type = 1;//short to float;
		/*--------------*/
		int ret = sscanf(tmp,"S2F[%f][%c][%f][%c][%f][%c][%f][%c][%f][%c][%f][%c][%f][%c][%f][%c][%f][%c][%f]",
			&DATA_COMBOX.param_data[index].f_math[0],&DATA_COMBOX.param_data[index].asmf_type[0],
			&DATA_COMBOX.param_data[index].f_math[1],&DATA_COMBOX.param_data[index].asmf_type[1],
			&DATA_COMBOX.param_data[index].f_math[2],&DATA_COMBOX.param_data[index].asmf_type[2],
			&DATA_COMBOX.param_data[index].f_math[3],&DATA_COMBOX.param_data[index].asmf_type[3],
			&DATA_COMBOX.param_data[index].f_math[4],&DATA_COMBOX.param_data[index].asmf_type[4],
			&DATA_COMBOX.param_data[index].f_math[5],&DATA_COMBOX.param_data[index].asmf_type[5],
            &DATA_COMBOX.param_data[index].f_math[6],&DATA_COMBOX.param_data[index].asmf_type[6],
			&DATA_COMBOX.param_data[index].f_math[7],&DATA_COMBOX.param_data[index].asmf_type[7],
			&DATA_COMBOX.param_data[index].f_math[8],&DATA_COMBOX.param_data[index].asmf_type[8],
			&DATA_COMBOX.param_data[index].f_math[9]);
		/* if ok ? */
		if( ret != 0 )
		{
			if( ret == 1 )
			{
				DATA_COMBOX.param_data[index].math_number = 1;
			}else
			{
				if( ret >= 3 && ( ret & 0x1 ) )
				{
					for( int i = 1 ; i < ret ; i += 2 )
					{
						if( !(DATA_COMBOX.param_data[index].asmf_type[(i-1)/2] == '+' || 
							DATA_COMBOX.param_data[index].asmf_type[(i-1)/2] == '-' || 
							DATA_COMBOX.param_data[index].asmf_type[(i-1)/2] == '*' || 
							DATA_COMBOX.param_data[index].asmf_type[(i-1)/2] == '/' || 
							DATA_COMBOX.param_data[index].asmf_type[(i-1)/2] == '&'))
						{
							msg_out("条件式参数错误");
							/*---------------------*/
							return (-1);
						}
					}
				}else
				{
					msg_out("条件式参数错误");
					/*---------------------*/
					return (-1);
				}
				/*==========*/
				DATA_COMBOX.param_data[index].math_number = ret;
			}
		}else
		{
			msg_out("条件式参数错误");
			/*---------------------*/
			return (-1);
		}
	}else if( DATA_COMBOX.param_data[index].math_type == 2 )
	{
		DATA_COMBOX.param_data[index].math_type = 2;//short to float with mupity data
		/*-----------------------------------*/
		int ret = sscanf(tmp,"THIS[%c][%f][%c][%f][%c][%f][%c][%f][%c][%f][%c][%f][%c][%f][%c][%f][%c][%f][%c][%f]",
			/* seek */
			&DATA_COMBOX.param_data[index].asmf_type[0],&DATA_COMBOX.param_data[index].f_math[0],
			&DATA_COMBOX.param_data[index].asmf_type[1],&DATA_COMBOX.param_data[index].f_math[1],
			&DATA_COMBOX.param_data[index].asmf_type[2],&DATA_COMBOX.param_data[index].f_math[2],
			&DATA_COMBOX.param_data[index].asmf_type[3],&DATA_COMBOX.param_data[index].f_math[3],
			&DATA_COMBOX.param_data[index].asmf_type[4],&DATA_COMBOX.param_data[index].f_math[4],
			&DATA_COMBOX.param_data[index].asmf_type[5],&DATA_COMBOX.param_data[index].f_math[5],
            &DATA_COMBOX.param_data[index].asmf_type[6],&DATA_COMBOX.param_data[index].f_math[6],
			&DATA_COMBOX.param_data[index].asmf_type[7],&DATA_COMBOX.param_data[index].f_math[7],
			&DATA_COMBOX.param_data[index].asmf_type[8],&DATA_COMBOX.param_data[index].f_math[8],
			&DATA_COMBOX.param_data[index].asmf_type[9],&DATA_COMBOX.param_data[index].f_math[9]);
		/* if ok ? */
		if( ret != 0 )
		{
			if( ret >= 2 && ( ! ( ret & 0x1 ) ) )
			{
				for( int i = 0 ; i < ret ; i += 2 )
				{
					if( !(DATA_COMBOX.param_data[index].asmf_type[i/2] == '+' || 
						DATA_COMBOX.param_data[index].asmf_type[i/2] == '-' || 
						DATA_COMBOX.param_data[index].asmf_type[i/2] == '*' || 
						DATA_COMBOX.param_data[index].asmf_type[i/2] == '/' || 
						DATA_COMBOX.param_data[index].asmf_type[i/2] == '&'))
					{
						msg_out("条件式参数错误");
						/*---------------------*/
						return (-1);
					}
				}
			}else
			{
				msg_out("条件式参数错误");
				/*---------------------*/
				return (-1);
			}
			/*==========*/
			DATA_COMBOX.param_data[index].math_number = ret;
		}else
		{
			msg_out("条件式参数错误");
			/*---------------------*/
			return (-1);
		}
	}
	/* return ok */
	return 0;
}
int C_PROCOTOL_CREATE::setting_data(int index,int mode)
{
    char buffer[512];
	int len,ret;
	/* memset */
	memset(buffer,0,sizeof(buffer));
	/*---------*/
    /* get global name */
    len = get_string_edit(IDC_EDIT14,buffer,512);
    /* le */
    if( assert_msg(0,len,"请输入数据名") != 0 )
	{
		return (-1);
	}
	if( assert_msg(32,len,"数据名应小于32个") != 0 )
	{
		return (-1);
	}
	if( decode_data_math(index) != 0 )
	{
		return (-1);
	}
	/* check little enable */
	/*------------------------*/
	//if( 
	/*------------------------------*/
	if( mode == 0 )
	{
		ret = combox_data_add(DATA_COMBOX.num,buffer,mode);
	}else
	{
		ret = combox_data_add(index,buffer,mode);
	}
	/* return */
	return ret;
}
void C_PROCOTOL_CREATE::OnBnClickedButton16()
{
	if( setting_data(DATA_COMBOX.num,0) == 0 )
	{
		flush_cfs(0);
	}
}


void C_PROCOTOL_CREATE::OnBnClickedButton15()
{
	int seq = m_combox_data.GetCurSel();
	/*--------*/
	if( DATA_COMBOX.param_data[seq].cmd == 0xff )
	{
		return;//had deleted
	}
	/* set the name to '-' */
	memset(DATA_COMBOX.param_data[seq].name,'-',31);
	/* set block not available */
	DATA_COMBOX.param_data[seq].cmd = 0xff;
	/*-------------------------*/
	combox_update();
	/*-----*/
	m_combox_data.SetCurSel(seq);
	/* -- delete -- */
	global_num_update(0,2,0);
	/*---*/
	flush_cfs(0);
}


void C_PROCOTOL_CREATE::OnEnChangeEdit9()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void C_PROCOTOL_CREATE::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
}
/* combox data check up */
void C_PROCOTOL_CREATE::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString show;
	/*----------------*/
	int seq = m_combox_data.GetCurSel();
	/*---------------*/
	USES_CONVERSION;
	/* change */
	show = A2T(DATA_COMBOX.param_data[seq].name);
	/*---show name--*/
	a_name.SetWindowTextW(show);
	/* show buffer */
	show = A2T(DATA_COMBOX.param_data[seq].buffer);
	/* show buffer */
	a_if.SetWindowTextW(show);
}

void C_PROCOTOL_CREATE::OnBnClickedButton24()
{
	// TODO: 在此添加控件通知处理程序代码
	int seq = m_combox_data.GetCurSel();
	if(  setting_data(seq,1) == 0 )
	{
	   m_combox_data.SetCurSel(seq);
	   flush_cfs(0);
	}
}
/*----------------*/
void C_PROCOTOL_CREATE::flush_cfs(unsigned int msg)
{
	// TODO: 在此添加控件通知处理程序代码
	if( CFG_CURRENT.cfs_global_msg.ok != 0xff )
	{
		msg_out("没有进行全局设置");
		/*---------------------*/
		return;
	}
 //   /* check data num */
	//if( CFG_CURRENT.cfs_global_msg.sample_num == 0 )
	//{
	//	msg_out("当前没有保存采样数据点");
	//	/*---------------------*/
	//	return;
	//}
	/* ok */
	if( fp_cfs == NULL )
	{
		char buffer[200];
		/*-------------------------------------------------*/
		sprintf_s(buffer,"%s.CFS",CFG_CURRENT.cfs_global_msg.name);
		/* create */
		fopen_s(&fp_cfs,buffer,"wb+");
		/* ok ? */
		if( fp_cfs == NULL )
		{
			CString show;
			USES_CONVERSION;
			/*---------------------------------------------*/
			show = A2T(buffer);
			/*---------------------------------------------*/
			if( DeleteFile(show) == 0 )
			{
				/*---------------------------------------------*/
				msg_out("文件无法删除");
				/*---------------------*/
				return;
			}
			/* create again */
			fopen_s(&fp_cfs,buffer,"wb+");
			/*-------------*/
			if( fp_cfs == NULL )
			{
				msg_out("文件创建失败");
				/*---------------------*/
				return;
			}
		}
	}else
	{
		/* create */
		fopen_s(&fp_cfs,file_name_buffer,"wb+");
		/* ok ? */
		if( fp_cfs == NULL )
		{
			CString show;
			USES_CONVERSION;
			/*---------------------------------------------*/
			show = A2T(file_name_buffer);
			/*---------------------------------------------*/
			if( DeleteFile(show) == 0 )
			{
				/*---------------------------------------------*/
				msg_out("文件无法删除");
				/*---------------------*/
				return;
			}
			/* create again */
			fopen_s(&fp_cfs,file_name_buffer,"wb+");
			/*-------------*/
			if( fp_cfs == NULL )
			{
				msg_out("文件创建失败");
				/*---------------------*/
				return;
			}
		}
	}
    /* start write */
	fwrite(&CFG_CURRENT.cfs_global_msg,1,sizeof(CFG_CURRENT.cfs_global_msg),fp_cfs);
	fwrite(&CFG_CURRENT.private_protocol,1,sizeof(CFG_CURRENT.private_protocol),fp_cfs);
	/* write data */
	unsigned int i = 0,cnt = 0;
	/* write */
	for(  i = 0 ; i < DATA_COMBOX.num ; i ++ )
	{
		if( DATA_COMBOX.param_data[i].cmd != 0xff )
		{
			fwrite(&DATA_COMBOX.param_data[i],1,sizeof(DATA_COMBOX.param_data[i]),fp_cfs);
			cnt++;
		}
	}
	/**/
	if( cnt != CFG_CURRENT.cfs_global_msg.sample_num )
	{
		msg_out("数据块数据导常");
		/*---------------------*/
	}
	/*--------------*/
	fclose(fp_cfs);
	/*--------*/
	if( msg == 1 )
	{
	  msg_out("保存成功");
	}
}
/* save CFS file */
void C_PROCOTOL_CREATE::OnBnClickedButton23()
{
	
}
/* read_file_to_buffer */
void C_PROCOTOL_CREATE::read_file_to_current(char *path)
{
    /* create */
	fopen_s(&fp_cfs,path,"rb");
	/* ok ? */
	if( fp_cfs == NULL )
	{
		msg_out("读取文件失败");
		/*---------------------*/
		fclose(fp_cfs);
		/*---------------------*/
		return;
	}
	/*----------------------*/
	int len = fread(&CFG_CURRENT,1,sizeof(CFG_CURRENT),fp_cfs);
	/*-----------------*/
	fclose(fp_cfs);
	/*  lose some data */
	if( len == sizeof(CFG_CURRENT) )
	{
		msg_out("有一些数据可能会丢失");
	}
	/*--------------------------*/
	memset(&DATA_COMBOX,0,sizeof(DATA_COMBOX));
	/*--------------------------*/
	for( unsigned int i = 0 ; i < CFG_CURRENT.cfs_global_msg.sample_num ; i++ )
	{
		if( CFG_CURRENT.pmd[i].cmd != 1 )
		{
			msg_out("数据块读取错误");
			fp_cfs = NULL;
			/*----------*/
			return;
		}
		/* copy */
		memcpy(&DATA_COMBOX.param_data[i],&CFG_CURRENT.pmd[i],sizeof(DATA_COMBOX.param_data[i]));
		/* index ++ */
		DATA_COMBOX.num ++;
	}
	/*--------------------------*/
	combox_update();
	/* show global name */
	char buffer[100];
	CString show;
	/*---------------*/
	USES_CONVERSION;
	/*------------*/
	show = A2T(CFG_CURRENT.cfs_global_msg.name);
	m_global_protocol_name.SetWindowTextW(show);
	/*------type-----*/
	m_combox_protocol_type.SetCurSel(CFG_CURRENT.cfs_global_msg.procotol_type);
	/* select */
	m_global_select.SetCurSel(CFG_CURRENT.cfs_global_msg.procotol_select);
	/*----length----*/
	sprintf_s(buffer,"%d",CFG_CURRENT.cfs_global_msg.block_size);
	show = A2T(buffer);
	m_block_size.SetWindowTextW(show);
	/* width */
	sprintf_s(buffer,"%d",CFG_CURRENT.cfs_global_msg.offset);
	show = A2T(buffer);
	m_width.SetWindowTextW(show);
	/*----head----*/
	m_head_msg.SetCurSel(CFG_CURRENT.cfs_global_msg.head_msg);
	/*-----param num-----*/
	sprintf_s(buffer,"%d",CFG_CURRENT.cfs_global_msg.param_num);
	show = A2T(buffer);
	m_param_num.SetWindowTextW(show);
	/*-----sample num-----*/
	sprintf_s(buffer,"%d",CFG_CURRENT.cfs_global_msg.sample_num);
	show = A2T(buffer);
	m_sample_point.SetWindowTextW(show);
	/*-----math num-----*/
	sprintf_s(buffer,"%d",CFG_CURRENT.cfs_global_msg.math_num);
	show = A2T(buffer);
	m_math_num.SetWindowTextW(show);

}

void C_PROCOTOL_CREATE::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
}
