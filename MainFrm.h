
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "MainToolbar.h"
#include <functional>

class CefDemoView;

class CMainFrame : public CFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// Attributes
public:

	// Operations
public:
	void SetUrl(CString const & url);
	void ZoomIn();
	void ZoomOut();
	void SetOnZoomChanged(void(*zoomCallback)(double zoom, CefRefPtr<CefBrowser> browser), CefRefPtr<CefBrowser> browserInstance);

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;

	// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMainToolBar      m_wndToolBar;
	CStatusBar        m_wndStatusBar;

	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

private:
	void OnGo();
	void OnGoBack();
	void OnGoForward();
	void OnUpdateGo(CCmdUI*);
	void OnUpdateGoBack(CCmdUI*);
	void OnUpdateGoForward(CCmdUI*);

	CefDemoView* GetView();
	void(*zoomCallback)(double zoom, CefRefPtr<CefBrowser> browser);
	CefRefPtr<CefBrowser> browserInstance;
	const double MAX_ZOOM = 10.0;
	const double MIN_ZOOM = -5.0;
	const double ZOOM_INCREMENT = 0.5;
	double currentZoom = 0.0;
};


