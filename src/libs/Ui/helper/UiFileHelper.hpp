// "src/libs/Ui/helper/UiFileHelper.hpp"
#pragma once
#include "Xplorer.hpp"
class UiFileHelper {
    public:
        static String FormatTabLabel(String filename);
        static String ReadFile(String path);
        static bool WriteFile(String path, String content);
};