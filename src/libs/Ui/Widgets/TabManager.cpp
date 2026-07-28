// "src/libs/Ui/Widgets/TabManager.cpp"
#include "TabManager.hpp"
#include "libs/Ui/Views/FileEditor.hpp"
#include "libs/Ui/Views/Terminal.hpp"
#include "libs/Context/Context.hpp"

namespace {
struct RenameData {
    TabManager* tm;
    GtkWindow* dialog;
    GtkEntry* entry;
    int page;
};

void OnRenameConfirmed(GtkButton* btn, gpointer user_data) {
    RenameData* rd = static_cast<RenameData*>(user_data);
    const char* text = gtk_editable_get_text(GTK_EDITABLE(rd->entry));
    rd->tm->RenameTab(rd->page, String(text));
    gtk_window_close(rd->dialog);
    delete rd;
}
}

TabManager::TabManager() { notebook = nullptr; }

void TabManager::Initialize() {
    notebook = GTK_NOTEBOOK(gtk_notebook_new());
    gtk_notebook_set_scrollable(notebook, TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(notebook), TRUE);
    gtk_widget_set_hexpand(GTK_WIDGET(notebook), TRUE);
}

GtkWidget* TabManager::GetWidget() { return GTK_WIDGET(notebook); }

void TabManager::OnCloseClicked(GtkWidget* btn, gpointer user_data) {
    TabManager* tm = static_cast<TabManager*>(user_data);
    GtkWidget* box = gtk_widget_get_parent(btn);
    GtkWidget* child = static_cast<GtkWidget*>(g_object_get_data(G_OBJECT(box), "child"));
    int page = gtk_notebook_page_num(tm->notebook, child);
    if (page != -1) tm->CloseTab(page);
}

void TabManager::OnTabDoubleClicked(GtkGesture* gesture, int n_press, double x, double y, gpointer user_data) {
    if (n_press == 2) {
        TabManager* tm = static_cast<TabManager*>(user_data);
        GtkWidget* box = gtk_event_controller_get_widget(GTK_EVENT_CONTROLLER(gesture));
        GtkWidget* child = static_cast<GtkWidget*>(g_object_get_data(G_OBJECT(box), "child"));
        int page = gtk_notebook_page_num(tm->notebook, child);
        if (page != -1) tm->ShowRenameDialog(page);
    }
}

GtkWidget* TabManager::CreateTabLabel(String title, GtkWidget* child) {
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    GtkWidget* label = gtk_label_new(title.c_str());
    GtkWidget* btn = gtk_button_new_from_icon_name("window-close-symbolic");
    gtk_widget_set_tooltip_text(btn, "Close tab");
    gtk_button_set_has_frame(GTK_BUTTON(btn), FALSE);
    
    g_object_set_data(G_OBJECT(box), "child", child);
    
    g_signal_connect(btn, "clicked", G_CALLBACK(OnCloseClicked), this);
    
    GtkGesture* click = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(click), GDK_BUTTON_PRIMARY);
    g_signal_connect(click, "released", G_CALLBACK(OnTabDoubleClicked), this);
    gtk_widget_add_controller(box, GTK_EVENT_CONTROLLER(click));
    
    gtk_box_append(GTK_BOX(box), label);
    gtk_box_append(GTK_BOX(box), btn);
    return box;
}

FileEditor* TabManager::AddEditorTab(String title, Context* ctx) {
    FileEditor* ed = new FileEditor(ctx);
    ed->Initialize();
    GtkWidget* child = ed->GetWidget();
    GtkWidget* label_box = CreateTabLabel(title, child);
    gtk_notebook_append_page(notebook, child, label_box);
    return ed;
}

void TabManager::AppendTab(String title, GtkWidget* widget) {
    GtkWidget* label_box = CreateTabLabel(title, widget);
    gtk_notebook_append_page(notebook, widget, label_box);
}

void TabManager::AppendTerminalTab(String title, Terminal* term) {
    GtkWidget* child = term->GetWidget();
    GtkWidget* label_box = CreateTabLabel(title, child);
    gtk_notebook_append_page(notebook, child, label_box);
}

FileEditor* TabManager::GetCurrentEditor() {
    int page = gtk_notebook_get_current_page(notebook);
    if (page < 0) return nullptr;
    GtkWidget* child = gtk_notebook_get_nth_page(notebook, page);
    return static_cast<FileEditor*>(g_object_get_data(G_OBJECT(child), "editor_obj"));
}

Terminal* TabManager::GetCurrentTerminal() {
    int page = gtk_notebook_get_current_page(notebook);
    if (page < 0) return nullptr;
    GtkWidget* child = gtk_notebook_get_nth_page(notebook, page);
    return static_cast<Terminal*>(g_object_get_data(G_OBJECT(child), "terminal_obj"));
}

void TabManager::RenameCurrentTab(String title) {
    int page = gtk_notebook_get_current_page(notebook);
    RenameTab(page, title);
}

void TabManager::RenameTab(int page, String newTitle) {
    if (page < 0) return;
    GtkWidget* child = gtk_notebook_get_nth_page(notebook, page);
    GtkWidget* box = gtk_notebook_get_tab_label(notebook, child);
    GtkWidget* l = gtk_widget_get_first_child(box);
    while (l) {
        if (GTK_IS_LABEL(l)) {
            gtk_label_set_text(GTK_LABEL(l), newTitle.c_str());
            break;
        }
        l = gtk_widget_get_next_sibling(l);
    }
}

void TabManager::RemoveCurrentTab() {
    int page = gtk_notebook_get_current_page(notebook);
    CloseTab(page);
}

void TabManager::CloseTab(int page) {
    if (page < 0) return;
    if (gtk_notebook_get_n_pages(notebook) <= 1) return; // Prevent closing last tab
    gtk_notebook_remove_page(notebook, page);
}

void TabManager::ShowRenameDialog(int page) {
    if (page < 0) return;
    GtkWidget* child = gtk_notebook_get_nth_page(notebook, page);
    GtkWidget* box = gtk_notebook_get_tab_label(notebook, child);
    GtkWidget* label_widget = gtk_widget_get_first_child(box);
    String currentTitle = "";
    if (GTK_IS_LABEL(label_widget)) {
        currentTitle = gtk_label_get_text(GTK_LABEL(label_widget));
    }
    
    GtkWindow* parent = GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(notebook)));
    GtkWindow* dialog = GTK_WINDOW(gtk_window_new());
    gtk_window_set_title(dialog, "Rename Tab");
    gtk_window_set_transient_for(dialog, parent);
    gtk_window_set_modal(dialog, TRUE);
    gtk_window_set_default_size(dialog, 300, 100);
    
    GtkBox* dialogBox = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 10));
    gtk_widget_set_margin_top(GTK_WIDGET(dialogBox), 12);
    gtk_widget_set_margin_bottom(GTK_WIDGET(dialogBox), 12);
    gtk_widget_set_margin_start(GTK_WIDGET(dialogBox), 12);
    gtk_widget_set_margin_end(GTK_WIDGET(dialogBox), 12);
    
    GtkWidget* entry = gtk_entry_new();
    gtk_editable_set_text(GTK_EDITABLE(entry), currentTitle.c_str());
    
    GtkWidget* btn = gtk_button_new_with_label("Rename");
    
    RenameData* data = new RenameData{this, dialog, GTK_ENTRY(entry), page};
    g_signal_connect(btn, "clicked", G_CALLBACK(OnRenameConfirmed), data);
    
    gtk_box_append(dialogBox, entry);
    gtk_box_append(dialogBox, btn);
    gtk_window_set_child(dialog, GTK_WIDGET(dialogBox));
    gtk_window_present(dialog);
}