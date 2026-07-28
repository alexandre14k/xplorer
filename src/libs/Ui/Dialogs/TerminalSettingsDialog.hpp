// "src/libs/Ui/Dialogs/TerminalSettingsDialog.hpp"
#pragma once
#include "Xplorer.hpp"
#include "libs/Ui/Views/Terminal.hpp"
#include <gtk/gtk.h>
class TerminalSettingsDialog {
    public:
        void Show(GtkWindow* parent, Terminal* term);
};