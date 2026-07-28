// "src/libs/Ui/Ui.hpp"
#pragma once
#include "Xplorer.hpp"
#include "libs/Context/Context.hpp"
#include "libs/Actions/Actions.hpp"
#include "libs/Ui/Menu/MenuBar.hpp"
#include "libs/Ui/Widgets/SplitView.hpp"
#include "libs/Ui/Widgets/TabManager.hpp"
#include "libs/Ui/Views/FileEditor.hpp"
#include "libs/Ui/Views/Terminal.hpp"
#include "libs/Ui/Views/Browser.hpp"
#include <gtk/gtk.h>
class Ui {
    public:
        Ui(Context* ctx);
        void Initialize(GtkApplication* app);
        void NewEditorTab();
        void OpenFile();
        void SaveFile(bool saveAs);
        void LoadFile(String path);
        void SaveFileAs(String path);
        String GetCurrentEditorText();
        void UpdateEditorWrap();
        void SwapPanes(bool swapped);
        void RefreshEditors();
        Terminal* GetTerminal();
        void AddTerminalTab();
        void AddBrowserTab();
    private:
        Context* ctx;
        Actions* actions;
        MenuBar* menuBar;
        SplitView* hSplit;
        SplitView* vSplit;
        TabManager* topLeftTabs;
        TabManager* topRightTabs;
        TabManager* bottomTabs;
        GtkApplication* app;
        GtkWindow* window;
        GtkBox* rootBox;
        GtkLabel* statusBar;
        void ApplyConstraints();
        void BuildLayout();
};