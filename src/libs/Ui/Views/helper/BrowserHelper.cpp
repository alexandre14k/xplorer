// "src/libs/Ui/Views/helper/BrowserHelper.cpp"
#include "BrowserHelper.hpp"
String BrowserHelper::FormatUrl(String rawUrl) {
    String url = rawUrl;
    // If it's already a proper URL or about:blank, leave it alone
    if (url.find("://") != String::npos || url.find("about:") == 0) {
        return url;
    }
    // If it has no dot and no space, treat as search. If it has a dot, treat as domain.
    if (url.find(".") == String::npos) {
        for (char& c : url) { if (c == ' ') c = '+'; }
        url = "https://duckduckgo.com/?q=" + url;
    } else {
        url = "https://" + url;
    }
    return url;
}
void BrowserHelper::ClearCookies(WebKitWebView* webView) {
    WebKitNetworkSession* session =
        webkit_web_view_get_network_session(webView);
    WebKitWebsiteDataManager* manager =
        webkit_network_session_get_website_data_manager(session);
    webkit_website_data_manager_clear(
        manager, WEBKIT_WEBSITE_DATA_COOKIES, 0,
        nullptr, nullptr, nullptr
    );
}