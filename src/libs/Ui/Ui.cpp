// "src/libs/Ui/Ui.cpp"
#include "Ui.hpp"
#include "libs/Ui/helper/UiFileHelper.hpp"
namespace {
void OnOpenResult(GObject* src, GAsyncResult* res, gpointer d) {
    GFile* file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(src), res, nullptr);
    if (file) {
        char* path = g_file_get_path(file);
        static_cast<Ui*>(d)->LoadFile(path);
        g_free(path);
        g_object_unref(file);
    }
}
void OnSaveResult(GObject* src, GAsyncResult* res, gpointer d) {
    GFile* file = gtk_file_dialog_save_finish(GTK_FILE_DIALOG(src), res, nullptr);
    if (file) {
        char* path = g_file_get_path(file);
        static_cast<Ui*>(d)->SaveFileAs(path);
        g_free(path);
        g_object_unref(file);
    }
}
void OnPlusClickedTerm(GtkButton* btn, gpointer d) {
    static_cast<Ui*>(d)->AddTerminalTab();
}
void OnPlusClickedBrowser(GtkButton* btn, gpointer d) {
    static_cast<Ui*>(d)->AddBrowserTab();
}
}
Ui::Ui(Context* context) : ctx(context), app(nullptr), window(nullptr), 
    rootBox(nullptr), menuBar(nullptr), hSplit(nullptr), vSplit(nullptr),
    topLeftTabs(nullptr), topRightTabs(nullptr), bottomTabs(nullptr), 
    statusBar(nullptr), actions(new Actions(ctx, this)) {}
void Ui::ApplyConstraints() {
    gtk_widget_set_size_request(GTK_WIDGET(window), 400, 300);
    if (ctx->GetFullscreen()) gtk_window_maximize(window);
}
void Ui::BuildLayout() {
    menuBar = new MenuBar();
    menuBar->Initialize();
    gtk_box_append(rootBox, menuBar->GetWidget());
    
    hSplit = new SplitView(ctx, GTK_ORIENTATION_HORIZONTAL);
    hSplit->Initialize();
    gtk_widget_set_vexpand(hSplit->GetWidget(), TRUE);
    gtk_widget_set_hexpand(hSplit->GetWidget(), TRUE);
    
    vSplit = new SplitView(ctx, GTK_ORIENTATION_VERTICAL);
    vSplit->Initialize();
    gtk_widget_set_vexpand(vSplit->GetWidget(), TRUE);
    gtk_widget_set_hexpand(vSplit->GetWidget(), TRUE);
    
    topLeftTabs = new TabManager();
    topLeftTabs->Initialize();
    topRightTabs = new TabManager();
    topRightTabs->Initialize();
    bottomTabs = new TabManager();
    bottomTabs->Initialize();
    
    GtkWidget* plusBtnTerm = gtk_button_new_from_icon_name("list-add-symbolic");
    gtk_button_set_has_frame(GTK_BUTTON(plusBtnTerm), FALSE);
    gtk_widget_set_tooltip_text(plusBtnTerm, "New Terminal");
    g_signal_connect(plusBtnTerm, "clicked", G_CALLBACK(OnPlusClickedTerm), this);
    gtk_notebook_set_action_widget(GTK_NOTEBOOK(bottomTabs->GetWidget()), plusBtnTerm, GTK_PACK_START);
    
    GtkWidget* plusBtnBrowser = gtk_button_new_from_icon_name("list-add-symbolic");
    gtk_button_set_has_frame(GTK_BUTTON(plusBtnBrowser), FALSE);
    gtk_widget_set_tooltip_text(plusBtnBrowser, "New Browser Tab");
    g_signal_connect(plusBtnBrowser, "clicked", G_CALLBACK(OnPlusClickedBrowser), this);
    gtk_notebook_set_action_widget(GTK_NOTEBOOK(topRightTabs->GetWidget()), plusBtnBrowser, GTK_PACK_START);
    
    statusBar = GTK_LABEL(gtk_label_new("Ready"));
    gtk_label_set_xalign(statusBar, 0.0f);
    gtk_widget_set_margin_start(GTK_WIDGET(statusBar), 6);
    gtk_widget_set_margin_top(GTK_WIDGET(statusBar), 2);
    gtk_widget_set_margin_bottom(GTK_WIDGET(statusBar), 2);
    
    AddTerminalTab();
    AddBrowserTab();
    NewEditorTab();
    
    GtkPaned* hPane = GTK_PANED(hSplit->GetWidget());
    if (ctx->GetSwapPanes()) {
        gtk_paned_set_start_child(hPane, topRightTabs->GetWidget());
        gtk_paned_set_end_child(hPane, topLeftTabs->GetWidget());
    } else {
        gtk_paned_set_start_child(hPane, topLeftTabs->GetWidget());
        gtk_paned_set_end_child(hPane, topRightTabs->GetWidget());
    }
    gtk_paned_set_position(hPane, ctx->GetWidth() / 2);
    
    GtkPaned* vPane = GTK_PANED(vSplit->GetWidget());
    gtk_paned_set_start_child(vPane, hSplit->GetWidget());
    gtk_paned_set_end_child(vPane, bottomTabs->GetWidget());
    gtk_paned_set_position(vPane, (ctx->GetHeight() * 2) / 3);
    
    gtk_box_append(rootBox, vSplit->GetWidget());
    gtk_box_append(rootBox, GTK_WIDGET(statusBar));
}
void Ui::AddTerminalTab() {
    Terminal* term = new Terminal(ctx);
    term->Initialize();
    bottomTabs->AppendTerminalTab("Terminal", term);
    int page = gtk_notebook_get_n_pages(GTK_NOTEBOOK(bottomTabs->GetWidget())) - 1;
    gtk_notebook_set_current_page(GTK_NOTEBOOK(bottomTabs->GetWidget()), page);
}
void Ui::AddBrowserTab() {
    Browser* br = new Browser(ctx, statusBar);
    br->Initialize();
    topRightTabs->AppendTab("Browser", br->GetWidget());
    int page = gtk_notebook_get_n_pages(GTK_NOTEBOOK(topRightTabs->GetWidget())) - 1;
    gtk_notebook_set_current_page(GTK_NOTEBOOK(topRightTabs->GetWidget()), page);
}
void Ui::Initialize(GtkApplication* application) {
    app = application;
    window = GTK_WINDOW(gtk_application_window_new(app));
    gtk_window_set_title(window, "Xplorer Editor");
    gtk_window_set_default_size(window, ctx->GetWidth(), ctx->GetHeight());
    rootBox = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
    gtk_window_set_child(window, GTK_WIDGET(rootBox));
    actions->RegisterActions(app);
    BuildLayout();
    ApplyConstraints();
    gtk_window_present(window);
}
void Ui::NewEditorTab() {
    topLeftTabs->AddEditorTab("new file", ctx);
    GtkNotebook* nb = GTK_NOTEBOOK(topLeftTabs->GetWidget());
    int page = gtk_notebook_get_n_pages(nb) - 1;
    gtk_notebook_set_current_page(nb, page);
    
    Terminal* term = bottomTabs->GetCurrentTerminal();
    if (term) term->ChangeDirectory(g_get_home_dir());
    gtk_label_set_text(statusBar, "New file created");
}
void Ui::OpenFile() {
    GtkFileDialog* dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Open File");
    gtk_file_dialog_open(dialog, window, nullptr, OnOpenResult, this);
}
void Ui::LoadFile(String path) {
    String label = UiFileHelper::FormatTabLabel(path);
    FileEditor* ed = topLeftTabs->AddEditorTab(label, ctx);
    ed->SetText(UiFileHelper::ReadFile(path));
    ed->SetFilePath(path);
    
    GtkNotebook* nb = GTK_NOTEBOOK(topLeftTabs->GetWidget());
    int page = gtk_notebook_get_n_pages(nb) - 1;
    gtk_notebook_set_current_page(nb, page);
    gtk_widget_grab_focus(GTK_WIDGET(ed->GetTextView()));
    
    size_t slash = path.find_last_of('/');
    String dir = (slash != String::npos) ? path.substr(0, slash) : path;
    Terminal* term = bottomTabs->GetCurrentTerminal();
    if (term) term->ChangeDirectory(dir);
    gtk_label_set_text(statusBar, ("Opened: " + path).c_str());
}
void Ui::SaveFile(bool saveAs) {
    FileEditor* ed = topLeftTabs->GetCurrentEditor();
    if (!ed) return;
    String path = ed->GetFilePath();
    if (path.empty() || saveAs) {
        GtkFileDialog* dialog = gtk_file_dialog_new();
        gtk_file_dialog_set_title(dialog, "Save File");
        gtk_file_dialog_save(dialog, window, nullptr, OnSaveResult, this);
        return;
    }
    SaveFileAs(path);
}
void Ui::SaveFileAs(String path) {
    FileEditor* ed = topLeftTabs->GetCurrentEditor();
    if (!ed) return;
    if (UiFileHelper::WriteFile(path, ed->GetText())) {
        ed->SetFilePath(path);
        topLeftTabs->RenameCurrentTab(UiFileHelper::FormatTabLabel(path));
        gtk_label_set_text(statusBar, ("Saved: " + path).c_str());
    }
}
String Ui::GetCurrentEditorText() {
    FileEditor* ed = topLeftTabs->GetCurrentEditor();
    return ed ? ed->GetText() : "";
}
void Ui::UpdateEditorWrap() {
    FileEditor* ed = topLeftTabs->GetCurrentEditor();
    if (ed) {
        ed->UpdateWrapState();
        gtk_label_set_text(statusBar, ctx->GetWrapEnabled() ? "Wrap enabled" : "Wrap disabled");
    }
}
void Ui::SwapPanes(bool swapped) {
    GtkPaned* pane = GTK_PANED(hSplit->GetWidget());
    GtkWidget* left = topLeftTabs->GetWidget();
    GtkWidget* right = topRightTabs->GetWidget();
    g_object_ref(left);
    g_object_ref(right);
    gtk_paned_set_start_child(pane, nullptr);
    gtk_paned_set_end_child(pane, nullptr);
    if (swapped) {
        gtk_paned_set_start_child(pane, right);
        gtk_paned_set_end_child(pane, left);
    } else {
        gtk_paned_set_start_child(pane, left);
        gtk_paned_set_end_child(pane, right);
    }
    g_object_unref(left);
    g_object_unref(right);
    gtk_label_set_text(statusBar, "Swapped panes");
}
void Ui::RefreshEditors() {
    FileEditor* ed = topLeftTabs->GetCurrentEditor();
    if (ed) {
        ed->ApplyFont(ctx->GetEditorFontFamily(), ctx->GetEditorFontSize());
        ed->TriggerDraw();
    }
    gtk_label_set_text(statusBar, "Editor settings applied");
}
Terminal* Ui::GetTerminal() { 
    return bottomTabs->GetCurrentTerminal(); 
}