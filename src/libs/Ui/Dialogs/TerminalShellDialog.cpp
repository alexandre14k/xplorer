// "src/libs/Ui/Dialogs/TerminalShellDialog.cpp"
#include "TerminalShellDialog.hpp"
namespace {
struct SelectionContext {
    GtkWindow* window;
    std::function<void(String)> onSelected;
    String path;
};
void OnShellChosen(GtkButton*, gpointer user_data) {
    SelectionContext* selection =
        static_cast<SelectionContext*>(user_data);
    selection->onSelected(selection->path);
    gtk_window_close(selection->window);
}
void OnSelectionDataFreed(gpointer data, GClosure*) {
    delete static_cast<SelectionContext*>(data);
}
void OnCancelChosen(GtkButton*, gpointer user_data) {
    gtk_window_close(GTK_WINDOW(user_data));
}
}
bool TerminalShellDialog::IsExecutable(String path) {
    return g_file_test(
        path.c_str(),
        static_cast<GFileTest>(
            G_FILE_TEST_IS_EXECUTABLE | G_FILE_TEST_EXISTS
        )
    );
}
void TerminalShellDialog::Show(
    GtkWindow* parent,
    String currentShell,
    std::function<void(String)> onSelected
) {
    String candidates[] = {
        "/bin/bash", "/bin/zsh", "/bin/fish",
        "/bin/dash", "/bin/sh",
        "/usr/bin/bash", "/usr/bin/zsh", "/usr/bin/fish"
    };
    GtkWindow* window = GTK_WINDOW(gtk_window_new());
    gtk_window_set_title(window, "Select Shell");
    gtk_window_set_transient_for(window, parent);
    gtk_window_set_modal(window, TRUE);
    gtk_window_set_default_size(window, 320, 240);
    GtkBox* content = GTK_BOX(gtk_box_new(
        GTK_ORIENTATION_VERTICAL, 6
    ));
    gtk_widget_set_margin_top(GTK_WIDGET(content), 12);
    gtk_widget_set_margin_bottom(GTK_WIDGET(content), 12);
    gtk_widget_set_margin_start(GTK_WIDGET(content), 12);
    gtk_widget_set_margin_end(GTK_WIDGET(content), 12);
    bool foundAny = false;
    for (String candidate : candidates) {
        if (!IsExecutable(candidate)) { continue; }
        foundAny = true;
        String label = candidate;
        if (candidate == currentShell) {
            label = candidate + "  (current)";
        }
        GtkWidget* row = gtk_button_new_with_label(
            label.c_str()
        );
        SelectionContext* selection = new SelectionContext{
            window, onSelected, candidate
        };
        g_signal_connect_data(
            row, "clicked",
            G_CALLBACK(OnShellChosen), selection,
            OnSelectionDataFreed,
            static_cast<GConnectFlags>(0)
        );
        gtk_box_append(content, row);
    }
    if (!foundAny) {
        GtkWidget* emptyLabel = gtk_label_new(
            "No shells detected."
        );
        gtk_box_append(content, emptyLabel);
    }
    GtkWidget* cancelBtn = gtk_button_new_with_label("Cancel");
    gtk_widget_set_halign(cancelBtn, GTK_ALIGN_END);
    g_signal_connect(
        cancelBtn, "clicked",
        G_CALLBACK(OnCancelChosen), window
    );
    gtk_box_append(content, cancelBtn);
    gtk_window_set_child(window, GTK_WIDGET(content));
    gtk_window_present(window);
}