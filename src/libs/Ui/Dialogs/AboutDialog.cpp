// "src/libs/Ui/Dialogs/AboutDialog.cpp"
#include "AboutDialog.hpp"
String AboutDialog::AssembleString(String raw) {
    VectorChar chars;
    for (char c : raw) { chars.push_back(c); }
    return String(chars.begin(), chars.end());
}
void AboutDialog::Show(GtkWindow* parent) {
    GtkWindow* window = GTK_WINDOW(gtk_window_new());
    gtk_window_set_title(window, "About Xplorer");
    gtk_window_set_transient_for(window, parent);
    gtk_window_set_modal(window, TRUE);
    gtk_window_set_default_size(window, 400, 250);

    GtkBox* box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 10));
    gtk_widget_set_margin_top(GTK_WIDGET(box), 4);
    gtk_widget_set_margin_bottom(GTK_WIDGET(box), 4);
    gtk_widget_set_margin_start(GTK_WIDGET(box), 4);
    gtk_widget_set_margin_end(GTK_WIDGET(box), 4);
    gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_CENTER);

    // Load embedded image from GResource
    GtkWidget* picture = gtk_picture_new_for_resource("/local/alexandre14/xplorer/xplorer.png");
    gtk_widget_set_size_request(picture, 300, 100);
    gtk_picture_set_content_fit(GTK_PICTURE(picture), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_halign(picture, GTK_ALIGN_CENTER);
    gtk_box_append(box, picture);

    String text = AssembleString(
        "Xplorer v1.0.0\n"
        "Tab-based code editor\n"
        "Edit Browse and Debug\n\n"
        "License: AGPL-3.0+\n"
        "Designed by Alexandre Raduly\n"
        "contact: https://github.com/alexandre14k\n\n"
        "Built with opensource tools\n"
        "gcc version 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04.1)\n"
        "xmake v3.0.9+20260702\n"
        "gui support with gtk4\n"
        "terminal support with libvte-2.91-gtk4-dev\n"
        "web-browser support with libwebkitgtk-6.0-dev\n"
        "syntax color with libgtksourceview-5-dev\n\n"
        "Made in France 2026"
    );
    GtkWidget* label = gtk_label_new(text.c_str());
    gtk_label_set_wrap(GTK_LABEL(label), TRUE);
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_box_append(box, label);

    gtk_window_set_child(window, GTK_WIDGET(box));
    gtk_window_present(window);
}