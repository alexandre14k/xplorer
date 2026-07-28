// "src/libs/Ui/Dialogs/AboutDialog.hpp"
#pragma once
#include "Xplorer.hpp"
#include <gtk/gtk.h>
class AboutDialog {
    public:
        void Show(GtkWindow* parent);
    private:
        String AssembleString(String raw);
};