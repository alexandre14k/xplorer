// "src/libs/Ui/Views/helper/BrowserHelper.hpp"
#pragma once
#include "Xplorer.hpp"
#include <webkit/webkit.h>
class BrowserHelper {
    public:
        static String FormatUrl(String rawUrl);
        static void ClearCookies(WebKitWebView* webView);
};