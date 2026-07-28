# Xplorer

A thoughtful, minimal editor built with GTK4, VTE, and WebKit.
Features an integrated terminal, web browser, and syntax highlighting.

## Architecture

A graphical application, much like a well-built city, requires distinct districts for its various functions.

To support split views, multiple tabbed environments, and system integration like themes and printing, we must separate concerns carefully.

Here is a more comprehensive arrangement. 
```bash
.
├── libs
│   ├── Actions
│   │   ├── Actions.cpp
│   │   ├── Actions.hpp
│   │   └── helper
│   │       ├── ActionsRegistrar.cpp
│   │       └── ActionsRegistrar.hpp
│   ├── Config
│   │   ├── Config.cpp
│   │   └── Config.hpp
│   ├── Context
│   │   ├── Context.cpp
│   │   └── Context.hpp
│   ├── Logger
│   │   ├── Logger.cpp
│   │   └── Logger.hpp
│   └── Ui
│       ├── Dialogs
│       │   ├── AboutDialog.cpp
│       │   ├── AboutDialog.hpp
│       │   ├── EditorSettingsDialog.cpp
│       │   ├── EditorSettingsDialog.hpp
│       │   ├── HelpDialog.cpp
│       │   ├── HelpDialog.hpp
│       │   ├── PrintDialog.cpp
│       │   ├── PrintDialog.hpp
│       │   ├── TerminalSettingsDialog.cpp
│       │   ├── TerminalSettingsDialog.hpp
│       │   ├── TerminalShellDialog.cpp
│       │   └── TerminalShellDialog.hpp
│       ├── helper
│       │   ├── UiFileHelper.cpp
│       │   └── UiFileHelper.hpp
│       ├── Menu
│       │   ├── MenuBar.cpp
│       │   └── MenuBar.hpp
│       ├── Ui.cpp
│       ├── Ui.hpp
│       ├── Views
│       │   ├── Browser.cpp
│       │   ├── Browser.hpp
│       │   ├── FileEditor.cpp
│       │   ├── FileEditor.hpp
│       │   ├── helper
│       │   │   ├── BrowserHelper.cpp
│       │   │   ├── BrowserHelper.hpp
│       │   │   ├── FileEditorHelper.cpp
│       │   │   └── FileEditorHelper.hpp
│       │   ├── Terminal.cpp
│       │   └── Terminal.hpp
│       └── Widgets
│           ├── SplitView.cpp
│           ├── SplitView.hpp
│           ├── TabManager.cpp
│           └── TabManager.hpp
├── ress
│   ├── resources.gresource.xml
│   └── xplorer.png
├── Xplorer.cpp
└── Xplorer.hpp
```

It borrows from the Unix philosophy of modularity, isolating the window manager, the individual views, and the actions that bind them together.

The Context module holds the shared application state, such as the current theme or window
dimensions. 

The Config module handles the persistence of these states to the disk, reading and writing user preference files.

We introduce Actions to manage GTK4 Actions.

This cleanly separates the logic of toggling line wrap or opening a file from the visual buttons themselves.

Inside the Ui module, we distinguish between Widgets, which are structural elements like the sliding split view and the tab manager.

Views contain the actual content rendering—the text editor, the embedded terminal, and browser.

Finally, Dialogs and Menu handle the transient and persistent interfaces, such as the about screen, print dialogs, and the top menu bar.

This structure allows you to debug a printing issue in PrintDialog without traversing the logic of your tab manager.

It is a quiet, orderly workspace.

## Screenshot

<div style="text-align: center;">
  <img src="screenshot/xplorer-editor-swapped-panes.png" />
</div>


## License

SPDX-License-Identifier: AGPL-3.0-or-later
Copyright (C) 2026 Alexandre Raduly <alexander14k28@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

See [LICENSE](LICENSE) for the license governing this project.
