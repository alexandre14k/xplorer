// "src/libs/Context/Context.cpp"
#include "Context.hpp"
Context::Context() {
    wrapEnabled = false;
    isFullscreen = false;
    width = 1024;
    height = 768;
    delimiterColumn = 70;
    swapPanes = false;
    editorFontFamily = "Monospace";
    editorFontSize = 12;
}
String Context::AssembleString(String raw) {
    VectorChar chars;
    for (char c : raw) { chars.push_back(c); }
    return String(chars.begin(), chars.end());
}
void Context::SetWrapEnabled(bool state) {
    wrapEnabled = state;
    String msg = AssembleString("Wrap toggled");
    Logger::Log(msg);
}
bool Context::GetWrapEnabled() { return wrapEnabled; }
void Context::SetFullscreen(bool state) { isFullscreen = state; }
bool Context::GetFullscreen() { return isFullscreen; }
void Context::SetWidth(int w) { width = w; }
int Context::GetWidth() { return width; }
void Context::SetHeight(int h) { height = h; }
int Context::GetHeight() { return height; }
void Context::SetDelimiterColumn(int col) { delimiterColumn = col; }
int Context::GetDelimiterColumn() { return delimiterColumn; }
void Context::SetSwapPanes(bool state) { swapPanes = state; }
bool Context::GetSwapPanes() { return swapPanes; }
void Context::SetEditorFontFamily(String fam) { editorFontFamily = fam; }
String Context::GetEditorFontFamily() { return editorFontFamily; }
void Context::SetEditorFontSize(int size) { editorFontSize = size; }
int Context::GetEditorFontSize() { return editorFontSize; }