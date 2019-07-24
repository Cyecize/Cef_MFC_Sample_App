// CefDemoView.cpp : implementation of the CefDemoView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "cefmfcdemo.h"
#endif

#include "CefDoc.h"
#include "CefView.h"
#include "CefWindowsHelpers.h"
#include "include\internal\cef_string_wrappers.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CefDemoView

IMPLEMENT_DYNCREATE(CefDemoView, CView)

BEGIN_MESSAGE_MAP(CefDemoView, CView)
		ON_WM_SIZE()
END_MESSAGE_MAP()

// CefDemoView construction/destruction

CefDemoView::CefDemoView()
{
	// TODO: add construction code here
}

CefDemoView::~CefDemoView()
{
	if (this->clientHandler != nullptr)
		this->clientHandler->DetachDelegate();
}

BOOL CefDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CView::PreCreateWindow(cs);
}

void CefDemoView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	this->mainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	return CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

// CefDemoView drawing
void CefDemoView::OnDraw(CDC* /*pDC*/)
{
	CefDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

// CefDemoView diagnostics
#ifdef _DEBUG
void CefDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CefDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CefDoc* CefDemoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CefDoc)));
	return (CefDoc*)m_pDocument;
}
#endif //_DEBUG

void CefDemoView::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
	this->browserInstance = browser;

	this->mainFrame->SetOnZoomChanged([](double zoomLevel, CefRefPtr<CefBrowser> browserInstance)
	{
		browserInstance->GetHost()->SetZoomLevel(zoomLevel);
	}, browser);
}

void CefDemoView::OnBrowserClosing(CefRefPtr<CefBrowser> browser)
{
}

void CefDemoView::OnBrowserClosed(CefRefPtr<CefBrowser> browser)
{
	if (this->browserInstance != nullptr && this->browserInstance->GetIdentifier() == browser->GetIdentifier())
	{
		this->browserInstance = nullptr;
		this->clientHandler->DetachDelegate();
	}
}

void CefDemoView::OnSetAddress(std::string const& url)
{
	if (this->mainFrame != nullptr)
	{
		auto newurl = CString{url.c_str()};
		if (newurl.Find(this->startingUrl) >= 0)
			newurl = "";

		this->mainFrame->SetUrl(newurl);
	}
}

void CefDemoView::OnSetTitle(std::string const& title)
{
	::SetWindowText(m_hWnd, CefString(title).ToWString().c_str());
}

void CefDemoView::OnSetFullscreen(bool const fullscreen)
{
	if (this->browserInstance != nullptr)
	{
		if (fullscreen)
			CefWindowsHelpers::Maximize(this->browserInstance);
		else
			CefWindowsHelpers::Restore(this->browserInstance);
	}
}

void CefDemoView::OnSetLoadingState(bool const isLoading,
                                    bool const canGoBack,
                                    bool const canGoForward)
{
}

void CefDemoView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	InitStartUrl();

	auto rect = RECT{0};
	GetClientRect(&rect);

	CefWindowInfo info;
	info.SetAsChild(GetSafeHwnd(), rect);

	CefBrowserSettings browserSettings;
	browserSettings.web_security = STATE_DISABLED;

	this->clientHandler = new ClientHandler(this);
	this->clientHandler->CreateBrowser(info, browserSettings, CefString(this->startingUrl));
}

void CefDemoView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (this->clientHandler != nullptr)
	{
		if (this->browserInstance != nullptr)
		{
			auto hwnd = this->browserInstance->GetHost()->GetWindowHandle();
			auto rect = RECT{0};
			GetClientRect(&rect);

			::SetWindowPos(
				hwnd,
				HWND_TOP,
				rect.left,
				rect.top,
				rect.right - rect.left,
				rect.bottom - rect.top,
				SWP_NOZORDER
			);
		}
	}
}

BOOL CefDemoView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
		if (pMsg->wParam == VK_F5)
			this->browserInstance->Reload();

	return CView::PreTranslateMessage(pMsg);
}

void CefDemoView::Navigate(CString const& url)
{
	if (this->browserInstance != nullptr)
	{
		auto frame = this->browserInstance->GetMainFrame();
		if (frame != nullptr)
			frame->LoadURL(CefString(url));
	}
}

void CefDemoView::NavigateBack()
{
	if (this->browserInstance != nullptr)
		this->browserInstance->GoBack();
}

void CefDemoView::NavigateForward()
{
	if (this->browserInstance != nullptr)
		this->browserInstance->GoForward();
}

bool CefDemoView::CanNavigateBack()
{
	return this->browserInstance != nullptr && this->browserInstance->CanGoBack();
}

bool CefDemoView::CanNavigateForward()
{
	return this->browserInstance != nullptr && this->browserInstance->CanGoForward();
}

void CefDemoView::InitStartUrl()
{
	this->startingUrl = "https://jesterous.net/?lang=en";
}

void CefDemoView::CloseBrowser()
{
	if (this->browserInstance != nullptr)
	{
		::DestroyWindow(this->browserInstance->GetHost()->GetWindowHandle());
		//this->browserInstance->GetHost()->CloseBrowser(true);
	}
}
