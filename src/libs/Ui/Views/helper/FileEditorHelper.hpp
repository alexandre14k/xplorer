// "src/libs/Ui/Views/helper/FileEditorHelper.hpp"
#pragma once
#include "Xplorer.hpp"
#include "libs/Context/Context.hpp"
#include <gtk/gtk.h>
class FileEditor;
class FileEditorHelper {
    public:
        FileEditorHelper(FileEditor* ed);
        ~FileEditorHelper();
        void Initialize(
            GtkDrawingArea* lines, GtkDrawingArea* overlay, 
            GtkDrawingArea* highlight
        );
        void ChangeFontSize(int delta);
        void ApplyFont(String family, int size);
        void UpdateLineHighlight();
        void TriggerDraw();
        int GetCurrentLineY();
        int GetCurrentLineHeight();
        FileEditor* GetEditor();
    private:
        FileEditor* editor;
        GtkDrawingArea* lineNumbers;
        GtkDrawingArea* marginOverlay;
        GtkDrawingArea* lineHighlightOverlay;
        GtkCssProvider* cssProvider;
        int currentLineY;
        int currentLineHeight;
        int fontSize;
        String fontFamily;
};