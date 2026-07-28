// "src/libs/Actions/helper/ActionsRegistrar.hpp"
#pragma once
#include "Xplorer.hpp"
#include <gtk/gtk.h>
class Actions;
class Context;
class ActionsRegistrar {
    public:
        static void Register(
            GtkApplication* app, Actions* actions, Context* ctx
        );
    private:
        static GSimpleAction* AddStateless(
            GtkApplication* app, Actions* actions, 
            String name, GCallback callback
        );
        static GSimpleAction* AddToggle(
            GtkApplication* app, Actions* actions, 
            String name, gboolean initial, GCallback callback
        );
};