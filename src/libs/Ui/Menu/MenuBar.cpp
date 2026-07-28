// "src/libs/Ui/Menu/MenuBar.cpp"
#include "MenuBar.hpp"
MenuBar::MenuBar() { menuBar = nullptr; }
GMenu* MenuBar::BuildFileMenu() {
    GMenu* fileMenu = g_menu_new();
    g_menu_append(fileMenu, "New", "app.new");
    g_menu_append(fileMenu, "Open", "app.open");
    g_menu_append(fileMenu, "Save", "app.save");
    g_menu_append(fileMenu, "Save As", "app.save_as");
    g_menu_append(fileMenu, "Print", "app.print");
    GMenu* exitSection = g_menu_new();
    g_menu_append(exitSection, "Exit", "app.exit");
    g_menu_append_section(fileMenu, nullptr, G_MENU_MODEL(exitSection));
    g_object_unref(exitSection);
    return fileMenu;
}
GMenu* MenuBar::BuildToolsMenu() {
    GMenu* toolsMenu = g_menu_new();
    g_menu_append(toolsMenu, "Wrap", "app.wrap");
    g_menu_append(toolsMenu, "Swap", "app.swap");
    g_menu_append(toolsMenu, "Terminal Settings", "app.term_settings");
    g_menu_append(toolsMenu, "Editor Settings", "app.edit_settings");
    return toolsMenu;
}
GMenu* MenuBar::BuildHelpMenu() {
    GMenu* helpMenu = g_menu_new();
    g_menu_append(helpMenu, "About Xplorer", "app.about");
    g_menu_append(helpMenu, "Help", "app.help");
    return helpMenu;
}
void MenuBar::Initialize() {
    GMenu* root = g_menu_new();
    GMenu* fileMenu = BuildFileMenu();
    GMenu* toolsMenu = BuildToolsMenu();
    GMenu* helpMenu = BuildHelpMenu();
    g_menu_append_submenu(root, "File", G_MENU_MODEL(fileMenu));
    g_menu_append_submenu(root, "Tools", G_MENU_MODEL(toolsMenu));
    g_menu_append_submenu(root, "Help", G_MENU_MODEL(helpMenu));
    g_object_unref(fileMenu);
    g_object_unref(toolsMenu);
    g_object_unref(helpMenu);
    menuBar = gtk_popover_menu_bar_new_from_model(G_MENU_MODEL(root));
    g_object_unref(root);
    String msg = "MenuBar Initialized";
    Logger::Log(msg);
}
GtkWidget* MenuBar::GetWidget() { return menuBar; }