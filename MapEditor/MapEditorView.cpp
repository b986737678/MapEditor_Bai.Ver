
// MapEditorView.cpp : CMapEditorView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "MapEditor.h"
#endif

#include "MapEditorDoc.h"
#include "MapEditorView.h"

#include"CCreateFileDlg.h"

#include"Paint.h"
#include"WriteOrRead.h"
#include"Calculate.h"
#include"PointParameterDlg.h"
#include"LineParameterDlg.h"
#include"RegionParameterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///-------------------------��������ص�ȫ�ֿ��Ʊ���------------------------///
bool	GPntFCreated = false;						//��ʱ�ļ��Ƿ񴴽�
CString GPntFName;									//�����ļ�������·����
CString GPntTmpFName = CString("tempPntF.dat");		//��ʱ�ļ�������·����
bool	GPntChanged = false;						//�Ƿ����
int		GPntNum = 0;								//������
int		GPntLNum = 0;								//�߼���
CFile	*GPntTmpF = new CFile();					//��ȡ��ʱ�ļ���ָ�����

///-------------------------��������ص�ȫ�ֿ��Ʊ���------------------------///
bool	GLinFCreated = false;						//��ʱ�ļ��Ƿ񴴽�
CString	GLinFName;									//�����ļ�������·����
CString	GLinTmpNdxFName = CString("tempLinF.ndx");	//��ʱ�����ļ�������·����
CString GLinTmpDatFName = CString("tempLinF.dat");	//��ʱ�����ļ�������·����
bool	GLinChanged = false;						//�Ƿ����
int		GLinNum = 0;								//������
int		GLinLNum = 0;								//�߼���
CFile	*GLinTmpDatF = new CFile();					//��ȡ��ʱ�����ļ���ָ�����
CFile	*GLinTmpNdxF = new CFile();					//��ȡ��ʱ�����ļ���ָ�����

///-------------------------��������ص�ȫ�ֿ��Ʊ���------------------------///
bool	GRegFCreated = false;						//��ʱ�ļ��Ƿ񴴽�
CString	GRegFName;									//�����ļ�������·����
CString	GRegTmpNdxFName = CString("tempRegF.ndx");	//��ʱ�����ļ�������·����
CString GRegTmpDatFName = CString("tempRegF.dat");	//��ʱ�����ļ�������·����
bool	GRegChanged = false;						//�Ƿ����
int		GRegNum = 0;								//������
int		GRegLNum = 0;								//�߼���
CFile	*GRegTmpDatF = new CFile();					//��ȡ��ʱ�����ļ���ָ�����
CFile	*GRegTmpNdxF = new CFile();					//��ȡ��ʱ�����ļ���ָ�����

///-------------------------------��������--------------------------------///
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
};//ö�ٲ���״̬
Action GCurOperState;//��������

///-------------------------Ĭ�ϵ�ṹ����ʱ��ṹ--------------------------///
PNT_STRU GPnt = {	
	GPnt.isDel = 0,
	GPnt.color = RGB(0,0,0),
	GPnt.pattern = 0 
};													//Ĭ�ϵ����

///-------------------------�ļ��汾��Ϣ��ȫ�ֱ���--------------------------///
VERSION GPntVer = {
	GPntVer.flag[0] = 'P',
	GPntVer.flag[1] = 'N',
	GPntVer.flag[2] = 'T',
	GPntVer.version = 10	//Ĭ�ϰ汾��
};

///-------------------------�ҵ��ĵ�λ���ļ��е�λ��------------------------///
int GPntNdx = -1;

///-------------------------��ʱ�㣬�洢�ҵ��ĵ�����------------------------///
PNT_STRU GTPnt;
///------------------Ĭ���������ṹ����ʱ�������ṹ�������-----------------///
LIN_NDX_STRU GLin = { 								//Ĭ���߽ṹ
	GLin.isDel = 0,
	GLin.color = RGB(0,0,0),
	GLin.pattern = 0,
	GLin.dotNum = 0,
	GLin.datOff = 0 
};
LIN_NDX_STRU GTLin;									//��
POINT GLPnt = { GLPnt.x = -1,GLPnt.y = -1 };		//��¼�߶ε����
CPoint GMPnt(-1, -1);								//��¼�����һ��״̬�ĵ�
///---------------------���ļ��İ汾��Ϣ���Լ����ʼ��----------------------///
VERSION GLinVer =
{
	GLinVer.flag[0] = 'L',
	GLinVer.flag[1] = 'I',
	GLinVer.flag[2] = 'N',
	GLinVer.version = 10							//Ĭ�ϰ汾��
};
///-------------------------�ҵ���λ���ļ��е�λ��--------------------------///
int GLinNdx = -1;

///---------------------------�ƶ������ȫ�ֱ���----------------------------///
CPoint			GLinLBDPnt(-1, -1);					// ��¼���������µ�λ�ã���������ƫ����
CPoint			GLinMMPnt(-1, -1);					// ��¼����ƶ�����һ��״̬�����������ƶ�ʱ��ǰһ����
long			GLinMMOffestX = 0;					// ��¼����ƶ�ʱ���X���ƫ����
long			GLinMMOffestY = 0;					// ��¼����ƶ�ʱ���Y���ƫ����
LIN_NDX_STRU	GLinMMTmpNdx;						// ��¼���ѡ�е��ߵ�����

///-------------------------���ڷŴ���С��ȫ�ֱ���--------------------------///
CPoint GZoomLBDPnt(-1, -1);							//�Ŵ�ʱ������̧��ĵ�
CPoint GZoomMMPnt(-1, -1);							//�Ŵ�ʱ����ƶ�ǰһ״̬

double	GZoomOffset_x = 0;							//ƫ������
double	GZoomOffset_y = 0;
double	GZoom = 1.0;								//����ϵ��
int		GZoomStyle = 0;								//�Ŵ�ʽ

///---------------------------���������ȫ�ֱ���----------------------------///
LIN_NDX_STRU	GStartLin = GLin;					// ѡ�еĵ�һ����
int				GnStart = -1;
LIN_NDX_STRU	GEndLin = GLin;						// ѡ�еĵڶ�����
int				GnEnd = -1;
int				GnLine = 0;

///---------------------------����������ص�����----------------------------///
CPoint			GRegCreateMMPnt(-1, -1);
CPoint			GRegCreateStartPnt(-1, -1);

///-------------------Ĭ���������ṹ����ʱ�����ṹ�������------------------///
REG_NDX_STRU	GReg = {
	GReg.isDel = 0,
	GReg.color = RGB(0,0,0),
	GReg.pattern = 0,
	GReg.dotNum = 0,
	GReg.datOff = 0
};
REG_NDX_STRU	GTReg;

int GRegNdx = -1;									//�ҵ�����λ���ļ��е�λ��

///---------------------------�ƶ������ȫ�ֱ���----------------------------///
REG_NDX_STRU	GRegMMTmpNdx;						// ��¼���ѡ����������
CPoint			GRegLBDPnt(-1, -1);					// ��¼���������µ�λ�ã���������ƫ����
CPoint			GRegMMPnt(-1, -1);					// ��¼����ƶ�ʱ��һ״̬�������ƶ�ʱǰһ����
long			GRegMMOffsetX = 0;					// ��¼����ƶ�ʱ��X��ƫ����
long			GRegMMOffsetY = 0;					// ��¼����ƶ�ʱ��Y��ƫ����

///--------------------------�ƶ��������ȫ�ֱ���---------------------------///
CPoint			GWinMoveLBDPnt(-1, -1);				//�ƶ�����ʱ������µ�
CPoint			GWinMoveMMPnt(-1, -1);				//�ƶ�����ʱ����ƶ�ǰ״̬��λ��

///---------------------------������εĶ�������----------------------------///
double			GMaxX = 0;
double			GMaxY = 0;
double			GMinX = 0;
double			GMinY = 0;

///--------------------------��ʾ״̬���ȫ�ֱ���---------------------------///
enum State
{
	SHOWSTATE_UNDEL, 
	SHOWSTATE_DEL
};													//ö����ʾ����

State			GCurShowState = SHOWSTATE_UNDEL;	//��ʾ״̬��Ĭ����ʾ��ɾ��״̬
bool			GShowPnt = true;					//��ǰ��ʾ�Ľṹ�Ƿ��ǵ�
bool			GShowLin = true;					//��ǰ��ʾ�Ľṹ�Ƿ�����
bool			GShowReg = true;					//��ǰ��ʾ�Ľṹ�Ƿ�����



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
	ON_UPDATE_COMMAND_UI(ID_WINDOW_SHOW_POINT, &CMapEditorView::OnUpdateWindowShowPoint)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_SHOW_LINE, &CMapEditorView::OnUpdateWindowShowLine)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_SHOW_REGION, &CMapEditorView::OnUpdateWindowShowRegion)
	ON_UPDATE_COMMAND_UI(ID_POINT_SHOW_DELETED, &CMapEditorView::OnUpdatePointShowDeleted)
	ON_UPDATE_COMMAND_UI(ID_LINE_SHOW_DELETED, &CMapEditorView::OnUpdateLineShowDeleted)
	ON_UPDATE_COMMAND_UI(ID_REGION_SHOW_DELETED, &CMapEditorView::OnUpdateRegionShowDeleted)
END_MESSAGE_MAP()

// CMapEditorView ����/����

CMapEditorView::CMapEditorView()
{
	// TODO: �ڴ˴���ӹ������

}

CMapEditorView::~CMapEditorView()
{
}

BOOL CMapEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMapEditorView ����

void CMapEditorView::OnDraw(CDC* /*pDC*/)
{
	CMapEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	CRect mrect;
	GetClientRect(&mrect);										//��ȡ���ڿͻ���������
	CClientDC dc(this);
	dc.FillSolidRect(0, 0, mrect.Width(), mrect.Height(), dc.GetBkColor());//��һ����ɫ���һ������
	dc.SetROP2(R2_NOTXORPEN);									//���û�ͼģʽ
	if (GShowPnt)												//������ʾ���е�
		ShowAllPnt(&dc, GPntTmpF, GPntNum, GZoomOffset_x, GZoomOffset_y, GZoom, GCurShowState);
	if (GShowLin)												//������ʾ������
		ShowAllLin(&dc, GLinTmpNdxF, GLinTmpDatF, GLinNum, GZoomOffset_x, GZoomOffset_y, GZoom, GCurShowState);
	if (GShowReg)												//������ʾ������
		ShowAllReg(&dc, GRegTmpNdxF, GRegTmpDatF, GRegNum, GZoomOffset_x, GZoomOffset_y, GZoom, GCurShowState);
	ReleaseDC(&dc);												//�ͷ�dc
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
				WriteLinNdxToFile(GLinTmpNdxF, GLinNum, GTLin);//���������ļ�д������ʱ�����ļ���
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
	if (GRegFCreated)
	{
		switch (GCurOperState)
		{

		case OPERSTATE_INPUT_REG:								//��ǰΪ��������״̬
			if (GTReg.dotNum > 2)
			{
				WriteRegNdxToFile(GRegTmpNdxF, GRegNum, GTReg);
				++GRegNum;
				++GRegLNum;
				POINT * pt = new POINT[3];
				D_DOT dot;
				ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, 0, dot);
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(pt[0], dot);
				ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, GTReg.dotNum - 1, dot);
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(pt[1], dot);
				pt[2] = point;
				DrawReg(&dc, GTReg, pt, 3);
				delete[] pt;
				GReg.datOff += (GTReg.dotNum * sizeof(D_DOT));
				memset(&GTReg, 0, sizeof(REG_NDX_STRU));
				GRegCreateMMPnt = CPoint(-1, -1);
				GRegCreateStartPnt = CPoint(-1, -1);
			}
			else if (GTReg.dotNum == 2)
			{
				POINT * pt = new POINT[3];
				D_DOT dot;
				ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, 0, dot);
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(pt[0], dot);
				ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, GTReg.dotNum - 1, dot);
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(pt[1], dot);
				pt[2] = point;
				DrawReg(&dc, GTReg, pt, 3);
				delete[] pt;
				memset(&GTReg, 0, sizeof(REG_NDX_STRU));
				GRegCreateMMPnt = CPoint(-1, -1);;
				GRegCreateStartPnt = CPoint(-1, -1);
			}
			else if (GTReg.dotNum == 1)
			{
				LIN_NDX_STRU tln = { tln.pattern = GTReg.pattern, tln.color = GTReg.color };
				DrawSeg(&dc, tln, GRegCreateStartPnt, GRegCreateMMPnt);
				memset(&GTReg, 0, sizeof(REG_NDX_STRU));
				GRegCreateMMPnt = CPoint(-1, -1);
				GRegCreateStartPnt = CPoint(-1, -1);
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


// CMapEditorView ���

#ifdef _DEBUG
void CMapEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CMapEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapEditorDoc* CMapEditorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapEditorDoc)));
	return (CMapEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapEditorView ��Ϣ�������

void CMapEditorView::OnFileNew()
{
	// TODO: �ڴ���������������
																//����Ƿ����½���ʱ�ļ���������ʱ�ļ��򷵻�
	if (GPntFCreated&&GLinFCreated&&GRegFCreated)
	{
		MessageBox(L"File have been created.",L"Message",MB_OK);
		return;
	}
	CCreateFileDlg dlg;											//�������½���ʱ�ļ����Ի������
	if (dlg.DoModal() != IDOK)									//�жϵ�ǰ�����Ƿ�Ϊ��ȷ������ť�������򷵻�
		return;
	CString str;												//���������Ϣ����
	if (!GPntFCreated)											//�жϵ���ʱ�ļ��Ƿ���ڣ����������½�
	{
		GPntTmpFName = dlg.m_add + CString("\\") + GPntTmpFName;//��ʱ�������ļ���
		if (GPntTmpF->Open(GPntTmpFName, CFile::modeCreate 
			| CFile::modeReadWrite | CFile::typeBinary))
		{
			GPntFCreated = true;								//���õ���ʱ�ļ��½��ɹ���־ֵ
			str += "tempPntF.dat\n";
		}
		else
		{
			GPntTmpFName = CString("tempPntF.dat");
			TRACE(_T("File could not be opened n"));
		}
	}
	if (!GLinFCreated)												  //�ж�����ʱ�ļ��Ƿ���ڣ����������½�
	{
		GLinTmpNdxFName = dlg.m_add + CString("\\") + GLinTmpNdxFName;//��ʱ�������ļ���
		GLinTmpDatFName = dlg.m_add + CString("\\") + GLinTmpDatFName;//��ʱ�������ļ���
		if (GLinTmpNdxF->Open(GLinTmpNdxFName, CFile::modeCreate 
			| CFile::modeReadWrite | CFile::typeBinary)&&GLinTmpDatF->Open(GLinTmpDatFName,
				CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary))
		{
			GLinFCreated = true;									  //��������ʱ�ļ��½��ɹ���־ֵ
			str += "tempLinF.dat tempLinF.ndx\n";
		}
		else
		{
			GLinTmpDatFName = CString("tempLinF.dat");
			GLinTmpNdxFName = CString("tempLinF.ndx");
			TRACE(_T("File could not be opened \n"));
		}
	}
	if (!GRegFCreated)												  //�ж�����ʱ�ļ��Ƿ���ڣ����������½�
	{
		GRegTmpNdxFName = dlg.m_add + CString("\\") + GRegTmpNdxFName;//����ʱ�����ļ���
		GRegTmpDatFName = dlg.m_add + CString("\\") + GRegTmpDatFName;//����ʱ�����ļ���
		if (GRegTmpNdxF->Open(GRegTmpNdxFName, CFile::modeCreate
			| CFile::modeReadWrite | CFile::typeBinary)&&GRegTmpDatF->Open(GRegTmpDatFName,
				CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary))
		{
			GRegFCreated = true;									  //��������ʱ�ļ��½��ɹ���־
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
		MessageBox(str, L"message", MB_OK);							  //�½���ʱ�ļ��ɹ�������ʾ��
	}
	//dlg.DoModal();
}


void CMapEditorView::OnWindowZoomIn()
{
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		GCurOperState = OPERSTATE_ZOOM_IN;							  //��ǰΪ�Ŵ����״̬
	}
	else
	{
		MessageBox(L"TempFile have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnFileSavePoint()
{
	// TODO: �ڴ���������������
	GCurOperState = Noaction;
	//1.�����û���½���򿪵��ļ�������ʾ�ļ���û�д򿪣�Ȼ�󷵻�
	if (GPntFCreated == false)
	{
		MessageBox(L"File have not been created.", L"Message", MB_OK);
		return;
	}
	CFile*pntF = new CFile();
	//2.������ļ���Ϊ�գ���ɾ��ԭ�����ļ����������CFileDialog�����û������ļ���
	if (GPntFName.IsEmpty() == false)
	{
		CFile::Remove(GPntFName);
	}
	else
	{
		LPCTSTR lpszFilters;
		lpszFilters = _T("��(*.pnt)|*.pnt||");
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
	//3.���´����������ļ�����д��汾��Ϣ�������������߼���
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
	//4.������ʱ�ļ��е���Ϣ����д�������ļ���
	for (int i = 0; i < GPntNum; i++)
	{
		ReadTempFileToPnt(GPntTmpF, i, tempPnt);
		pntF->Write(&tempPnt, sizeof(PNT_STRU));
	}
	pntF->Close();
	delete pntF;
	//5.�޸����ݱ仯��־���ı������ڱ�������
	GPntChanged = false;
	int nPos = GPntFName.ReverseFind(_T('\\'));
	CString windowText = GPntFName.Right(GPntFName.GetLength() - nPos - 1) + "-MapEditor";
	GetParent()->SetWindowTextW(windowText);
}




void CMapEditorView::OnFileOpenPoint()
{
	CFileDialog dlg(true);
	dlg.m_ofn.lpstrFilter = L"pnt\0*.pnt";
	//�����ʱ�ļ��е������Ѿ��ı䣬����ʾ�Ƿ񱣴棬���������OnFileSavePoint()
	if (GPntChanged == true)
	{
		if (IDYES == AfxMessageBox(L"File has not been saved.Does save File?"), MB_YESNO, MB_ICONQUESTION)
			OnFileSavePoint();
	}
	if (dlg.DoModal() == IDOK)								//�������ļ��Ի������û�ָ��Ҫ�򿪵��ļ�
	{
		GPntFName = dlg.m_ofn.lpstrFile;					//�����ļ�(��·��)
		CFile*pntF = new CFile();
		if (!pntF->Open(GPntFName, CFile::modeRead | CFile::typeBinary))
		{
			TRACE(_T("File could not be opened \n"));
			return;
		}
		int nPos = GPntFName.ReverseFind(_T('\\'));
		CString floderAdd = GPntFName.Left(nPos);
		if (GPntTmpFName != "tempPntF.dat")					//�����ʱ���ļ��Ѿ�������λ���ļ���ͷ
		{
			GPntTmpF->SeekToBegin();
		}
		else
		{													//���򴴽���ʱ�ļ�
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
		ReadPntPermanentFileToTemp(pntF, GPntTmpF, GPntNum, GPntLNum);//���������ļ�����ʱ�ļ�
		pntF->Close();
		delete pntF;
		CString windowText = dlg.GetFileName() + "-MapEditor";
		GetParent()->SetWindowTextW(windowText);
		this->InvalidateRect(NULL);							//���Ӵ�����Ч������MFC����OnDraw�����ػ洰��
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
			OnFileSaveLine();								//�����ߴ���ʱ�ļ�д�������ļ��У�
		}
	}
	if (dlg.DoModal() == IDOK)
	{
		GLinFCreated = false;
		int IsCreate = 0;
		GLinFName = dlg.m_ofn.lpstrFile;					//�����ļ�����·����
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
			GLinNum, GLinLNum, GLin.datOff);				//�������ļ���ȡ�����ݵ���ʱ�ļ�
		LinF->Close();										//�ر��������ļ�
		delete LinF;
		CString windowTest = dlg.GetFileName() + "-MapEditor";
		GetParent()->SetWindowTextW(windowTest);
		this->InvalidateRect(NULL);							//�ػ洰����ʾ
		GCurOperState = Noaction;
	}
}


void CMapEditorView::OnFileOpenRegion()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnFileSaveLine()
{
	if (GLinFCreated)
	{														//�Ѿ�������ʱ�ļ�
		CFile*LinF = new CFile();
		if (GLinFName.IsEmpty())
		{
			LPCTSTR lpszFilters;
			lpszFilters = _T("��(*.lin)|*.lin||");
			CFileDialog dlg(false, _T("lin"), NULL, OFN_HIDEREADONLY 
				| OFN_OVERWRITEPROMPT, lpszFilters);		//�����ߵĶԻ���
			int nPos = GLinTmpDatFName.ReverseFind(_T('\\'));
			CString folderAdd = GLinTmpDatFName.Left(nPos);
			dlg.m_ofn.lpstrInitialDir = folderAdd;
			if (dlg.DoModal() == IDOK)
			{
				GLinFName = dlg.GetPathName();				//���ļ�������
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
			GLinNum, GLinLNum);								//���ߵ������͵�����д�������ļ�
		LinF->Close();
		delete LinF;

		GLinChanged = false;								//�������ޱ��
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
	// TODO: �ڴ���������������
}


void CMapEditorView::OnFileSaveAsPoint()
{
	// TODO: �ڴ���������������
	CString tempFName = GPntFName;							//����ԭ�������ļ���
	GPntFName = CString("");								//���������ļ�����Ϊ��
	OnFileSavePoint();										//����OnFileSavePoint�����������ļ���Ϊ��ʱ�Զ������ļ����Ի���
	if (GPntFName == "")									//�����ʧ�ܣ���ԭԭ�����ļ���
	{
		GPntFName = tempFName;
	}
}


void CMapEditorView::OnFileSaveAsLine()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnFileSaveAsRegion()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnAppExit()
{
	// TODO: �ڴ���������������
	
															//1.����������Ѹı䣬�򱣴�
	if (GPntChanged == true)
	{
		if (IDYES == AfxMessageBox(L"File has not been saved.Does save File?"), MB_YESNO, MB_ICONQUESTION)
		{
			OnFileSavePoint();								//�����
		}
	}
															//2.���ø�����CMainFrame�����ٴ��ں���DestroyWindow()
	GetParent()->DestroyWindow();
}


void CMapEditorView::OnWindowZoomOut()
{
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		GCurOperState = OPERSTATE_ZOOM_OUT;					//����Ϊ��С����״̬
	}
	else
	{
		MessageBox(L"TempFile have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnWindowMove()
{
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		GCurOperState = OPERSTATE_WINDOW_MOVE;				//����Ϊ�����ƶ�����״̬
	}
	else
	{
		MessageBox(L"TempFile have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnWindowReset()
{
	GCurShowState = SHOWSTATE_UNDEL;		//���õ�ǰΪ��ʾδɾ��״̬
	this->Invalidate();
	GShowPnt = true;
	GShowLin = true;
	GShowReg = true;
	//����ƫ�����ͷŴ���
	GZoomOffset_x = 0;										// ƫ������x
	GZoomOffset_y = 0;										// ƫ������y
	GZoom = 1.0;											// �Ŵ�ϵ��
	// �����㡢�ߡ����ĵ����ݣ�������������
	D_DOT tempPt;
	PNT_STRU tempPnt;
	LIN_NDX_STRU tempLin;
	REG_NDX_STRU tempReg;
	bool isInit = false;
	// û��ͼ��
	if (GPntLNum == 0 && GLinLNum == 0 && GRegLNum == 0)
		return;
	// ��ʼ���������
	if (isInit == false && GPntLNum > 0)					// ��ʼ������������
	{
		for (int i = 0; i < GPntNum; ++i)
		{
			ReadTempFileToPnt(GPntTmpF, i, tempPnt);		// ����ʱ�ļ��ж�ȡ��
			if (tempPnt.isDel)
				continue;
			else
			{
				GMaxX = tempPnt.x;
				GMinX = tempPnt.x;
				GMaxY = tempPnt.y;
				GMinY = tempPnt.y;
				isInit = true;
				break;
			}
		}
	}
	if (isInit == false && GLinLNum > 0)					//��ʼ���ߵ���������
	{
		for (int i = 0; i < GLinNum; ++i)
		{
			ReadTempFileToLinNdx(GLinTmpNdxF, i, tempLin);	// ����ʱ�ļ���ȡ������
			if (tempLin.isDel)
				continue;
			else
			{
				for (int j = 0; j < tempLin.dotNum; ++j)
				{
					ReadTempFileToLinDat(GLinTmpDatF, tempLin.datOff, j, tempPt);
					GMaxX = tempPt.x;
					GMinX = tempPt.x;
					GMaxY = tempPt.y;
					GMinY = tempPt.y;
					isInit = true;
					break;
				}
			}
		}
	}
	if (isInit == false & &GRegLNum > 0)					//��ʼ��������������
	{
		for (int i = 0; i < GRegNum; ++i)
		{
			ReadTempFileToRegNdx(GRegTmpNdxF, i, tempReg);	//����ʱ�ļ���������
			if (tempReg.isDel)
				continue;
			else
			{
				for (int j = 0; j < tempReg.dotNum; ++j)
				{
					ReadTempFileToRegDat(GRegTmpDatF, tempReg.datOff, j, tempPt);
					GMaxX = tempPt.x;
					GMinX = tempPt.x;
					GMaxY = tempPt.y;
					GMinY = tempPt.y;
					isInit = true;
					break;
				}
			}
		}
	}
	// δ�ܳ�ʼ���ɹ�
	if (isInit == false)
	{
		this->Invalidate();
		return;
	}
	// �������еĵ�
	if (GPntFCreated)
	{
		for (int i = 0; i < GPntNum; i++)
		{
			ReadTempFileToPnt(GLinTmpNdxF, i, tempPnt);
			if (tempPnt.isDel)
				continue;
			else
			{
				if (tempPnt.x > GMaxX)
					GMaxX = tempPnt.x;
				if (tempPnt.y > GMaxY)
					GMaxY = tempPnt.y;
				if (tempPnt.x < GMinX)
					GMinX = tempPnt.x;
				if (tempPnt.y < GMinY)
					GMinY = tempPnt.y;
			}
		}
	}
	// �������е���
	if (GLinFCreated)
	{
		for (int i = 0; i < GLinNum; i++)
		{
			ReadTempFileToLinNdx(GLinTmpNdxF, i, tempLin);
			if (tempLin.isDel)
				continue;
			else
			{
				for (int j = 0; j < tempLin.dotNum; j++)
				{
					ReadTempFileToLinDat(GLinTmpDatF, tempLin.datOff, j, tempPt);
					if (tempPt.x > GMaxX)
						GMaxX = tempPt.x;
					if (tempPt.y > GMaxY)
						GMaxY = tempPt.y;
					if (tempPt.x < GMinX)
						GMinX = tempPt.x;
					if (tempPt.y < GMinY)
						GMinY = tempPt.y;
				}
			}
		}
	}
	// �������е���
	if (GRegFCreated)
	{
		for (int i = 0; i < GRegLNum; i++)
		{
			ReadTempFileToRegNdx(GRegTmpNdxF, i, tempReg);
			if (tempReg.isDel)
				continue;
			else
			{
				for (int j = 0; j < tempReg.dotNum; j++)
				{
					ReadTempFileToRegDat(GRegTmpDatF, tempReg.datOff, j, tempPt);
					if (tempPt.x > GMaxX)
						GMaxX = tempPt.x;
					if (tempPt.y > GMaxY)
						GMaxY = tempPt.y;
					if (tempPt.x < GMinX)
						GMinX = tempPt.x;
					if (tempPt.y < GMinY)
						GMinY = tempPt.y;
				}
			}
		}
	}
	GMaxX += 20;
	GMinX -= 20;
	GMaxY += 20;
	GMinY -= 20;

	RECT rect, client;
	double zoom;

	GetClientRect(&client);
	rect.right = (long)GMaxX;
	rect.left = (long)GMinX;
	rect.bottom = (long)GMaxY;
	rect.top = (long)GMinY;
	modulusZoom(client, rect, zoom);
	GMaxX += 20 / zoom;
	GMinX -= 20 / zoom;
	GMaxY += 20 / zoom;
	GMinY -= 20 / zoom;
	rect.right = (long)GMaxX;
	rect.left = (long)GMinX;
	rect.bottom = (long)GMaxY;
	rect.top = (long)GMinY;
	// �����������μ���ƫ�����ͷŴ��������ػ�ͻ���
	modulusZoom(client, rect, zoom);
	double x0 = GetCenter(rect).x - (client.right / 2.0) + (client.right*(zoom - 1) / (2.0* zoom));
	double y0 = GetCenter(rect).y - (client.bottom / 2.0) + (client.bottom*(zoom - 1) / (2.0* zoom));
	GZoomOffset_x += (x0 / GZoom);
	GZoomOffset_y += (y0 / GZoom);
	GZoom *= zoom;
	GCurOperState = Noaction;
	this->Invalidate();
}


void CMapEditorView::OnWindowShowPoint()
{
	//����ǰ��ʾ״̬����ʾɾ��״̬�����Ȱ�������ʾ���ض��ر�
	if (GCurShowState == SHOWSTATE_DEL)
	{
		GShowPnt = false;
		GShowLin = false;
		GShowReg = false;
	}
	GCurShowState = SHOWSTATE_UNDEL;//����ʾ״̬����Ϊ��ʾδɾ��״̬
	//����ǰ�Ѿ�����ʾ�㡱���򽫹رտ��أ����١���ʾ�㡱
	if (GShowPnt == true)
	{
		GShowPnt = false;
	}
	else
	{
		GShowPnt = true;
	}
	this->InvalidateRect(NULL);		//ˢ�´���
}


void CMapEditorView::OnWindowShowLine()
{
	//����ǰ��ʾ״̬����ʾɾ��״̬�����Ȱ�������ʾ���ض��ر�
	if (GCurShowState == SHOWSTATE_DEL)
	{
		GShowPnt = false;
		GShowLin = false;
		GShowReg = false;
	}
	GCurShowState = SHOWSTATE_UNDEL;//����ʾ״̬����Ϊ��ʾδɾ��״̬
	//����ǰ�Ѿ�����ʾ�ߡ����򽫹رտ��أ����١���ʾ�ߡ�
	if (GShowLin == true)
	{
		GShowLin = false;
	}
	else
	{
		GShowLin = true;
	}
	this->InvalidateRect(NULL);		//ˢ�´���
}


void CMapEditorView::OnWindowShowRegion()
{
	//����ǰ��ʾ״̬����ʾɾ��״̬�����Ȱ�������ʾ���ض��ر�
	if (GCurShowState == SHOWSTATE_DEL)
	{
		GShowPnt = false;
		GShowLin = false;
		GShowReg = false;
	}
	GCurShowState = SHOWSTATE_UNDEL;//����ʾ״̬����Ϊ��ʾδɾ��״̬
	//����ǰ�Ѿ�����ʾ�������򽫹رտ��أ����١���ʾ����
	if (GShowReg == true)
	{
		GShowReg = false;
	}
	else
	{
		GShowReg = true;
	}
	this->InvalidateRect(NULL);		//ˢ�´���
}

void CMapEditorView::OnPointCreate()
{
	// TODO: �ڴ���������������
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_INPUT_PNT;			// ����Ϊ����㡱״̬
		GCurShowState = SHOWSTATE_UNDEL;				//���õ�ǰΪ��ʾδɾ��״̬
		this->Invalidate();
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	else
	{
		MessageBox(L"File have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnPointMove()
{
	// TODO: �ڴ���������������
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_MOVE_PNT;				//���ò���״̬���ƶ��㣩
		GCurShowState = SHOWSTATE_UNDEL;				//���õ�ǰΪ��ʾδɾ��״̬
		this->Invalidate();
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	else
	{
		MessageBox(L"TempFile have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnPointDelete()
{
	// TODO: �ڴ���������������
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_DELETE_PNT;			//���ò���״̬(ɾ����)
		GCurShowState = SHOWSTATE_UNDEL;				//���õ�ǰΪ��ʾδɾ��״̬
		this->Invalidate();
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	else
	{
		MessageBox(L"TempFile has not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnPointSetDefparameter()
{
	CPointParameterDlg dlg;								//��������õĶԻ���
	dlg.m_Pattern = GPnt.pattern;						//����
	dlg.m_ColorButton.SetColor(GPnt.color);				//��ɫ
	if (IDOK == dlg.DoModal())
	{
		GPnt.pattern = dlg.m_Pattern;
		COLORREF tempColor = dlg.m_ColorButton.GetColor();
		memcpy_s(&GPnt.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
	}
}


void CMapEditorView::OnPointShowDeleted()
{
	//����ǰ��ʾ״̬������ʾɾ��״̬�����л�Ϊ��ʾɾ��״̬����ʾ��
	if (GCurShowState != SHOWSTATE_DEL)
	{
		GCurShowState = SHOWSTATE_DEL;					//����Ϊ��ʾɾ��״̬
		GShowPnt = true;
		GShowLin = false;
		GShowReg = false;
	}
	//����ǰ״̬����ʾɾ��״̬������ǰ��ʾ�Ĳ��ǵ㣬����ʾ��Ŀ��ش�
	else if (GCurShowState == SHOWSTATE_DEL && GShowPnt != true)
	{
		GShowPnt = true;
		GShowLin = false;
		GShowReg = false;
	}
	//�������������ʾ״̬����Ϊ��ʾδɾ����״̬���������е���ʾ�Ŀ���
	else
	{
		GCurShowState = SHOWSTATE_UNDEL;				//����Ϊ��ʾδɾ��״̬
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	this->InvalidateRect(NULL);							//ˢ�´���
}


void CMapEditorView::OnPointUndelete()
{
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_UNDELETE_PNT;			//��ǰ����״̬���ָ��㣩
		GCurShowState = SHOWSTATE_DEL;					//��ǰ��ʾ״̬��ɾ���㣩
		this->Invalidate();
		GShowPnt = true;								//����ʾ��
		GShowLin = false;								//�ر���ʾ��
		GShowReg = false;								//�ر���ʾ��
	}
	else
	{
		MessageBox(L"File have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnPointModifyParameter()
{
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_MODIFY_POINT_PARAMETER;//��ǰ����״̬���ָ��㣩
		GCurShowState = SHOWSTATE_UNDEL;				//��ǰ��ʾ״̬����ɾ���㣩
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GShowPnt = true;								//����ʾ��					 ????????????????????????????????????????
		GShowLin = true;								//�ر���ʾ��             ��������????????????????????????????????????????
		GShowReg = true;								//�ر���ʾ��				     ????????????????????????????????????????
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		this->Invalidate();
	}
	else
	{
		MessageBox(L"File have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineCreate()
{
	// TODO: �ڴ���������������
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_INPUT_LIN;
		GCurShowState = SHOWSTATE_UNDEL;			//���õ�ǰΪ��ʾδɾ��״̬
		this->Invalidate();
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
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
		GCurOperState = OPERSTATE_MOVE_LIN;			// ��ǰΪ�ƶ��߲���״̬
		GCurShowState = SHOWSTATE_UNDEL;			//���õ�ǰΪ��ʾδɾ��״̬
		this->Invalidate();
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
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
		GCurOperState = OPERSTATE_DELETE_LIN;
		GCurShowState = SHOWSTATE_UNDEL;			//���õ�ǰΪ��ʾδɾ��״̬
		this->Invalidate();
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	else
	{
		MessageBox(L"Temp File have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineShowDeleted()
{
	//����ǰ��ʾ״̬������ʾɾ��״̬�����л�Ϊ��ʾɾ��״̬����ʾ��
	if (GCurShowState != SHOWSTATE_DEL)
	{
		GCurShowState = SHOWSTATE_DEL;					//����Ϊ��ʾɾ��״̬
		GShowPnt = false;
		GShowLin = true;
		GShowReg = false;
	}
	//����ǰ״̬����ʾɾ��״̬������ǰ��ʾ�Ĳ����ߣ�����ʾ�ߵĿ��ش�
	else if (GCurShowState == SHOWSTATE_DEL && GShowLin != true)
	{
		GShowPnt = false;
		GShowLin = true;
		GShowReg = false;
	}
	//�������������ʾ״̬����Ϊ��ʾδɾ����״̬���������е���ʾ�Ŀ���
	else
	{
		GCurShowState = SHOWSTATE_UNDEL;				//����Ϊ��ʾδɾ��״̬
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	this->InvalidateRect(NULL);							//ˢ�´���
}


void CMapEditorView::OnLineUndeleted()
{
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_UNDELETE_LIN;			//��ǰ����״̬���ָ��ߣ�
		GCurShowState = SHOWSTATE_DEL;					//��ǰ��ʾ״̬��ɾ���ߣ�
		this->Invalidate();
		GShowPnt = false;								//�ر���ʾ��
		GShowLin = true;								//����ʾ��
		GShowReg = false;								//�ر���ʾ��
	}
	else
	{
		MessageBox(L"File have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineDeleteDot()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnLineAddDot()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnLineLink()
{
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_LINK_LIN;					//����Ϊ�����߲���״̬
	}
	else
	{
		MessageBox(L"TempFile have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineModifyParameter()
{
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_MODIFY_LINE_PARAMETER;//��ǰ����״̬���ָ��㣩
		GCurShowState = SHOWSTATE_UNDEL;				//��ǰ��ʾ״̬����ɾ����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GShowPnt = true;								//�ر���ʾ��					 ????????????????????????????????????????
		GShowLin = true;								//����ʾ��             ��������????????????????????????????????????????
		GShowReg = true;								//�ر���ʾ��				     ????????????????????????????????????????
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		this->Invalidate();
	}
	else
	{
		MessageBox(L"File have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineSetDefparameter()
{
	CLineParameterDlg dlg;								//�߲������õĶԻ���
	dlg.m_Pattern = GLin.pattern;						//����
	dlg.m_ColorButton.SetColor(GLin.color);				//��ɫ
	if (IDOK == dlg.DoModal())
	{
		GLin.pattern = dlg.m_Pattern;
		COLORREF tempColor = dlg.m_ColorButton.GetColor();
		memcpy_s(&GLin.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
	}
}


void CMapEditorView::OnRegionCreate()
{
	if (GRegFCreated)
	{
		GCurOperState = OPERSTATE_INPUT_REG;				//��ǰ����Ϊ��������״̬
	}
	else
	{
		MessageBox(L"TempFile have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnRegionMove()
{
	if (GRegFCreated)
	{
		GCurOperState = OPERSTATE_MOVE_REG;					//��ǰ����Ϊ�ƶ�������״̬
	}
	else
	{
		MessageBox(L"TempFile have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnRegionDelete()
{
	if (GRegFCreated)
	{
		GCurOperState = OPERSTATE_DELETE_REG;				//���õ�ǰΪɾ��������״̬
	}
	else
	{
		MessageBox(L"TempFile have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnRegionShowDeleted()
{
	//����ǰ��ʾ״̬������ʾɾ��״̬�����л�Ϊ��ʾɾ��״̬����ʾ��
	if (GCurShowState != SHOWSTATE_DEL)
	{
		GCurShowState = SHOWSTATE_DEL;					//����Ϊ��ʾɾ��״̬
		GShowPnt = false;
		GShowLin = false;
		GShowReg = true;
	}
	//����ǰ״̬����ʾɾ��״̬������ǰ��ʾ�Ĳ�����������ʾ���Ŀ��ش�
	else if (GCurShowState == SHOWSTATE_DEL && GShowReg != true)
	{
		GShowPnt = false;
		GShowLin = false;
		GShowReg = true;
	}
	//�������������ʾ״̬����Ϊ��ʾδɾ����״̬���������е���ʾ�Ŀ���
	else
	{
		GCurShowState = SHOWSTATE_UNDEL;				//����Ϊ��ʾδɾ��״̬
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	this->InvalidateRect(NULL);							//ˢ�´���
}


void CMapEditorView::OnRegionUndeleted()
{
	if (GRegFCreated)
	{
		GCurOperState = OPERSTATE_UNDELETE_REG;			//��ǰ����״̬���ָ�����
		GCurShowState = SHOWSTATE_DEL;					//��ǰ��ʾ״̬��ɾ������
		this->Invalidate();
		GShowPnt = false;								//�ر���ʾ��
		GShowLin = false;								//�ر���ʾ��
		GShowReg = true;								//����ʾ��
	}
	else
	{
		MessageBox(L"File have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnRegionModifyParameter()
{
	if (GRegFCreated)
	{
		GCurOperState = OPERSTATE_MODIFY_REGION_PARAMETER;//��ǰ����״̬���޸�����
		GCurShowState = SHOWSTATE_UNDEL;				//��ǰ��ʾ״̬����ɾ����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GShowPnt = true;								//�ر���ʾ��					 ????????????????????????????????????????
		GShowLin = true;								//����ʾ��             ��������????????????????????????????????????????
		GShowReg = true;								//�ر���ʾ��				     ????????????????????????????????????????
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		this->Invalidate();
	}
	else
	{
		MessageBox(L"File have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnRegionSetDefparameter()
{
	CRegionParameterDlg dlg;							//���������õĶԻ���
	dlg.m_Pattern = GReg.pattern;						//����
	dlg.m_ColorButton.SetColor(GReg.color);				//��ɫ
	if (IDOK == dlg.DoModal())
	{
		GReg.pattern = dlg.m_Pattern;
		COLORREF tempColor = dlg.m_ColorButton.GetColor();
		memcpy_s(&GReg.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
	}
}


void CMapEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (GPntFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_PNT:									//��ǰΪ�ƶ���״̬
			D_DOT dot;
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);	// ����ϵת��
			DotToPnt(point, dot);
			GTPnt = FindPnt(point, GPntNum, GPntTmpF, GPntNdx);		// ���������
			dot.x = GTPnt.x;
			dot.y = GTPnt.y;
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);	// ����ϵת��
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
		case OPERSTATE_MOVE_LIN:									//��ǰΪ�ƶ���״̬
		{
			D_DOT dot;
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);	// ����ϵת��
			DotToPnt(point, dot);
			GLinMMTmpNdx = FindLin(GLinTmpNdxF, GLinTmpDatF, point, 
				GLinNum, GLinNdx);									//���ҵ����������һ����
			GLinMMOffestX = 0;
			GLinMMOffestY = 0;
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);	// ����ϵת��
			DotToPnt(point, dot);
			GLinLBDPnt = point;
			GLinMMPnt = point;
			break;
		}
		default:
			break;
		}
	}
	if (GRegFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_REG:							//��ǰΪ�ƶ�������״̬
			GRegLBDPnt = point;
			GRegMMPnt = point;
			D_DOT dot;
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);// ����ת����
			DotToPnt(point, dot);
			GRegMMTmpNdx = FindReg(GRegTmpNdxF, GRegTmpDatF, point, GRegNum, GRegNdx);// ���������������ѡ�е���
			GRegMMOffsetX = 0;
			GRegMMOffsetY = 0;
			break;
		default:
			break;
		}
	}
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_ZOOM_IN:								//��ǰΪ�Ŵ����״̬
			GZoomLBDPnt = point;
			GZoomMMPnt = point;
			break;
		case OPERSTATE_WINDOW_MOVE:							//��ǰΪ�����ƶ�״̬
			GWinMoveLBDPnt = point;
			GWinMoveMMPnt = point;
			break;
		default:
			break;
		}
	}

	CView::OnLButtonDown(nFlags, point);
}


void CMapEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CClientDC dc(this);										//���ʣ�����dcʱ���ù��캯��	
	dc.SetROP2(R2_NOTXORPEN);								//��ͼ��ģʽ����
	if (GPntFCreated)										//�Ѵ�����ʱ�ļ�
	{
		D_DOT dot;
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_PNT:							//��ǰΪ���Ƶ�״̬
			PNT_STRU pnt;									//�����
			memcpy_s(&pnt, sizeof(PNT_STRU), &GPnt, sizeof(PNT_STRU));
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//����ϵת��
			pnt.x = dot.x;									//���õ������x
			pnt.y = dot.y;									//���õ������y
			WritePntToFile(GPntTmpF, GPntNum, pnt);			//����д����ʱ�ļ�
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//����ϵת��
			pnt.x = dot.x;
			pnt.y = dot.y;
			DrawPnt(&dc, pnt);								//���Ƶ�
			GPntNum++;										//����������1
			GPntLNum++;										//���߼�����1
			GPntChanged = true;								//�Ƿ���ı�־����Ϊtrue
			break;
		case OPERSTATE_DELETE_PNT:							//��ǰΪɾ����״̬
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//ת������ϵ
			DotToPnt(point, dot);
			FindPnt(point, GPntNum, GPntTmpF, GPntNdx);
			if (GPntNdx != -1)								//����ҵ�
			{
				PNT_STRU pnt;
				ReadTempFileToPnt(GPntTmpF, GPntNdx, pnt);	//����ʱ�ļ�����
				pnt.isDel = 1;								//ɾ���������Ϊ1
				UpdatePnt(GPntTmpF, GPntNdx, pnt);			//���¸õ�����
				dot.x = pnt.x;
				dot.y = pnt.y;
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//ת������ϵ
				pnt.x = dot.x;
				pnt.y = dot.y;
				DrawPnt(&dc,pnt);							//���ģʽ�ػ�õ��������Ļ
				GPntNdx = -1;
				GPntChanged = true;							//���ݷ������
				GPntLNum--;									//ɾ��һ���㣬�߼�����1��������洢����
			}
			break;
		case OPERSTATE_MOVE_PNT:							//��ǰΪ�ƶ������״̬
			if (GPntNdx != -1)
			{
				PNT_STRU pnt;
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);// ����ϵת��
				ReadTempFileToPnt(GPntTmpF, GPntNdx, pnt);	// ����ʱ�ļ���ȡ��
				pnt.x = dot.x;								// �ƶ���ĵ�����x
				pnt.y = dot.y;								// �ƶ���ĵ�����y
				UpdatePnt(GPntTmpF, GPntNdx, pnt);			// ���µ�����(д����ʱ�ļ�)
				GPntNdx = -1;
				GPntChanged = true;							//���ݷ������
			}
			break;
		case OPERSTATE_UNDELETE_PNT:						//��ǰ״̬���ָ��㣩
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//����ת��������
			DotToPnt(point, dot);
			FindDeletePnt(point, GPntNum, GPntTmpF, GPntNdx);//���������ɾ����
			if (GPntNdx != -1)
			{
				PNT_STRU pnt;
				ReadTempFileToPnt(GPntTmpF, GPntNdx, pnt);	//����ʱ�ļ��ж���
				pnt.isDel = 0;								//����ɾ�����Ϊ0������ɾ��
				UpdatePnt(GPntTmpF, GPntNdx, pnt);			//���µ�
				dot.x = pnt.x;
				dot.y = pnt.y;
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//����ת����
				pnt.x = dot.x;
				pnt.y = dot.y;
				DrawPnt(&dc, pnt);							//�ڵ�ǰ��ͼ�������ģʽ�����ָ��ĵ�
				GPntChanged = true;
				GPntNdx = -1;
			}
			break;
		case OPERSTATE_MODIFY_POINT_PARAMETER:				//��ǰΪ�޸ĵ��������״̬
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//����ת��������ϵ
			DotToPnt(point, dot);
			PNT_STRU tempPoint;
			memcpy_s(&tempPoint, sizeof(PNT_STRU), 
				&FindPnt(point, GPntNum, GPntTmpF, GPntNdx), sizeof(PNT_STRU));
															//���������
			if (GPntNdx != -1)
			{
				CPointParameterDlg dlg;						//��������öԻ���
				dlg.m_ColorButton.SetColor(tempPoint.color);
				dlg.m_Pattern = tempPoint.pattern;
				if (IDOK == dlg.DoModal())
				{
					COLORREF tempColor = dlg.m_ColorButton.GetColor();
					memcpy_s(&tempPoint.color, sizeof(COLORREF), &tempColor,
						sizeof(COLORREF));
					tempPoint.pattern = dlg.m_Pattern;
					GPntTmpF->Seek(GPntNdx * sizeof(PNT_STRU), CFile::begin);
					GPntTmpF->Write(&tempPoint, sizeof(PNT_STRU));//д�������
				}
				this->Invalidate();
				GPntChanged = true;
				GPntNdx = -1;
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
		case OPERSTATE_INPUT_LIN:							//��ǰΪ������״̬
			if (GTLin.dotNum == 0)
				memcpy_s(&GTLin, sizeof(LIN_NDX_STRU), &GLin,
					sizeof(LIN_NDX_STRU));
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);// ����ϵת��
			WriteLinDatToFile(GLinTmpDatF, GLin.datOff, GTLin.dotNum, dot);
			//���ߵĵ�����д����ʱ�ļ���
			GTLin.dotNum++;									//�߽ڵ㴦��1
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);// ����ϵת��
			GLPnt.x = (long)dot.x;							//�����߶ε����(x)
			GLPnt.y = (long)dot.y;							//�����߶ε����(y)
			GLinChanged = true;								//�����ݱ��
			break;
		case OPERSTATE_DELETE_LIN:							//��ǰΪɾ����״̬
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);// ����ϵת��
			DotToPnt(point, dot);
			FindLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx);//�������
			if (GLinNdx != -1)
			{
				GLinLNum--;									//�߼�����1
				GLinChanged = true;							//�����ݱ��
				LIN_NDX_STRU TmpLinNdx;
				D_DOT dot1, dot2;
				POINT pnt1, pnt2;
				//����ʱ�������ļ��ж�ȡ������
				ReadTempFileToLinNdx(GLinTmpNdxF, GLinNdx, TmpLinNdx);
				TmpLinNdx.isDel = 1;						//����ɾ����־
				UpdateLin(GLinTmpNdxF, GLinNdx, TmpLinNdx);	//����������
				for (int i = 0; i < TmpLinNdx.dotNum - 1; ++i)
				{
					//����ʱ�ߵĵ������ļ��ж�ȡ��
					ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i, dot1);
					ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i + 1, dot2);
					//����ϵת��������ת��������ϵ��
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					DrawSeg(&dc, TmpLinNdx, pnt1, pnt2);	//�ػ棨���ģʽ������
				}
				GLinNdx = -1;
			}
			break;
		case OPERSTATE_MOVE_LIN:							//��ǰΪ�ƶ��߲���״̬
			if (GLinNdx != -1)
			{
				if (GLinLBDPnt.x != -1 && GLinLBDPnt.y != -1)
				{
					D_DOT dot1, dot2;
					PntToDot(dot1, point);
					PntVPtoDP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);// ����ϵת��
					PntToDot(dot2, GLinLBDPnt);
					PntVPtoDP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);// ����ϵת��
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
		case OPERSTATE_LINK_LIN:							//��ǰΪ������״̬
			if (GnLine < 2)
			{
				LIN_NDX_STRU line;
				D_DOT dot;
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);// ����ϵת��
				DotToPnt(point, dot);
				// ��������λ���������
				line = FindLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx);
				if (GLinNdx != -1)
				{
					GnLine++;
					if (GnLine == 1)						// ѡ�е�һ����
					{
						GStartLin = line;
						GnStart = GLinNdx;
					}
					else if (GnLine == 2)					// ѡ�еڶ�����
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
					if (GnLine == 1)						// ѡ�е�һ���ߣ���˵㻭Բ���
					{
						// ����ʱ�ļ��ж�ȡ�ߵ���㲢����תΪ�������꣬��Բ
						ReadTempFileToLinDat(GLinTmpDatF, GStartLin.datOff, 0, pt);
						PntDPtoVP(pt, GZoom, GZoomOffset_x, GZoomOffset_y);
						dc.Ellipse((long)pt.x - 2, (long)pt.y - 2, (long)pt.x + 2, (long)pt.y + 2);
						// ����ʱ�ļ��ж�ȡ�ߵ��յ㲢����תΪ�������꣬��Բ
						ReadTempFileToLinDat(GLinTmpDatF, GStartLin.datOff, GStartLin.dotNum - 1, pt);
						PntDPtoVP(pt, GZoom, GZoomOffset_x, GZoomOffset_y);
						dc.Ellipse((long)pt.x - 2, (long)pt.y - 2, (long)pt.x + 2, (long)pt.y + 2);
					}
					else									//ѡ�еڶ����ߣ�������
					{
						// �ı��ߵĵ����ݣ����������ߵĵ�����д���ļ���
						AlterLindot(GLinTmpDatF, GStartLin, GEndLin, GnStart, GnEnd, GLin.datOff);
						AlterStartLin(GLinTmpNdxF, GLin.datOff, GnStart, GStartLin.dotNum + GEndLin.dotNum);// �޸ĵ�һ��������
						AlterEndLin(GLinTmpNdxF, GnEnd);	// �޸ĵڶ���������
						GLin.datOff += (GStartLin.dotNum + GEndLin.dotNum) * sizeof(D_DOT);// ����������
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
		case OPERSTATE_UNDELETE_LIN:						//��ǰ״̬���ָ��ߣ�
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);// ����ϵת��
			DotToPnt(point, dot);
			GLinLNum++;
			FindDeleteLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx);//�������
			if (GLinNdx != -1)
			{
				GLinChanged = true;							//�����ݱ��
				LIN_NDX_STRU TmpLinNdx;
				D_DOT dot1, dot2;
				POINT pnt1, pnt2;
				//����ʱ�������ļ��ж�ȡ������
				ReadTempFileToLinNdx(GLinTmpNdxF, GLinNdx, TmpLinNdx);
				TmpLinNdx.isDel = 0;						//����ɾ����־
				UpdateLin(GLinTmpNdxF, GLinNdx, TmpLinNdx);	//����������
				for (int i = 0; i < TmpLinNdx.dotNum - 1; ++i)
				{
					//����ʱ�ߵĵ������ļ��ж�ȡ��
					ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i, dot1);
					ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i + 1, dot2);
					//����ϵת��������ת��������ϵ��
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					DrawSeg(&dc, TmpLinNdx, pnt1, pnt2);	//�ػ棨���ģʽ������
				}
				GLinNdx = -1;
			}
			break;
		case OPERSTATE_MODIFY_LINE_PARAMETER:				//��ǰΪ�޸��߲�������״̬
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//����ת��������ϵ
			DotToPnt(point, dot);
			LIN_NDX_STRU tempLine;
			memcpy_s(&tempLine, sizeof(LIN_NDX_STRU),
				&FindLin(GLinTmpNdxF,GLinTmpDatF,point,GLinNum,GLinNdx), sizeof(LIN_NDX_STRU));
			//���������
			if (GLinNdx != -1)
			{
				CLineParameterDlg dlg;						//�߲������öԻ���
				dlg.m_ColorButton.SetColor(tempLine.color);
				dlg.m_Pattern = tempLine.pattern;
				if (IDOK == dlg.DoModal())
				{
					COLORREF tempColor = dlg.m_ColorButton.GetColor();
					memcpy_s(&tempLine.color, sizeof(COLORREF), &tempColor,
						sizeof(COLORREF));
					tempLine.pattern = dlg.m_Pattern;
					GLinTmpNdxF->Seek(GLinNdx * sizeof(LIN_NDX_STRU), CFile::begin);
					GLinTmpNdxF->Write(&tempLine, sizeof(LIN_NDX_STRU));//д�������
				}
				this->Invalidate();
				GLinChanged = true;
				GLinNdx = -1;
			}
			break;
		default:
			break;
		}
	}
	if (GRegFCreated)
	{
		D_DOT dot;
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_REG:							//��ǰΪ��������״̬
			if (GTReg.dotNum == 0)
			{
				memcpy_s(&GTReg, sizeof(REG_NDX_STRU), &GReg, sizeof(REG_NDX_STRU));
			}
			if (GRegCreateStartPnt.x == -1 && GRegCreateStartPnt.y == -1)
			{
				GRegCreateStartPnt = point;
			}
			if (GRegCreateMMPnt.x == -1 && GRegCreateMMPnt.y == -1)
			{
				GRegCreateMMPnt = point;
			}
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//����ת����
			WriteRegDatToFile(GRegTmpDatF, GReg.datOff, GTReg.dotNum, dot);
																//�����ĵ�����д���ļ�
			GTReg.dotNum++;										//���ڵ�����һ
			if (GTReg.dotNum == 2)
			{
				this->Invalidate();								//���ڵ�������3����ȡ������
			}
			GRegChanged = true;
			break;
		case OPERSTATE_DELETE_REG:								//��ǰΪɾ��������״̬
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//����ת��������
			DotToPnt(point,dot);
			FindReg(GRegTmpNdxF, GRegTmpDatF, point, GRegNum, GRegNdx);//������
			if (GRegNdx != -1) {
				GRegLNum--;
				GRegChanged = true;
				REG_NDX_STRU TmpRegNdx;
				ReadTempFileToRegNdx(GRegTmpNdxF, GRegNdx, TmpRegNdx);//����ʱ�ļ��ж�ȡɾ����������
				TmpRegNdx.isDel = 1;							//����ɾ�����
				UpdateReg(GRegTmpNdxF, GRegNdx, TmpRegNdx);		//����������
				D_DOT* dot = new D_DOT[TmpRegNdx.dotNum];
				GRegTmpDatF->Seek(TmpRegNdx.datOff, CFile::begin);
				GRegTmpDatF->Read(dot, TmpRegNdx.dotNum * sizeof(D_DOT));
				for (int i = 0; i < TmpRegNdx.dotNum; ++i)
				{
																// ��ɾ�����ĵ���������תΪ��������
					PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
				}
				POINT * pnt = new POINT[TmpRegNdx.dotNum];
				DotToPnt(pnt, dot, TmpRegNdx.dotNum);
				DrawReg(&dc, TmpRegNdx, pnt, TmpRegNdx.dotNum);	//�ػ�(������)
				delete[] pnt;
				delete[] dot;
				GRegNdx = -1;
			}
			break;
		case OPERSTATE_MOVE_REG:								//��ǰΪ�ƶ�������״̬
			if (GRegNdx != -1)
			{
				if (GRegLBDPnt.x != -1 && GRegLBDPnt.y != -1)
				{
					D_DOT dot1, dot2;
					PntToDot(dot1, point);
					PntVPtoDP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntToDot(dot2, GRegLBDPnt);
					PntVPtoDP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					double offset_x = dot1.x - dot2.x;
					double offset_y = dot1.y - dot2.y;
					UpdateReg(GRegTmpNdxF, GRegTmpDatF, GRegNdx, offset_x, offset_y);// ����������
					GRegNdx = -1;
					GRegMMOffsetX = 0;
					GRegMMOffsetY = 0;
					GRegChanged = true;
				}
			}
			break;
		case OPERSTATE_UNDELETE_REG:						//��ǰ״̬���ָ�����
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);// ����ϵת��
			DotToPnt(point, dot);
			FindDeleteReg(GRegTmpNdxF, GRegTmpDatF, point, GRegNum, GRegNdx);//������
			if (GRegNdx != -1) {
				GRegLNum++;
				GRegChanged = true;
				REG_NDX_STRU TmpRegNdx;
				ReadTempFileToRegNdx(GRegTmpNdxF, GRegNdx, TmpRegNdx);//����ʱ�ļ��ж�ȡ�ָ���������
				TmpRegNdx.isDel = 0;							//����ɾ�����
				UpdateReg(GRegTmpNdxF, GRegNdx, TmpRegNdx);		//����������
				D_DOT* dot = new D_DOT[TmpRegNdx.dotNum];
				GRegTmpDatF->Seek(TmpRegNdx.datOff, CFile::begin);
				GRegTmpDatF->Read(dot, TmpRegNdx.dotNum * sizeof(D_DOT));
				for (int i = 0; i < TmpRegNdx.dotNum; ++i)
				{
					// ���ָ����ĵ���������תΪ��������
					PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
				}
				POINT * pnt = new POINT[TmpRegNdx.dotNum];
				DotToPnt(pnt, dot, TmpRegNdx.dotNum);
				DrawReg(&dc, TmpRegNdx, pnt, TmpRegNdx.dotNum);	//�ػ�(������)
				delete[] pnt;
				delete[] dot;
				GRegNdx = -1;
			}
			break;
		case OPERSTATE_MODIFY_REGION_PARAMETER:				//��ǰΪ�޸�����������״̬
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//����ת��������ϵ
			DotToPnt(point, dot);
			REG_NDX_STRU tempRegion;
			memcpy_s(&tempRegion, sizeof(REG_NDX_STRU),
				&FindReg(GRegTmpNdxF, GRegTmpDatF, point, GRegNum, GRegNdx), sizeof(REG_NDX_STRU));
			//���������
			if (GRegNdx != -1)
			{
				CRegionParameterDlg dlg;					//���������öԻ���
				dlg.m_ColorButton.SetColor(tempRegion.color);
				dlg.m_Pattern = tempRegion.pattern;
				if (IDOK == dlg.DoModal())
				{
					COLORREF tempColor = dlg.m_ColorButton.GetColor();
					memcpy_s(&tempRegion.color, sizeof(COLORREF), &tempColor,
						sizeof(COLORREF));
					tempRegion.pattern = dlg.m_Pattern;
					GRegTmpNdxF->Seek(GRegNdx * sizeof(REG_NDX_STRU), CFile::begin);
					GRegTmpNdxF->Write(&tempRegion, sizeof(REG_NDX_STRU));//д��������
				}
				this->Invalidate();
				GRegChanged = true;
				GRegNdx = -1;
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
		case OPERSTATE_ZOOM_IN:								//��ǰΪ�Ŵ����״̬
			GetClientRect(&client);
			if (abs(GZoomLBDPnt.x - GZoomMMPnt.x) <= 10 && 
				abs(GZoomLBDPnt.y - GZoomMMPnt.y) <= 10)
			{
				GZoomStyle = 0;								//�����Ŵ�
			}
			else
			{
				GZoomStyle = 1;								//����Ŵ�
			}
			if (GZoomStyle == 0)
			{												//�����Ŵ�
				double x0 = point.x - (client.right / 2.0) + (client.right / 8.0);
				double y0 = point.y - (client.bottom / 2.0) + (client.bottom / 8.0);
				GZoomOffset_x += (x0 / GZoom);				//ƫ������x
				GZoomOffset_y += (y0 / GZoom);				//ƫ������y
				GZoom *= 4 / 3.0;							//����ϵ����4/3
			}
			else
			{												//����Ŵ�
				rect.right = max(point.x, GZoomLBDPnt.x);
				rect.left = min(point.x, GZoomLBDPnt.x);
				rect.bottom = max(point.y, GZoomLBDPnt.y);
				rect.top = min(point.y, GZoomLBDPnt.y);
				modulusZoom(client, rect, zoom);
				double x0 = GetCenter(rect).x - (client.right / 2.0) + 
					(client.right *(zoom - 1) / (2.0* zoom));
				double y0 = GetCenter(rect).y - (client.bottom / 2.0) + 
					(client.bottom* (zoom - 1) / (2.0* zoom));
				GZoomOffset_x += (x0 / GZoom);				//ƫ������x
				GZoomOffset_y += (y0 / GZoom);				//ƫ������y
				GZoom *= zoom;								//����ϵ��
				GZoomStyle = 0;
			}
			GZoomLBDPnt = CPoint(-1, -1);
			GZoomMMPnt = CPoint(-1, -1);
			this->Invalidate();
			break;
		case OPERSTATE_ZOOM_OUT:							//��ǰΪ��С����״̬
			if (true)
			{
				GetClientRect(&client);
				double x0 = point.x - (client.right / 2.0) - (client.right / 8.0);
				double y0 = point.y - (client.bottom / 2.0) + (client.bottom / 8.0);
				GZoomOffset_x += (x0 / GZoom);				//ƫ������x
				GZoomOffset_y += (y0 / GZoom);				//ƫ������y
				GZoom *= 3 / 4.0;							//����ϵ��Ϊ3/4
				this->Invalidate();
			}
			break;
		case OPERSTATE_WINDOW_MOVE:							//��ǰΪ�����ƶ�����״̬
			GWinMoveLBDPnt = CPoint(-1, 1);					//��λ�ƶ�����ʱ������µ�
			GWinMoveMMPnt = CPoint(-1, -1);					//��λ�ƶ������ƶ�ǰ״̬��λ��
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

	// TODO: �ڴ˴������Ϣ����������
	if (GPntFCreated)//�������ʱ�ļ��Ѵ�������رղ�ɾ��
	{
		if (GPntTmpF->m_hFile != CFile::hFileNull)
		{
			GPntTmpF->Close();
			GPntTmpF->Remove(GPntTmpFName);
		}
	}
	delete GPntTmpF;		//ɾ������ʱ�ļ�����
	//�ߡ�����ع�����δʵ�֣����Ȱ����´������
	if (GLinFCreated)		//�������ʱ�ļ��Ѵ�������رղ�ɾ��
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
	delete GLinTmpDatF;									//ɾ������ʱ�ļ�����
	delete GLinTmpNdxF;		
	if (GRegFCreated)									//�������ʱ�ļ��Ѵ�������رղ�ɾ��
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
	delete GRegTmpDatF;									//ɾ������ʱ�ļ�����
	delete GRegTmpNdxF;		
}


void CMapEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GPntFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_PNT:						//�ƶ������
			if (GPntNdx != -1)
			{
				CClientDC dc(this);						//��ñ����ڻ�ǰ���ͼ
				dc.SetROP2(R2_NOTXORPEN);				//�������ģʽ����
				DrawPnt(&dc, GTPnt);					//��ԭλ�û������ԭ��ͼ��
				GTPnt.x = point.x;						//�ƶ��������x
				GTPnt.y = point.y;						//�ƶ��������y
				DrawPnt(&dc, GTPnt);					//����λ�û�
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
		case OPERSTATE_INPUT_LIN:						//��ǰΪ������״̬
			if (GTLin.dotNum > 0)
			{
				CClientDC dc(this);						//��ñ����ڻ�ǰ���ͼ
				dc.SetROP2(R2_NOTXORPEN);				//�������ģʽ����
				if (GMPnt.x != -1 && GMPnt.y != -1)
				{
					DrawSeg(&dc, GTLin, GLPnt, GMPnt);	//Ĭ����ʽ�����߶�
				}
				GMPnt.x = point.x;						//���������һ״̬��(x)
				GMPnt.y = point.y;						//���������һ�����(y)
				POINT mpoint = { mpoint.x = point.x,mpoint.y = point.y };
				DrawSeg(&dc, GTLin, GLPnt, mpoint);		//Ĭ����ʽ�����߶�
			}
			break;
		case OPERSTATE_MOVE_LIN:						//��ǰΪ�ƶ���״̬
			if (GLinNdx != -1)
			{
				CClientDC dc(this);
				dc.SetROP2(R2_NOTXORPEN);				//�������ģʽ
				D_DOT dot1, dot2;
				POINT pnt1, pnt2;
														// ����ԭ������
				for (int i = 0; i < GLinMMTmpNdx.dotNum - 1; i++)
				{
														// ����ʱ�ļ��ж�ȡ�ߵĵ�
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i, dot1);
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i + 1, dot2);
														//����ϵת��������ת���ڣ�
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					pnt1.x += GLinMMOffestX;
					pnt1.y += GLinMMOffestY;
					pnt2.x += GLinMMOffestX;
					pnt2.y += GLinMMOffestY;
					DrawSeg(&dc, GLinMMTmpNdx, pnt1, pnt2);//�ػ棨���ģʽ������
				}
														// ����ƫ����
				GLinMMOffestX = GLinMMOffestX + point.x - GLinMMPnt.x;
				GLinMMOffestY = GLinMMOffestY + point.y - GLinMMPnt.y;
														// ���µ�λ�û���һ���µ��߶�
				for (int i = 0; i < GLinMMTmpNdx.dotNum - 1; i++)
				{
														// ����ʱ�ļ��ж�ȡ�ߵĵ�
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i, dot1);
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i + 1, dot2);
														//ת������ϵ������ת���ڣ�
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					pnt1.x += GLinMMOffestX;
					pnt1.y += GLinMMOffestY;
					pnt2.x += GLinMMOffestX;
					pnt2.y += GLinMMOffestY;
					DrawSeg(&dc, GLinMMTmpNdx, pnt1, pnt2);//�ػ棨�������ߣ�
				}
				GLinMMPnt = point;
			}
			break;
		default:
			break;
		}
	}
	if (GRegFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_REG:								//��ǰΪ��������״̬
			if (GRegCreateMMPnt.x != -1 && GRegCreateMMPnt.y != -1)
			{
				CClientDC dc(this);
				dc.SetROP2(R2_NOTXORPEN);
				LIN_NDX_STRU tln = { tln.pattern = GTReg.pattern, tln.color = GTReg.color };
				//����������
				if (GTReg.dotNum == 1)
				{
					DrawSeg(&dc, tln, GRegCreateStartPnt, GRegCreateMMPnt);
					DrawSeg(&dc, tln, GRegCreateStartPnt, point);
				}
				else
				{
					D_DOT* dot = new D_DOT[GTReg.dotNum];
					for (int i = 0; i < GTReg.dotNum; ++i)
					{
						ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, i, dot[i]);
						//����ʱ�ļ��ж�ȡ���ĵ�����
						PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
						//�����ĵ���������ת��Ϊ��������
					}
					POINT* pnt = new POINT[GTReg.dotNum + 1];
					DotToPnt(pnt, dot, GTReg.dotNum);
					pnt[GTReg.dotNum] = GRegCreateMMPnt;
					DrawReg(&dc, GTReg, pnt, GTReg.dotNum + 1);
					pnt[GTReg.dotNum] = point;
					DrawReg(&dc, GTReg, pnt, GTReg.dotNum + 1);
					delete[] dot;
					delete[] pnt;
				}
				GRegCreateMMPnt = point;
			}
			break;
		case OPERSTATE_MOVE_REG:
			if (GRegNdx != -1)
			{
				CClientDC dc(this);
				dc.SetROP2(R2_NOTXORPEN);							// ����������
				D_DOT * dot = new D_DOT[GRegMMTmpNdx.dotNum];
																	// ����ԭ������
				for (int i = 0; i < GRegMMTmpNdx.dotNum; i++)
				{
					ReadTempFileToRegDat(GRegTmpDatF, GRegMMTmpNdx.datOff, i, dot[i]);
					PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
					dot[i].x += GRegMMOffsetX;
					dot[i].y += GRegMMOffsetY;
				}
				POINT* pnt = new POINT[GRegMMTmpNdx.dotNum];
				DotToPnt(pnt, dot, GRegMMTmpNdx.dotNum);
				DrawReg(&dc, GRegMMTmpNdx, pnt, GRegMMTmpNdx.dotNum);
																	// ����ƫ����
				GRegMMOffsetX = GRegMMOffsetX + point.x - GRegMMPnt.x;
				GRegMMOffsetY = GRegMMOffsetY + point.y - GRegMMPnt.y;
																	// ���µ�λ�û���һ����
				for (int i = 0; i < GRegMMTmpNdx.dotNum; i++)
				{
					ReadTempFileToRegDat(GRegTmpDatF, GRegMMTmpNdx.datOff, i, dot[i]);
					PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
					dot[i].x += GRegMMOffsetX;
					dot[i].y += GRegMMOffsetY;
				}
				DotToPnt(pnt, dot, GRegMMTmpNdx.dotNum);
				DrawReg(&dc, GRegMMTmpNdx, pnt, GRegMMTmpNdx.dotNum);
				delete[] dot;
				delete[] pnt;
				GRegMMPnt = point;
			}
			break;
		default:
			break;
		}
	}
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		CClientDC dc(this);								//��ñ����ڻ�ǰ���ͼ
		CPen pen(PS_DOT, 1, RGB(0, 0, 0));
		CPen * oldPen = dc.SelectObject(&pen);
		switch (GCurOperState)
		{
		case OPERSTATE_ZOOM_IN:							//��ǰΪ�Ŵ����״̬
			if (GZoomLBDPnt.x != -1 && GZoomMMPnt.y != -1)
			{
				dc.SetROP2(R2_NOTXORPEN);				//����Ϊ���ģʽ����
				dc.Rectangle(GZoomLBDPnt.x, GZoomLBDPnt.y, GZoomMMPnt.x, GZoomMMPnt.y);
				dc.Rectangle(GZoomLBDPnt.x, GZoomLBDPnt.y, point.x, point.y);
				GZoomMMPnt = point;
				dc.SelectObject(oldPen);
			}
			break;
		case OPERSTATE_WINDOW_MOVE:						//��ǰΪ�����ƶ�����״̬
			if (GWinMoveMMPnt.x != -1 && GWinMoveMMPnt.y != -1)
			{
				CPoint offset(0, 0);					//����ƶ�ƫ����
				offset.x = point.x - GWinMoveLBDPnt.x;
				offset.y = point.y - GWinMoveLBDPnt.y;
				GZoomOffset_x -= offset.x / GZoom;		//�任�Ŵ�����Сʱ��ƫ����
				GZoomOffset_y -= offset.y / GZoom;
				GWinMoveLBDPnt = point;
				this->Invalidate();
			}
			break;
		default:
			break;
		}
	}
	CView::OnMouseMove(nFlags, point);
}


void CMapEditorView::OnUpdateWindowShowPoint(CCmdUI *pCmdUI)
{
	//��ʾ״̬����ʾδɾ��״̬������ʾ���򽫲˵����ѡ��
	if (GCurShowState == SHOWSTATE_UNDEL && GShowPnt == true)
	{
		pCmdUI->SetCheck(1);//�˵�ѡ�б��
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}


void CMapEditorView::OnUpdateWindowShowLine(CCmdUI *pCmdUI)
{
	//��ʾ״̬����ʾδɾ��״̬������ʾ���򽫲˵����ѡ��
	if (GCurShowState == SHOWSTATE_UNDEL && GShowLin == true)
	{
		pCmdUI->SetCheck(1);//�˵�ѡ�б��
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}


void CMapEditorView::OnUpdateWindowShowRegion(CCmdUI *pCmdUI)
{
	//��ʾ״̬����ʾδɾ��״̬������ʾ���򽫲˵����ѡ��
	if (GCurShowState == SHOWSTATE_UNDEL && GShowReg == true)
	{
		pCmdUI->SetCheck(1);//�˵�ѡ�б��
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}


void CMapEditorView::OnUpdatePointShowDeleted(CCmdUI *pCmdUI)
{
	//����ǰ��ʾ״̬����ʾɾ��״̬����ʾ�㣬�˵����ѡ�У�����ȡ��
	if (GCurShowState == SHOWSTATE_DEL && GShowPnt == true)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}


void CMapEditorView::OnUpdateLineShowDeleted(CCmdUI *pCmdUI)
{
	//����ǰ��ʾ״̬����ʾɾ��״̬����ʾ�ߣ��˵����ѡ�У�����ȡ��
	if (GCurShowState == SHOWSTATE_DEL && GShowLin == true)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}


void CMapEditorView::OnUpdateRegionShowDeleted(CCmdUI *pCmdUI)
{
	//����ǰ��ʾ״̬����ʾɾ��״̬����ʾ�����˵����ѡ�У�����ȡ��
	if (GCurShowState == SHOWSTATE_DEL && GShowReg == true)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}
