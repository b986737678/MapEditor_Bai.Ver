#pragma once
#ifndef MYDATATYPE_H
#define MYDATATYPE_H

typedef struct
{
	double   x;
	double   y;
	COLORREF color;
	int		 pattern;
	char	 isDel;
}PNT_STRU;

typedef struct
{
	char	flag[3];	//��־�� PNT  LIN  REG
	int		version;	//10��������Ϊ1.0�汾
}VERSION;

typedef struct
{
	char		isDel;		//�Ƿ�ɾ��
	COLORREF	color;		//����ɫ
	int			pattern;	//����(��)
	long		dotNum;		//�߽ڵ���
	long		datOff;		//�߽ڵ��������ݴ洢λ��
}LIN_NDX_STRU;

typedef struct
{
	double		x;			//�ڵ�x����	
	double		y;			//�ڵ�y����
}D_DOT;
#endif