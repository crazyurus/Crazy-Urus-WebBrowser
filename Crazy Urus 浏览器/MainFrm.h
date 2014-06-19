// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
    void ChangeAddr(CString);
// ����
public:
    CMFCRibbonLabel *StatusBar;
    CMFCRibbonProgressBar *ProgressBar;
    BOOL isForward;
    BOOL isBack;
// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// �ؼ���Ƕ���Ա
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	CMFCCaptionBar    m_wndCaptionBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	DECLARE_MESSAGE_MAP()

	BOOL CreateCaptionBar();
public:
    afx_msg void OnAdd();
    afx_msg void OnAddr();
    afx_msg double GetZoom();
    afx_msg void SetProgress(long value,long max);
protected:
    afx_msg LRESULT OnNewUrl(WPARAM wParam, LPARAM lParam);
public:
    afx_msg void OnZoom();
    afx_msg void SetStatusText(LPCTSTR str);
    afx_msg void OnClose();
    afx_msg void ButtonEnable(CCmdUI *pCmdUI);
    afx_msg void SetButtonState(BOOL,BOOL);
    afx_msg void OnButtonSystem();
};


