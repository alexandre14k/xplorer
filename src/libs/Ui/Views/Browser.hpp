// "src/libs/Ui/Views/Browser.hpp"
#pragma once
#include "Xplorer.hpp"
#include "libs/Context/Context.hpp"
#include <gtk/gtk.h>
#include <webkit/webkit.h>
class Browser {
    public:
        Browser(Context* ctx, GtkLabel* appStatus);
        void Initialize();
        GtkWidget* GetWidget();
        void LoadUrl(String url);
        void Refresh();
        void ClearCookies();
        void GoHome();
        void HandleAddressActivate();
        void HandleRefreshClicked();
        void HandleClearCookiesClicked();
        void HandleHomeClicked();
        void HandleSiteSelected(GtkListBoxRow* row);
        void SetHoverText(String text);
    private:
        Context* ctx;
        GtkLabel* appStatusBar;
        GtkBox* container;
        GtkEntry* addressEntry;
        GtkButton* refreshButton;
        GtkButton* cookiesButton;
        GtkButton* homeButton;
        GtkMenuButton* hamburgerButton;
        WebKitWebView* webView;
        GtkLabel* hoverLabel;
        String currentUrl;
        void SetStatus(String text);
};