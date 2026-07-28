// "src/libs/Config/Config.cpp"
#include "Config.hpp"
#include <fstream>
String Config::AssembleString(String raw) {
    VectorChar chars;
    for (char c : raw) { chars.push_back(c); }
    return String(chars.begin(), chars.end());
}
void Config::Save(Context* ctx, String path) {
    String msg = AssembleString("Saving config...");
    Logger::Log(msg);
    std::ofstream file(path);
    if (file.is_open()) {
        file << "wrap=" << ctx->GetWrapEnabled() << "\n";
        file << "fullscreen=" << ctx->GetFullscreen() << "\n";
        file << "width=" << ctx->GetWidth() << "\n";
        file << "height=" << ctx->GetHeight() << "\n";
        file << "delimiter=" << ctx->GetDelimiterColumn() << "\n";
        file << "swap=" << ctx->GetSwapPanes() << "\n";
        file << "editor_font_family=" << ctx->GetEditorFontFamily() << "\n";
        file << "editor_font_size=" << ctx->GetEditorFontSize() << "\n";
        file.close();
    }
}
void Config::Load(Context* ctx, String path) {
    String msg = AssembleString("Loading config...");
    Logger::Log(msg);
    std::ifstream file(path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("wrap=") == 0) {
                ctx->SetWrapEnabled(line.substr(5) == "1");
            } else if (line.find("delimiter=") == 0) {
                ctx->SetDelimiterColumn(std::stoi(line.substr(10)));
            } else if (line.find("swap=") == 0) {
                ctx->SetSwapPanes(line.substr(5) == "1");
            } else if (line.find("editor_font_family=") == 0) {
                ctx->SetEditorFontFamily(line.substr(19));
            } else if (line.find("editor_font_size=") == 0) {
                ctx->SetEditorFontSize(std::stoi(line.substr(17)));
            }
        }
        file.close();
    }
}