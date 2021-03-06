#pragma once


// CLineParameterDlg 对话框

class CLineParameterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLineParameterDlg)

public:
	CLineParameterDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLineParameterDlg();
	int m_Pattern;								   //线型参数
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LINE_PARAMETER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboBox;
	CMFCColorButton m_ColorButton;
	afx_msg void OnSelchangeLinePattern();
	virtual BOOL OnInitDialog();
};
