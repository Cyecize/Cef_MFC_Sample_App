// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
#include "stdafx.h"
#include "ClientHandler.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

ClientHandler::ClientHandler(Delegate* delegate)
   : m_delegate(delegate)
{
}

ClientHandler::~ClientHandler()
{
}

void ClientHandler::CreateBrowser(CefWindowInfo const & info, CefBrowserSettings const & settings, CefString const & url)
{
   CefBrowserHost::CreateBrowser(info, this, url, settings, nullptr);
}

void ClientHandler::OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url)
{
   CEF_REQUIRE_UI_THREAD();

   // Only update the address for the main (top-level) frame.
   if(frame->IsMain())
   {
      if(m_delegate != nullptr)
         m_delegate->OnSetAddress(url);
   }
}

void ClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
   CEF_REQUIRE_UI_THREAD();

   if(m_delegate != nullptr)
      m_delegate->OnSetTitle(title);
}

//void ClientHandler::OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen)
//{
//   CEF_REQUIRE_UI_THREAD();
//
//   if(m_delegate != nullptr)
//      m_delegate->OnSetFullscreen(fullscreen);
//}

void ClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
   CEF_REQUIRE_UI_THREAD();

   if (m_delegate != nullptr)
	   m_delegate->OnBrowserCreated(browser);
}

bool ClientHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
   CEF_REQUIRE_UI_THREAD();

   if(m_delegate != nullptr)
      m_delegate->OnBrowserClosing(browser);

   return false;
}

void ClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
   CEF_REQUIRE_UI_THREAD();

   if(m_delegate != nullptr)
      m_delegate->OnBrowserClosed(browser);
}

void ClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
   CefRefPtr<CefFrame> frame,
   ErrorCode errorCode,
   const CefString& errorText,
   const CefString& failedUrl)
{
   CEF_REQUIRE_UI_THREAD();

   // Don't display an error for downloaded files.
   if(errorCode == ERR_ABORTED)
      return;

   // Display a load error message.
   std::stringstream ss;
   ss << "<html><body bgcolor=\"white\">"
      "<h2>Failed to load URL " << std::string(failedUrl) <<
      " with error " << std::string(errorText) << " (" << errorCode <<
      ").</h2></body></html>";
   frame->LoadString(ss.str(), failedUrl);
}

void ClientHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
{
   CEF_REQUIRE_UI_THREAD();

   if(m_delegate != nullptr)
      m_delegate->OnSetLoadingState(isLoading, canGoBack, canGoForward);
}

void ClientHandler::DetachDelegate()
{
   m_delegate = nullptr;
}