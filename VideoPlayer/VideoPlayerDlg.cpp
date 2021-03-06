
// VideoPlayerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "VideoPlayer.h"
#include "VideoPlayerDlg.h"
#include "afxdialogex.h"
#include "CSpeed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVideoPlayerDlg 对话框



CVideoPlayerDlg::CVideoPlayerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIDEOPLAYER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVideoPlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, &CVideoPlayerDlg::OnOpen)
	ON_BN_CLICKED(IDC_PLAY, &CVideoPlayerDlg::OnPlay)
	ON_BN_CLICKED(IDC_PAUSE, &CVideoPlayerDlg::OnPause)
	ON_BN_CLICKED(IDC_PRE, &CVideoPlayerDlg::OnPre)
	ON_BN_CLICKED(IDC_NEXT, &CVideoPlayerDlg::OnNext)
	ON_BN_CLICKED(IDC_STOP, &CVideoPlayerDlg::OnStop)
	ON_COMMAND(IDC_SOUND, &CVideoPlayerDlg::OnSound)
	ON_COMMAND(IDC_FULLSCREEN, &CVideoPlayerDlg::OnFullscreen)
	ON_COMMAND(IDC_LOOP, &CVideoPlayerDlg::OnLoop)
	ON_COMMAND(IDC_SPEED, &CVideoPlayerDlg::OnSpeed)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CVideoPlayerDlg 消息处理程序

BOOL CVideoPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_Menu.LoadMenu(IDR_MAINFRAME);
	SetMenu(&m_Menu);

	display = new CDialog;
	display->Create(IDD_DISPLAY);
	SetTimer(1, 100, NULL);



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVideoPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVideoPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVideoPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVideoPlayerDlg::OnOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Filter, str;
	Filter = " 视频文件(*.AVI)|*.AVI||";

	// 得到窗口尺寸
	CRect rect;
	this->GetWindowRect(rect);

	CFileDialog FileDlg(true, NULL, NULL, OFN_HIDEREADONLY, Filter);
	if (FileDlg.DoModal() == IDOK)//单击文件选择对话框的“确定”键
	{
		if (!m_Video.Open(FileDlg.GetPathName(), display, 0, 0))//打开视频文件
		{
			MessageBox("不能打开文件!", "Error", MB_OK | MB_ICONSTOP);
			return;
		}
		//将显示屏幕定位在主对话框的下方，并根据视频大小确定屏幕大小
		int Border, Caption;
		Border = GetSystemMetrics(SM_CXSIZEFRAME);
		Caption = GetSystemMetrics(SM_CYCAPTION);

		display->SetWindowPos(this, rect.left + 30, rect.bottom + 30,
			m_Video.GetWidth() + Border, m_Video.GetHeight() + Caption + Border,
			SWP_SHOWWINDOW);

		display->ShowWindow(true);
		display->SetWindowText(FileDlg.GetFileName());
		SetDlgItemText(IDC_VIDEO_NAME, FileDlg.GetFileName());
		str.Format("%d×%d", m_Video.GetWidth(), m_Video.GetHeight());
		SetDlgItemText(IDC_DEMENSION, str);
		m_bIsOpen = true;
	}

}


void CVideoPlayerDlg::OnPlay()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Video.Play(m_bLoop, m_bFullScreen);
}


void CVideoPlayerDlg::OnPause()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Video.Pause();
}


void CVideoPlayerDlg::OnPre()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Video.Backward(1);
}


void CVideoPlayerDlg::OnNext()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Video.Forward(1);
}


void CVideoPlayerDlg::OnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Video.Stop();
}


void CVideoPlayerDlg::OnSound()
{
	// TODO: 在此添加命令处理程序代码
	m_bSound = !m_bSound;
	m_Video.SetSound(m_bSound);
}


void CVideoPlayerDlg::OnFullscreen()
{
	// TODO: 在此添加命令处理程序代码
	m_bFullScreen = !m_bFullScreen;
}


void CVideoPlayerDlg::OnLoop()
{
	// TODO: 在此添加命令处理程序代码
	m_bLoop = !m_bLoop;
}


void CVideoPlayerDlg::OnSpeed()
{
	// TODO: 在此添加命令处理程序代码
	CSpeed dlg;
	dlg.m_speed = m_Video.GetSpeed();
	if (dlg.DoModal() == IDOK)
		m_Video.SetSpeed(dlg.m_speed);

}


void CVideoPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWnd *m_pWnd = GetDlgItem(IDC_PLAY);
	m_pWnd->EnableWindow(m_bIsOpen);
	m_pWnd = GetDlgItem(IDC_PAUSE);
	m_pWnd->EnableWindow(m_bIsOpen);
	m_pWnd = GetDlgItem(IDC_STOP);
	m_pWnd->EnableWindow(m_bIsOpen);
	m_pWnd = GetDlgItem(IDC_NEXT);
	m_pWnd->EnableWindow(m_bIsOpen);
	m_pWnd = GetDlgItem(IDC_PRE);
	m_pWnd->EnableWindow(m_bIsOpen);

	if (m_bIsOpen)
	{
		CString Info;
		Info.Format("当前播放帧数:%d", m_Video.GetFrame());
		m_pWnd = GetDlgItem(IDC_DISPLAY);//获得静态图像控件位置
		CClientDC dc(m_pWnd);
		CRect rect;
		CBrush brush(COLORREF(0)), *OldBrush;
		OldBrush = (CBrush*)dc.SelectObject(brush);
		m_pWnd->GetClientRect(rect);//获得静态图像控件大小
		//dc.SetTextColor(COLORREF RGB(0, 0, 255));
		//dc.SetBkColor(COLORREF RGB(0, 0, 0));
		//dc.FillRect(rect, &brush);
		CSize size = dc.GetTextExtent(Info);//获得文本尺寸
		dc.TextOut((rect.Width() - size.cx) / 2, (rect.Height() - size.cy) / 2, Info);
		dc.SelectObject(&OldBrush);

		CString str;
		str.Format("%d", m_Video.GetSpeed());
		SetDlgItemText(IDC_PLAY_SPEED, str);
		str.Format("%d", m_Video.GetFrames());
		SetDlgItemText(IDC_FRAMES, str);
		if (m_Video.GetSound())
			str = _T("否");
		else
			str = _T("是");
		SetDlgItemText(IDC_PLAY_SOUND, str);
	}
	CDialogEx::OnTimer(nIDEvent);
}
