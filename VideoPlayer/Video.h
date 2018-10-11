// Video.h: interface for the CVideo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIDEO_H__B35A40D1_7DB7_11D3_860F_F6E44D6C9F3C__INCLUDED_)
#define AFX_VIDEO_H__B35A40D1_7DB7_11D3_860F_F6E44D6C9F3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxwin.h>
#include <mmsystem.h>
#include <mciavi.h>
#include <digitalv.h>
//#include <videoplayer.h>
class CVideo  //������Ƶ��
{
public:
	CVideo();//���캯��
	virtual ~CVideo();//��������

	//��������-����Ƶ�ļ���pWnd-��Ҫ������Ƶ���Ŵ��ڵ������ڣ�
	//left��top-��Ƶ���Ŵ������������е����Ͻ�����
	BOOL Open (CString, CWnd *pWnd, int left = 0, int top = 0);
	void Close (void);//�ر���Ƶ�ļ�
	//��������-������Ƶ�ļ���bLoop-�Ƿ�ѭ����ӳ,bFullScreen-�Ƿ�ȫ������
	BOOL Play (BOOL bLoop = FALSE, BOOL bFullScreen = FALSE);
	void Stop (void);//ֹͣ����
	void Pause (void);//��ͣ����
	int GetWidth (void);//�õ���Ƶ���
	int GetHeight (void);//�����Ƶ�߶�
	int GetFrames (void);//�����Ƶ��֡��
	int GetFrame (void);//��õ�ǰ֡
	BOOL IsPlaying (BOOL *);//�Ƿ����ڲ���
	BOOL SeekTo (int);//����ĳһλ��
	void Forward (int);//���
	void Backward (int);//����
	int GetSpeed (void);//��ò����ٶ�
	void SetSpeed (int);//���ò����ٶ�
	void SetSound (BOOL);//��������
	BOOL GetSound (void);//�õ�����
	
private:
	BOOL m_bOpened;//�Ƿ���ý���ļ���
	BOOL m_bPaused;//�Ƿ���ͣ
	BOOL m_bPlaying;//�Ƿ����ڲ���
	WORD m_wDeviceID;//�����豸ID
};

#endif // !defined(AFX_VIDEO_H__B35A40D1_7DB7_11D3_860F_F6E44D6C9F3C__INCLUDED_)
