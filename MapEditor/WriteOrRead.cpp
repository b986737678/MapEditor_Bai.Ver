#include"stdafx.h"
#include"WriteOrRead.h"


void WritePntToFile(CFile * PntTmpF, int i, PNT_STRU point)
{
	PntTmpF->Seek(i * sizeof(PNT_STRU), CFile::begin);
	PntTmpF->Write(&point, sizeof(PNT_STRU));
}

/*从临时点文件读取点数据*/
void ReadTempFileToPnt(CFile * PntTmpF, int i, PNT_STRU & point)
{
	PntTmpF->Seek(i * sizeof(PNT_STRU), CFile::begin);	//重新定位指针
	PntTmpF->Read(&point, sizeof(PNT_STRU));			//读取数据
}

/*将点数据从永久文件读到临时文件*/
void ReadPntPermanentFileToTemp(CFile * pntF, CFile * pntTmpF, int & nPnt, int & nLPnt)
{
	PNT_STRU point;
	pntF->Seek(sizeof(VERSION), CFile::begin);	//将文件指针放到文件头后面
	pntF->Read(&nPnt, sizeof(int));				//读点的物理个数
	pntF->Read(&nLPnt, sizeof(int));			//读点的逻辑个数
	for (int i = 0; i < nPnt; i++)
	{
		pntF->Read(&point, sizeof(PNT_STRU));	//逐个读出点数据
		pntTmpF->Write(&point, sizeof(PNT_STRU));//逐个写入点数据
	}
}

/*将第i条线的索引写入临时索引文件*/
void WriteLinNdxToFile(CFile * linTmpNdxF, int i, LIN_NDX_STRU line)
{
	linTmpNdxF->Seek(i * sizeof(LIN_NDX_STRU), CFile::begin);
	linTmpNdxF->Write(&line, sizeof(LIN_NDX_STRU));
}

/*将线的点数据写入文件*/
void WriteLinDatToFile(CFile * linTmpDatF, long datOff, int i, D_DOT point)
{
	linTmpDatF->Seek(datOff + i * sizeof(D_DOT), CFile::begin);
	linTmpDatF->Write(&point, sizeof(D_DOT));
}

/*从临时线数据文件中读取线的点数据*/
void ReadTempFileToLinDat(CFile * LinTmpDatF, long datOff, int i, D_DOT & pnt)
{
	LinTmpDatF->Seek(datOff + i * sizeof(D_DOT), CFile::begin);
	LinTmpDatF->Read(&pnt, sizeof(D_DOT));
}

/*从临时线索引文件中读取线索引*/
void ReadTempFileToLinNdx(CFile * LinTmpNdxF, int i, LIN_NDX_STRU & LinNdx)
{
	LinTmpNdxF->Seek(i * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Read(&LinNdx, sizeof(LIN_NDX_STRU));
}

/*将线的索引和点数据写入永久文件*/
void WriteTempToLinPermanentFile(CFile * LinF, CFile * LinTmpDatF, CFile * LinTmpNdxF, VERSION LinVer, int nLin, int nLLin)
{
	LIN_NDX_STRU TempLinNdx;
	D_DOT Pnt;
	long LinNdxOffset = sizeof(VERSION) + sizeof(int) * 2;
	long LinDatOffset = LinNdxOffset + sizeof(LIN_NDX_STRU)*nLin;
	LinF->Write(&LinVer, sizeof(VERSION));		//写入版本信息
	LinF->Write(&nLin, sizeof(int));			//写入物理数
	LinF->Write(&nLLin, sizeof(int));			//写入逻辑数
	for (int i = 0; i < nLin; i++)
	{
		//从临时线索引文件中读取线索引
		ReadTempFileToLinNdx(LinTmpNdxF, i, TempLinNdx);
		LinF->Seek(LinDatOffset, CFile::begin);
		for (int j = 0; j < TempLinNdx.dotNum; j++)
		{
			//从临时线数据文件中读取线的点数据
			ReadTempFileToLinDat(LinTmpDatF, TempLinNdx.datOff, j, Pnt);
			//将线的点数据写入永久文件
			LinF->Write(&Pnt, sizeof(D_DOT));
		}
		LinF->Seek(LinNdxOffset, CFile::begin);
		TempLinNdx.datOff = LinDatOffset;
		//将线的索引写入永久文件
		LinF->Write(&TempLinNdx, sizeof(LIN_NDX_STRU));
		LinNdxOffset += sizeof(LIN_NDX_STRU);
		LinDatOffset += (sizeof(D_DOT)*TempLinNdx.dotNum);
	}
}

/*从永久文件读取线数据到临时文件*/
void ReadLinPermanentFileToTemp(CFile*LinF, CFile*LinTmpDatF,
	CFile*LinTmpNdxF, VERSION&LinVer, int&nLin, int&nLLin, long&TmpFLinDatOffset)
{
	LinF->Seek(0, CFile::begin);
	LinF->Read(&LinVer, sizeof(VERSION));
	LinF->Read(&nLin, sizeof(int));		//读物理个数
	LinF->Read(&nLLin, sizeof(int));	//读逻辑个数
	long LinNdxOffset = sizeof(VERSION) + sizeof(int) * 2;
	long LinDatOffset = LinNdxOffset + sizeof(LIN_NDX_STRU)*nLin;
	TmpFLinDatOffset = 0;
	LIN_NDX_STRU TempLinNdx;
	D_DOT Pnt;
	for (int i = 0; i < nLin; i++)
	{
		LinF->Seek(LinNdxOffset, CFile::begin);
		LinF->Read(&TempLinNdx, sizeof(LIN_NDX_STRU));
		LinF->Seek(TempLinNdx.datOff, CFile::begin);
		for (int j = 0; j < TempLinNdx.dotNum; j++)
		{
			LinF->Read(&Pnt, sizeof(D_DOT));
			LinTmpDatF->Write(&Pnt, sizeof(D_DOT));
		}
		TempLinNdx.datOff = TmpFLinDatOffset;
		LinTmpNdxF->Write(&TempLinNdx, sizeof(LIN_NDX_STRU));
		TmpFLinDatOffset += (sizeof(D_DOT)*TempLinNdx.dotNum);
		LinDatOffset += (sizeof(D_DOT)*TempLinNdx.dotNum);
		LinNdxOffset += sizeof(LIN_NDX_STRU);
	}
}

/*在线的临时索引文件中更新线数据*/
void UpdateLin(CFile * LinTmpNdxF, int nLin, LIN_NDX_STRU line)
{
	WriteLinNdxToFile(LinTmpNdxF, nLin, line);
}

/*更新线的点数据到临时文件*/
void UpdateLin(CFile * LinTmpNdxF, CFile * LinTmpDatF, int LinNdx, double offset_x, double offset_y)
{
	LIN_NDX_STRU tLin;
	D_DOT pt;
	ReadTempFileToLinNdx(LinTmpNdxF, LinNdx, tLin);
	for (int i = 0; i < tLin.dotNum; ++i)
	{
		LinTmpDatF->Seek(tLin.datOff + i * sizeof(D_DOT), CFile::begin);
		LinTmpDatF->Read(&pt, sizeof(D_DOT));
		pt.x = pt.x + offset_x;
		pt.y = pt.y + offset_y;
		LinTmpDatF->Seek(tLin.datOff + i * sizeof(D_DOT), CFile::begin);
		LinTmpDatF->Write(&pt, sizeof(D_DOT));
	}
}

/*修改第一条线索引*/
void AlterStartLin(CFile * LinTmpNdxF, long subdatOff, int nLine, int subNum)
{
	LIN_NDX_STRU LinNdx;
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Read(&LinNdx, sizeof(LIN_NDX_STRU));
	LinNdx.datOff = subdatOff;		// 更新线的点索引
	LinNdx.dotNum = subNum;			// 更新线的点数目
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Write(&LinNdx, sizeof(LIN_NDX_STRU));
}

/*修改第二条线索引*/
void AlterEndLin(CFile * LinTmpNdxF, int nLine)
{
	LIN_NDX_STRU LinNdx;
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Read(&LinNdx, sizeof(LIN_NDX_STRU));
	LinNdx.dotNum = 0;				// 线的点数为0
	LinNdx.isDel = 0;				// 设置删除标记
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Write(&LinNdx, sizeof(LIN_NDX_STRU));
}



