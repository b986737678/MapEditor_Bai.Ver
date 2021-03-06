#pragma once


// CRegionParameterDlg 对话框

class CRegionParameterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegionParameterDlg)

public:
	CRegionParameterDlg(CWnd* pParent = nullptr);  // 标准构造函数
	virtual ~CRegionParameterDlg();
	int m_Pattern;								   //区型参数
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGION_PARAMETER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboBox;
	CMFCColorButton m_ColorButton;
	afx_msg void OnSelchangeRegionPattern();
	virtual BOOL OnInitDialog();
};
