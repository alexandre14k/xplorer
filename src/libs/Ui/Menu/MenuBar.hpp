// "src/libs/Ui/Menu/MenuBar.hpp"
#pragma once
#include "Xplorer.hpp"
#include <gtk/gtk.h>
class MenuBar {
    public:
        MenuBar();
        void Initialize();
        GtkWidget* GetWidget();
    private:
        GtkWidget* menuBar;
        GMenu* BuildFileMenu();
        GMenu* BuildToolsMenu();
        GMenu* BuildHelpMenu();
};