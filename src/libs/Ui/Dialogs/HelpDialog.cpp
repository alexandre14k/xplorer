// "src/libs/Ui/Dialogs/HelpDialog.cpp"
#include "HelpDialog.hpp"
String HelpDialog::AssembleString(String raw) {
    VectorChar chars;
    for (char c : raw) { chars.push_back(c); }
    return String(chars.begin(), chars.end());
}
void HelpDialog::Show(GtkWindow* parent) {
    GtkWindow* window = GTK_WINDOW(gtk_window_new());
    gtk_window_set_title(window, "Help Tutorial");
    gtk_window_set_transient_for(window, parent);
    gtk_window_set_modal(window, TRUE);
    gtk_window_set_default_size(window, 600, 300);
    GtkBox* box = GTK_BOX(gtk_box_new(
        GTK_ORIENTATION_VERTICAL, 10
    ));
    gtk_widget_set_margin_top(GTK_WIDGET(box), 4);
    gtk_widget_set_margin_bottom(GTK_WIDGET(box), 4);
    gtk_widget_set_margin_start(GTK_WIDGET(box), 4);
    gtk_widget_set_margin_end(GTK_WIDGET(box), 4);
    String text = AssembleString(
        "Xplorer Help:\n"
        "- Follows gtk3 theme colors\n"
        "- Top Menu > \n" 
        "  - File > \n"
        "    - open, edit and print files\n"
        "  - Tools > \n"
        "    - wrap editor content\n"
        "    - swap editor and browser panels\n"
        "    - terminal settings panel colors, font, size\n"
        "    - editor settings panel delimiter, font, size\n"
        "  - Help > \n"
        "    - about program, author and license\n"
        "    - help show this guide\n"
        "- Left editor panel >\n"
        "  - shift focus on open file\n"
        "  - wrap text and color syntax\n"
        "  - closable and renameable tabs\n"
        "- Right browser panel >\n"
        "  - add new tabs\n"
        "  - closable and renameable tabs\n"
        "  - home button about:blank\n"
        "  - refresh load address content\n"
        "  - editable address bar\n"
        "  - cookies button clears session cookies\n"
        "  - dropdown button access usefull websites\n"
        "  - supports javascript\n"
        "  - bottom url inspector\n"
        "- Bottom terminal panel >\n"
        "  - add new tabs\n"
        "  - closable and renameable tabs\n"
        "- Bottom common status bar"
    );
    GtkWidget* label = gtk_label_new(text.c_str());
    gtk_label_set_wrap(GTK_LABEL(label), TRUE);
    gtk_box_append(box, label);
    gtk_window_set_child(window, GTK_WIDGET(box));
    gtk_window_present(window);
}