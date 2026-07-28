// "src/libs/Ui/helper/UiFileHelper.cpp"
#include "UiFileHelper.hpp"
#include <fstream>
String UiFileHelper::FormatTabLabel(String filename) {
    size_t slash = filename.find_last_of('/');
    String base = (slash != String::npos) ? 
        filename.substr(slash + 1) : filename;
    size_t dot = base.find_last_of('.');
    if (dot == String::npos) {
        if (base.length() > 8) return base.substr(0, 4) + "...";
        return base;
    }
    String stem = base.substr(0, dot);
    String ext = base.substr(dot + 1);
    if (stem.length() > 4) return stem.substr(0, 4) + "..." + ext;
    return base;
}
String UiFileHelper::ReadFile(String path) {
    std::ifstream file(path);
    if (file.is_open()) {
        String content(
            (std::istreambuf_iterator<char>(file)), 
            std::istreambuf_iterator<char>()
        );
        return content;
    }
    return "";
}
bool UiFileHelper::WriteFile(String path, String content) {
    std::ofstream file(path);
    if (file.is_open()) {
        file << content;
        file.close();
        return true;
    }
    return false;
}