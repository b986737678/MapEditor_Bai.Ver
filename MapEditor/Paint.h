#pragma once
#ifndef PAINT_H
#define PAINT_H
#include"MyDataType.h"
#include"WriteOrRead.h"
#include"Calculate.h"

void DrawPnt(CClientDC*dc, PNT_STRU point);

//��ʾ���е㺯������
void ShowAllPnt(CClientDC*dc, CFile*pntTmpF, int pntNum);

//�����߶εĺ�������
void DrawSeg(CClientDC*dc, LIN_NDX_STRU line, POINT point1, POINT point2);

//��ʾ�����ߵĺ�������
void ShowAllLin(CClientDC*dc, CFile*LinTmpNdxF, CFile*LinTmpDatF, int LinNum);

//��ʾ���е㣨�£��ĺ�������
void ShowAllPnt(CClientDC * dc, CFile * PntTmpF, int PntNum, 
	double zoomOffset_x, double zoomOffset_y, double zoom, char isDel);

//��ʾ�����ߣ��£��ĺ�������
void ShowAllLin(CClientDC * dc, CFile * LinTmpNdxF, CFile * LinTmpDatF, int LinNum, 
	double zoomOffset_x, double zoomOffset_y, double zoom, char isDel);


#endif // !PAINT_H
