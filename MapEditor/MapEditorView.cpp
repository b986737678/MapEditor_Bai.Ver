
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///-------------------------��������ص�ȫ�ֿ��Ʊ���-------------------------///
bool	GPntFCreated = false;						//��ʱ�ļ��Ƿ񴴽�
CString GPntFName;									//�����ļ�������·����
CString GPntTmpFName = CString("tempPntF.dat");		//��ʱ�ļ�������·����
bool	GPntChanged = false;						//�Ƿ����
int		GPntNum = 0;								//������
int		GPntLNum = 0;								//�߼���
CFile	*GPntTmpF = new CFile();					//��ȡ��ʱ�ļ���ָ�����

///-------------------------��������ص�ȫ�ֿ��Ʊ���-------------------------///
bool	GLinFCreated = false;						//��ʱ�ļ��Ƿ񴴽�
CString	GLinFName;									//�����ļ�������·����
CString	GLinTmpNdxFName = CString("tempLinF.ndx");	//��ʱ�����ļ�������·����
CString GLinTmpDatFName = CString("tempLinF.dat");	//��ʱ�����ļ�������·����
bool	GLinChanged = false;						//�Ƿ����
int		GLinNum = 0;								//������
int		GLinLNum = 0;								//�߼���
CFile	*GLinTmpDatF = new CFile();					//��ȡ��ʱ�����ļ���ָ�����
CFile	*GLinTmpNdxF = new CFile();					//��ȡ��ʱ�����ļ���ָ�����

///-------------------------��������ص�ȫ�ֿ��Ʊ���-------------------------///
bool	GRegFCreated = false;						//��ʱ�ļ��Ƿ񴴽�
CString	GRegFName;									//�����ļ�������·����
CString	GRegTmpNdxFName = CString("tempRegF.ndx");	//��ʱ�����ļ�������·����
CString GRegTmpDatFName = CString("tempRegF.dat");	//��ʱ�����ļ�������·����
bool	GRegChanged = false;						//�Ƿ����
int		GRegNum = 0;								//������
int		GRegLNum = 0;								//�߼���
CFile	*GRegTmpDatF = new CFile();					//��ȡ��ʱ�����ļ���ָ�����
CFile	*GRegTmpNdxF = new CFile();					//��ȡ��ʱ�����ļ���ָ�����

///-------------------------------��������---------------------------------///
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

///-------------------------Ĭ�ϵ�ṹ����ʱ��ṹ---------------------------///
PNT_STRU GPnt = {	
	GPnt.isDel = 0,
	GPnt.color = RGB(0,0,0),
	GPnt.pattern = 0 
};													//Ĭ�ϵ����

///-------------------------�ļ��汾��Ϣ��ȫ�ֱ���---------------------------///
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
///---------------------���ļ��İ汾��Ϣ���Լ����ʼ��---------------------///
VERSION GLinVer =
{
	GLinVer.flag[0] = 'L',
	GLinVer.flag[1] = 'I',
	GLinVer.flag[2] = 'N',
	GLinVer.version = 10							//Ĭ�ϰ汾��
};
///-------------------------�ҵ���λ���ļ��е�λ��-------------------------///
int GLinNdx = -1;

///---------------------------�ƶ������ȫ�ֱ���---------------------------///
CPoint			GLinLBDPnt(-1, -1);					// ��¼���������µ�λ�ã���������ƫ����
CPoint			GLinMMPnt(-1, -1);					// ��¼����ƶ�����һ��״̬�����������ƶ�ʱ��ǰһ����
long			GLinMMOffestX = 0;					// ��¼����ƶ�ʱ���X���ƫ����
long			GLinMMOffestY = 0;					// ��¼����ƶ�ʱ���Y���ƫ����
LIN_NDX_STRU	GLinMMTmpNdx;						// ��¼���ѡ�е��ߵ�����

///-------------------------���ڷŴ���С��ȫ�ֱ���-------------------------///
CPoint GZoomLBDPnt(-1, -1);							//�Ŵ�ʱ������̧��ĵ�
CPoint GZoomMMPnt(-1, -1);							//�Ŵ�ʱ����ƶ�ǰһ״̬

double	GZoomOffset_x = 0;							//ƫ������
double	GZoomOffset_y = 0;
double	GZoom = 1.0;								//����ϵ��
int		GZoomStyle = 0;								//�Ŵ�ʽ

///---------------------------���������ȫ�ֱ���---------------------------///
LIN_NDX_STRU	GStartLin = GLin;					// ѡ�еĵ�һ����
int				GnStart = -1;
LIN_NDX_STRU	GEndLin = GLin;						// ѡ�еĵڶ�����
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

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	CRect mrect;
	GetClientRect(&mrect);				//��ȡ���ڿͻ���������
	CClientDC dc(this);
	dc.FillSolidRect(0, 0, mrect.Width(), mrect.Height(), dc.GetBkColor());//��һ����ɫ���һ������
	dc.SetROP2(R2_NOTXORPEN);			//���û�ͼģʽ
	//ShowAllPnt(&dc, GPntTmpF, GPntNum);	//������ʾ���е�
	//ShowAllLin(&dc, GLinTmpNdxF, GLinTmpDatF, GLinNum);//������ʾ������
	//������ʾ���е�
	ShowAllPnt(&dc, GPntTmpF, GPntNum, GZoomOffset_x, GZoomOffset_y, GZoom, 0);
	//������ʾ������
	ShowAllLin(&dc, GLinTmpNdxF, GLinTmpDatF, GLinNum, GZoomOffset_x, GZoomOffset_y, GZoom, 0);

	ReleaseDC(&dc);						//�ͷ�dc
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
	// TODO: �ڴ���������������
}


void CMapEditorView::OnWindowReset()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnWindowShowPoint()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnWindowShowLine()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnWindowShowRegion()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnPointCreate()
{
	// TODO: �ڴ���������������
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_INPUT_PNT;				//����Ϊ����㡱״̬
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
		GCurOperState = OPERSTATE_MOVE_PNT;					//���ò���״̬���ƶ��㣩
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
		GCurOperState = OPERSTATE_DELETE_PNT;				//���ò���״̬
	}
	else
	{
		MessageBox(L"TempFile has not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnPointSetDefparameter()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnPointShowDeleted()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnPointUndelete()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnPointModifyParameter()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnLineCreate()
{
	// TODO: �ڴ���������������
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_INPUT_LIN;				//��ǰΪ����״̬
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
		GCurOperState = OPERSTATE_MOVE_LIN;					//��ǰΪ�ƶ��߲���״̬
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
		GCurOperState = OPERSTATE_DELETE_LIN;				//��ǰ״̬Ϊɾ����

	}
	else
	{
		MessageBox(L"Temp File have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineShowDeleted()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnLineUndeleted()
{
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
}


void CMapEditorView::OnLineSetDefparameter()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnRegionCreate()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnRegionMove()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnRegionDelete()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnRegionShowDeleted()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnRegionUndeleted()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnRegionModifyParameter()
{
	// TODO: �ڴ���������������
}


void CMapEditorView::OnRegionSetDefparameter()
{
	// TODO: �ڴ���������������
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
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_ZOOM_IN:								//��ǰΪ�Ŵ����״̬
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
				GLinNum--;
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
		default:
			break;
		}
	}
	CView::OnMouseMove(nFlags, point);
}
