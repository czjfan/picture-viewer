#pragma once

#include "BMP.h"

//_finddata_t filegroup;
extern bool bPicOn;
extern PicPackList picList;				// ͼƬ�б�
extern PicPack *pLastPicpack;
extern PicPack *pLivePicpack;			// ��ͼƬ��
extern BMP *pLiveBmp;					// ��ǰͼƬ linked with "PicPack"����������
extern WCHAR picInfoStr[];				// ͼƬ��Ϣ���棬���洢������Ϣ:�����٣����ʶ࣬����ʹ�û��棩

void UpdateLocalPicStr();				// ���±�����Ϣ�ַ���
void PostPicEvent();					// �л�ͼƬ�Ȳ�������Ҫ���еĲ���
inline bool HasPic();					// ���㵱ǰ�Ƿ�ͼƬ�ǿ�

void InitNonPic();						// ��ʼ����ͼƬ״̬�Ĳ���
bool SetPic(short picidx = 1);			// ����ָ���������ͼƬ
bool SetNewPic();						// ��λ�����һ��ͼ��������
bool SetTailPic();						// ��λ�����һ��ͼƬ
bool SetPrevPic();						// ��λ����һͼƬ
BMP *GetPrevPic();
bool SetNextPic();						// ��λ����һͼƬ
void Drop();							// �����ǰͼƬ