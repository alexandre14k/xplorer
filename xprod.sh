#!/bin/bash

APP_NAME="xplorer"
BINARY="out/linux/x86_64/release/$APP_NAME"
EXPORT_DIR="prod"
LIB_DIR="$EXPORT_DIR/lib"

echo "cleaning previous $EXPORT_DIR setup..."
rm -rf "$EXPORT_DIR"

mkdir -p "$LIB_DIR"

if [ ! -f "$BINARY" ]; then
    echo "error: binary $BINARY absent"
    echo "build the project with 'xmake'."
    exit 1
fi

echo "copying binary..."
cp "$BINARY" "$EXPORT_DIR/"

echo "copying dynamic libs..."
ldd "$BINARY" | grep "=>" | awk '{print $3}' | sort -u | while read -r lib; do
    if [ -f "$lib" ]; then
        cp -L "$lib" "$LIB_DIR/"
    fi
done

LDD_LINUX=$(ldd "$BINARY" | grep "ld-linux" | awk '{print $1}' | head -n 1)
if [ -f "$LDD_LINUX" ]; then
    cp -L "$LDD_LINUX" "$LIB_DIR/"
fi

echo "stripping debugging symbols from binaries and lib..."
strip --strip-unneeded "$EXPORT_DIR/$APP_NAME"
find "$LIB_DIR" -type f -name "*.so*" -exec strip --strip-unneeded {} \; 2>/dev/null

echo "creating run.sh wrapper..."
cat << EOF > "$EXPORT_DIR/run.sh"
#!/bin/bash
DIR="\$(cd "\$(dirname "\${BASH_SOURCE[0]}")" && pwd)"
export LD_LIBRARY_PATH="\$DIR/lib:\$LD_LIBRARY_PATH"
exec "\$DIR/$APP_NAME" "\$@"
EOF
chmod +x "$EXPORT_DIR/run.sh"

echo "done standalone built in '$EXPORT_DIR/'"
du -sh "$EXPORT_DIR"
echo "execute: ./$EXPORT_DIR/run.sh"
echo "beware of high webkit memory fingerprint in libs folder"