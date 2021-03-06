
// VideoPlayerDlg.h: 头文件
//

#pragma once
#include "Video.h"

// CVideoPlayerDlg 对话框
class CVideoPlayerDlg : public CDialogEx
{
// 构造
public:
	CVideoPlayerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEOPLAYER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CMenu m_Menu;
	
	CVideo m_Video;
	bool m_bIsOpen;
	bool m_bSound;
	bool m_bLoop;
	bool m_bFullScreen;
	CDialog *display;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpen();
	afx_msg void OnPlay();
	afx_msg void OnPause();
	afx_msg void OnPre();
	afx_msg void OnNext();
	afx_msg void OnStop();
	afx_msg void OnSound();
	afx_msg void OnFullscreen();
	afx_msg void OnLoop();
	afx_msg void OnSpeed();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
