#include "stdafx.h"
#include "picpacklist.h"

/*
* ͼƬ��
*/
//_finddata_t filegroup;
bool bPicOn;
PicPackList picList;					// ͼƬ�б�
PicPack *pLastPicpack;
PicPack *pLivePicpack;					// ��ͼƬ��
BMP *pLiveBmp;							// ��ǰͼƬ linked with "PicPack"����������
WCHAR picInfoStr[256];					// ͼƬ��Ϣ���棬���洢������Ϣ:�����٣����ʶ࣬����ʹ�û��棩

void UpdateLocalPicStr()
{
	if (pLivePicpack)
		StringCchCopy(picInfoStr, 256, pLivePicpack->GetPicInfoStr());// ͼƬ��Ϣ�ַ�������
	else
		picInfoStr[0] = L'\0';
}

void PostPicEvent()
{
	// ���±���ָ��
	pLivePicpack = picList.pLivePicPack;
	pLiveBmp = picList.pLiveBMP;

	UpdateLocalPicStr();// ���±�����Ϣ�ַ���

	bPicOn = HasPic();// ���±�־
}

void InitNonPic()
{
	pLastPicpack = NULL;
	pLivePicpack = NULL;
	pLiveBmp = NULL;

	StringCchPrintf(picInfoStr, 256, L"");

	bPicOn = HasPic();
}

bool SetPic(short picidx)
{
	pLastPicpack = pLivePicpack;
	picList.SetPicPack(picidx);

	PostPicEvent();

	return true;
}

bool SetNewPic()
{
	pLastPicpack = pLivePicpack;
	picList.SetTailPicPack();

	PostPicEvent();

	return true;
}

bool SetTailPic()
{
	pLastPicpack = pLivePicpack;
	picList.SetTailPicPack();

	PostPicEvent();

	return true;
}

bool SetPrevPic()
{
	pLastPicpack = pLivePicpack;
	picList.SetPrev();

	PostPicEvent();

	return true;
}

BMP * GetPrevPic()
{
	PicPack *pPre = picList.GetPrev();
	if (pPre != NULL)
		return pPre->GetPBMP();
	else
		return NULL;
}

bool SetNextPic()
{
	pLastPicpack = pLivePicpack;
	picList.SetNext();

	PostPicEvent();

	return true;
}

void Drop()
{
	pLastPicpack = NULL;
	picList.Drop();

	PostPicEvent();
}

inline bool HasPic()
{
	if (pLiveBmp)
		if (pLiveBmp->isNotEmpty())
			return true;
	return false;
}