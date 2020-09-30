
// RobotMonHDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "RobotMonH.h"
#include "RobotMonHDlg.h"
#include "afxdialogex.h"

#include "CDlgComSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRobotMonHDlg 对话框



CRobotMonHDlg::CRobotMonHDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ROBOTMONH_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRobotMonHDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRobotMonHDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_32772, &CRobotMonHDlg::OnSetComDlg)
END_MESSAGE_MAP()


// CRobotMonHDlg 消息处理程序

BOOL CRobotMonHDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//读取本地com的配置文件
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
		for (int i = 0; i < m_Struct_Com.GetCount(); i++)
		{
			if (m_Com_Info_Struct.m_iPortNum == m_Struct_Com[i].m_iNum)
			{
				findresult = 1;
				//设置当前的串口值				
				break;
			}
		}
		//表示没找到
		if (findresult == 0)
		{
			if (m_Struct_Com.GetCount() > 0)
			{
				m_Com_Info_Struct.m_iPortNum = m_Struct_Com[0].m_iNum;
			}
			else
			{
				m_Com_Info_Struct.m_iPortNum = 1;
			}			
		}
		//同理查找波特率等如果被修改就设置成默认值
		switch (m_Com_Info_Struct.m_iBand)
		{
		case 4800:	
		case 9600:		
		case 19200:	
		case 38400:
		case 57600:
		case 115200:	
			break;
		default:
			//找不到就默认115200
			m_Com_Info_Struct.m_iBand = 115200;
			break;
		}
		//校验位
		if (m_Com_Info_Struct.m_iCheckBit>2|| m_Com_Info_Struct.m_iCheckBit<0)
		{
			m_Com_Info_Struct.m_iCheckBit = 0;
		}
		//数据位
		if (m_Com_Info_Struct.m_iDataBit>8|| m_Com_Info_Struct.m_iDataBit<6)
		{
			m_Com_Info_Struct.m_iDataBit = 8;
		}	
		//停止位
		if (m_Com_Info_Struct.m_iStopBit>1|| m_Com_Info_Struct.m_iDataBit<0)
		{	
			m_Com_Info_Struct.m_iStopBit = 1;
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
		m_Com_Info_Struct.m_iBand = 115200;
		m_Com_Info_Struct.m_iCheckBit = 0;
		m_Com_Info_Struct.m_iDataBit = 8;
		m_Com_Info_Struct.m_iStopBit = 1;
	}



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRobotMonHDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRobotMonHDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRobotMonHDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRobotMonHDlg::OnSetComDlg()
{
	// TODO: 在此添加命令处理程序代码
	CDlgComSet m_DlgComSet;
	//传递读取到的初始值
	m_DlgComSet.m_Com_Info_Struct = m_Com_Info_Struct;
	int res = m_DlgComSet.DoModal();
	if (res)
	{
		m_Com_Info_Struct = m_DlgComSet.m_Com_Info_Struct;
		//设置串口参数

	}
}
