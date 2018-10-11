// CSpeed.cpp: 实现文件
//

#include "stdafx.h"
#include "VideoPlayer.h"
#include "CSpeed.h"
#include "afxdialogex.h"


// CSpeed 对话框

IMPLEMENT_DYNAMIC(CSpeed, CDialogEx)

CSpeed::CSpeed(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SPEED, pParent)
	, m_speed(0)
{

}

CSpeed::~CSpeed()
{
}

void CSpeed::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDCANCEL, m_speed);
}


BEGIN_MESSAGE_MAP(CSpeed, CDialogEx)
END_MESSAGE_MAP()


// CSpeed 消息处理程序
