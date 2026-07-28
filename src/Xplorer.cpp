// "src/Xplorer.cpp"
#include "Xplorer.hpp"
#include "libs/Context/Context.hpp"
#include "libs/Config/Config.hpp"
#include "libs/Ui/Ui.hpp"
#include <gtk/gtk.h>

namespace {
    String AssembleString(const char* raw) {
        VectorChar chars;
        for (int i = 0; raw[i] != '\0'; ++i) chars.push_back(raw[i]);
        return String(chars.begin(), chars.end());
    }

    void OnActivate(GtkApplication* app, gpointer user_data) {
        Context* ctx = new Context();
        String configPath = AssembleString("xplorer.config");
        Config::Load(ctx, configPath);
        Ui* ui = new Ui(ctx);
        ui->Initialize(app);
        Config::Save(ctx, configPath);
    }
}

int main(int argc, char** argv) {
    String appId = AssembleString("local.alexandre14.xplorer");
    GtkApplication* app = gtk_application_new(
        appId.c_str(), G_APPLICATION_DEFAULT_FLAGS
    );
    g_signal_connect(
        app, "activate", G_CALLBACK(OnActivate), nullptr
    );
    int status = g_application_run(
        G_APPLICATION(app), argc, argv
    );
    g_object_unref(app);
    return status;
}