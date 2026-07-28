// "src/libs/Ui/Dialogs/HelpDialog.hpp"
#pragma once
#include "Xplorer.hpp"
#include <gtk/gtk.h>
class HelpDialog {
    public:
        void Show(GtkWindow* parent);
    private:
        String AssembleString(String raw);
};