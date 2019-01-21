#pragma once
#ifndef CALCULATE_H
#define CALCULATE_H
#include"MyDataType.h"

//�����������ĺ�������
double Distance(double x1, double y1, double x2, double y2);

//���������ĺ�������
PNT_STRU FindPnt(CPoint mousePoint, int pntNum, CFile*pntTmpF, int &nPnt);

//�޸���ʱ�ļ��е����ݵĺ�������
void UpdatePnt(CFile*pntTmpF, int i, PNT_STRU pnt);

void PntToDot(D_DOT*dot, POINT*pnt, int num);

void PntToDot(D_DOT & dot, POINT pnt);

void DotToPnt(POINT*pnt, D_DOT*dot, int num);

void DotToPnt(POINT & pnt, D_DOT dot);

//������굥��λ�õ��ߵľ���ĺ�������
double DisPntToSeg(D_DOT pt1, D_DOT pt2, D_DOT pt);

//���������������ߵĺ�������
LIN_NDX_STRU FindLin(CFile * LinTmpNdxF, CFile * LinTmpDatF, 
	CPoint mousePoint, int LinNum, int &nLinNdx);

//��������ϵת������������ϵ�ĺ�������
void PntDPtoVP(D_DOT &pt, double zoom, double offset_x, double offset_y);

//����������ĵĺ�������
D_DOT GetCenter(RECT rect);

//��������Ŵ�ʱ�Ŵ����ĺ�������
void modulusZoom(RECT client, RECT rect, double&zoom);

//��������ϵת������������ϵ�ĺ�������
void PntVPtoDP(D_DOT&pt, double zoom, double offset_x, double offset_y);

// �ı��ߵĵ����ݵĺ�������
void AlterLindot(CFile * LinTmpDatF, LIN_NDX_STRU startLine, LIN_NDX_STRU endLine, int start, int end, long allDatOff);


#endif // !CALCULATE_H
