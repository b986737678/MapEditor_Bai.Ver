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

#endif // !WRITEORREAD_H


