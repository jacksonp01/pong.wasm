#/bin/bash
FOLDER=~/Projects/Pong
clang --target=wasm32 --no-standard-libraries -Wl,--export-all -Wl,--no-entry -o $FOLDER/output/main.wasm $FOLDER/main.c

