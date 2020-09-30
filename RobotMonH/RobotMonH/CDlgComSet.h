#pragma once


// CDlgComSet 对话框
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

class CDlgComSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgComSet)

public:
	CDlgComSet(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgComSet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_COMSET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	void TraversalCom();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_ComboBox_PortSet;
	CArray<Com_Port_Struct> m_Struct_Com;
	Com_Info_Struct m_Com_Info_Struct;
	CComboBox m_ComboBox_Band_Ctrl;
	CComboBox m_ComboBox_Data_Ctrl;
	CComboBox m_ComboBox_Check_Ctrl;
	CComboBox m_ComboBox_Stop_Ctrl;
};
