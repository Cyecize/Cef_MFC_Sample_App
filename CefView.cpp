
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
   if(m_clientHandler != nullptr)
      m_clientHandler->DetachDelegate();
}

BOOL CefDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CefDemoView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
   m_wndMain = AfxGetMainWnd();

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
   m_browser = browser;
}

void CefDemoView::OnBrowserClosing(CefRefPtr<CefBrowser> browser)
{
}

void CefDemoView::OnBrowserClosed(CefRefPtr<CefBrowser> browser)
{
   if(m_browser != nullptr && 
      m_browser->GetIdentifier() == browser->GetIdentifier())
   {
      m_browser = nullptr;

      m_clientHandler->DetachDelegate();
   }
}

void CefDemoView::OnSetAddress(std::string const & url)
{
   auto main = static_cast<CMainFrame*>(m_wndMain);
   if(main != nullptr)
   {
      auto newurl = CString {url.c_str()};
      if(newurl.Find(m_startUrl) >= 0)
         newurl = "";

      main->SetUrl(newurl);
   }
}

void CefDemoView::OnSetTitle(std::string const & title)
{
   ::SetWindowText(m_hWnd, CefString(title).ToWString().c_str());
}

void CefDemoView::OnSetFullscreen(bool const fullscreen)
{
   if(m_browser != nullptr)
   {
      if(fullscreen)
      {
         CefWindowsHelpers::Maximize(m_browser);
      }
      else 
      {
         CefWindowsHelpers::Restore(m_browser);
      }
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

   m_clientHandler = new ClientHandler(this);
   m_clientHandler->CreateBrowser(info, browserSettings, CefString(m_startUrl));
}

void CefDemoView::OnSize(UINT nType, int cx, int cy)
{
   CView::OnSize(nType, cx, cy);

   if(m_clientHandler != nullptr)
   {
      if(m_browser != nullptr)
      {
         auto hwnd = m_browser->GetHost()->GetWindowHandle();
         auto rect = RECT {0};
         GetClientRect(&rect);

         ::SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
      }
   }   
}

BOOL CefDemoView::PreTranslateMessage(MSG* pMsg)
{
   if(pMsg->message == WM_KEYDOWN)
   {
      if(pMsg->wParam == VK_F5)
      {
         m_browser->Reload();
      }
   }

   return CView::PreTranslateMessage(pMsg);  
}

void CefDemoView::Navigate(CString const & url)
{
   if(m_browser != nullptr)
   {
      auto frame = m_browser->GetMainFrame();
      if(frame != nullptr)
      {
         frame->LoadURL(CefString(url));
      }
   }
}

void CefDemoView::NavigateBack()
{
   if(m_browser != nullptr)
   {
      m_browser->GoBack();
   }
}

void CefDemoView::NavigateForward()
{
   if(m_browser != nullptr)
   {
      m_browser->GoForward();
   }
}

bool CefDemoView::CanNavigateBack()
{
   return m_browser != nullptr && m_browser->CanGoBack();
}

bool CefDemoView::CanNavigateForward()
{
	
   return m_browser != nullptr && m_browser->CanGoForward();

}

void CefDemoView::InitStartUrl()
{
	m_startUrl = "https://jesterous.net/?lang=en";
}

void CefDemoView::CloseBrowser()
{
   if(m_browser != nullptr)
   {
      ::DestroyWindow(m_browser->GetHost()->GetWindowHandle());
      //m_browser->GetHost()->CloseBrowser(true);
   }
}