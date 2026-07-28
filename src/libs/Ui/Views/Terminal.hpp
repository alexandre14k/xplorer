// "src/libs/Ui/Views/Terminal.hpp"
#pragma once
#include "Xplorer.hpp"
#include "libs/Context/Context.hpp"
#include <gtk/gtk.h>
#include <vte/vte.h>
class Terminal {
    public:
        Terminal(Context* ctx);
        void Initialize();
        GtkWidget* GetWidget();
        void ApplySettings(
            String fontFamily, int fontSize, 
            GdkRGBA* bg, GdkRGBA* fg
        );
        void ChangeDirectory(String dir);
        void ChangeFontSize(int delta);
        String GetFontFamily();
        int GetFontSize();
        GdkRGBA GetBgColor();
        GdkRGBA GetFgColor();
    private:
        Context* ctx;
        GtkBox* container;
        VteTerminal* vteTerm;
        GtkScrolledWindow* termScroll;
        String shellPath;
        String fontFamily;
        int fontSize;
        GdkRGBA bgColor;
        GdkRGBA fgColor;
        GdkRGBA cursorColor;
        void SpawnShell();
        void SetupMouseScrollEvents();
};