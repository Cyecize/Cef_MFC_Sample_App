
// cefmfcdemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "cefmfcdemo.h"
#include "MainFrm.h"

#include "CefDoc.h"
#include "CefView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CefMfcdDemoApp

BEGIN_MESSAGE_MAP(CefMfcdDemoApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CefMfcdDemoApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CefMfcdDemoApp construction

CefMfcdDemoApp::CefMfcdDemoApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("cefmfcdemo.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CefMfcdDemoApp object

CefMfcdDemoApp theApp;


// CefMfcdDemoApp initialization

BOOL CefMfcdDemoApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	if (!InitializeCef())
		return FALSE;

	CWinApp::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CefDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CefDemoView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

BOOL CefMfcdDemoApp::PreTranslateMessage(MSG* pMsg)
{
	auto msg = pMsg->message;
	if (pMsg->message == WM_KEYDOWN)
	{
		BOOL bCtrl = ::GetKeyState(VK_CONTROL) & 0x8000;

		if (bCtrl)
		{
			auto mainWindow = static_cast<CMainFrame*>(AfxGetMainWnd());
			CString urlMsg;

			switch (pMsg->wParam)
			{
			case VK_OEM_PLUS:
			case 0x6B: //num pad +	
				urlMsg = CString(_T("Plus Pressed"));
				mainWindow->SetUrl(urlMsg);
				break;
			case VK_OEM_MINUS:
			case 0x6D: //num pad -
				urlMsg = CString(_T("Minus pressed!"));
				mainWindow->SetUrl(urlMsg);
				break;
			default: break;
			}
		}
	}

	return CWinApp::PreTranslateMessage(pMsg);
}

int CefMfcdDemoApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	UninitializeCef();

	return CWinApp::ExitInstance();
}

BOOL CefMfcdDemoApp::PumpMessage()
{
	auto result = CWinApp::PumpMessage();

	// If there are other messages on queue then return right away
	// otherwise CEF has a habit of eating keystrokes not meant for it
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		return result;

	// Allow Cef to do his thing
	CefDoMessageLoopWork();

	return result;
}

// CefMfcdDemoApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CefMfcdDemoApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CefMfcdDemoApp message handlers


bool CefMfcdDemoApp::InitializeCef()
{
	// Initialize Chrome
	m_app = new CefMfcCefApp;
	CefMainArgs mainargs(m_hInstance);

	// See if this is a Chrome process starting. If so, we're done with our (and MFC) initialization.
	const auto exit_code = CefExecuteProcess(mainargs, m_app.get(), nullptr);
	if (exit_code >= 0)
		return false;

	CefSettings settings;
	settings.multi_threaded_message_loop = false;
	settings.no_sandbox = true;
	return CefInitialize(mainargs, settings, m_app.get(), nullptr);
}

void CefMfcdDemoApp::UninitializeCef()
{
	CefShutdown();
}

