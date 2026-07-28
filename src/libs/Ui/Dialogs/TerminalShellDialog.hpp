// "src/libs/Ui/Dialogs/TerminalShellDialog.hpp"
#pragma once
#include "Xplorer.hpp"
#include <gtk/gtk.h>
#include <functional>
class TerminalShellDialog {
    public:
        void Show(
            GtkWindow* parent,
            String currentShell,
            std::function<void(String)> onSelected
        );
    private:
        static bool IsExecutable(String path);
};