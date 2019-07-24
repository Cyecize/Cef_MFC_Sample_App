
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "cefmfcdemo.h"

#include "MainFrm.h"
#include "CefDoc.h"
#include "CefView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_BUTTON_BACK, OnGoBack)
	ON_COMMAND(ID_BUTTON_GO, OnGo)
	ON_COMMAND(ID_BUTTON_FORWARD, OnGoForward)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_BACK, OnUpdateGoBack)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_GO, OnUpdateGo)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_FORWARD, OnUpdateGoForward)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	//set up the edit control as a snap mode select box
	//First get the index of the place holder's position in the toolbar
	auto index = 0;
	while (m_wndToolBar.GetItemID(index) != ID_ADDRESS_CONTROL) index++;

	//next convert that button to a separator and get its position
	m_wndToolBar.SetButtonInfo(index, ID_ADDRESS_CONTROL, TBBS_SEPARATOR, 400);
	auto rect = RECT{ 0 };
	m_wndToolBar.GetItemRect(index, &rect);

	// then .Create the combo box and show it
	if (!m_wndToolBar.m_wndAddress.Create(
		WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
		rect, &m_wndToolBar, IDC_EDIT_ADDRESS))
	{
		TRACE0("Failed to create address control\n");
		return FALSE;
	}

	m_wndToolBar.m_wndAddress.SetFont(m_wndStatusBar.GetFont());
	m_wndToolBar.m_wndAddress.ShowWindow(SW_SHOW);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnGo()
{
	auto view = GetView();
	if (view != nullptr)
	{
		auto url = CString{};
		m_wndToolBar.m_wndAddress.GetWindowText(url);

		view->Navigate(url);
	}
}

void CMainFrame::OnGoBack()
{
	auto view = GetView();
	if (view != nullptr)
	{
		view->NavigateBack();
	}
}

void CMainFrame::OnGoForward()
{
	auto view = GetView();
	if (view != nullptr)
	{
		view->NavigateForward();
	}
}

void CMainFrame::OnUpdateGo(CCmdUI* cmd)
{
	auto url = CString{};
	m_wndToolBar.m_wndAddress.GetWindowText(url);

	cmd->Enable(!url.IsEmpty() ? TRUE : FALSE);
}

void CMainFrame::OnUpdateGoBack(CCmdUI* cmd)
{
	auto view = GetView();
	cmd->Enable(view != nullptr && view->CanNavigateBack() ? TRUE : FALSE);
}

void CMainFrame::OnUpdateGoForward(CCmdUI* cmd)
{
	auto view = GetView();
	cmd->Enable(view != nullptr && view->CanNavigateForward() ? TRUE : FALSE);
}

void CMainFrame::SetUrl(CString const & url)
{
	m_wndToolBar.m_wndAddress.SetWindowText(url);
}

void CMainFrame::ZoomIn()
{
	if (currentZoom < MAX_ZOOM) {
		currentZoom += ZOOM_INCREMENT;
		this->zoomCallback(this->currentZoom, this->browserInstance);
	}
}

void CMainFrame::ZoomOut()
{
	if (currentZoom > MIN_ZOOM) {
		currentZoom -= ZOOM_INCREMENT;
		this->zoomCallback(this->currentZoom, this->browserInstance);
	}
}

void CMainFrame::SetOnZoomChanged(void(*zoomCallback)(double zoom, CefRefPtr<CefBrowser> browser), CefRefPtr<CefBrowser> browserInstance)
{
	this->browserInstance = browserInstance;
	this->zoomCallback = zoomCallback;
}

CefDemoView* CMainFrame::GetView()
{
	auto frame = GetActiveFrame();
	auto view = frame->GetActiveView();

	if (!view->IsKindOf(RUNTIME_CLASS(CefDemoView)))
	{
		return nullptr;
	}

	return static_cast<CefDemoView*>(view);
}

void CMainFrame::OnClose()
{
	auto view = GetView();
	if (view != nullptr)
		view->CloseBrowser();

	CFrameWnd::OnClose();
}