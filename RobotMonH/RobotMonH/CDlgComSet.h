#pragma once


// CDlgComSet 对话框
#include "RobotMonHDlg.h"

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
