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
class CVideo  //处理视频类
{
public:
	CVideo();//构造函数
	virtual ~CVideo();//构析函数

	//函数功能-打开视频文件，pWnd-将要包含视频播放窗口的主窗口，
	//left、top-视频播放窗口在主窗口中的左上角坐标
	BOOL Open (CString, CWnd *pWnd, int left = 0, int top = 0);
	void Close (void);//关闭视频文件
	//函数功能-播放视频文件，bLoop-是否循环放映,bFullScreen-是否全屏播放
	BOOL Play (BOOL bLoop = FALSE, BOOL bFullScreen = FALSE);
	void Stop (void);//停止播放
	void Pause (void);//暂停播放
	int GetWidth (void);//得到视频宽度
	int GetHeight (void);//获得视频高度
	int GetFrames (void);//获得视频总帧数
	int GetFrame (void);//获得当前帧
	BOOL IsPlaying (BOOL *);//是否正在播放
	BOOL SeekTo (int);//跳到某一位置
	void Forward (int);//快进
	void Backward (int);//后退
	int GetSpeed (void);//获得播放速度
	void SetSpeed (int);//设置播放速度
	void SetSound (BOOL);//设置声音
	BOOL GetSound (void);//得到声音
	
private:
	BOOL m_bOpened;//是否有媒体文件打开
	BOOL m_bPaused;//是否暂停
	BOOL m_bPlaying;//是否正在播放
	WORD m_wDeviceID;//播放设备ID
};

#endif // !defined(AFX_VIDEO_H__B35A40D1_7DB7_11D3_860F_F6E44D6C9F3C__INCLUDED_)
