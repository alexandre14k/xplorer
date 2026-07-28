// "src/libs/Ui/Dialogs/PrintDialog.cpp"
#include "PrintDialog.hpp"
#include "libs/Logger/Logger.hpp"
namespace {
void OnBeginPrint(
    GtkPrintOperation* op, GtkPrintContext* ctx, gpointer user_data
) {
    String* text = static_cast<String*>(user_data);
    int lines = 1;
    for (char c : *text) { if (c == '\n') lines++; }
    gtk_print_operation_set_n_pages(op, (lines + 45) / 46);
}
void OnDrawPage(
    GtkPrintOperation* op, GtkPrintContext* ctx,
    int page_num, gpointer user_data
) {
    String* text = static_cast<String*>(user_data);
    cairo_t* cr = gtk_print_context_get_cairo_context(ctx);
    PangoLayout* layout = gtk_print_context_create_pango_layout(ctx);
    PangoFontDescription* desc = pango_font_description_from_string(
        "monospace 10"
    );
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);
    int start_line = page_num * 46;
    std::vector<String> all_lines;
    String current;
    for (char c : *text) {
        if (c == '\n') { all_lines.push_back(current); current = ""; }
        else { current += c; }
    }
    all_lines.push_back(current);
    String page_text;
    for (int i = start_line; i < start_line + 46 && i < (int)all_lines.size(); i++) {
        page_text += all_lines[i] + "\n";
    }
    pango_layout_set_text(layout, page_text.c_str(), -1);
    cairo_save(cr);
    cairo_move_to(cr, 20, 20);
    pango_cairo_show_layout(cr, layout);
    cairo_restore(cr);
    g_object_unref(layout);
}
}
String PrintDialog::AssembleString(String raw) {
    VectorChar chars;
    for (char c : raw) { chars.push_back(c); }
    return String(chars.begin(), chars.end());
}
void PrintDialog::Show(GtkWindow* parent, String text) {
    GtkPrintOperation* op = gtk_print_operation_new();
    gtk_print_operation_set_print_settings(op,
        gtk_print_settings_new()
    );
    g_signal_connect(op, "begin-print", G_CALLBACK(OnBeginPrint), &text);
    g_signal_connect(op, "draw-page", G_CALLBACK(OnDrawPage), &text);
    GtkPrintOperationResult res = gtk_print_operation_run(
        op, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, parent, nullptr
    );
    if (res == GTK_PRINT_OPERATION_RESULT_ERROR) {
        Logger::Log(AssembleString("Print failed"));
    }
    g_object_unref(op);
}