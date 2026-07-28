// "src/libs/Ui/Dialogs/EditorSettingsDialog.cpp"
#include "EditorSettingsDialog.hpp"
namespace {
struct DialogData {
    Context* ctx;
    GtkSpinButton* spin;
    GtkDropDown* familyDrop;
    GtkDropDown* sizeDrop;
    std::function<void()> onConfirm;
    GtkWindow* window;
};
void OnConfirmClicked(GtkButton* btn, gpointer user_data) {
    DialogData* data = static_cast<DialogData*>(user_data);
    data->ctx->SetDelimiterColumn(gtk_spin_button_get_value_as_int(data->spin));
    
    const char* family = gtk_string_object_get_string(
        GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(data->familyDrop))
    );
    data->ctx->SetEditorFontFamily(family);
    
    int size = std::stoi(gtk_string_object_get_string(
        GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(data->sizeDrop))
    ));
    data->ctx->SetEditorFontSize(size);
    
    if (data->onConfirm) data->onConfirm();
    gtk_window_close(data->window);
    delete data;
}
}
void EditorSettingsDialog::Show(GtkWindow* parent, Context* ctx, std::function<void()> onConfirm) {
    GtkWindow* window = GTK_WINDOW(gtk_window_new());
    gtk_window_set_title(window, "Editor Settings");
    gtk_window_set_transient_for(window, parent);
    gtk_window_set_modal(window, TRUE);
    gtk_window_set_default_size(window, 300, 200);
    GtkBox* box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 10));
    gtk_widget_set_margin_top(GTK_WIDGET(box), 12);
    gtk_widget_set_margin_bottom(GTK_WIDGET(box), 12);
    gtk_widget_set_margin_start(GTK_WIDGET(box), 12);
    gtk_widget_set_margin_end(GTK_WIDGET(box), 12);
    
    GtkWidget* famLabel = gtk_label_new("Font Family:");
    gtk_widget_set_halign(famLabel, GTK_ALIGN_START);
    const char* families[] = {"Monospace", "Sans", "Serif", nullptr};
    GtkWidget* famDrop = gtk_drop_down_new_from_strings(families);
    String currentFam = ctx->GetEditorFontFamily();
    int famIdx = 0;
    if (currentFam == "Sans") famIdx = 1;
    else if (currentFam == "Serif") famIdx = 2;
    gtk_drop_down_set_selected(GTK_DROP_DOWN(famDrop), famIdx);
    
    GtkWidget* sizeLabel = gtk_label_new("Font Size:");
    gtk_widget_set_halign(sizeLabel, GTK_ALIGN_START);
    const char* sizes[] = {"10", "12", "14", "16", "18", nullptr};
    GtkWidget* sizeDrop = gtk_drop_down_new_from_strings(sizes);
    int currentSize = ctx->GetEditorFontSize();
    int sizeIdx = 1;
    if (currentSize == 10) sizeIdx = 0;
    else if (currentSize == 14) sizeIdx = 2;
    else if (currentSize == 16) sizeIdx = 3;
    else if (currentSize == 18) sizeIdx = 4;
    gtk_drop_down_set_selected(GTK_DROP_DOWN(sizeDrop), sizeIdx);
    
    GtkWidget* delLabel = gtk_label_new("Delimiter Column:");
    gtk_widget_set_halign(delLabel, GTK_ALIGN_START);
    GtkAdjustment* adj = gtk_adjustment_new(ctx->GetDelimiterColumn(), 1.0, 200.0, 1.0, 5.0, 0.0);
    GtkWidget* spin = gtk_spin_button_new(adj, 1.0, 0);
    
    GtkWidget* confirmBtn = gtk_button_new_with_label("Confirm");
    DialogData* data = new DialogData{
        ctx, GTK_SPIN_BUTTON(spin), GTK_DROP_DOWN(famDrop), GTK_DROP_DOWN(sizeDrop), onConfirm, window
    };
    g_signal_connect(confirmBtn, "clicked", G_CALLBACK(OnConfirmClicked), data);
    
    gtk_box_append(box, famLabel);
    gtk_box_append(box, famDrop);
    gtk_box_append(box, sizeLabel);
    gtk_box_append(box, sizeDrop);
    gtk_box_append(box, delLabel);
    gtk_box_append(box, spin);
    gtk_box_append(box, confirmBtn);
    gtk_window_set_child(window, GTK_WIDGET(box));
    gtk_window_present(window);
}