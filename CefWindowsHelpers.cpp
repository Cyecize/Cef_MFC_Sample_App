#include "stdafx.h"
#include "CefWindowsHelpers.h"

namespace
{
   HWND GetRootHwnd(CefRefPtr<CefBrowser> browser)
   {
      return ::GetAncestor(browser->GetHost()->GetWindowHandle(), GA_ROOT);
   }

   // Toggles the current display state.
   void Toggle(HWND const root_hwnd, UINT const nCmdShow)
   {
      // Retrieve current window placement information.
      WINDOWPLACEMENT placement;
      ::GetWindowPlacement(root_hwnd, &placement);

      if(placement.showCmd == nCmdShow)
         ::ShowWindow(root_hwnd, SW_RESTORE);
      else
         ::ShowWindow(root_hwnd, nCmdShow);
   }
}

namespace CefWindowsHelpers
{
   void ModifyBounds(const CefRect& display, CefRect& window) 
   {
      window.x += display.x;
      window.y += display.y;

      if(window.x < display.x)
         window.x = display.x;
      if(window.y < display.y)
         window.y = display.y;
      if(window.width < 100)
         window.width = 100;
      else if(window.width >= display.width)
         window.width = display.width;
      if(window.height < 100)
         window.height = 100;
      else if(window.height >= display.height)
         window.height = display.height;
      if(window.x + window.width >= display.x + display.width)
         window.x = display.x + display.width - window.width;
      if(window.y + window.height >= display.y + display.height)
         window.y = display.y + display.height - window.height;
   }

   void SetPos(CefRefPtr<CefBrowser> browser,
      int const x, int const y, int const width, int const height)
   {
      HWND root_hwnd = GetRootHwnd(browser);

      // Retrieve current window placement information.
      WINDOWPLACEMENT placement;
      ::GetWindowPlacement(root_hwnd, &placement);

      // Retrieve information about the display that contains the window.
      HMONITOR monitor = MonitorFromRect(&placement.rcNormalPosition,
         MONITOR_DEFAULTTONEAREST);
      MONITORINFO info;
      info.cbSize = sizeof(info);
      GetMonitorInfo(monitor, &info);

      // Make sure the window is inside the display.
      CefRect display_rect(
         info.rcWork.left,
         info.rcWork.top,
         info.rcWork.right - info.rcWork.left,
         info.rcWork.bottom - info.rcWork.top);
      CefRect window_rect(x, y, width, height);
      ModifyBounds(display_rect, window_rect);

      if(placement.showCmd == SW_MINIMIZE || placement.showCmd == SW_MAXIMIZE) 
      {
         // The window is currently minimized or maximized. Restore it to the desired
         // position.
         placement.rcNormalPosition.left = window_rect.x;
         placement.rcNormalPosition.right = window_rect.x + window_rect.width;
         placement.rcNormalPosition.top = window_rect.y;
         placement.rcNormalPosition.bottom = window_rect.y + window_rect.height;
         ::SetWindowPlacement(root_hwnd, &placement);
         ::ShowWindow(root_hwnd, SW_RESTORE);
      }
      else 
      {
         // Set the window position.
         ::SetWindowPos(root_hwnd, NULL, window_rect.x, window_rect.y,
            window_rect.width, window_rect.height, SWP_NOZORDER);
      }
   }

   void Minimize(CefRefPtr<CefBrowser> browser) 
   {
      Toggle(GetRootHwnd(browser), SW_MINIMIZE);
   }

   void Maximize(CefRefPtr<CefBrowser> browser) 
   {
      Toggle(GetRootHwnd(browser), SW_MAXIMIZE);
   }

   void Restore(CefRefPtr<CefBrowser> browser) 
   {
      ::ShowWindow(GetRootHwnd(browser), SW_RESTORE);
   }
}