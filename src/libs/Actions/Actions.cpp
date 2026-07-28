// "src/libs/Actions/Actions.cpp"
#include "Actions.hpp"
#include "libs/Ui/Ui.hpp"
#include "libs/Actions/helper/ActionsRegistrar.hpp"
void Actions::OnActivateNew(GSimpleAction*, GVariant*, gpointer d) { static_cast<Actions*>(d)->New(); }
void Actions::OnActivateOpen(GSimpleAction*, GVariant*, gpointer d) { static_cast<Actions*>(d)->Open(); }
void Actions::OnActivateSave(GSimpleAction*, GVariant*, gpointer d) { static_cast<Actions*>(d)->Save(); }
void Actions::OnActivateSaveAs(GSimpleAction*, GVariant*, gpointer d) { static_cast<Actions*>(d)->SaveAs(); }
void Actions::OnActivatePrint(GSimpleAction*, GVariant*, gpointer d) { static_cast<Actions*>(d)->Print(); }
void Actions::OnActivateExit(GSimpleAction*, GVariant*, gpointer d) { static_cast<Actions*>(d)->Exit(); }
void Actions::OnActivateAbout(GSimpleAction*, GVariant*, gpointer d) { static_cast<Actions*>(d)->About(); }
void Actions::OnActivateHelp(GSimpleAction*, GVariant*, gpointer d) { static_cast<Actions*>(d)->Help(); }
void Actions::OnChangeStateWrap(GSimpleAction* a, GVariant* v, gpointer d) {
    Actions* self = static_cast<Actions*>(d);
    self->ChangeWrapState(g_variant_get_boolean(v));
    g_simple_action_set_state(a, v);
}
void Actions::OnChangeStateSwap(GSimpleAction* a, GVariant* v, gpointer d) {
    Actions* self = static_cast<Actions*>(d);
    self->ChangeSwapState(g_variant_get_boolean(v));
    g_simple_action_set_state(a, v);
}
void Actions::OnActivateTermSettings(GSimpleAction*, GVariant*, gpointer d) { static_cast<Actions*>(d)->TerminalSettings(); }
void Actions::OnActivateEditSettings(GSimpleAction*, GVariant*, gpointer d) { static_cast<Actions*>(d)->EditorSettings(); }
Actions::Actions(Context* context, Ui* uiPtr) {
    ctx = context;
    ui = uiPtr;
    app = nullptr;
}
GtkWindow* Actions::GetParentWindow() {
    if (app == nullptr) { return nullptr; }
    return gtk_application_get_active_window(app);
}
void Actions::RegisterActions(GtkApplication* application) {
    app = application;
    ActionsRegistrar::Register(app, this, ctx);
}
void Actions::New() { ui->NewEditorTab(); }
void Actions::Open() { ui->OpenFile(); }
void Actions::Save() { ui->SaveFile(false); }
void Actions::SaveAs() { ui->SaveFile(true); }
void Actions::Print() {
    String text = ui->GetCurrentEditorText();
    if (!text.empty()) {
        GtkWindow* parent = GetParentWindow();
        if (parent != nullptr) { printDialog.Show(parent, text); }
    }
}
void Actions::Exit() {
    if (app != nullptr) { g_application_quit(G_APPLICATION(app)); }
}
void Actions::About() {
    GtkWindow* parent = GetParentWindow();
    if (parent != nullptr) { aboutDialog.Show(parent); }
}
void Actions::Help() {
    GtkWindow* parent = GetParentWindow();
    if (parent != nullptr) { helpDialog.Show(parent); }
}
void Actions::ChangeWrapState(bool state) {
    ctx->SetWrapEnabled(state);
    ui->UpdateEditorWrap();
}
void Actions::ChangeSwapState(bool state) {
    ctx->SetSwapPanes(state);
    ui->SwapPanes(state);
}
void Actions::TerminalSettings() {
    GtkWindow* parent = GetParentWindow();
    if (parent != nullptr) { terminalSettingsDialog.Show(parent, ui->GetTerminal()); }
}
void Actions::EditorSettings() {
    GtkWindow* parent = GetParentWindow();
    if (parent != nullptr) { 
        editorSettingsDialog.Show(parent, ctx, [this]() { ui->RefreshEditors(); });
    }
}