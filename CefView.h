// CefDemoView.h : interface of the CefDemoView class
//

#pragma once

#include "ClientHandler.h"

class CefDemoView : public CView, public ClientHandler::Delegate
{
protected: // create from serialization only
	CefDemoView();
DECLARE_DYNCREATE(CefDemoView)

	// Attributes
public:
	CefDoc* GetDocument() const;

	// Operations
public:
	void Navigate(CString const& url);
	void NavigateBack();
	void NavigateForward();
	bool CanNavigateBack();
	bool CanNavigateForward();
	void CloseBrowser();

	// Overrides
public:
	virtual void OnDraw(CDC* pDC); // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:

	// Implementation
public:
	virtual ~CefDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;

	// Called when the browser is closing.
	virtual void OnBrowserClosing(CefRefPtr<CefBrowser> browser) override;

	// Called when the browser has been closed.
	virtual void OnBrowserClosed(CefRefPtr<CefBrowser> browser) override;

	// Set the window URL address.
	virtual void OnSetAddress(std::string const& url) override;

	// Set the window title.
	virtual void OnSetTitle(std::string const& title) override;

	// Set fullscreen mode.
	virtual void OnSetFullscreen(bool const fullscreen) override;

	// Set the loading state.
	virtual void OnSetLoadingState(bool const isLoading,
	                               bool const canGoBack,
	                               bool const canGoForward) override;

	// Generated message map functions
protected:
DECLARE_MESSAGE_MAP()

	virtual void OnInitialUpdate();
	void OnSize(UINT nType, int cx, int cy);

private:
	void InitStartUrl();

	CefRefPtr<ClientHandler> clientHandler;
	CefRefPtr<CefBrowser> browserInstance;
	CMainFrame* mainFrame = nullptr;
	CString startingUrl;
};

#ifndef _DEBUG  // debug version in CefDemoView.cpp
inline CefDoc* CefDemoView::GetDocument() const
   { return reinterpret_cast<CefDoc*>(m_pDocument); }
#endif
