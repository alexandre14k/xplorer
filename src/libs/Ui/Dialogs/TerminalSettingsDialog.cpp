// "src/libs/Ui/Dialogs/TerminalSettingsDialog.cpp"
#include "TerminalSettingsDialog.hpp"
namespace {
struct TermDialogData {
    Terminal* term;
    GtkDropDown* familyDrop;
    GtkDropDown* sizeDrop;
    GtkColorButton* bgBtn;
    GtkColorButton* fgBtn;
    GtkWindow* window;
};
void OnTermConfirmClicked(GtkButton* btn, gpointer user_data) {
    TermDialogData* data = static_cast<TermDialogData*>(user_data);
    
    const char* family = gtk_string_object_get_string(
        GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(data->familyDrop))
    );
    int size = 12;
    if (gtk_drop_down_get_selected_item(data->sizeDrop)) {
        size = std::stoi(gtk_string_object_get_string(
            GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(data->sizeDrop))
        ));
    }
    
    GdkRGBA bg, fg;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(data->bgBtn), &bg);
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(data->fgBtn), &fg);
#pragma GCC diagnostic pop
    
    data->term->ApplySettings(family, size, &bg, &fg);
    gtk_window_close(data->window);
}
void OnDestroy(GtkWindow* window, gpointer user_data) {
    TermDialogData* data = static_cast<TermDialogData*>(user_data);
    delete data;
}
}
void TerminalSettingsDialog::Show(GtkWindow* parent, Terminal* term) {
    GtkWindow* window = GTK_WINDOW(gtk_window_new());
    gtk_window_set_title(window, "Terminal Settings");
    gtk_window_set_transient_for(window, parent);
    gtk_window_set_modal(window, TRUE);
    gtk_window_set_default_size(window, 300, 250);
    GtkBox* box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 10));
    gtk_widget_set_margin_top(GTK_WIDGET(box), 12);
    gtk_widget_set_margin_bottom(GTK_WIDGET(box), 12);
    gtk_widget_set_margin_start(GTK_WIDGET(box), 12);
    gtk_widget_set_margin_end(GTK_WIDGET(box), 12);
    
    GtkWidget* famLabel = gtk_label_new("Font Family:");
    gtk_widget_set_halign(famLabel, GTK_ALIGN_START);
    const char* families[] = {"Monospace", "Sans", "Serif", nullptr};
    GtkWidget* famDrop = gtk_drop_down_new_from_strings(families);
    String currentFam = term->GetFontFamily();
    int famIdx = 0;
    if (currentFam == "Sans") famIdx = 1;
    else if (currentFam == "Serif") famIdx = 2;
    gtk_drop_down_set_selected(GTK_DROP_DOWN(famDrop), famIdx);
    
    GtkWidget* sizeLabel = gtk_label_new("Font Size:");
    gtk_widget_set_halign(sizeLabel, GTK_ALIGN_START);
    const char* sizes[] = {"10", "12", "14", "16", "18", nullptr};
    GtkWidget* sizeDrop = gtk_drop_down_new_from_strings(sizes);
    int currentSize = term->GetFontSize();
    int sizeIdx = 1;
    if (currentSize == 10) sizeIdx = 0;
    else if (currentSize == 14) sizeIdx = 2;
    else if (currentSize == 16) sizeIdx = 3;
    else if (currentSize == 18) sizeIdx = 4;
    gtk_drop_down_set_selected(GTK_DROP_DOWN(sizeDrop), sizeIdx);
    
    GtkWidget* bgLabel = gtk_label_new("Background:");
    gtk_widget_set_halign(bgLabel, GTK_ALIGN_START);
    
    GtkWidget* fgLabel = gtk_label_new("Foreground:");
    gtk_widget_set_halign(fgLabel, GTK_ALIGN_START);
    
    GdkRGBA currentBg = term->GetBgColor();
    GdkRGBA currentFg = term->GetFgColor();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    GtkWidget* bgBtn = gtk_color_button_new_with_rgba(&currentBg);
    GtkWidget* fgBtn = gtk_color_button_new_with_rgba(&currentFg);
#pragma GCC diagnostic pop
    
    GtkWidget* confirmBtn = gtk_button_new_with_label("Confirm");
    TermDialogData* data = new TermDialogData{
        term, GTK_DROP_DOWN(famDrop), GTK_DROP_DOWN(sizeDrop),
        GTK_COLOR_BUTTON(bgBtn), GTK_COLOR_BUTTON(fgBtn), window
    };
    g_signal_connect(confirmBtn, "clicked", G_CALLBACK(OnTermConfirmClicked), data);
    g_signal_connect(window, "destroy", G_CALLBACK(OnDestroy), data);
    
    gtk_box_append(box, famLabel);
    gtk_box_append(box, famDrop);
    gtk_box_append(box, sizeLabel);
    gtk_box_append(box, sizeDrop);
    gtk_box_append(box, bgLabel);
    gtk_box_append(box, bgBtn);
    gtk_box_append(box, fgLabel);
    gtk_box_append(box, fgBtn);
    gtk_box_append(box, confirmBtn);
    
    gtk_window_set_child(window, GTK_WIDGET(box));
    gtk_window_present(window);
}