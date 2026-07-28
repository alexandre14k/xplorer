// "src/libs/Ui/Views/Browser.cpp"
#include "Browser.hpp"
#include "libs/Ui/Views/helper/BrowserHelper.hpp"
namespace {
void OnAddressActivate(GtkEntry*, gpointer d) { static_cast<Browser*>(d)->HandleAddressActivate(); }
void OnRefreshClicked(GtkButton*, gpointer d) { static_cast<Browser*>(d)->HandleRefreshClicked(); }
void OnClearCookiesClicked(GtkButton*, gpointer d) { static_cast<Browser*>(d)->HandleClearCookiesClicked(); }
void OnHomeClicked(GtkButton*, gpointer d) { static_cast<Browser*>(d)->HandleHomeClicked(); }
void OnSiteSelected(GtkListBox* b, GtkListBoxRow* r, gpointer d) { if(r) static_cast<Browser*>(d)->HandleSiteSelected(r); }
void OnMouseTargetChanged(WebKitWebView* wv, WebKitHitTestResult* htr, guint mods, gpointer d) {
    Browser* br = static_cast<Browser*>(d);
    if (webkit_hit_test_result_context_is_link(htr)) {
        const char* uri = webkit_hit_test_result_get_link_uri(htr);
        br->SetHoverText(String(uri));
    } else {
        br->SetHoverText("");
    }
}
}
Browser::Browser(Context* context, GtkLabel* appStatus) {
    ctx = context;
    appStatusBar = appStatus;
    container = nullptr;
    addressEntry = nullptr;
    refreshButton = nullptr;
    cookiesButton = nullptr;
    homeButton = nullptr;
    hamburgerButton = nullptr;
    webView = nullptr;
    hoverLabel = nullptr;
    currentUrl = "about:blank";
}
void Browser::SetStatus(String text) { 
    if (appStatusBar) gtk_label_set_text(appStatusBar, text.c_str()); 
}
void Browser::SetHoverText(String text) {
    gtk_label_set_text(hoverLabel, text.c_str());
}
void Browser::Initialize() {
    container = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
    
    g_object_set_data_full(G_OBJECT(container), "browser_obj", this, [](gpointer data) {
        delete static_cast<Browser*>(data);
    });
    
    gtk_widget_set_vexpand(GTK_WIDGET(container), TRUE);
    gtk_widget_set_hexpand(GTK_WIDGET(container), TRUE);
    GtkBox* toolbar = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6));
    gtk_widget_set_margin_top(GTK_WIDGET(toolbar), 6);
    gtk_widget_set_margin_bottom(GTK_WIDGET(toolbar), 6);
    gtk_widget_set_margin_start(GTK_WIDGET(toolbar), 6);
    gtk_widget_set_margin_end(GTK_WIDGET(toolbar), 6);
    homeButton = GTK_BUTTON(gtk_button_new_with_label("Home"));
    refreshButton = GTK_BUTTON(gtk_button_new_from_icon_name("view-refresh-symbolic"));
    addressEntry = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(addressEntry, "Enter address");
    gtk_widget_set_hexpand(GTK_WIDGET(addressEntry), TRUE);
    cookiesButton = GTK_BUTTON(gtk_button_new_with_label("Cookies"));
    hamburgerButton = GTK_MENU_BUTTON(gtk_menu_button_new());
    GtkPopover* popover = GTK_POPOVER(gtk_popover_new());
    GtkListBox* list = GTK_LIST_BOX(gtk_list_box_new());
    gtk_list_box_set_selection_mode(list, GTK_SELECTION_SINGLE);
    const char* sites[] = {"www.google.fr", "github.com"};
    for (const char* s : sites) gtk_list_box_append(list, gtk_label_new(s));
    g_signal_connect(list, "row-selected", G_CALLBACK(OnSiteSelected), this);
    gtk_popover_set_child(popover, GTK_WIDGET(list));
    gtk_menu_button_set_popover(hamburgerButton, GTK_WIDGET(popover));
    g_signal_connect(homeButton, "clicked", G_CALLBACK(OnHomeClicked), this);
    g_signal_connect(refreshButton, "clicked", G_CALLBACK(OnRefreshClicked), this);
    g_signal_connect(addressEntry, "activate", G_CALLBACK(OnAddressActivate), this);
    g_signal_connect(cookiesButton, "clicked", G_CALLBACK(OnClearCookiesClicked), this);
    gtk_box_append(toolbar, GTK_WIDGET(homeButton));
    gtk_box_append(toolbar, GTK_WIDGET(refreshButton));
    gtk_box_append(toolbar, GTK_WIDGET(addressEntry));
    gtk_box_append(toolbar, GTK_WIDGET(cookiesButton));
    gtk_box_append(toolbar, GTK_WIDGET(hamburgerButton));
    
    GtkWidget* overlay = gtk_overlay_new();
    webView = WEBKIT_WEB_VIEW(webkit_web_view_new());
    gtk_widget_set_vexpand(GTK_WIDGET(webView), TRUE);
    gtk_widget_set_hexpand(GTK_WIDGET(webView), TRUE);
    gtk_overlay_set_child(GTK_OVERLAY(overlay), GTK_WIDGET(webView));
    
    hoverLabel = GTK_LABEL(gtk_label_new(""));
    gtk_widget_set_halign(GTK_WIDGET(hoverLabel), GTK_ALIGN_START);
    gtk_widget_set_valign(GTK_WIDGET(hoverLabel), GTK_ALIGN_END);
    gtk_widget_set_margin_start(GTK_WIDGET(hoverLabel), 6);
    gtk_widget_set_margin_bottom(GTK_WIDGET(hoverLabel), 6);
    gtk_widget_add_css_class(GTK_WIDGET(hoverLabel), "hover-label");
    
    gtk_label_set_wrap(GTK_LABEL(hoverLabel), TRUE);
    gtk_label_set_wrap_mode(GTK_LABEL(hoverLabel), PANGO_WRAP_WORD_CHAR);
    gtk_label_set_max_width_chars(GTK_LABEL(hoverLabel), 80);
    gtk_label_set_natural_wrap_mode(GTK_LABEL(hoverLabel), GTK_NATURAL_WRAP_INHERIT);
    
    // CSS for solid white background sticking to left bottom
    GtkCssProvider* css = gtk_css_provider_new();
    gtk_css_provider_load_from_string(css, 
        ".hover-label { background: white; padding: 2px 6px; color: black; border-radius: 4px; border: 1px solid gray; }"
    );
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(hoverLabel)), GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(css);
    
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), GTK_WIDGET(hoverLabel));
    g_signal_connect(webView, "mouse-target-changed", G_CALLBACK(OnMouseTargetChanged), this);
    
    gtk_box_append(container, GTK_WIDGET(toolbar));
    gtk_box_append(container, overlay);
    
    SetStatus("Ready");
    LoadUrl(currentUrl);
}
GtkWidget* Browser::GetWidget() { return GTK_WIDGET(container); }
void Browser::LoadUrl(String rawUrl) {
    currentUrl = BrowserHelper::FormatUrl(rawUrl);
    webkit_web_view_load_uri(webView, currentUrl.c_str());
    gtk_editable_set_text(GTK_EDITABLE(addressEntry), currentUrl.c_str());
    SetStatus("Loaded: " + currentUrl);
}
void Browser::Refresh() { webkit_web_view_reload(webView); }
void Browser::ClearCookies() { BrowserHelper::ClearCookies(webView); SetStatus("Cookies cleared"); }
void Browser::GoHome() { LoadUrl("about:blank"); }
void Browser::HandleAddressActivate() { LoadUrl(gtk_editable_get_text(GTK_EDITABLE(addressEntry))); }
void Browser::HandleRefreshClicked() { Refresh(); }
void Browser::HandleClearCookiesClicked() { ClearCookies(); }
void Browser::HandleHomeClicked() { GoHome(); }
void Browser::HandleSiteSelected(GtkListBoxRow* row) {
    const char* text = gtk_label_get_text(GTK_LABEL(gtk_list_box_row_get_child(row)));
    LoadUrl("https://" + String(text));
}