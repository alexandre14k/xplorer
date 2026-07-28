// "src/libs/Actions/Actions.hpp"
#pragma once
#include "Xplorer.hpp"
#include "libs/Context/Context.hpp"
#include "libs/Ui/Dialogs/AboutDialog.hpp"
#include "libs/Ui/Dialogs/HelpDialog.hpp"
#include "libs/Ui/Dialogs/PrintDialog.hpp"
#include "libs/Ui/Dialogs/EditorSettingsDialog.hpp"
#include "libs/Ui/Dialogs/TerminalSettingsDialog.hpp"
#include <gtk/gtk.h>
class Ui; 
class Actions {
    public:
        Actions(Context* ctx, Ui* ui);
        void New();
        void Open();
        void Save();
        void SaveAs();
        void Print();
        void Exit();
        void About();
        void Help();
        void ChangeWrapState(bool state);
        void ChangeSwapState(bool state);
        void TerminalSettings();
        void EditorSettings();
        void RegisterActions(GtkApplication* application);
        static void OnActivateNew(GSimpleAction*, GVariant*, gpointer);
        static void OnActivateOpen(GSimpleAction*, GVariant*, gpointer);
        static void OnActivateSave(GSimpleAction*, GVariant*, gpointer);
        static void OnActivateSaveAs(GSimpleAction*, GVariant*, gpointer);
        static void OnActivatePrint(GSimpleAction*, GVariant*, gpointer);
        static void OnActivateExit(GSimpleAction*, GVariant*, gpointer);
        static void OnActivateAbout(GSimpleAction*, GVariant*, gpointer);
        static void OnActivateHelp(GSimpleAction*, GVariant*, gpointer);
        static void OnChangeStateWrap(GSimpleAction*, GVariant*, gpointer);
        static void OnChangeStateSwap(GSimpleAction*, GVariant*, gpointer);
        static void OnActivateTermSettings(GSimpleAction*, GVariant*, gpointer);
        static void OnActivateEditSettings(GSimpleAction*, GVariant*, gpointer);
    private:
        Context* ctx;
        Ui* ui;
        GtkApplication* app;
        AboutDialog aboutDialog;
        HelpDialog helpDialog;
        PrintDialog printDialog;
        EditorSettingsDialog editorSettingsDialog;
        TerminalSettingsDialog terminalSettingsDialog;
        GtkWindow* GetParentWindow();
};