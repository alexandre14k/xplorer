// "src/libs/Ui/Widgets/SplitView.hpp"
#pragma once
#include "Xplorer.hpp"
#include "libs/Context/Context.hpp"
#include <gtk/gtk.h>
class SplitView {
    public:
        SplitView(Context* ctx, GtkOrientation orient);
        void Initialize();
        GtkWidget* GetWidget();
        void SetPosition(int pos);
        int GetPosition();
    private:
        Context* ctx;
        GtkPaned* paned;
        GtkOrientation orientation;
        String AssembleString(String raw);
};