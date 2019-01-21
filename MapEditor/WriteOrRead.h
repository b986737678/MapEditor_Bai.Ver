#pragma once
#ifndef WRITEORREAD_H
#include"MyDataType.h"

//将点数据写入临时文件的函数声明
void WritePntToFile(CFile*PntTmpF, int i, PNT_STRU point);		

//从临时点文件读取点数据的函数声明
void ReadTempFileToPnt(CFile*PntTmpF, int i, PNT_STRU&point);	

//从永久文件读到临时文件的函数声明（点）
void ReadPntPermanentFileToTemp(CFile*pntF, CFile*pntTmpF, int&nPnt, int &nLPnt);

//将线数据写入到临时索引文件的函数声明
void WriteLinNdxToFile(CFile*linTmpNdxF, int i, LIN_NDX_STRU line);

//将线节点数据写入到线临时数据文件的函数声明
void WriteLinDatToFile(CFile*linTmpDatF, long datOff, int i, D_DOT point);

//从临时线数据文件中读取线的点数据的函数声明
void ReadTempFileToLinDat(CFile*LinTmpDatF, long datOff, int i, D_DOT&pnt);

//从临时线索引文件中读取线索引的函数声明
void ReadTempFileToLinNdx(CFile*LinTmpNdxF, int i, LIN_NDX_STRU&LinNdx);

//将线的索引和点数据写入到永久文件的函数声明
void WriteTempToLinPermanentFile(CFile*LinF, CFile*LinTmpDatF, CFile*
	LinTmpNdxF, VERSION LinVer, int nLin, int nLLin);

//从永久文件读取线数据到临时文件的函数声明
void ReadLinPermanentFileToTemp(CFile*LinF, CFile*LinTmpDatF,
	CFile*LinTmpNdxF, VERSION&LinVer, int&nLin, int&nLLin, long&TmpFLinDatOffset);

//更新线数据的函数声明
void UpdateLin(CFile*LinTmpNdxF, int nLin, LIN_NDX_STRU line);

//更新线的点数据到临时文件的函数声明
void UpdateLin(CFile * LinTmpNdxF, CFile * LinTmpDatF, int LinNdx,
	double offset_x, double offset_y);

//修改第一条线索引的函数声明
void AlterStartLin(CFile * LinTmpNdxF, long subdatOff, int nLine, int subNum);
// 修改第二条线索引的函数声明
void AlterEndLin(CFile * LinTmpNdxF, int nLine);

#endif // !WRITEORREAD_H


