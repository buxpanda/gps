
// RobotMonHDlg.h: 头文件
//

#pragma once

struct Com_Port_Struct
{
	CString m_strPort;//实际串口号的cstring
	int		m_iNum;//实际的串口号值
	int		m_Index;//在下拉菜单的位置
};


struct Com_Info_Struct
{
	int		m_iPortNum;
	int		m_iBand;
	int		m_iDataBit;
	int		m_iCheckBit;//0 代表 None  1代表Even 2代表ODD
	int		m_iStopBit;
};
// CRobotMonHDlg 对话框
class CRobotMonHDlg : public CDialogEx
{
// 构造
public:
	CRobotMonHDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROBOTMONH_DIALOG };
#endif

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
	afx_msg void OnSetComDlg();



public:
	CArray<Com_Port_Struct> m_Struct_Com;
	Com_Info_Struct m_Com_Info_Struct;
};
