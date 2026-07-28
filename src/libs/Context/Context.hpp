// "src/libs/Context/Context.hpp"
#pragma once
#include "Xplorer.hpp"
class Context {
    public:
        Context();
        void SetWrapEnabled(bool state);
        bool GetWrapEnabled();
        void SetFullscreen(bool state);
        bool GetFullscreen();
        void SetWidth(int w);
        int GetWidth();
        void SetHeight(int h);
        int GetHeight();
        void SetDelimiterColumn(int col);
        int GetDelimiterColumn();
        void SetSwapPanes(bool state);
        bool GetSwapPanes();
        void SetEditorFontFamily(String fam);
        String GetEditorFontFamily();
        void SetEditorFontSize(int size);
        int GetEditorFontSize();
    private:
        bool wrapEnabled;
        bool isFullscreen;
        int width;
        int height;
        int delimiterColumn;
        bool swapPanes;
        String editorFontFamily;
        int editorFontSize;
        String AssembleString(String raw);
};