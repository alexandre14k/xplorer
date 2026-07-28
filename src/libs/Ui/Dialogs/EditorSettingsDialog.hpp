// "src/libs/Ui/Dialogs/EditorSettingsDialog.hpp"
#pragma once
#include "Xplorer.hpp"
#include "libs/Context/Context.hpp"
#include <gtk/gtk.h>
#include <functional>
class EditorSettingsDialog {
    public:
        void Show(
            GtkWindow* parent, Context* ctx, 
            std::function<void()> onConfirm
        );
};