// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

#include "include/base/cef_lock.h"
#include "include/cef_client.h"

class ClientHandler : public CefClient,
                      public CefDisplayHandler,
                      public CefLifeSpanHandler,
                      public CefLoadHandler 
{
public:
   // Implement this interface to receive notification of ClientHandler
   // events. The methods of this class will be called on the main thread.
   class Delegate 
   {
   public:
      // Called when the browser is created.
      virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) = 0;

      // Called when the browser is closing.
      virtual void OnBrowserClosing(CefRefPtr<CefBrowser> browser) = 0;

      // Called when the browser has been closed.
      virtual void OnBrowserClosed(CefRefPtr<CefBrowser> browser) = 0;

      // Set the window URL address.
      virtual void OnSetAddress(std::string const & url) = 0;

      // Set the window title.
      virtual void OnSetTitle(std::string const & title) = 0;

      // Set fullscreen mode.
      virtual void OnSetFullscreen(bool const fullscreen) = 0;

      // Set the loading state.
      virtual void OnSetLoadingState(bool const isLoading,
         bool const canGoBack,
         bool const canGoForward) = 0;

   protected:
      virtual ~Delegate() {}
   };

 public:
  ClientHandler(Delegate* delegate);
  ~ClientHandler();

  void CreateBrowser(CefWindowInfo const & info, CefBrowserSettings const & settings, CefString const & url);

  // CefClient methods:
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }

  // CefDisplayHandler methods:
  virtual void OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) override;
  virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) override;
  //virtual void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen) override;

  // CefLifeSpanHandler methods:
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
  virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

  // CefLoadHandler methods:
  virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
     bool isLoading,
     bool canGoBack,
     bool canGoForward) override;

  virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           ErrorCode errorCode,
                           const CefString& errorText,
                           const CefString& failedUrl) override;

  // This object may outlive the Delegate object so it's necessary for the
  // Delegate to detach itself before destruction.
  void DetachDelegate();

private:
  
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(ClientHandler);
  // Include the default locking implementation.
  IMPLEMENT_LOCKING(ClientHandler);

private:
   Delegate* m_delegate;
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
