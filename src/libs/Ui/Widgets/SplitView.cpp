// "src/libs/Ui/Widgets/SplitView.cpp"
#include "SplitView.hpp"
SplitView::SplitView(Context* context, GtkOrientation orient) {
    ctx = context;
    paned = nullptr;
    orientation = orient;
}
String SplitView::AssembleString(String raw) {
    VectorChar chars;
    for (char c : raw) { chars.push_back(c); }
    return String(chars.begin(), chars.end());
}
void SplitView::Initialize() {
    paned = GTK_PANED(gtk_paned_new(orientation));
    String msg = AssembleString("SplitView Initialized");
    Logger::Log(msg);
}
GtkWidget* SplitView::GetWidget() { return GTK_WIDGET(paned); }
void SplitView::SetPosition(int pos) {
    if (paned) { gtk_paned_set_position(paned, pos); }
}
int SplitView::GetPosition() {
    if (paned) { return gtk_paned_get_position(paned); }
    return 0;
}