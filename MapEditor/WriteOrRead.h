#pragma once
#ifndef WRITEORREAD_H
#include"MyDataType.h"

//��������д����ʱ�ļ��ĺ�������
void WritePntToFile(CFile*PntTmpF, int i, PNT_STRU point);		

//����ʱ���ļ���ȡ�����ݵĺ�������
void ReadTempFileToPnt(CFile*PntTmpF, int i, PNT_STRU&point);	

//�������ļ�������ʱ�ļ��ĺ����������㣩
void ReadPntPermanentFileToTemp(CFile*pntF, CFile*pntTmpF, int&nPnt, int &nLPnt);

//��������д�뵽��ʱ�����ļ��ĺ�������
void WriteLinNdxToFile(CFile*linTmpNdxF, int i, LIN_NDX_STRU line);

//���߽ڵ�����д�뵽����ʱ�����ļ��ĺ�������
void WriteLinDatToFile(CFile*linTmpDatF, long datOff, int i, D_DOT point);

//����ʱ�������ļ��ж�ȡ�ߵĵ����ݵĺ�������
void ReadTempFileToLinDat(CFile*LinTmpDatF, long datOff, int i, D_DOT&pnt);

//����ʱ�������ļ��ж�ȡ�������ĺ�������
void ReadTempFileToLinNdx(CFile*LinTmpNdxF, int i, LIN_NDX_STRU&LinNdx);

//���ߵ������͵�����д�뵽�����ļ��ĺ�������
void WriteTempToLinPermanentFile(CFile*LinF, CFile*LinTmpDatF, CFile*
	LinTmpNdxF, VERSION LinVer, int nLin, int nLLin);

//�������ļ���ȡ�����ݵ���ʱ�ļ��ĺ�������
void ReadLinPermanentFileToTemp(CFile*LinF, CFile*LinTmpDatF,
	CFile*LinTmpNdxF, VERSION&LinVer, int&nLin, int&nLLin, long&TmpFLinDatOffset);

//���������ݵĺ�������
void UpdateLin(CFile*LinTmpNdxF, int nLin, LIN_NDX_STRU line);

//�����ߵĵ����ݵ���ʱ�ļ��ĺ�������
void UpdateLin(CFile * LinTmpNdxF, CFile * LinTmpDatF, int LinNdx,
	double offset_x, double offset_y);

//�޸ĵ�һ���������ĺ�������
void AlterStartLin(CFile * LinTmpNdxF, long subdatOff, int nLine, int subNum);

// �޸ĵڶ����������ĺ�������
void AlterEndLin(CFile * LinTmpNdxF, int nLine);

//��������д�뵽��ʱ�����ļ��ĺ�������
void WriteRegNdxToFile(CFile * RegTmpNdxF, int i, REG_NDX_STRU Region);

//�����߽�ڵ�����д�뵽����ʱ�����ļ��ĺ�������
void WriteRegDatToFile(CFile * RegTmpDatF, long datOff, int i, D_DOT point);

//����ʱ�������ļ��ж��������ĺ�������
void ReadTempFileToRegNdx(CFile * RegTmpNdxF, int i, REG_NDX_STRU &RegNdx);

//����ʱ�������ļ��ж�ȡ���ı߽�����ݵĺ�������
void ReadTempFileToRegDat(CFile * RegTempDatF, long datOff, int i, D_DOT &pnt);

//�����������͵�����д�뵽�����ļ��ĺ�������
void WriteTempToRegPermanentFile(CFile * RegF, CFile * RegTmpDatF, CFile * RegTmpNdxF, VERSION RegVer, int nReg, int nLReg);

//�������ļ���ȡ�����ݵ���ʱ�ļ��ĺ�������
void ReadRegPaermanentFileToTemp(CFile * RegF, CFile * RegTmpDatF, CFile * RegTmpNdxF, VERSION &RegVer, int &nReg, int &nLReg, long &TmpFRegDatOffset);

//��ʱ�����ļ����������ݵĺ�������
void UpdateReg(CFile* RegTmpNdxF, int nReg, REG_NDX_STRU Region);

//�������ĺ�������
void UpdateReg(CFile* RegTmpNdxF, CFile* RegTmpDatF, int RegNdx, double offset_x, double offset_y);




#endif // !WRITEORREAD_H


