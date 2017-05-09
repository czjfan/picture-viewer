#pragma once

#include <basetsd.h>
#include <fstream>

using std::ifstream;
using std::ios;

#define PICTYPE_UNKNOWN			0
#define PICTYPE_BMP				1
#define PICTYPE_PNG				2
#define PICTYPE_JPG				3
#define PICTYPE_GIF				4
#define PICTYPE_TIFF			5

typedef unsigned char byte;
typedef wchar_t WCHAR;

inline void WORD_L2BEndian(byte *bp)//WORD_L2BEndian��������
{
	byte temp = bp[0];
	bp[0] = bp[1];
	bp[1] = temp;
}
inline void DWORD_L2BEndian(byte *bp)
{
	byte temp = bp[0];
	bp[0] = bp[3];
	bp[3] = temp;
	temp = bp[1];
	bp[1] = bp[2];
	bp[2] = temp;
}

struct PicInfo {
	short pictype;		//ͼƬ�ļ����ͣ���Ҫ������
	UINT32 width;		//ͼƬ�ߴ�
	UINT32 height;
	UINT8 depthinfile;	//ͼƬ�ļ��д洢�����
	UINT8 generaldepth;	//λ��ȣ���Ҫ������
	UINT8 colortype;	//��ɫ���ͣ��Ҷ�/��ɫ/����/��ͨ���ȣ�
	UINT8 channels;		//ͨ��������Ҫ������
	UINT64 bytecount;	//�ֽ���

	PicInfo() { 
		width = 0; 
		height = 0; 
		depthinfile = 0; 
		generaldepth = 0; 
		colortype = 0;
		channels = 0;
		bytecount = 0;
		pictype = PICTYPE_UNKNOWN;
	}

	bool ReadFile(WCHAR file[]);
	bool GetSize(WCHAR file[]);
};