// "src/libs/Actions/helper/ActionsRegistrar.cpp"
#include "ActionsRegistrar.hpp"
#include "libs/Actions/Actions.hpp"
GSimpleAction* ActionsRegistrar::AddStateless(
    GtkApplication* app, Actions* actions, 
    String name, GCallback callback
) {
    GSimpleAction* action = g_simple_action_new(name.c_str(), nullptr);
    g_signal_connect(action, "activate", callback, actions);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(action));
    return action;
}
GSimpleAction* ActionsRegistrar::AddToggle(
    GtkApplication* app, Actions* actions, 
    String name, gboolean initial, GCallback callback
) {
    GSimpleAction* action = g_simple_action_new_stateful(
        name.c_str(), nullptr, g_variant_new_boolean(initial)
    );
    g_signal_connect(action, "change-state", callback, actions);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(action));
    return action;
}
void ActionsRegistrar::Register(
    GtkApplication* app, Actions* actions, Context* ctx
) {
    AddStateless(app, actions, "new", G_CALLBACK(Actions::OnActivateNew));
    AddStateless(app, actions, "open", G_CALLBACK(Actions::OnActivateOpen));
    AddStateless(app, actions, "save", G_CALLBACK(Actions::OnActivateSave));
    AddStateless(app, actions, "save_as", G_CALLBACK(Actions::OnActivateSaveAs));
    AddStateless(app, actions, "print", G_CALLBACK(Actions::OnActivatePrint));
    AddStateless(app, actions, "exit", G_CALLBACK(Actions::OnActivateExit));
    AddStateless(app, actions, "about", G_CALLBACK(Actions::OnActivateAbout));
    AddStateless(app, actions, "help", G_CALLBACK(Actions::OnActivateHelp));
    AddToggle(app, actions, "wrap", ctx->GetWrapEnabled(), G_CALLBACK(Actions::OnChangeStateWrap));
    AddToggle(app, actions, "swap", ctx->GetSwapPanes(), G_CALLBACK(Actions::OnChangeStateSwap));
    AddStateless(app, actions, "term_settings", G_CALLBACK(Actions::OnActivateTermSettings));
    AddStateless(app, actions, "edit_settings", G_CALLBACK(Actions::OnActivateEditSettings));
}