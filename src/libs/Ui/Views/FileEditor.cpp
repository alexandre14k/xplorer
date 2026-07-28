// "src/libs/Ui/Views/FileEditor.cpp"
#include "FileEditor.hpp"
#include "libs/Ui/Views/helper/FileEditorHelper.hpp"

namespace {
gboolean OnKeyPressed(GtkEventControllerKey* c, guint k, guint kc, GdkModifierType s, gpointer d) {
    if (k == GDK_KEY_Tab) {
        FileEditor* ed = static_cast<FileEditor*>(d);
        gtk_text_buffer_insert_at_cursor(
            gtk_text_view_get_buffer(ed->GetTextView()), "    ", 4
        );
        return TRUE;
    }
    return FALSE;
}

gboolean OnScroll(GtkEventControllerScroll* self, double dx, double dy, gpointer d) {
    GdkModifierType state = gtk_event_controller_get_current_event_state(
        GTK_EVENT_CONTROLLER(self)
    );
    if (state & GDK_CONTROL_MASK) {
        FileEditor* ed = static_cast<FileEditor*>(d);
        if (dy < 0) ed->ChangeFontSize(1);
        else if (dy > 0) ed->ChangeFontSize(-1);
        return TRUE;
    }
    return FALSE;
}

void OnScrollChanged(GtkAdjustment* adj, gpointer d) {
    FileEditor* ed = static_cast<FileEditor*>(d);
    ed->UpdateLineHighlight();
    ed->TriggerDraw();
}

void OnCursorMoved(GObject* b, GParamSpec* p, gpointer d) {
    FileEditor* ed = static_cast<FileEditor*>(d);
    ed->UpdateLineHighlight();
    ed->TriggerDraw();
}
}

FileEditor::FileEditor(Context* context) {
    ctx = context;
    container = nullptr;
    overlay = nullptr;
    textView = nullptr;
    scrollWindow = nullptr;
    lineNumbers = nullptr;
    marginOverlay = nullptr;
    lineHighlightOverlay = nullptr;
    helper = nullptr;
}

FileEditor::~FileEditor() { delete helper; }

void FileEditor::SetupKeyEvents() {
    GtkEventController* key = gtk_event_controller_key_new();
    g_signal_connect(key, "key-pressed", G_CALLBACK(OnKeyPressed), this);
    gtk_widget_add_controller(GTK_WIDGET(textView), key);
}

void FileEditor::SetupScrollEvents() {
    GtkAdjustment* vadj = gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(textView));
    g_signal_connect(vadj, "value-changed", G_CALLBACK(OnScrollChanged), this);
    GtkAdjustment* hadj = gtk_scrollable_get_hadjustment(GTK_SCROLLABLE(textView));
    g_signal_connect(hadj, "value-changed", G_CALLBACK(OnScrollChanged), this);
}

void FileEditor::SetupMouseScrollEvents() {
    GtkEventController* scroll = gtk_event_controller_scroll_new(
        static_cast<GtkEventControllerScrollFlags>(
            GTK_EVENT_CONTROLLER_SCROLL_VERTICAL | 
            GTK_EVENT_CONTROLLER_SCROLL_DISCRETE
        )
    );
    g_signal_connect(scroll, "scroll", G_CALLBACK(OnScroll), this);
    gtk_widget_add_controller(GTK_WIDGET(textView), scroll);
}

void FileEditor::Initialize() {
    container = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
    
    g_object_set_data_full(G_OBJECT(container), "editor_obj", this, [](gpointer data) {
        delete static_cast<FileEditor*>(data);
    });
    
    lineNumbers = GTK_DRAWING_AREA(gtk_drawing_area_new());
    gtk_widget_set_size_request(GTK_WIDGET(lineNumbers), 40, -1);
    overlay = GTK_OVERLAY(gtk_overlay_new());
    
    textView = GTK_SOURCE_VIEW(gtk_source_view_new());
    GtkSourceBuffer* buf = GTK_SOURCE_BUFFER(
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView))
    );
    gtk_source_buffer_set_highlight_syntax(buf, TRUE);
    
    GtkSourceStyleSchemeManager* scheme_mgr = gtk_source_style_scheme_manager_get_default();
    GtkSourceStyleScheme* scheme = gtk_source_style_scheme_manager_get_scheme(scheme_mgr, "classic");
    if (scheme) gtk_source_buffer_set_style_scheme(buf, scheme);

    gtk_widget_set_vexpand(GTK_WIDGET(textView), TRUE);
    gtk_widget_set_hexpand(GTK_WIDGET(textView), TRUE);
    
    // Padding disabled as requested
    
    scrollWindow = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new());
    gtk_widget_set_vexpand(GTK_WIDGET(scrollWindow), TRUE);
    gtk_widget_set_hexpand(GTK_WIDGET(scrollWindow), TRUE);
    gtk_scrolled_window_set_child(scrollWindow, GTK_WIDGET(textView));
    gtk_overlay_set_child(overlay, GTK_WIDGET(scrollWindow));
    
    // Line highlight overlay disabled
    // lineHighlightOverlay = GTK_DRAWING_AREA(gtk_drawing_area_new());
    // gtk_widget_set_can_target(GTK_WIDGET(lineHighlightOverlay), FALSE);
    // gtk_overlay_add_overlay(overlay, GTK_WIDGET(lineHighlightOverlay));
    
    marginOverlay = GTK_DRAWING_AREA(gtk_drawing_area_new());
    gtk_widget_set_can_target(GTK_WIDGET(marginOverlay), FALSE);
    gtk_overlay_add_overlay(overlay, GTK_WIDGET(marginOverlay));
    
    gtk_box_append(container, GTK_WIDGET(lineNumbers));
    gtk_box_append(container, GTK_WIDGET(overlay));
    gtk_widget_add_css_class(GTK_WIDGET(textView), "xplorer-editor");
    
    helper = new FileEditorHelper(this);
    helper->Initialize(lineNumbers, marginOverlay, nullptr);
    
    UpdateWrapState();
    SetupKeyEvents();
    SetupScrollEvents();
    SetupMouseScrollEvents();
    
    g_signal_connect(buf, "notify::cursor-position", G_CALLBACK(OnCursorMoved), this);
}

void FileEditor::ChangeFontSize(int delta) { helper->ChangeFontSize(delta); }
void FileEditor::ApplyFont(String family, int size) { helper->ApplyFont(family, size); }
GtkWidget* FileEditor::GetWidget() { return GTK_WIDGET(container); }

void FileEditor::SetText(String text) {
    gtk_text_buffer_set_text(
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView)), text.c_str(), -1
    );
}

String FileEditor::GetText() {
    GtkTextBuffer* b = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
    GtkTextIter s, e;
    gtk_text_buffer_get_bounds(b, &s, &e);
    char* r = gtk_text_buffer_get_text(b, &s, &e, FALSE);
    String res(r);
    g_free(r);
    return res;
}

void FileEditor::UpdateWrapState() {
    if (ctx && ctx->GetWrapEnabled()) {
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView), GTK_WRAP_WORD_CHAR);
    } else {
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView), GTK_WRAP_NONE);
    }
}

void FileEditor::SetFilePath(String path) {
    filePath = path;
    GtkSourceLanguageManager* lm = gtk_source_language_manager_get_default();
    GtkSourceLanguage* lang = gtk_source_language_manager_guess_language(lm, path.c_str(), nullptr);
    if (lang) {
        GtkSourceBuffer* buf = GTK_SOURCE_BUFFER(
            gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView))
        );
        gtk_source_buffer_set_language(buf, lang);
    }
}

String FileEditor::GetFilePath() { return filePath; }
void FileEditor::TriggerDraw() { helper->TriggerDraw(); }
GtkTextView* FileEditor::GetTextView() { return GTK_TEXT_VIEW(textView); }
Context* FileEditor::GetContext() { return ctx; }
void FileEditor::UpdateLineHighlight() { helper->UpdateLineHighlight(); }