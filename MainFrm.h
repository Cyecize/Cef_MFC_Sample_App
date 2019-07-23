
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "MainToolbar.h"

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
//TODO: set Zoom from here

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
};


