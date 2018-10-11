#pragma once


// CSpeed 对话框

class CSpeed : public CDialogEx
{
	DECLARE_DYNAMIC(CSpeed)

public:
	CSpeed(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSpeed();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPEED };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_speed;
};
