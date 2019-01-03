

#include "stdafx.h"
#include "map.h"


/* open start spy */
#define AIRCRAFT_DATA_ADDRESS 0x126EB50
#define GAME_CAPTION  "huba_tr"


static unsigned int search_addr;

static float data_read[12];

static unsigned int get_addr;

HANDLE processsH;

static unsigned char flags = 0;



int map::open_unity_thread(unsigned int addr)//  open
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	DWORD prcesssid;      //���ش��ڽ���ID
    DWORD byread;

	LPVOID chessaddress = (LPVOID)addr;

	HWND hwnd = ::FindWindow(NULL,_T(GAME_CAPTION));  //��ȡ���ھ��

	if(hwnd != NULL)//����򿪴��ڲ��ɹ�
	{
        GetWindowThreadProcessId(hwnd,&prcesssid); //��ȡ���ڽ���ID

		processsH = OpenProcess(PROCESS_ALL_ACCESS,FALSE, prcesssid) ;//��ָ������

		ReadProcessMemory(processsH,chessaddress,&data_read,sizeof(data_read),&byread); //��ȡ����ָ����ַ����С������(�������ݵ�chessaddress����

		if( data_read[0] == 1.2345f && data_read[1] == 1.2345f)
		{
			get_addr = addr;
        	flags = 1;
			return 0;
		}
		else
		{
			return (-1);
		}
//	    CloseHandle(processsH);//�رս���     
	}else
	{
        MessageBox(_T("can't find the handle"),_T("tips"),0);
		return (-1);
	}
}


void map::spy_send_data_to_unity(float roll,float pitch,float yaw,float pos_x,float pox_y,float pos_z)
{
	if(flags)
	{
		DWORD byread;
        data_read[0] = 1.2345f;
		data_read[1] = 1.2345f;
		data_read[2] = 1;
		data_read[3] = 1;
		/* rotation */
		data_read[4] = pitch;
		data_read[5] = yaw;
		data_read[6] = roll;
		/* position */
		data_read[7] = pos_x;
		data_read[8] = pox_y;
		data_read[9] = pos_z;

		LPVOID chessaddress = (LPVOID)get_addr;

		WriteProcessMemory(processsH,chessaddress,&data_read,sizeof(data_read),&byread);
	}
 }

void map::spy_send_return_home(void)
{
	if(flags)
	{
		DWORD byread;

		data_read[2] = 1;
		data_read[3] = 1;
		/* rotation */
		data_read[4] = 0;
		data_read[5] = 0;
		data_read[6] = 0;
		/* position */
		data_read[7] = 0;
		data_read[8] = 0;
		data_read[9] = 0;

		LPVOID chessaddress = (LPVOID)get_addr;

 		WriteProcessMemory(processsH,chessaddress,&data_read,sizeof(data_read),&byread);
   }
}
//void  Csimulation_demoDlg::OnBnClickedButton18()
//{
//    spy_send_return_home();
//}
unsigned int map::Auti_open(void)
{
		// TODO: �ڴ���ӿؼ�֪ͨ����������

	DWORD prcesssid;      //���ش��ڽ���ID
    DWORD byread;
    unsigned int addr = 0x01000000;
	unsigned int i = 0xffffffff;
    float buffer[1024];
  
	base_addr * base;
	LPVOID base_address = (LPVOID)addr;

	HWND hwnd = ::FindWindow(NULL,_T(GAME_CAPTION));  //��ȡ���ھ��

	if(hwnd != NULL)//����򿪴��ڲ��ɹ�
	{
        GetWindowThreadProcessId(hwnd,&prcesssid); //��ȡ���ڽ���ID

		processsH = OpenProcess(PROCESS_ALL_ACCESS,FALSE, prcesssid) ;//��ָ������
        
		while( i -- )
		{
             if(!ReadProcessMemory(processsH,base_address,&buffer,sizeof(buffer),&byread)) //��ȡ����ָ����ַ����С������(�������ݵ�base_address����
			 {
				 //��ҳ���ɶ�
                 if( (base = compare(buffer,byread/4)) != (NULL) )
				 {
                     MessageBox(_T("open ok"),_T("tips"),0);
					 get_addr = addr + base->addr_base * 4;
					 flags = 1;
					 return get_addr;
				 } 
				 addr+= 4096;
                 base_address = (LPVOID)addr;
				 continue;
			 }else
			 {
				 //��ҳ�ɶ�
               if( ( base = compare(buffer,byread/4)) != (NULL) )
			   {
                  MessageBox(_T("open ok"),_T("tips"),0);
				  get_addr = addr + base->addr_base * 4;
				  flags = 1;
				  return get_addr;
			   } 
			   addr+= 4096;
               base_address = (LPVOID)addr;
			 }
		}
//	    CloseHandle(processsH);//�رս���     
	}else
	{
        MessageBox(_T("��Ҫ�ֶ�����huba_tr.exe"),_T("tips"),0);
		return (-1);
	}
	return (-1);
}
base_addr * map::compare(float * read,unsigned int len)
{
	static char flags = 0;
    static base_addr base;
    unsigned int * inter = (unsigned int *)read;

    for( unsigned int i = 0 ; i < len ; i++ )
	{
		if( !(flags & 0x1) )
		{
			if( read[i] == 1.2345f && read[i+1] == 1.2345f )
			{
				flags |= 0x1;
				base.addr_base = i;
			}
		}
		if( !(flags & 0x2) )
		{
			if( inter[i] == 0x12345678 && inter[i] == 0x12345670)
			{
			   flags |= 0x2;
			   base.addr_camera = i;
			}
		}
        
		if( flags == 0x1 )
		{
           flags = 0;
		   return &base;
		}

	}
	return NULL;
}