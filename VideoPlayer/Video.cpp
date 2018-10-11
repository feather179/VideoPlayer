// Video.cpp: implementation of the CVideo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Video.h"
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVideo::CVideo()
{
	// ��ʼ����Ա����
	m_bOpened = m_bPaused = m_bPlaying = FALSE;
	m_wDeviceID = 0;
}

CVideo::~CVideo()
{
	Close ();
}

BOOL CVideo::Open (CString Filename, CWnd *pWnd, int left, int top)
{
	if (!m_bOpened){
		// ��ʼ����Ա����
		m_bOpened = m_bPaused = m_bPlaying = FALSE;
		m_wDeviceID = 0;

		// �ļ��Ƿ����
		CFileStatus Status;
		if (!CFile::GetStatus (Filename, Status))
			return FALSE;

		// ���豸
		MCI_DGV_OPEN_PARMS OpenParms;
		OpenParms.lpstrDeviceType = "avivideo";//���ò����豸Ϊ��Ƶ�����豸
		OpenParms.lpstrElementName = (LPSTR) Filename.GetBuffer (0);
		OpenParms.wDeviceID = 0;
		OpenParms.hWndParent = pWnd->m_hWnd;//��Ҫ������Ƶ�ļ���������
		OpenParms.dwStyle = WS_CHILD | WS_VISIBLE;
		if (mciSendCommand (NULL, MCI_OPEN, MCI_WAIT | MCI_OPEN_TYPE | MCI_OPEN_ELEMENT | MCI_DGV_OPEN_PARENT | MCI_DGV_OPEN_WS, (DWORD)(LPVOID) &OpenParms))
			return FALSE;
		m_wDeviceID = OpenParms.wDeviceID;
		m_bOpened = TRUE;

		// ���ļ���������Ϊ֡
		MCI_DGV_SET_PARMS SetParms;
		SetParms.dwTimeFormat = MCI_FORMAT_FRAMES;
		if (mciSendCommand (m_wDeviceID, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID) &SetParms)){
			Close ();
			return FALSE;
			}

		// �ҵ���Ƶ�ļ��Ŀ�ʼλ��
		mciSendCommand (m_wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, NULL);

		//�����Ż�������������֮��
		MCI_DGV_PUT_PARMS PutParms;
		PutParms.rc.left = left;
		PutParms.rc.top = top;
		PutParms.rc.right = GetWidth ();
		PutParms.rc.bottom = GetHeight ();
		if (mciSendCommand (m_wDeviceID, MCI_PUT, MCI_WAIT | MCI_DGV_PUT_CLIENT | MCI_DGV_PUT_WINDOW | MCI_DGV_RECT, (DWORD)(LPVOID) &PutParms)){
			Close ();
			return FALSE;
		}
	}

	return TRUE;
}

void CVideo::Close (void)
{
	if (m_bOpened){
		// ֹͣ���Ų��ر��豸
		if (m_bPlaying || m_bPaused)
			mciSendCommand (m_wDeviceID, MCI_STOP, NULL, NULL);
		mciSendCommand (m_wDeviceID, MCI_CLOSE, NULL, NULL);

		// ��ճ�Ա����
		m_bOpened = m_bPaused = m_bPlaying = FALSE;
		m_wDeviceID = 0;
		}
}

//�����Ƶ�ļ����ڵĿ��
int CVideo::GetWidth (void)
{
	if (m_bOpened){
		MCI_DGV_WHERE_PARMS WhereParms;
		if (mciSendCommand (m_wDeviceID, MCI_WHERE, MCI_DGV_WHERE_SOURCE, (DWORD)(LPVOID) &WhereParms)){
			return 0;
			}
		return (int) WhereParms.rc.right;
		}

	return 0;
}

//�����Ƶ�ļ����ڵĸ߶�
int CVideo::GetHeight (void)
{
	if (m_bOpened){
		MCI_DGV_WHERE_PARMS WhereParms;
		if (mciSendCommand (m_wDeviceID, MCI_WHERE, MCI_DGV_WHERE_SOURCE, (DWORD)(LPVOID) &WhereParms)){
			return 0;
			}
		return (int) WhereParms.rc.bottom;
		}

	return 0;
}

BOOL CVideo::Play (BOOL bLoop, BOOL bFullScreen)
{
	if (m_bOpened){
		// �������Ƶ��β�������¿�ʼ����
		if (GetFrame () >= GetFrames ())
			mciSendCommand (m_wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, NULL);
		// ����
		MCI_DGV_PLAY_PARMS PlayParms;                                     
		PlayParms.dwCallback = NULL;                    
		PlayParms.dwFrom = GetFrame ();
		DWORD dwFlags = MCI_FROM;
		if (bLoop)
			dwFlags |= MCI_DGV_PLAY_REPEAT;
		if (bFullScreen)
			dwFlags |= MCI_MCIAVI_PLAY_FULLSCREEN;
		if (mciSendCommand (m_wDeviceID, MCI_PLAY, dwFlags, (DWORD)(LPVOID) &PlayParms))
			return FALSE;
		m_bPaused = FALSE;
		m_bPlaying = TRUE;
		return TRUE;
		}

	return FALSE;
}

void CVideo::Stop (void)
{
	if (m_bOpened && m_bPlaying){
		mciSendCommand (m_wDeviceID, MCI_STOP, NULL, NULL);
		mciSendCommand (m_wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, NULL);
		m_bPlaying = FALSE;
		m_bPaused = FALSE;
		}
}

void CVideo::Pause( void )
{
	if (m_bOpened && m_bPlaying){
		MCI_DGV_PAUSE_PARMS PauseParms;
		if (mciSendCommand (m_wDeviceID, MCI_PAUSE, 0, (DWORD)(LPVOID) &PauseParms))
			return;
		m_bPaused = TRUE;
		}
}

int CVideo::GetFrames (void)
{
	if (m_bOpened){
		MCI_DGV_STATUS_PARMS StatusParms;
		StatusParms.dwItem = MCI_STATUS_LENGTH;
		if (mciSendCommand (m_wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)(LPVOID) &StatusParms))
			return 0;
		return (int) StatusParms.dwReturn;
		}

	return 0;
}

int CVideo::GetFrame (void)
{
	if (m_bOpened){
		MCI_DGV_STATUS_PARMS StatusParms;
		StatusParms.dwItem = MCI_STATUS_POSITION;
		if (mciSendCommand (m_wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)(LPVOID) &StatusParms))
			return -1;
		return (int) StatusParms.dwReturn;
		}

	return -1;
}

BOOL CVideo::IsPlaying (BOOL *pbPaused)
{
	*pbPaused = m_bPaused;

	if (m_bOpened)
	{
		MCI_DGV_STATUS_PARMS StatusParms;
		StatusParms.dwItem = MCI_STATUS_MODE;
		if (mciSendCommand (m_wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)(LPVOID) &StatusParms))
			return FALSE;
		if (StatusParms.dwReturn == MCI_MODE_PLAY)
		{
			m_bPlaying = TRUE;
			m_bPaused = FALSE;
			return TRUE;
		}
		else if (StatusParms.dwReturn == MCI_MODE_PAUSE)
		{
			m_bPlaying = TRUE;
			m_bPaused = FALSE;
			return TRUE;
		}
		else
		{
			m_bPlaying = FALSE;
			m_bPaused = FALSE;
			return FALSE;
		}
	}

	return FALSE;
}

BOOL CVideo::SeekTo (int nFrame)
{
	if (m_bOpened){
		MCI_SEEK_PARMS SeekParms;
		SeekParms.dwTo = nFrame;
		if (m_bPlaying && !m_bPaused)
			mciSendCommand (m_wDeviceID, MCI_PAUSE, 0, NULL);
		if (mciSendCommand (m_wDeviceID, MCI_SEEK, MCI_TO | MCI_WAIT, (DWORD)(LPVOID) &SeekParms))
			return FALSE;
		if (m_bPlaying && !m_bPaused)
			Play ();
		return TRUE;
		}

	return FALSE;
}

void CVideo::Forward (int nFrames)
{
	if (m_bOpened){
		if (m_bPlaying && !m_bPaused)
			Pause ();
		MCI_DGV_STEP_PARMS StepParms;
		StepParms.dwFrames = nFrames;
		mciSendCommand (m_wDeviceID, MCI_STEP, MCI_DGV_STEP_FRAMES, (DWORD)(LPVOID) &StepParms);
		}
}

void CVideo::Backward (int nFrames)
{
	if (m_bOpened){
		if (m_bPlaying && !m_bPaused)
			Pause ();
		MCI_DGV_STEP_PARMS StepParms;
		StepParms.dwFrames = nFrames;
		mciSendCommand (m_wDeviceID, MCI_STEP, MCI_DGV_STEP_REVERSE | MCI_DGV_STEP_FRAMES, (DWORD)(LPVOID) &StepParms);
		}
}

int CVideo::GetSpeed (void)
{
	if (m_bOpened){
		MCI_DGV_STATUS_PARMS StatusParms;
		StatusParms.dwItem = MCI_DGV_STATUS_SPEED;
		if (mciSendCommand (m_wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)(LPVOID) &StatusParms))
			return -1;
		return (int) StatusParms.dwReturn;
		}

	return -1;
}

void CVideo::SetSpeed (int nSpeed)
{
	if (m_bOpened){
		MCI_DGV_SET_PARMS SetParms;
		SetParms.dwSpeed = nSpeed;
		mciSendCommand (m_wDeviceID, MCI_SET, MCI_WAIT | MCI_DGV_SET_SPEED, (DWORD)(LPVOID) &SetParms);
		}
}

BOOL CVideo::GetSound (void)
{
	if (m_bOpened)
	{
		MCI_DGV_STATUS_PARMS StatusParms;
		StatusParms.dwItem = MCI_DGV_STATUS_AUDIO;
		if (mciSendCommand (m_wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)(LPVOID) &StatusParms))
			return FALSE;
		if (StatusParms.dwReturn == MCI_ON)
			return TRUE;
		else
			return FALSE;
	}

	return FALSE;
}

void CVideo::SetSound (BOOL bSound)
{
	if (m_bOpened)
	{
		MCI_DGV_SET_PARMS SetParms;
		SetParms.dwAudio = MCI_SET_AUDIO_ALL;
		DWORD dwFlags = MCI_SET_AUDIO;
		if (bSound)
			dwFlags |= MCI_SET_ON;
		else
			dwFlags |= MCI_SET_OFF;
		mciSendCommand (m_wDeviceID, MCI_SET, dwFlags, (DWORD)(LPVOID) &SetParms);
	}
}
