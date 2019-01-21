#pragma once
#ifndef PAINT_H
#define PAINT_H
#include"MyDataType.h"
#include"WriteOrRead.h"
#include"Calculate.h"

void DrawPnt(CClientDC*dc, PNT_STRU point);

//显示所有点函数声明
void ShowAllPnt(CClientDC*dc, CFile*pntTmpF, int pntNum);

//构造线段的函数声明
void DrawSeg(CClientDC*dc, LIN_NDX_STRU line, POINT point1, POINT point2);

//显示所有线的函数声明
void ShowAllLin(CClientDC*dc, CFile*LinTmpNdxF, CFile*LinTmpDatF, int LinNum);

//显示所有点（新）的函数声明
void ShowAllPnt(CClientDC * dc, CFile * PntTmpF, int PntNum, 
	double zoomOffset_x, double zoomOffset_y, double zoom, char isDel);

//显示所有线（新）的函数声明
void ShowAllLin(CClientDC * dc, CFile * LinTmpNdxF, CFile * LinTmpDatF, int LinNum, 
	double zoomOffset_x, double zoomOffset_y, double zoom, char isDel);


#endif // !PAINT_H
