// "src/libs/Ui/Widgets/TabManager.hpp"
#pragma once
#include "Xplorer.hpp"
#include <gtk/gtk.h>
class FileEditor;
class Terminal;
class Context;
class TabManager {
    public:
        TabManager();
        void Initialize();
        GtkWidget* GetWidget();
        FileEditor* AddEditorTab(String title, Context* ctx);
        FileEditor* GetCurrentEditor();
        Terminal* GetCurrentTerminal();
        void RenameCurrentTab(String title);
        void RemoveCurrentTab();
        void AppendTab(String title, GtkWidget* widget);
        void AppendTerminalTab(String title, Terminal* term);
        void CloseTab(int page);
        void RenameTab(int page, String newTitle);
        void ShowRenameDialog(int page);
    private:
        GtkNotebook* notebook;
        GtkWidget* CreateTabLabel(String title, GtkWidget* child);
        static void OnCloseClicked(GtkWidget* btn, gpointer user_data);
        static void OnTabDoubleClicked(GtkGesture* gesture, int n_press, double x, double y, gpointer user_data);
};