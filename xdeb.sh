#!/bin/bash

APP_NAME="xplorer"
BINARY="out/linux/x86_64/release/$APP_NAME"
EXPORT_DIR="prod"
LICENSE="AGPL-3.0+"

if [ -f "src/ress/xplorer.png" ]; then
    ICON_SRC="src/ress/xplorer.png"
else
    ICON_SRC=""
fi

DEB_WORKDIR="deb_build"
PKG_DIR="$DEB_WORKDIR/local.alexandre14.xplorer"
VERSION="1.0.0"
ARCH="amd64"
COMPILER="gcc"
DEB_FILE="$EXPORT_DIR/xplorer-editor_${VERSION}_${COMPILER}-${ARCH}.deb"

if [ ! -f "$BINARY" ]; then
    echo "error: binary not found at $BINARY"
    echo "please build the project first using 'xmake'."
    exit 1
fi

mkdir -p "$EXPORT_DIR"

echo "cleaning up old deb build directory..."
rm -rf "$DEB_WORKDIR"
mkdir -p "$PKG_DIR/DEBIAN"
mkdir -p "$PKG_DIR/usr/bin"
mkdir -p "$PKG_DIR/usr/share/applications"
mkdir -p "$PKG_DIR/usr/share/icons/hicolor/256x256/apps"

echo "copying binary..."
cp "$BINARY" "$PKG_DIR/usr/bin/$APP_NAME"
chmod 755 "$PKG_DIR/usr/bin/$APP_NAME"

if [ -n "$ICON_SRC" ]; then
    echo "copying icon..."
    cp "$ICON_SRC" "$PKG_DIR/usr/share/icons/hicolor/256x256/apps/$APP_NAME.png"
    chmod 644 "$PKG_DIR/usr/share/icons/hicolor/256x256/apps/$APP_NAME.png"
else
    echo "warning: icon absent. skipping"
fi

echo "creating desktop entry..."
cat << EOF > "$PKG_DIR/usr/share/applications/local.alexandre14k.xplorer.desktop"
[Desktop Entry]
Version=1.0
Name=Xplorer Editor
Comment=A thoughtful, minimal editor
Exec=$APP_NAME
Icon=$APP_NAME
Terminal=false
Type=Application
Categories=Development;TextEditor;IDE;
EOF
chmod 644 "$PKG_DIR/usr/share/applications/local.alexandre14k.xplorer.desktop"

echo "creating control file..."
cat << EOF > "$PKG_DIR/DEBIAN/control"
Package: local.alexandre14k.xplorer
Version: $VERSION
Architecture: $ARCH
License: $LICENSE
Maintainer: Alexandre Raduly <https://github.com/alexandre14k>
Depends: libgtk-4-1, libgtk-4-dev, libwebkitgtk-6.0-dev, libvte-2.91-gtk4-dev, libgtksourceview-5-dev
Description: Xplorer Editor - A thoughtful, minimal editor built with GTK4, VTE, and WebKit. Features an integrated terminal, web browser, and syntax highlighting.
EOF

echo "building .deb package to $EXPORT_DIR/..."
dpkg-deb --build --root-owner-group "$PKG_DIR" "$DEB_FILE"

echo "cleaning up intermediate files..."
rm -rf "$DEB_WORKDIR"

echo ""
echo "done: $DEB_FILE"
echo ""
echo "install method: sudo apt install ./$DEB_FILE"
echo ""
echo "once done launch it from your app menu or by typing '$APP_NAME' in the terminal."