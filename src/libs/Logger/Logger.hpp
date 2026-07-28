// "src/libs/Logger/Logger.hpp"
#pragma once
#include <iostream>
#include "Xplorer.hpp"
class Logger {
    public:
        static void Log(String msg);
    private:
        static String AssembleString(String raw);
};