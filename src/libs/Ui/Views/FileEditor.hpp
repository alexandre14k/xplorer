// "src/libs/Ui/Views/FileEditor.hpp"
#pragma once
#include "Xplorer.hpp"
#include "libs/Context/Context.hpp"
#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

class FileEditorHelper;
class FileEditor {
    public:
        FileEditor(Context* ctx);
        ~FileEditor();
        void Initialize();
        GtkWidget* GetWidget();
        void SetText(String text);
        String GetText();
        void UpdateWrapState();
        void SetFilePath(String path);
        String GetFilePath();
        void TriggerDraw();
        GtkTextView* GetTextView();
        Context* GetContext();
        void UpdateLineHighlight();
        void ChangeFontSize(int delta);
        void ApplyFont(String family, int size);
    private:
        Context* ctx;
        GtkBox* container;
        GtkOverlay* overlay;
        GtkSourceView* textView;
        GtkScrolledWindow* scrollWindow;
        GtkDrawingArea* lineNumbers;
        GtkDrawingArea* marginOverlay;
        GtkDrawingArea* lineHighlightOverlay;
        FileEditorHelper* helper;
        String filePath;
        void SetupKeyEvents();
        void SetupScrollEvents();
        void SetupMouseScrollEvents();
};