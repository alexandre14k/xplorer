// "src/libs/Config/Config.hpp"
#pragma once
#include "Xplorer.hpp"
#include "libs/Context/Context.hpp"
class Config {
    public:
        static void Save(Context* ctx, String path);
        static void Load(Context* ctx, String path);
    private:
        static String AssembleString(String raw);
};