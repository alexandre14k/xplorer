// "src/libs/Ui/Dialogs/PrintDialog.hpp"
#pragma once
#include "Xplorer.hpp"
#include <gtk/gtk.h>
class PrintDialog {
    public:
        void Show(GtkWindow* parent, String text);
    private:
        String AssembleString(String raw);
};