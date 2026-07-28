// "src/libs/Ui/Views/Terminal.cpp"
#include "Terminal.hpp"
namespace {
gboolean OnScroll(GtkEventControllerScroll* self, double dx, double dy, gpointer d) {
    GdkModifierType state = gtk_event_controller_get_current_event_state(
        GTK_EVENT_CONTROLLER(self)
    );
    if (state & GDK_CONTROL_MASK) {
        Terminal* term = static_cast<Terminal*>(d);
        if (dy < 0) term->ChangeFontSize(1);
        else if (dy > 0) term->ChangeFontSize(-1);
        return TRUE;
    }
    return FALSE;
}
}
Terminal::Terminal(Context* context) {
    ctx = context;
    container = nullptr;
    vteTerm = nullptr;
    termScroll = nullptr;
    shellPath = "/bin/bash";
    fontFamily = "Monospace";
    fontSize = 12;
    bgColor = {1.0, 1.0, 1.0, 1.0};
    fgColor = {0.0, 0.0, 0.0, 1.0};
    cursorColor = {0.0, 0.0, 1.0, 1.0}; // Changed to blue
}
void Terminal::SpawnShell() {
    char* argv[2] = { const_cast<char*>(shellPath.c_str()), nullptr };
    char** envp = g_get_environ();
    const char* home = g_get_home_dir();
    vte_terminal_spawn_async(
        vteTerm,
        VTE_PTY_DEFAULT,
        home,
        argv,
        envp,
        G_SPAWN_SEARCH_PATH,
        nullptr, nullptr, nullptr,
        -1, nullptr, nullptr, nullptr
    );
    g_strfreev(envp);
}
void Terminal::SetupMouseScrollEvents() {
    GtkEventController* scroll = gtk_event_controller_scroll_new(
        static_cast<GtkEventControllerScrollFlags>(
            GTK_EVENT_CONTROLLER_SCROLL_VERTICAL | 
            GTK_EVENT_CONTROLLER_SCROLL_DISCRETE
        )
    );
    g_signal_connect(scroll, "scroll", G_CALLBACK(OnScroll), this);
    gtk_widget_add_controller(GTK_WIDGET(vteTerm), scroll);
}
void Terminal::Initialize() {
    container = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
    
    g_object_set_data_full(G_OBJECT(container), "terminal_obj", this, [](gpointer data) {
        delete static_cast<Terminal*>(data);
    });
    
    gtk_widget_set_vexpand(GTK_WIDGET(container), TRUE);
    gtk_widget_set_hexpand(GTK_WIDGET(container), TRUE);
    
    vteTerm = VTE_TERMINAL(vte_terminal_new());
    gtk_widget_set_vexpand(GTK_WIDGET(vteTerm), TRUE);
    gtk_widget_set_hexpand(GTK_WIDGET(vteTerm), TRUE);
    
    termScroll = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new());
    gtk_widget_set_vexpand(GTK_WIDGET(termScroll), TRUE);
    gtk_widget_set_hexpand(GTK_WIDGET(termScroll), TRUE);
    gtk_scrolled_window_set_child(termScroll, GTK_WIDGET(vteTerm));
    gtk_box_append(container, GTK_WIDGET(termScroll));
    
    vte_terminal_set_cursor_shape(vteTerm, VTE_CURSOR_SHAPE_BLOCK);
    SetupMouseScrollEvents();
    
    ApplySettings(fontFamily, fontSize, &bgColor, &fgColor);
    SpawnShell();
}
GtkWidget* Terminal::GetWidget() { 
    return GTK_WIDGET(container); 
}
void Terminal::ApplySettings(String fam, int size, GdkRGBA* bg, GdkRGBA* fg) {
    fontFamily = fam;
    fontSize = size;
    if (bg) bgColor = *bg;
    if (fg) fgColor = *fg;
    
    String fontDesc = fontFamily + " " + std::to_string(fontSize);
    PangoFontDescription* desc = pango_font_description_from_string(fontDesc.c_str());
    vte_terminal_set_font(vteTerm, desc);
    pango_font_description_free(desc);
    
    vte_terminal_set_color_background(vteTerm, &bgColor);
    vte_terminal_set_color_foreground(vteTerm, &fgColor);
    vte_terminal_set_color_cursor(vteTerm, &cursorColor);
}
void Terminal::ChangeFontSize(int delta) {
    fontSize += delta;
    if (fontSize < 6) fontSize = 6;
    if (fontSize > 48) fontSize = 48;
    String fontDesc = fontFamily + " " + std::to_string(fontSize);
    PangoFontDescription* desc = pango_font_description_from_string(fontDesc.c_str());
    vte_terminal_set_font(vteTerm, desc);
    pango_font_description_free(desc);
}
void Terminal::ChangeDirectory(String dir) {
    if (dir.empty()) dir = g_get_home_dir();
    String cmd = " cd \"" + dir + "\"\n";
    vte_terminal_feed_child(vteTerm, cmd.c_str(), cmd.length());
}
String Terminal::GetFontFamily() { return fontFamily; }
int Terminal::GetFontSize() { return fontSize; }
GdkRGBA Terminal::GetBgColor() { return bgColor; }
GdkRGBA Terminal::GetFgColor() { return fgColor; }