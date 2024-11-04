// MainFrm.h : CMainFrame 类的接口
//

#pragma once
class CControlDlg;             ///< 声明对话框类
class CMainFrame : public CFrameWnd
{
	
	CSplitterWnd m_wndSplitter;          ///< 将视图拆分为左右
public: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:
	CSplitterWnd m_wndSplitterLeft;      ///< 将右边视图拆分为上下左右4个窗口
	CControlDlg* m_objControlDlg;

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	CWnd* m_pWnd[4];

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};


