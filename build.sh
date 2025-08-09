#!/bin/bash

# Required packages for OpenGL and X11 development
REQUIRED_PKGS=(libgl1-mesa-dev libx11-dev libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev build-essential)

echo "Checking and installing required packages..."

# Check for each package, install if missing
for pkg in "${REQUIRED_PKGS[@]}"; do
    dpkg -s "$pkg" &> /dev/null
    if [ $? -ne 0 ]; then
        echo "Installing $pkg..."
        sudo apt-get update
        sudo apt-get install -y "$pkg"
    else
        echo "$pkg is already installed."
    fi
done

# Compile the game
echo "Compiling the game..."

gcc src/*.c -o game \
    -Iinclude \
    -Llib/Lin64 \
    -lraylib \
    -lGL -lm -lpthread -ldl -lrt -lX11

if [ $? -eq 0 ]; then
    echo "Compilation succeeded! Run your game with:"
    echo "LD_LIBRARY_PATH=lib/Lin64 ./game"
    
    # Prompt to launch the game
    read -p "Launch the game now? (y/n) " answer
    case "$answer" in
        [Yy]* )
            LD_LIBRARY_PATH=lib/Lin64 ./game
            ;;
        * )
            echo "You can launch the game later with:"
            echo "LD_LIBRARY_PATH=lib/Lin64 ./game"
            ;;
    esac
else
    echo "Compilation failed."
fi
