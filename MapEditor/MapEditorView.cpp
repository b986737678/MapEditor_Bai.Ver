
// MapEditorView.cpp : CMapEditorView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MapEditor.h"
#endif

#include "MapEditorDoc.h"
#include "MapEditorView.h"

#include"CCreateFileDlg.h"

#include"Paint.h"
#include"WriteOrRead.h"
#include"Calculate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///-------------------------点数据相关的全局控制变量-------------------------///
bool	GPntFCreated = false;						//临时文件是否创建
CString GPntFName;									//永久文件名（含路径）
CString GPntTmpFName = CString("tempPntF.dat");		//临时文件名（含路径）
bool	GPntChanged = false;						//是否更改
int		GPntNum = 0;								//物理数
int		GPntLNum = 0;								//逻辑树
CFile	*GPntTmpF = new CFile();					//读取临时文件的指针对象

///-------------------------线数据相关的全局控制变量-------------------------///
bool	GLinFCreated = false;						//临时文件是否创建
CString	GLinFName;									//永久文件名（含路径）
CString	GLinTmpNdxFName = CString("tempLinF.ndx");	//临时索引文件名（含路径）
CString GLinTmpDatFName = CString("tempLinF.dat");	//临时数据文件名（含路径）
bool	GLinChanged = false;						//是否更改
int		GLinNum = 0;								//物理数
int		GLinLNum = 0;								//逻辑数
CFile	*GLinTmpDatF = new CFile();					//读取临时数据文件的指针对象
CFile	*GLinTmpNdxF = new CFile();					//读取临时索引文件的指针对象

///-------------------------区数据相关的全局控制变量-------------------------///
bool	GRegFCreated = false;						//临时文件是否创建
CString	GRegFName;									//永久文件名（含路径）
CString	GRegTmpNdxFName = CString("tempRegF.ndx");	//临时索引文件名（含路径）
CString GRegTmpDatFName = CString("tempRegF.dat");	//临时数据文件名（含路径）
bool	GRegChanged = false;						//是否更改
int		GRegNum = 0;								//物理数
int		GRegLNum = 0;								//逻辑数
CFile	*GRegTmpDatF = new CFile();					//读取临时数据文件的指针对象
CFile	*GRegTmpNdxF = new CFile();					//读取临时索引文件的指针对象

///-------------------------------与操作相关---------------------------------///
enum Action
{
	Noaction,
	OPERSTATE_INPUT_PNT,
	OPERSTATE_DELETE_PNT,
	OPERSTATE_MOVE_PNT,
	OPERSTATE_INPUT_LIN,
	OPERSTATE_DELETE_LIN,
	OPERSTATE_MOVE_LIN,
	OPERSTATE_INPUT_REG,
	OPERSTATE_DELETE_REG,
	OPERSTATE_MOVE_REG,
	OPERSTATE_ZOOM_IN,
	OPERSTATE_ZOOM_OUT,
	OPERSTATE_WINDOW_MOVE,
	OPERSTATE_LIN_DELETE_PNT,
	OPERSTATE_LIN_ADD_PNT,
	OPERSTATE_LINK_LIN,
	OPERSTATE_MODIFY_POINT_PARAMETER,
	OPERSTATE_MODIFY_LINE_PARAMETER,
	OPERSTATE_MODIFY_REGION_PARAMETER,
	OPERSTATE_UNDELETE_PNT,
	OPERSTATE_UNDELETE_LIN,
	OPERSTATE_UNDELETE_REG
};//枚举操作状态
Action GCurOperState;//操作参数

///-------------------------默认点结构与临时点结构---------------------------///
PNT_STRU GPnt = {	
	GPnt.isDel = 0,
	GPnt.color = RGB(0,0,0),
	GPnt.pattern = 0 
};													//默认点参数

///-------------------------文件版本信息的全局变量---------------------------///
VERSION GPntVer = {
	GPntVer.flag[0] = 'P',
	GPntVer.flag[1] = 'N',
	GPntVer.flag[2] = 'T',
	GPntVer.version = 10	//默认版本号
};

///-------------------------找到的点位于文件中的位置------------------------///
int GPntNdx = -1;

///-------------------------临时点，存储找到的点数据------------------------///
PNT_STRU GTPnt;
///------------------默认线索引结构、临时线索引结构及其相关-----------------///
LIN_NDX_STRU GLin = { 								//默认线结构
	GLin.isDel = 0,
	GLin.color = RGB(0,0,0),
	GLin.pattern = 0,
	GLin.dotNum = 0,
	GLin.datOff = 0 
};
LIN_NDX_STRU GTLin;									//线
POINT GLPnt = { GLPnt.x = -1,GLPnt.y = -1 };		//记录线段的起点
CPoint GMPnt(-1, -1);								//记录鼠标上一个状态的点
///---------------------线文件的版本信息，以及其初始化---------------------///
VERSION GLinVer =
{
	GLinVer.flag[0] = 'L',
	GLinVer.flag[1] = 'I',
	GLinVer.flag[2] = 'N',
	GLinVer.version = 10							//默认版本号
};
///-------------------------找到线位于文件中的位置-------------------------///
int GLinNdx = -1;

///---------------------------移动线相关全局变量---------------------------///
CPoint			GLinLBDPnt(-1, -1);					// 记录鼠标左键按下的位置，用来计算偏移量
CPoint			GLinMMPnt(-1, -1);					// 记录鼠标移动的上一个状态，用来擦除移动时的前一条线
long			GLinMMOffestX = 0;					// 记录鼠标移动时候的X轴的偏移量
long			GLinMMOffestY = 0;					// 记录鼠标移动时候的Y轴的偏移量
LIN_NDX_STRU	GLinMMTmpNdx;						// 记录鼠标选中的线的索引

///-------------------------关于放大缩小的全局变量-------------------------///
CPoint GZoomLBDPnt(-1, -1);							//放大时鼠标左键抬起的点
CPoint GZoomMMPnt(-1, -1);							//放大时鼠标移动前一状态

double	GZoomOffset_x = 0;							//偏移向量
double	GZoomOffset_y = 0;
double	GZoom = 1.0;								//缩放系数
int		GZoomStyle = 0;								//放大方式

///---------------------------连接线相关全局变量---------------------------///
LIN_NDX_STRU	GStartLin = GLin;					// 选中的第一条线
int				GnStart = -1;
LIN_NDX_STRU	GEndLin = GLin;						// 选中的第二条线
int				GnEnd = -1;
int				GnLine = 0;

// CMapEditorView

IMPLEMENT_DYNCREATE(CMapEditorView, CView)

BEGIN_MESSAGE_MAP(CMapEditorView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_WINDOW_ZOOM_IN, &CMapEditorView::OnWindowZoomIn)
	ON_COMMAND(ID_FILE_SAVE_POINT, &CMapEditorView::OnFileSavePoint)
	ON_COMMAND(ID_FILE_NEW, &CMapEditorView::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN_POINT, &CMapEditorView::OnFileOpenPoint)
	ON_COMMAND(ID_FILE_OPEN_LINE, &CMapEditorView::OnFileOpenLine)
	ON_COMMAND(ID_FILE_OPEN_REGION, &CMapEditorView::OnFileOpenRegion)
	ON_COMMAND(ID_FILE_SAVE_LINE, &CMapEditorView::OnFileSaveLine)
	ON_COMMAND(ID_FILE_SAVE_REGION, &CMapEditorView::OnFileSaveRegion)
	ON_COMMAND(ID_FILE_SAVE_AS_POINT, &CMapEditorView::OnFileSaveAsPoint)
	ON_COMMAND(ID_FILE_SAVE_AS_LINE, &CMapEditorView::OnFileSaveAsLine)
	ON_COMMAND(ID_FILE_SAVE_AS_REGION, &CMapEditorView::OnFileSaveAsRegion)
	ON_COMMAND(ID_APP_EXIT, &CMapEditorView::OnAppExit)
	ON_COMMAND(ID_WINDOW_ZOOM_OUT, &CMapEditorView::OnWindowZoomOut)
	ON_COMMAND(ID_WINDOW_MOVE, &CMapEditorView::OnWindowMove)
	ON_COMMAND(ID_WINDOW_RESET, &CMapEditorView::OnWindowReset)
	ON_COMMAND(ID_WINDOW_SHOW_POINT, &CMapEditorView::OnWindowShowPoint)
	ON_COMMAND(ID_WINDOW_SHOW_LINE, &CMapEditorView::OnWindowShowLine)
	ON_COMMAND(ID_WINDOW_SHOW_REGION, &CMapEditorView::OnWindowShowRegion)
	ON_COMMAND(ID_POINT_CREATE, &CMapEditorView::OnPointCreate)
	ON_COMMAND(ID_POINT_MOVE, &CMapEditorView::OnPointMove)
	ON_COMMAND(ID_POINT_DELETE, &CMapEditorView::OnPointDelete)
	ON_COMMAND(ID_POINT_SET_DEFPARAMETER, &CMapEditorView::OnPointSetDefparameter)
	ON_COMMAND(ID_POINT_SHOW_DELETED, &CMapEditorView::OnPointShowDeleted)
	ON_COMMAND(ID_POINT_UNDELETE, &CMapEditorView::OnPointUndelete)
	ON_COMMAND(ID_POINT_MODIFY_PARAMETER, &CMapEditorView::OnPointModifyParameter)
	ON_COMMAND(ID_LINE_CREATE, &CMapEditorView::OnLineCreate)
	ON_COMMAND(ID_LINE_MOVE, &CMapEditorView::OnLineMove)
	ON_COMMAND(ID_LINE_DELETE, &CMapEditorView::OnLineDelete)
	ON_COMMAND(ID_LINE_SHOW_DELETED, &CMapEditorView::OnLineShowDeleted)
	ON_COMMAND(ID_LINE_UNDELETED, &CMapEditorView::OnLineUndeleted)
	ON_COMMAND(ID_LINE_DELETE_DOT, &CMapEditorView::OnLineDeleteDot)
	ON_COMMAND(ID_LINE_ADD_DOT, &CMapEditorView::OnLineAddDot)
	ON_COMMAND(ID_LINE_LINK, &CMapEditorView::OnLineLink)
	ON_COMMAND(ID_LINE_MODIFY_PARAMETER, &CMapEditorView::OnLineModifyParameter)
	ON_COMMAND(ID_LINE_SET_DEFPARAMETER, &CMapEditorView::OnLineSetDefparameter)
	ON_COMMAND(ID_REGION_CREATE, &CMapEditorView::OnRegionCreate)
	ON_COMMAND(ID_REGION_MOVE, &CMapEditorView::OnRegionMove)
	ON_COMMAND(ID_REGION_DELETE, &CMapEditorView::OnRegionDelete)
	ON_COMMAND(ID_REGION_SHOW_DELETED, &CMapEditorView::OnRegionShowDeleted)
	ON_COMMAND(ID_REGION_UNDELETED, &CMapEditorView::OnRegionUndeleted)
	ON_COMMAND(ID_REGION_MODIFY_PARAMETER, &CMapEditorView::OnRegionModifyParameter)
	ON_COMMAND(ID_REGION_SET_DEFPARAMETER, &CMapEditorView::OnRegionSetDefparameter)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CMapEditorView 构造/析构

CMapEditorView::CMapEditorView()
{
	// TODO: 在此处添加构造代码

}

CMapEditorView::~CMapEditorView()
{
}

BOOL CMapEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMapEditorView 绘制

void CMapEditorView::OnDraw(CDC* /*pDC*/)
{
	CMapEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CRect mrect;
	GetClientRect(&mrect);				//获取窗口客户区的坐标
	CClientDC dc(this);
	dc.FillSolidRect(0, 0, mrect.Width(), mrect.Height(), dc.GetBkColor());//用一个单色填充一个矩形
	dc.SetROP2(R2_NOTXORPEN);			//设置绘图模式
	//ShowAllPnt(&dc, GPntTmpF, GPntNum);	//绘制显示所有点
	//ShowAllLin(&dc, GLinTmpNdxF, GLinTmpDatF, GLinNum);//绘制显示所有线
	//绘制显示所有点
	ShowAllPnt(&dc, GPntTmpF, GPntNum, GZoomOffset_x, GZoomOffset_y, GZoom, 0);
	//绘制显示所有线
	ShowAllLin(&dc, GLinTmpNdxF, GLinTmpDatF, GLinNum, GZoomOffset_x, GZoomOffset_y, GZoom, 0);

	ReleaseDC(&dc);						//释放dc
}

void CMapEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	/*ClientToScreen(&point);
	OnContextMenu(this, point);*/
	CClientDC dc(this);
	dc.SetROP2(R2_NOTXORPEN);
	if (GLinFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_LIN:
			if (GTLin.dotNum > 1)
			{
				WriteLinNdxToFile(GLinTmpNdxF, GLinNum, GTLin);//将线索引文件写入线临时索引文件中
				GLinNum++;
				GLinLNum++;
				DrawSeg(&dc, GTLin, GLPnt, point);
				GLin.datOff += (GTLin.dotNum * sizeof(D_DOT));
				memset(&GTLin, 0, sizeof(LIN_NDX_STRU));
				GMPnt.SetPoint(-1, -1);
				GLPnt.x = -1;
				GLPnt.y = -1;
			}
			else if (GTLin.dotNum == 1)
			{
				DrawSeg(&dc, GTLin, GLPnt, point);
				memset(&GTLin, 0, sizeof(LIN_NDX_STRU));
				GMPnt.SetPoint(-1, -1);
				GLPnt.x = -1;
				GLPnt.y = -1;
			}
			break;
		default:
			break;
		}
	}
}

void CMapEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMapEditorView 诊断

#ifdef _DEBUG
void CMapEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CMapEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapEditorDoc* CMapEditorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapEditorDoc)));
	return (CMapEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapEditorView 消息处理程序

void CMapEditorView::OnFileNew()
{
	// TODO: 在此添加命令处理程序代码
																//检测是否已新建临时文件，已有临时文件则返回
	if (GPntFCreated&&GLinFCreated&&GRegFCreated)
	{
		MessageBox(L"File have been created.",L"Message",MB_OK);
		return;
	}
	CCreateFileDlg dlg;											//创建“新建临时文件”对话框对象
	if (dlg.DoModal() != IDOK)									//判断当前操作是否为“确定”按钮，不是则返回
		return;
	CString str;												//创建输出信息对象
	if (!GPntFCreated)											//判断点临时文件是否存在，不存在则新建
	{
		GPntTmpFName = dlg.m_add + CString("\\") + GPntTmpFName;//临时点数据文件名
		if (GPntTmpF->Open(GPntTmpFName, CFile::modeCreate 
			| CFile::modeReadWrite | CFile::typeBinary))
		{
			GPntFCreated = true;								//设置点临时文件新建成功标志值
			str += "tempPntF.dat\n";
		}
		else
		{
			GPntTmpFName = CString("tempPntF.dat");
			TRACE(_T("File could not be opened n"));
		}
	}
	if (!GLinFCreated)												  //判断线临时文件是否存在，不存在则新建
	{
		GLinTmpNdxFName = dlg.m_add + CString("\\") + GLinTmpNdxFName;//临时线索引文件名
		GLinTmpDatFName = dlg.m_add + CString("\\") + GLinTmpDatFName;//临时线数据文件名
		if (GLinTmpNdxF->Open(GLinTmpNdxFName, CFile::modeCreate 
			| CFile::modeReadWrite | CFile::typeBinary)&&GLinTmpDatF->Open(GLinTmpDatFName,
				CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary))
		{
			GLinFCreated = true;									  //设置线临时文件新建成功标志值
			str += "tempLinF.dat tempLinF.ndx\n";
		}
		else
		{
			GLinTmpDatFName = CString("tempLinF.dat");
			GLinTmpNdxFName = CString("tempLinF.ndx");
			TRACE(_T("File could not be opened \n"));
		}
	}
	if (!GRegFCreated)												  //判断区临时文件是否存在，不存在则新建
	{
		GRegTmpNdxFName = dlg.m_add + CString("\\") + GRegTmpNdxFName;//区临时索引文件名
		GRegTmpDatFName = dlg.m_add + CString("\\") + GRegTmpDatFName;//区临时数据文件名
		if (GRegTmpNdxF->Open(GRegTmpNdxFName, CFile::modeCreate
			| CFile::modeReadWrite | CFile::typeBinary)&&GRegTmpDatF->Open(GRegTmpDatFName,
				CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary))
		{
			GRegFCreated = true;									  //设置区临时文件新建成功标志
			str += "tempRegF.dat tempRegF.ndx\n";
		}
		else
		{
			GRegTmpNdxFName = CString("tempRegF.ndx");
			GRegTmpDatFName = CString("tempRegF.dat");
			TRACE(_T("File could not be opened \n"));
		}
	}
	if (GPntFCreated&&GLinFCreated&&GRegFCreated)
	{
		str += "create successful !";
		MessageBox(str, L"message", MB_OK);							  //新建临时文件成功弹出提示框
	}
	//dlg.DoModal();
}


void CMapEditorView::OnWindowZoomIn()
{
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		GCurOperState = OPERSTATE_ZOOM_IN;							  //当前为放大操作状态
	}
	else
	{
		MessageBox(L"TempFile have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnFileSavePoint()
{
	// TODO: 在此添加命令处理程序代码
	GCurOperState = Noaction;
	//1.如果还没有新建或打开点文件，则提示文件还没有打开，然后返回
	if (GPntFCreated == false)
	{
		MessageBox(L"File have not been created.", L"Message", MB_OK);
		return;
	}
	CFile*pntF = new CFile();
	//2.如果点文件不为空，则删除原来的文件，否则调用CFileDialog类让用户输入文件名
	if (GPntFName.IsEmpty() == false)
	{
		CFile::Remove(GPntFName);
	}
	else
	{
		LPCTSTR lpszFilters;
		lpszFilters = _T("点(*.pnt)|*.pnt||");
		CFileDialog dlg(false, _T("pnt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilters);
		int nPos = GPntTmpFName.ReverseFind(_T('\\'));
		CString folderAdd = GPntTmpFName.Left(nPos);
		dlg.m_ofn.lpstrInitialDir = folderAdd;
		if (dlg.DoModal() == IDOK)
		{
			GPntFName = dlg.GetPathName();
		}
		else
		{
			return;
		}
	}
	PNT_STRU tempPnt;
	//3.重新创建点永久文件，并写入版本信息，点物理数和逻辑数
	if (pntF->Open(GPntFName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		pntF->Write(&GPntVer, sizeof(VERSION));
		pntF->Write(&GPntNum, sizeof(int));
		pntF->Write(&GPntLNum, sizeof(int));
	}
	else
	{
		TRACE(_T("File could not be opened\n"));
	}
	//4.将点临时文件中的信息逐条写到永久文件中
	for (int i = 0; i < GPntNum; i++)
	{
		ReadTempFileToPnt(GPntTmpF, i, tempPnt);
		pntF->Write(&tempPnt, sizeof(PNT_STRU));
	}
	pntF->Close();
	delete pntF;
	//5.修改数据变化标志，改变主窗口标题名称
	GPntChanged = false;
	int nPos = GPntFName.ReverseFind(_T('\\'));
	CString windowText = GPntFName.Right(GPntFName.GetLength() - nPos - 1) + "-MapEditor";
	GetParent()->SetWindowTextW(windowText);
}




void CMapEditorView::OnFileOpenPoint()
{
	CFileDialog dlg(true);
	dlg.m_ofn.lpstrFilter = L"pnt\0*.pnt";
	//如果临时文件中的数据已经改变，则提示是否保存，保存则调用OnFileSavePoint()
	if (GPntChanged == true)
	{
		if (IDYES == AfxMessageBox(L"File has not been saved.Does save File?"), MB_YESNO, MB_ICONQUESTION)
			OnFileSavePoint();
	}
	if (dlg.DoModal() == IDOK)								//弹出打开文件对话框让用户指定要打开的文件
	{
		GPntFName = dlg.m_ofn.lpstrFile;					//永久文件(含路径)
		CFile*pntF = new CFile();
		if (!pntF->Open(GPntFName, CFile::modeRead | CFile::typeBinary))
		{
			TRACE(_T("File could not be opened \n"));
			return;
		}
		int nPos = GPntFName.ReverseFind(_T('\\'));
		CString floderAdd = GPntFName.Left(nPos);
		if (GPntTmpFName != "tempPntF.dat")					//如果临时点文件已经创建则定位到文件开头
		{
			GPntTmpF->SeekToBegin();
		}
		else
		{													//否则创建临时文件
			GPntTmpFName = floderAdd + CString("\\") + GPntTmpFName;
			if (!GPntTmpF->Open(GPntTmpFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GPntTmpFName = CString("tempPntF.dat");
				TRACE(_T("File could not be opened \n"));
			}
			else
			{
				GPntFCreated = true;
			}
		}
		ReadPntPermanentFileToTemp(pntF, GPntTmpF, GPntNum, GPntLNum);//读点永久文件到临时文件
		pntF->Close();
		delete pntF;
		CString windowText = dlg.GetFileName() + "-MapEditor";
		GetParent()->SetWindowTextW(windowText);
		this->InvalidateRect(NULL);							//让视窗口无效，触发MFC调用OnDraw函数重绘窗口
	}
	GCurOperState = Noaction;
}


void CMapEditorView::OnFileOpenLine()
{
	CFileDialog dlg(true);
	dlg.m_ofn.lpstrFilter = L"lin\0*.lin";
	if (GLinChanged == true)
	{
		if (IDYES == AfxMessageBox(L"File have not been saved.Does save File?", MB_YESNO, MB_ICONQUESTION))
		{
			OnFileSaveLine();								//保存线从临时文件写入永久文件中）
		}
	}
	if (dlg.DoModal() == IDOK)
	{
		GLinFCreated = false;
		int IsCreate = 0;
		GLinFName = dlg.m_ofn.lpstrFile;					//永久文件（含路径）
		CFile * LinF = new CFile();
		if (!LinF->Open(GLinFName, CFile::modeRead | CFile::typeBinary))
		{
			TRACE(_T("File could not be opened.\n"));
			return;
		}
		int nPos = GLinFName.ReverseFind(_T('\\'));
		CString floderAdd = GLinFName.Left(nPos);
		if (GLinTmpDatFName != "tempLinF.dat")
		{
			GLinTmpDatF->SeekToBegin();
			++IsCreate;
		}
		else
		{
			GLinTmpDatFName = floderAdd + CString("\\") + GLinTmpDatFName;
			if (!GLinTmpDatF->Open(GLinTmpDatFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GLinTmpDatFName = CString("tempLinF.dat");
				TRACE(_T("File could not be opened\n"));
			}
			else
			{
				++IsCreate;
			}
		}
		if (GLinTmpNdxFName != "tempLinF.ndx")
		{
			GLinTmpNdxF->SeekToBegin();
			++IsCreate;
		}
		else
		{
			GLinTmpNdxFName = floderAdd + CString("\\") + GLinTmpNdxFName;
			if (!GLinTmpNdxF->Open(GLinTmpNdxFName, CFile::modeCreate | CFile::
				modeReadWrite | CFile::typeBinary))
			{
				GLinTmpNdxFName = CString("tempLinF.ndx");
				TRACE(_T("File could not be opened\n"));
			}
			else
			{
				++IsCreate;
			}
		}
		if (2 == IsCreate)
		{
			GLinFCreated = true;
		}
		ReadLinPermanentFileToTemp(LinF, GLinTmpDatF, GLinTmpNdxF, GLinVer, 
			GLinNum, GLinLNum, GLin.datOff);				//从永久文件读取线数据到临时文件
		LinF->Close();										//关闭线永久文件
		delete LinF;
		CString windowTest = dlg.GetFileName() + "-MapEditor";
		GetParent()->SetWindowTextW(windowTest);
		this->InvalidateRect(NULL);							//重绘窗口显示
		GCurOperState = Noaction;
	}
}


void CMapEditorView::OnFileOpenRegion()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnFileSaveLine()
{
	if (GLinFCreated)
	{														//已经存在临时文件
		CFile*LinF = new CFile();
		if (GLinFName.IsEmpty())
		{
			LPCTSTR lpszFilters;
			lpszFilters = _T("线(*.lin)|*.lin||");
			CFileDialog dlg(false, _T("lin"), NULL, OFN_HIDEREADONLY 
				| OFN_OVERWRITEPROMPT, lpszFilters);		//保存线的对话框
			int nPos = GLinTmpDatFName.ReverseFind(_T('\\'));
			CString folderAdd = GLinTmpDatFName.Left(nPos);
			dlg.m_ofn.lpstrInitialDir = folderAdd;
			if (dlg.DoModal() == IDOK)
			{
				GLinFName = dlg.GetPathName();				//线文件的名称
			}
			else
			{
				return;
			}
		}
		else
		{
			LinF->Remove(GLinFName);
		}
		if (!LinF->Open(GLinFName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			TRACE(_T("File could not be opened \n"));
			return;
		}
		WriteTempToLinPermanentFile(LinF, GLinTmpDatF, GLinTmpNdxF, GLinVer, 
			GLinNum, GLinLNum);								//将线的索引和点数据写入永久文件
		LinF->Close();
		delete LinF;

		GLinChanged = false;								//线数据无变更
		int nPos = GLinFName.ReverseFind(_T('\\'));
		CString windowText = GLinFName.Right(GLinFName.GetLength() - nPos - 1) 
			+ " -MapEditor";
		GetParent()->SetWindowTextW(windowText);
	}
	else
	{
		MessageBox(L"File have not been created.", L"Message", MB_OK);
	}
	GCurOperState = Noaction;
	
}


void CMapEditorView::OnFileSaveRegion()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnFileSaveAsPoint()
{
	// TODO: 在此添加命令处理程序代码
	CString tempFName = GPntFName;							//保留原点永久文件名
	GPntFName = CString("");								//将点永久文件名设为空
	OnFileSavePoint();										//调用OnFileSavePoint函数，永久文件名为空时自动弹出文件名对话框
	if (GPntFName == "")									//若另存失败，则还原原永久文件名
	{
		GPntFName = tempFName;
	}
}


void CMapEditorView::OnFileSaveAsLine()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnFileSaveAsRegion()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnAppExit()
{
	// TODO: 在此添加命令处理程序代码
	
															//1.如果点数据已改变，则保存
	if (GPntChanged == true)
	{
		if (IDYES == AfxMessageBox(L"File has not been saved.Does save File?"), MB_YESNO, MB_ICONQUESTION)
		{
			OnFileSavePoint();								//保存点
		}
	}
															//2.调用父窗口CMainFrame的销毁窗口函数DestroyWindow()
	GetParent()->DestroyWindow();
}


void CMapEditorView::OnWindowZoomOut()
{
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		GCurOperState = OPERSTATE_ZOOM_OUT;					//设置为缩小操作状态
	}
	else
	{
		MessageBox(L"TempFile have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnWindowMove()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnWindowReset()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnWindowShowPoint()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnWindowShowLine()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnWindowShowRegion()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnPointCreate()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_INPUT_PNT;				//设置为“造点”状态
	}
	else
	{
		MessageBox(L"File have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnPointMove()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_MOVE_PNT;					//设置操作状态（移动点）
	}
	else
	{
		MessageBox(L"TempFile have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnPointDelete()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_DELETE_PNT;				//设置操作状态
	}
	else
	{
		MessageBox(L"TempFile has not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnPointSetDefparameter()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnPointShowDeleted()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnPointUndelete()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnPointModifyParameter()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLineCreate()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_INPUT_LIN;				//当前为造线状态
	}
	else
	{
		MessageBox(L"TempFile have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineMove()
{
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_MOVE_LIN;					//当前为移动线操作状态
	}
	else
	{
		MessageBox(L"TempFile have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineDelete()
{
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_DELETE_LIN;				//当前状态为删除线

	}
	else
	{
		MessageBox(L"Temp File have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineShowDeleted()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLineUndeleted()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLineDeleteDot()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLineAddDot()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLineLink()
{
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_LINK_LIN;					//设置为连接线操作状态
	}
	else
	{
		MessageBox(L"TempFile have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineModifyParameter()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLineSetDefparameter()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnRegionCreate()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnRegionMove()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnRegionDelete()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnRegionShowDeleted()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnRegionUndeleted()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnRegionModifyParameter()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnRegionSetDefparameter()
{
	// TODO: 在此添加命令处理程序代码
}


void CMapEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (GPntFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_PNT:									//当前为移动点状态
			D_DOT dot;
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);	// 坐标系转换
			DotToPnt(point, dot);
			GTPnt = FindPnt(point, GPntNum, GPntTmpF, GPntNdx);		// 查找最近点
			dot.x = GTPnt.x;
			dot.y = GTPnt.y;
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);	// 坐标系转换
			GTPnt.x = dot.x;
			GTPnt.y = dot.y;
			break;
		default:
			break;
		}
	}
	if (GLinFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_LIN:									//当前为移动线状态
		{
			D_DOT dot;
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);	// 坐标系转换
			DotToPnt(point, dot);
			GLinMMTmpNdx = FindLin(GLinTmpNdxF, GLinTmpDatF, point, 
				GLinNum, GLinNdx);									//查找单击点最近的一条线
			GLinMMOffestX = 0;
			GLinMMOffestY = 0;
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);	// 坐标系转换
			DotToPnt(point, dot);
			GLinLBDPnt = point;
			GLinMMPnt = point;
			break;
		}
		default:
			break;
		}
	}
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_ZOOM_IN:								//当前为放大操作状态
			GZoomLBDPnt = point;
			GZoomMMPnt = point;
			break;
		default:
			break;
		}
	}

	CView::OnLButtonDown(nFlags, point);
}


void CMapEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CClientDC dc(this);										//画笔，定义dc时调用构造函数	
	dc.SetROP2(R2_NOTXORPEN);								//绘图的模式设置
	if (GPntFCreated)										//已创建临时文件
	{
		D_DOT dot;
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_PNT:							//当前为绘制点状态
			PNT_STRU pnt;									//点对象
			memcpy_s(&pnt, sizeof(PNT_STRU), &GPnt, sizeof(PNT_STRU));
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//坐标系转换
			pnt.x = dot.x;									//设置点坐标的x
			pnt.y = dot.y;									//设置点坐标的y
			WritePntToFile(GPntTmpF, GPntNum, pnt);			//将点写入临时文件
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//坐标系转换
			pnt.x = dot.x;
			pnt.y = dot.y;
			DrawPnt(&dc, pnt);								//绘制点
			GPntNum++;										//点物理数加1
			GPntLNum++;										//点逻辑数加1
			GPntChanged = true;								//是否更改标志设置为true
			break;
		case OPERSTATE_DELETE_PNT:							//当前为删除点状态
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//转换坐标系
			DotToPnt(point, dot);
			FindPnt(point, GPntNum, GPntTmpF, GPntNdx);
			if (GPntNdx != -1)								//如果找到
			{
				PNT_STRU pnt;
				ReadTempFileToPnt(GPntTmpF, GPntNdx, pnt);	//从临时文件读点
				pnt.isDel = 1;								//删除标记设置为1
				UpdatePnt(GPntTmpF, GPntNdx, pnt);			//更新该点数据
				dot.x = pnt.x;
				dot.y = pnt.y;
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//转换坐标系
				pnt.x = dot.x;
				pnt.y = dot.y;
				DrawPnt(&dc,pnt);							//异或模式重绘该点以清除屏幕
				GPntNdx = -1;
				GPntChanged = true;							//数据发生变更
				GPntLNum--;									//删除一个点，逻辑数减1，但物理存储不变
			}
			break;
		case OPERSTATE_MOVE_PNT:							//当前为移动点操作状态
			if (GPntNdx != -1)
			{
				PNT_STRU pnt;
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);// 坐标系转换
				ReadTempFileToPnt(GPntTmpF, GPntNdx, pnt);	// 从临时文件读取点
				pnt.x = dot.x;								// 移动后的点坐标x
				pnt.y = dot.y;								// 移动后的点坐标y
				UpdatePnt(GPntTmpF, GPntNdx, pnt);			// 更新点数据(写入临时文件)
				GPntNdx = -1;
				GPntChanged = true;							//数据发生变更
			}
			break;
		default:
			break;
		}
	}
	if (GLinFCreated)
	{
		D_DOT dot;
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_LIN:							//当前为绘制线状态
			if (GTLin.dotNum == 0)
				memcpy_s(&GTLin, sizeof(LIN_NDX_STRU), &GLin,
					sizeof(LIN_NDX_STRU));
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);// 坐标系转换
			WriteLinDatToFile(GLinTmpDatF, GLin.datOff, GTLin.dotNum, dot);
															//将线的点数据写入临时文件中
			GTLin.dotNum++;									//线节点处加1
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);// 坐标系转换
			GLPnt.x = (long)dot.x;							//设置线段的起点(x)
			GLPnt.y = (long)dot.y;							//设置线段的起点(y)
			GLinChanged = true;								//线数据变更
			break;
		case OPERSTATE_DELETE_LIN:							//当前为删除线状态
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);// 坐标系转换
			DotToPnt(point, dot);
			FindLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx);//找最近线
			if (GLinNdx != -1)
			{
				GLinNum--;
				GLinChanged = true;							//线数据变更
				LIN_NDX_STRU TmpLinNdx;
				D_DOT dot1, dot2;
				POINT pnt1, pnt2;
															//从临时线索引文件中读取线索引
				ReadTempFileToLinNdx(GLinTmpNdxF, GLinNdx, TmpLinNdx);
				TmpLinNdx.isDel = 1;						//设置删除标志
				UpdateLin(GLinTmpNdxF, GLinNdx, TmpLinNdx);	//更新线数据
				for (int i = 0; i < TmpLinNdx.dotNum - 1; ++i)
				{
															//从临时线的点数据文件中读取点
					ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i, dot1);
					ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i + 1, dot2);
															//坐标系转换（数据转窗口坐标系）
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					DrawSeg(&dc, TmpLinNdx, pnt1, pnt2);	//重绘（异或模式擦除）
				}
				GLinNdx = -1;
			}
			break;
		case OPERSTATE_MOVE_LIN:							//当前为移动线操作状态
			if (GLinNdx != -1)
			{
				if (GLinLBDPnt.x != -1 && GLinLBDPnt.y != -1)
				{
					D_DOT dot1, dot2;
					PntToDot(dot1, point);
					PntVPtoDP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);// 坐标系转换
					PntToDot(dot2, GLinLBDPnt);
					PntVPtoDP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);// 坐标系转换
					double offset_x = dot1.x - dot2.x;
					double offset_y = dot1.y - dot2.y;
					UpdateLin(GLinTmpNdxF, GLinTmpDatF, GLinNdx, offset_x, offset_y);
					GLinNdx = -1;
					GLinMMOffestX = 0;
					GLinMMOffestY = 0;
					GLinChanged = true;
				}
			}
			break;
		case OPERSTATE_LINK_LIN:							//当前为连接线状态
			if (GnLine < 2)
			{
				LIN_NDX_STRU line;
				D_DOT dot;
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);// 坐标系转换
				DotToPnt(point, dot);
															// 查找鼠标点位置最近的线
				line = FindLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx);
				if (GLinNdx != -1)
				{
					GnLine++;
					if (GnLine == 1)						// 选中第一条线
					{
						GStartLin = line;
						GnStart = GLinNdx;
					}
					else if (GnLine == 2)					// 选中第二条线
					{
						if (GnStart != GLinNdx)
						{
							GEndLin = line;
							GnEnd = GLinNdx;
						}
						else
						{
							GnLine--;
						}
					}
				}
				if (GnLine != 0)
				{
					D_DOT pt;
					if (GnLine == 1)						// 选中第一条线，其端点画圆标记
					{
															// 从临时文件中读取线的起点并将其转为窗口坐标，画圆
						ReadTempFileToLinDat(GLinTmpDatF, GStartLin.datOff, 0, pt);
						PntDPtoVP(pt, GZoom, GZoomOffset_x, GZoomOffset_y);
						dc.Ellipse((long)pt.x - 2, (long)pt.y - 2, (long)pt.x + 2, (long)pt.y + 2);
															// 从临时文件中读取线的终点并将其转为窗口坐标，画圆
						ReadTempFileToLinDat(GLinTmpDatF, GStartLin.datOff, GStartLin.dotNum - 1, pt);
						PntDPtoVP(pt, GZoom, GZoomOffset_x, GZoomOffset_y);
						dc.Ellipse((long)pt.x - 2, (long)pt.y - 2, (long)pt.x + 2, (long)pt.y + 2);
					}
					else									//选中第二条线，连接线
					{
															// 改变线的点数据，即将连接线的点数据写入文件中
						AlterLindot(GLinTmpDatF, GStartLin, GEndLin, GnStart, GnEnd, GLin.datOff);
						AlterStartLin(GLinTmpNdxF, GLin.datOff, GnStart, GStartLin.dotNum + GEndLin.dotNum);// 修改第一条线索引
						AlterEndLin(GLinTmpNdxF, GnEnd);	// 修改第二条线索引
						GLin.datOff += (GStartLin.dotNum + GEndLin.dotNum) * sizeof(D_DOT);// 连接线索引
						GnLine = 0;
						GLinLNum--;
						GLinChanged = true;
						GnStart = -1;
						GnEnd = -1;
						GLinNdx = -1;
						this->Invalidate();
					}
				}
			}
			break;
		default:
			break;
		}
	}
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		RECT client, rect;
		double zoom = 1.0;
		switch (GCurOperState)
		{
		case OPERSTATE_ZOOM_IN:								//当前为放大操作状态
			GetClientRect(&client);
			if (abs(GZoomLBDPnt.x - GZoomMMPnt.x) <= 10 && 
				abs(GZoomLBDPnt.y - GZoomMMPnt.y) <= 10)
			{
				GZoomStyle = 0;								//单击放大
			}
			else
			{
				GZoomStyle = 1;								//拉框放大
			}
			if (GZoomStyle == 0)
			{												//单击放大
				double x0 = point.x - (client.right / 2.0) + (client.right / 8.0);
				double y0 = point.y - (client.bottom / 2.0) + (client.bottom / 8.0);
				GZoomOffset_x += (x0 / GZoom);				//偏移向量x
				GZoomOffset_y += (y0 / GZoom);				//偏移向量y
				GZoom *= 4 / 3.0;							//缩放系数是4/3
			}
			else
			{												//拉框放大
				rect.right = max(point.x, GZoomLBDPnt.x);
				rect.left = min(point.x, GZoomLBDPnt.x);
				rect.bottom = max(point.y, GZoomLBDPnt.y);
				rect.top = min(point.y, GZoomLBDPnt.y);
				modulusZoom(client, rect, zoom);
				double x0 = GetCenter(rect).x - (client.right / 2.0) + 
					(client.right *(zoom - 1) / (2.0* zoom));
				double y0 = GetCenter(rect).y - (client.bottom / 2.0) + 
					(client.bottom* (zoom - 1) / (2.0* zoom));
				GZoomOffset_x += (x0 / GZoom);				//偏移向量x
				GZoomOffset_y += (y0 / GZoom);				//偏移向量y
				GZoom *= zoom;								//缩放系数
				GZoomStyle = 0;
			}
			GZoomLBDPnt = CPoint(-1, -1);
			GZoomMMPnt = CPoint(-1, -1);
			this->Invalidate();
			break;
		case OPERSTATE_ZOOM_OUT:							//当前为缩小操作状态
			if (true)
			{
				GetClientRect(&client);
				double x0 = point.x - (client.right / 2.0) - (client.right / 8.0);
				double y0 = point.y - (client.bottom / 2.0) + (client.bottom / 8.0);
				GZoomOffset_x += (x0 / GZoom);				//偏移向量x
				GZoomOffset_y += (y0 / GZoom);				//偏移向量y
				GZoom *= 3 / 4.0;							//缩放系数为3/4
				this->Invalidate();
			}
			break;
		default:
			break;
		}
	}
	CView::OnLButtonUp(nFlags, point);
}


void CMapEditorView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (GPntFCreated)//如果点临时文件已创建，则关闭并删除
	{
		if (GPntTmpF->m_hFile != CFile::hFileNull)
		{
			GPntTmpF->Close();
			GPntTmpF->Remove(GPntTmpFName);
		}
	}
	delete GPntTmpF;		//删除点临时文件对象
	//线、区相关功能尚未实现，但先把以下代码加上
	if (GLinFCreated)		//如果线临时文件已创建，则关闭并删除
	{
		if (GLinTmpDatF->m_hFile != CFile::hFileNull)
		{
			GLinTmpDatF->Close();
			GLinTmpDatF->Remove(GLinTmpDatFName);
		}
		if (GLinTmpNdxF->m_hFile != CFile::hFileNull)
		{
			GLinTmpNdxF->Close();
			GLinTmpNdxF->Remove(GLinTmpNdxFName);
		}
	}
	delete GLinTmpDatF;									//删除线临时文件对象
	delete GLinTmpNdxF;		
	if (GRegFCreated)									//如果区临时文件已创建，则关闭并删除
	{
		if (GRegTmpDatF->m_hFile != CFile::hFileNull)
		{
			GRegTmpDatF->Close();
			GRegTmpDatF->Remove(GRegTmpDatFName);
		}
		if (GRegTmpNdxF->m_hFile != CFile::hFileNull)
		{
			GRegTmpNdxF->Close();
			GRegTmpNdxF->Remove(GRegTmpNdxFName);
		}
	}
	delete GRegTmpDatF;									//删除区临时文件对象
	delete GRegTmpNdxF;		
}


void CMapEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GPntFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_PNT:						//移动点操作
			if (GPntNdx != -1)
			{
				CClientDC dc(this);						//获得本窗口或当前活动视图
				dc.SetROP2(R2_NOTXORPEN);				//设置异或模式画点
				DrawPnt(&dc, GTPnt);					//在原位置画，清除原点图形
				GTPnt.x = point.x;						//移动点的坐标x
				GTPnt.y = point.y;						//移动点的坐标y
				DrawPnt(&dc, GTPnt);					//在新位置画
			}
			break;
		default:
			break;
		}
	}
	if (GLinFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_LIN:						//当前为绘制线状态
			if (GTLin.dotNum > 0)
			{
				CClientDC dc(this);						//获得本窗口或当前活动视图
				dc.SetROP2(R2_NOTXORPEN);				//设置异或模式画点
				if (GMPnt.x != -1 && GMPnt.y != -1)
				{
					DrawSeg(&dc, GTLin, GLPnt, GMPnt);	//默认样式绘制线段
				}
				GMPnt.x = point.x;						//设置鼠标上一状态点(x)
				GMPnt.y = point.y;						//设置鼠标上一坐标点(y)
				POINT mpoint = { mpoint.x = point.x,mpoint.y = point.y };
				DrawSeg(&dc, GTLin, GLPnt, mpoint);		//默认样式绘制线段
			}
			break;
		case OPERSTATE_MOVE_LIN:						//当前为移动点状态
			if (GLinNdx != -1)
			{
				CClientDC dc(this);
				dc.SetROP2(R2_NOTXORPEN);				//设置异或模式
				D_DOT dot1, dot2;
				POINT pnt1, pnt2;
														// 擦除原来的线
				for (int i = 0; i < GLinMMTmpNdx.dotNum - 1; i++)
				{
														// 从临时文件中读取线的点
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i, dot1);
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i + 1, dot2);
														//坐标系转换（数据转窗口）
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					pnt1.x += GLinMMOffestX;
					pnt1.y += GLinMMOffestY;
					pnt2.x += GLinMMOffestX;
					pnt2.y += GLinMMOffestY;
					DrawSeg(&dc, GLinMMTmpNdx, pnt1, pnt2);//重绘（异或模式擦除）
				}
														// 计算偏移量
				GLinMMOffestX = GLinMMOffestX + point.x - GLinMMPnt.x;
				GLinMMOffestY = GLinMMOffestY + point.y - GLinMMPnt.y;
														// 在新的位置绘制一条新的线段
				for (int i = 0; i < GLinMMTmpNdx.dotNum - 1; i++)
				{
														// 从临时文件中读取线的点
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i, dot1);
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i + 1, dot2);
														//转换坐标系（数据转窗口）
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					pnt1.x += GLinMMOffestX;
					pnt1.y += GLinMMOffestY;
					pnt2.x += GLinMMOffestX;
					pnt2.y += GLinMMOffestY;
					DrawSeg(&dc, GLinMMTmpNdx, pnt1, pnt2);//重绘（绘制新线）
				}
				GLinMMPnt = point;
			}
			break;
		default:
			break;
		}
	}
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		CClientDC dc(this);								//获得本窗口或当前活动视图
		CPen pen(PS_DOT, 1, RGB(0, 0, 0));
		CPen * oldPen = dc.SelectObject(&pen);
		switch (GCurOperState)
		{
		case OPERSTATE_ZOOM_IN:							//当前为放大操作状态
			if (GZoomLBDPnt.x != -1 && GZoomMMPnt.y != -1)
			{
				dc.SetROP2(R2_NOTXORPEN);				//设置为异或模式画线
				dc.Rectangle(GZoomLBDPnt.x, GZoomLBDPnt.y, GZoomMMPnt.x, GZoomMMPnt.y);
				dc.Rectangle(GZoomLBDPnt.x, GZoomLBDPnt.y, point.x, point.y);
				GZoomMMPnt = point;
				dc.SelectObject(oldPen);
			}
			break;
		default:
			break;
		}
	}
	CView::OnMouseMove(nFlags, point);
}
