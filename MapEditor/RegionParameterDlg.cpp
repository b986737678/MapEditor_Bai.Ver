// RegionParameterDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapEditor.h"
#include "RegionParameterDlg.h"
#include "afxdialogex.h"


// CRegionParameterDlg 对话框

IMPLEMENT_DYNAMIC(CRegionParameterDlg, CDialogEx)

CRegionParameterDlg::CRegionParameterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REGION_PARAMETER, pParent)
{
	m_Pattern = 0;						//设置区型参数初始值
}

CRegionParameterDlg::~CRegionParameterDlg()
{
}

void CRegionParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REGION_PATTERN, m_ComboBox);
	DDX_Control(pDX, IDC_REGION_COLOR, m_ColorButton);
}


BEGIN_MESSAGE_MAP(CRegionParameterDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_REGION_PATTERN, &CRegionParameterDlg::OnSelchangeRegionPattern)
END_MESSAGE_MAP()


// CRegionParameterDlg 消息处理程序


void CRegionParameterDlg::OnSelchangeRegionPattern()
{
	m_Pattern = m_ComboBox.GetCurSel();	//设置当前选择的点型
}


BOOL CRegionParameterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ComboBox.SetCurSel(m_Pattern);	//设置m_ComboBox的初始值

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
