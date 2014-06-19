// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "WebBrowser.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "CloseDlg.h"

#define WM_NEWURL WM_USER+104
#define ID_STATUSBAR_PROGRESS 9999

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	// ȫ�ְ�������
	ON_COMMAND(ID_HELP_FINDER, &CMDIFrameWndEx::OnHelpFinder)
	ON_COMMAND(ID_HELP, &CMDIFrameWndEx::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, &CMDIFrameWndEx::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, &CMDIFrameWndEx::OnHelpFinder)
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
    ON_UPDATE_COMMAND_UI(ID_EDIT_BACK,&CMainFrame::ButtonEnable)
    ON_UPDATE_COMMAND_UI(ID_EDIT_FORWARD,&CMainFrame::ButtonEnable)
    ON_UPDATE_COMMAND_UI(ID_STATUSBAR_PANE1,&CMainFrame::ButtonEnable)
	ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
    ON_COMMAND(ID_ADDR, &CMainFrame::OnAddr)
    ON_MESSAGE(WM_NEWURL, &CMainFrame::OnNewUrl)
    ON_COMMAND(ID_BUTTON18, &CMainFrame::OnZoom)
    ON_WM_CLOSE()
    ON_COMMAND(ID_BUTTON_SYSTEM, &CMainFrame::OnButtonSystem)
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
    isBack=isForward=FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // ����������ʽ...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // ����Ϊ FALSE �Ὣ�رհ�ť������ѡ�������Ҳ�
	mdiTabParams.m_bTabIcons = TRUE;    // ����Ϊ TRUE ���� MDI ѡ��������ĵ�ͼ��
	mdiTabParams.m_bAutoColor = TRUE;    // ����Ϊ FALSE ������ MDI ѡ����Զ���ɫ
	mdiTabParams.m_bDocumentMenu = TRUE; // ��ѡ�������ұ�Ե�����ĵ��˵�
    mdiTabParams.m_bEnableTabSwap = TRUE;
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
    ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
    StatusBar=new CMFCRibbonLabel(strTitlePane1,FALSE);
    ProgressBar=new CMFCRibbonProgressBar(ID_STATUSBAR_PROGRESS,160);
	m_wndStatusBar.AddDynamicElement(StatusBar);
    m_wndStatusBar.AddExtendedElement(ProgressBar,L"����");
    m_wndStatusBar.AddSeparator();
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);
	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ����������:
	if (!CreateCaptionBar())
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	// ������ǿ�Ĵ��ڹ���Ի���
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// ���ĵ�����Ӧ�ó��������ڴ��ڱ������ϵ�˳����н�������
	// ���Ľ��������Ŀ����ԣ���Ϊ��ʾ���ĵ�����������ͼ��
	ModifyStyle(0, FWS_PREFIXTITLE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

BOOL CMainFrame::CreateCaptionBar()
{
	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
	{
		TRACE0("δ�ܴ���������\n");
		return FALSE;
	}

	BOOL bNameValid;

	CString strTemp, strTemp2;
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButtonToolTip(strTemp);

	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
	ASSERT(bNameValid);
	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);

	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;
	theApp.m_nAppLook = id;
	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;
	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;
	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;
	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;
	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;
	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;
	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;
	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;
		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;
		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;
		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);
	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}

void CMainFrame::OnOptions()
{
	CMFCRibbonCustomizeDialog *pOptionsDlg = new CMFCRibbonCustomizeDialog(this, &m_wndRibbonBar);
	ASSERT(pOptionsDlg != NULL);
	pOptionsDlg->DoModal();
	delete pOptionsDlg;
}

void CMainFrame::OnAddr()
{
    CMFCRibbonEdit *pEdit=DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_ADDR));  //��ȡ��ַ�����
    CString strUrl=pEdit->GetEditText();  //��ȡ��ַ���ı�
    if(strUrl=="")  //���ݺϷ����ж�
        MessageBox(L"������Ҫ���ʵ���ַ��",L"��ʾ",MB_ICONWARNING);
    else
    {
        CChildFrame *pChildFrame=(CChildFrame*)GetActiveFrame();
        CHtmlView *pBrowser=(CHtmlView*)pChildFrame->GetActiveView();
        if(pBrowser->GetLocationURL()!=strUrl) pBrowser->Navigate(strUrl);
    }
}

void CMainFrame::ChangeAddr(CString str)
{  //�޸ĵ�ַ������ı�
    CMFCRibbonEdit *pEdit=DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_ADDR));
    pEdit->SetEditText(str);
}

double CMainFrame::GetZoom()
{   //��ȡ�����ֵ
    CMFCRibbonSlider *pSlider=DYNAMIC_DOWNCAST(CMFCRibbonSlider, m_wndRibbonBar.FindByID(ID_SLIDER2));
    return (pSlider->GetPos())/10.0;
}

void CMainFrame::SetStatusText(LPCTSTR str)
{  //����״̬���ı�����Bug��
    CString tmp=str;
    static BOOL isFinish=TRUE;
    if(StatusBar&&!tmp.IsEmpty()&&isFinish) StatusBar->SetText(str);
    isFinish=tmp.Find(L"���",0);
}

void CMainFrame::SetProgress(long value,long max)
{  //���ý�����
    if(ProgressBar)
    {
        ProgressBar->SetRange(0,max);
        ProgressBar->SetPos(value);
    }
}

afx_msg LRESULT CMainFrame::OnNewUrl(WPARAM wParam, LPARAM lParam)
{  //CHtmlView������´���
    LPDISPATCH* ppDispatch=(LPDISPATCH*)wParam;
    SendMessage(WM_COMMAND, ID_FILE_NEW, 0);
    CChildFrame* pChildFrame = (CChildFrame*)GetActiveFrame();
    *ppDispatch=((CHtmlView*)pChildFrame->GetActiveView())->GetApplication();
    return 0;
}

void CMainFrame::OnZoom()
{
}

void CMainFrame::OnClose()
{  //����WM_CLOSE
    CCloseDlg closeDlg;
    UINT r=closeDlg.DoModal();  //�򿪹رնԻ���
    if(r==IDALL) CMDIFrameWndEx::OnClose();
    else if(r==IDCUR) GetActiveFrame()->DestroyWindow();
}

void CMainFrame::SetButtonState(BOOL sForward,BOOL sBack)
{  //���պ��ˡ�ǰ����ť״̬
    isBack=sBack;
    isForward=sForward;
}

void CMainFrame::ButtonEnable(CCmdUI *pCmdUI)
{  //���ð�ť����
    if(pCmdUI->m_nID==ID_EDIT_FORWARD) pCmdUI->Enable(isForward);
    else if(pCmdUI->m_nID==ID_EDIT_BACK) pCmdUI->Enable(isBack);
    else pCmdUI->Enable(TRUE);
}

void CMainFrame::OnButtonSystem()
{  //��ϵͳ��Ϣ
    ShellAbout(this->m_hWnd,L"Crazy Urus �����",L"Crazy Urus �����\n\r���ߣ����ǣ�Crazy Urus��",AfxGetApp()->LoadIcon(IDR_MAINFRAME));
}