// CDlgComSet.cpp: 实现文件
//

#include "pch.h"
#include "RobotMonH.h"
#include "CDlgComSet.h"
#include "afxdialogex.h"
#include <string>
#include <algorithm>
using namespace std;

// CDlgComSet 对话框

IMPLEMENT_DYNAMIC(CDlgComSet, CDialogEx)

CDlgComSet::CDlgComSet(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_COMSET, pParent)
{

}

CDlgComSet::~CDlgComSet()
{
}

bool Comp_i(Com_Port_Struct &p1, Com_Port_Struct &p2)
{
	return p1.m_iNum < p2.m_iNum;
}

void CDlgComSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SET_PORT, m_ComboBox_PortSet);
	DDX_Control(pDX, IDC_COMBO_BAND, m_ComboBox_Band_Ctrl);
	DDX_Control(pDX, IDC_COMBO_DATA_BIT, m_ComboBox_Data_Ctrl);
	DDX_Control(pDX, IDC_COMBO_CHECK_BIT, m_ComboBox_Check_Ctrl);
	DDX_Control(pDX, IDC_COMBO_STOP_BIT, m_ComboBox_Stop_Ctrl);
}

BOOL CDlgComSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	//读取注册表获取串口信息
	TraversalCom();



	if (m_Struct_Com.GetCount() != 0)
	{
		//根据串口号进行排序，不然显示顺序有可能会乱
		std::sort(m_Struct_Com.GetData(), m_Struct_Com.GetData() + m_Struct_Com.GetCount(), Comp_i);
		for (int i = 0; i < m_Struct_Com.GetCount(); i++)
		{
			m_ComboBox_PortSet.AddString(m_Struct_Com[i].m_strPort);
		}
	}
	//如果注册表读的串口是空的
	else
	{
		m_ComboBox_PortSet.AddString(L"COM1");
	}
	//读取配置文件，设置上次设置的串口信息
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用
	CString iniPath = _T("./RobotmonHConfig.ini");
	BOOL ifFind = finder.FindFile(iniPath);

	
	//有找到文件就读取ini文件的内容
	if (ifFind)
	{	
		m_Com_Info_Struct.m_iPortNum = GetPrivateProfileInt(_T("Database"), _T("ComPort"), 1, iniPath);//读出串口号
		m_Com_Info_Struct.m_iBand = GetPrivateProfileInt(_T("Database"), _T("Band"), 119200, iniPath);//读出波特率
		m_Com_Info_Struct.m_iCheckBit = GetPrivateProfileInt(_T("Database"), _T("CheckBit"), 0, iniPath);//读出校验位
		m_Com_Info_Struct.m_iStopBit = GetPrivateProfileInt(_T("Database"), _T("StopBit"), 1, iniPath);//读出停止位
		m_Com_Info_Struct.m_iDataBit = GetPrivateProfileInt(_T("Database"), _T("DataBit"), 8, iniPath);//读出数据位
		//如果ini文件的串口号的值 不在 注册表的值里面。那就设置为注册表的第一个值。防止ini被修改造成数据不对
		int findresult = 0;
		for (int i =0;i<m_Struct_Com.GetCount();i++)
		{
			if (m_Com_Info_Struct.m_iPortNum == m_Struct_Com[i].m_iNum)
			{
				findresult = 1;
				//设置当前的串口值
				m_ComboBox_PortSet.SetCurSel(i);
				break;
			}
		}
		//表示没找到
		if (findresult == 0)
		{
			if (m_Struct_Com.GetCount()>0)
			{
				m_Com_Info_Struct.m_iPortNum = m_Struct_Com[0].m_iNum;
			}
			else
			{
				m_Com_Info_Struct.m_iPortNum = 1;
			}	
			m_ComboBox_PortSet.SetCurSel(0);
		}
		//同理查找波特率等如果被修改就设置成默认值
		switch (m_Com_Info_Struct.m_iBand)
		{
		case 4800:
			m_ComboBox_Band_Ctrl.SetCurSel(0);
			break;
		case 9600:
			m_ComboBox_Band_Ctrl.SetCurSel(1);
			break;
		case 19200:
			m_ComboBox_Band_Ctrl.SetCurSel(2);
			break;
		case 38400:
			m_ComboBox_Band_Ctrl.SetCurSel(3);
			break;
		case 57600:
			m_ComboBox_Band_Ctrl.SetCurSel(4);
			break;
		case 115200:
			m_ComboBox_Band_Ctrl.SetCurSel(5);
			break;
		default:
			//找不到就默认115200
			m_Com_Info_Struct.m_iBand = 115200;
			m_ComboBox_Band_Ctrl.SetCurSel(5);
			break;
		}
		//校验位
		switch (m_Com_Info_Struct.m_iCheckBit)
		{
		case 0:
			m_ComboBox_Check_Ctrl.SetCurSel(0);
			break;
		case 1:
			m_ComboBox_Check_Ctrl.SetCurSel(1);
			break;
		case 2:
			m_ComboBox_Check_Ctrl.SetCurSel(2);
			break;
		default:
			//找不到就默认无校验
			m_Com_Info_Struct.m_iCheckBit = 0;
			m_ComboBox_Check_Ctrl.SetCurSel(0);
			break;
		}
		//数据位
		switch (m_Com_Info_Struct.m_iDataBit)
		{
		case 6:
			m_ComboBox_Data_Ctrl.SetCurSel(0);
			break;
		case 7:
			m_ComboBox_Data_Ctrl.SetCurSel(1);
			break;
		case 8:
			m_ComboBox_Data_Ctrl.SetCurSel(2);
			break;
		default:
			//找不到就默认8
			m_Com_Info_Struct.m_iDataBit = 8;
			m_ComboBox_Data_Ctrl.SetCurSel(2);
			break;
		}
		//停止位
		switch (m_Com_Info_Struct.m_iStopBit)
		{
		case 0:
			m_ComboBox_Stop_Ctrl.SetCurSel(0);
			break;
		case 1:
			m_ComboBox_Stop_Ctrl.SetCurSel(1);
			break;		
		default:
			//找不到就默认8
			m_Com_Info_Struct.m_iStopBit = 1;
			m_ComboBox_Stop_Ctrl.SetCurSel(1);
			break;
		}
	}
	//没找到就采用默认值注册表中的第一个元素
	else
	{		
		if (m_Struct_Com.GetCount() > 0)
		{
			m_Com_Info_Struct.m_iPortNum = m_Struct_Com[0].m_iNum;
		}
		else
		{
			m_Com_Info_Struct.m_iPortNum = 1;
		}
		m_ComboBox_PortSet.SetCurSel(0);
		m_ComboBox_Band_Ctrl.SetCurSel(5);
		m_ComboBox_Check_Ctrl.SetCurSel(0);
		m_ComboBox_Data_Ctrl.SetCurSel(2);
		m_ComboBox_Stop_Ctrl.SetCurSel(1);
	}
	return 0;
}


BEGIN_MESSAGE_MAP(CDlgComSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgComSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgComSet 消息处理程序


void CDlgComSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用
	CString iniPath = _T("./RobotmonHConfig.ini");
	//写入ini文件
	//获取当前设置的串口号
	m_Com_Info_Struct.m_iPortNum = m_Struct_Com[m_ComboBox_PortSet.GetCurSel()].m_iNum;
	//获取当前设置的波特率
	CString tempStr;
	tempStr.Format(_T("%d"), m_Com_Info_Struct.m_iPortNum);
	::WritePrivateProfileString(_T("Database"), _T("ComPort"), tempStr, iniPath);
	
	switch (m_ComboBox_Band_Ctrl.GetCurSel())
	{
	case 0:
		m_Com_Info_Struct.m_iBand = 4800;
		break;
	case 1:
		m_Com_Info_Struct.m_iBand = 9600;
		break;
	case 2:
		m_Com_Info_Struct.m_iBand = 19200;
		break;
	case 3:
		m_Com_Info_Struct.m_iBand = 38400;
		break;
	case 4:
		m_Com_Info_Struct.m_iBand = 57600;
		break;
	case 5:
		m_Com_Info_Struct.m_iBand = 115200;
		break;
	}
	GetDlgItem(IDC_COMBO_BAND)->GetWindowTextW(tempStr);
	::WritePrivateProfileString(_T("Database"), _T("Band"), tempStr, iniPath);	

	m_Com_Info_Struct.m_iCheckBit = m_ComboBox_Check_Ctrl.GetCurSel();
	tempStr.Format(_T("%d"), m_Com_Info_Struct.m_iCheckBit);
	::WritePrivateProfileString(_T("Database"), _T("CheckBit"), tempStr, iniPath);

	m_Com_Info_Struct.m_iDataBit = m_ComboBox_Data_Ctrl.GetCurSel()+6;
	GetDlgItem(IDC_COMBO_DATA_BIT)->GetWindowTextW(tempStr);	
	::WritePrivateProfileString(_T("Database"), _T("DataBit"), tempStr, iniPath);


	m_Com_Info_Struct.m_iStopBit = m_ComboBox_Stop_Ctrl.GetCurSel();
	GetDlgItem(IDC_COMBO_STOP_BIT)->GetWindowTextW(tempStr);
	::WritePrivateProfileString(_T("Database"), _T("StopBit"), tempStr, iniPath);
	
	


	CDialogEx::OnOK();
}





void CDlgComSet::TraversalCom()
{
	HKEY   hKey;
	int cntCOM = 0;
	
	m_Struct_Com.RemoveAll();
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Hardware\\DeviceMap\\SerialComm"), NULL, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		TCHAR       szPortName[256], szComName[256];
		DWORD       dwLong, dwSize;
		int         nCount = 0;
		while (true)
		{
			dwLong = dwSize = 256;
			if (RegEnumValue(hKey, nCount, szPortName, &dwLong, NULL, NULL, (PUCHAR)szComName, &dwSize) == ERROR_NO_MORE_ITEMS)
				break;
			//获取串口号字符串
			CString str;
			str.Format(_T("%s"), szComName);
			//根据字符串获取串口号
			int num;
			num = _ttoi(str.Mid(3));
			Com_Port_Struct temp;
			temp.m_iNum = num;
			temp.m_strPort = str;
			temp.m_Index = nCount;
			m_Struct_Com.Add(temp);
			nCount++;
			cntCOM = nCount;//串口的数目

			//每读取一次dwLong 和dwSize 都会被修改 
			//注意一定要重置,否则会出现很离奇的错误,本人就试过因没有重置,出现先插入串口号大的（如COM4）,再插入串口号小的（如COM3），此时虽能发现两个串口，但都是同一串口号（COM4）的问题，同时也读不了COM大于10以上的串口 
			dwLong = sizeof(szPortName);
			dwSize = sizeof(szComName);
		}

		RegCloseKey(hKey);
	}

	
}
