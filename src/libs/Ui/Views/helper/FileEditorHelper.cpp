// "src/libs/Ui/Views/helper/FileEditorHelper.cpp"
#include "FileEditorHelper.hpp"
#include "libs/Ui/Views/FileEditor.hpp"
namespace {
void OnDrawLines(GtkDrawingArea* area, cairo_t* cr, int w, int h, gpointer data) {
    FileEditorHelper* helper = static_cast<FileEditorHelper*>(data);
    FileEditor* ed = helper->GetEditor();
    GtkTextView* tv = ed->GetTextView();
    GtkTextBuffer* buf = gtk_text_view_get_buffer(tv);
    int count = gtk_text_buffer_get_line_count(buf);
    GtkAdjustment* adj = gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(tv));
    double y = gtk_adjustment_get_value(adj);
    PangoLayout* layout = gtk_widget_create_pango_layout(GTK_WIDGET(tv), "");
    cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
    for (int i = 0; i < count; i++) {
        GtkTextIter iter;
        gtk_text_buffer_get_iter_at_line(buf, &iter, i);
        GdkRectangle rect;
        gtk_text_view_get_iter_location(tv, &iter, &rect);
        if (rect.y - y < -rect.height || rect.y - y > h) continue;
        char num[16];
        snprintf(num, 16, "%d", i + 1);
        pango_layout_set_text(layout, num, -1);
        cairo_move_to(cr, 4, rect.y - y + 2);
        pango_cairo_show_layout(cr, layout);
    }
    g_object_unref(layout);
}
void OnDrawOverlay(GtkDrawingArea* area, cairo_t* cr, int w, int h, gpointer data) {
    FileEditorHelper* helper = static_cast<FileEditorHelper*>(data);
    FileEditor* ed = helper->GetEditor();
    int col = ed->GetContext() ? ed->GetContext()->GetDelimiterColumn() : 70;
    PangoLayout* layout = gtk_widget_create_pango_layout(GTK_WIDGET(ed->GetTextView()), "M");
    int charW = 0;
    pango_layout_get_pixel_size(layout, &charW, nullptr);
    g_object_unref(layout);
    int x = charW * col + 4;
    if (x < w) {
        cairo_set_source_rgba(cr, 0.9, 0.9, 0.9, 0.5);
        cairo_rectangle(cr, x, 0, w - x, h);
        cairo_fill(cr);
        cairo_set_source_rgba(cr, 0.6, 0.6, 0.6, 1.0);
        cairo_move_to(cr, x, 0);
        cairo_line_to(cr, x, h);
        cairo_stroke(cr);
    }
}
}
FileEditorHelper::FileEditorHelper(FileEditor* ed) {
    editor = ed;
    lineNumbers = nullptr;
    marginOverlay = nullptr;
    lineHighlightOverlay = nullptr;
    cssProvider = nullptr;
    currentLineY = 0;
    currentLineHeight = 0;
    fontSize = 12;
    fontFamily = "Monospace";
}
FileEditorHelper::~FileEditorHelper() {
    if (cssProvider) g_object_unref(cssProvider);
}
void FileEditorHelper::Initialize(GtkDrawingArea* lines, GtkDrawingArea* overlay, GtkDrawingArea* highlight) {
    lineNumbers = lines;
    marginOverlay = overlay;
    lineHighlightOverlay = highlight;
    gtk_drawing_area_set_draw_func(lineNumbers, OnDrawLines, this, nullptr);
    gtk_drawing_area_set_draw_func(marginOverlay, OnDrawOverlay, this, nullptr);
    
    cssProvider = gtk_css_provider_new();
    GdkDisplay* display = gtk_widget_get_display(GTK_WIDGET(editor->GetTextView()));
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    Context* ctx = editor->GetContext();
    if (ctx) {
        fontFamily = ctx->GetEditorFontFamily();
        fontSize = ctx->GetEditorFontSize();
    }
    ChangeFontSize(0);
}
void FileEditorHelper::ApplyFont(String family, int size) {
    fontFamily = family;
    fontSize = size;
    ChangeFontSize(0);
}
void FileEditorHelper::ChangeFontSize(int delta) {
    fontSize += delta;
    if (fontSize < 6) fontSize = 6;
    if (fontSize > 48) fontSize = 48;
    // Changed caret-color to blue
    String css = ".xplorer-editor { font-family: " + fontFamily + 
                 "; font-size: " + std::to_string(fontSize) + "pt; caret-color: blue; }";
    gtk_css_provider_load_from_string(cssProvider, css.c_str());
    Context* ctx = editor->GetContext();
    if (ctx) ctx->SetEditorFontSize(fontSize);
}
void FileEditorHelper::UpdateLineHighlight() {
    // Disabled
}
void FileEditorHelper::TriggerDraw() {
    gtk_widget_queue_draw(GTK_WIDGET(lineNumbers));
    gtk_widget_queue_draw(GTK_WIDGET(marginOverlay));
}
int FileEditorHelper::GetCurrentLineY() { return currentLineY; }
int FileEditorHelper::GetCurrentLineHeight() { return currentLineHeight; }
FileEditor* FileEditorHelper::GetEditor() { return editor; }