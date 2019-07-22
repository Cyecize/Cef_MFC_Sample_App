#pragma once

#include "include/internal/cef_types_wrappers.h"
#include "include/cef_browser.h"

namespace CefWindowsHelpers
{
   // Fit |window| inside |display|. Coordinates are relative to the upper-left
   // corner of the display.
   void ModifyBounds(CefRect const & display, CefRect& window);

   // Platform implementations.
   void SetPos(CefRefPtr<CefBrowser> browser, int const x, int const y, int const width, int const height);
   void Minimize(CefRefPtr<CefBrowser> browser);
   void Maximize(CefRefPtr<CefBrowser> browser);
   void Restore(CefRefPtr<CefBrowser> browser);
}