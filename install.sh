#!/bin/bash

# Detect the platform (Linux, macOS, or WSL for Windows)
OS="$(uname -s)"
case "$OS" in
    Linux*)     platform="Linux";;
    Darwin*)    platform="Mac";;
    CYGWIN*|MINGW32*|MSYS*|MINGW*) platform="Windows";;
    *)          platform="Unknown";;
esac

# Function to compile the C++ code
compile_devenv() {
    echo "Compiling devenv for $platform..."
    
    # Depending on the platform, compile the correct binary
    if [ "$platform" == "Linux" ]; then
        g++ main.cpp -o devenv
    elif [ "$platform" == "Mac" ]; then
        g++ main.cpp -o devenv
    elif [ "$platform" == "Windows" ]; then
        g++ main.cpp -o devenv.exe
    else
        echo "Unsupported platform: $platform"
        exit 1
    fi
}

# Function to install the executable
install_devenv() {
    echo "Installing devenv globally..."

    if [ "$platform" == "Linux" ] || [ "$platform" == "Mac" ]; then
        # Move the compiled binary to /usr/local/bin to make it globally accessible
        sudo mv devenv /usr/local/bin/devenv
        sudo chmod +x /usr/local/bin/devenv
    elif [ "$platform" == "Windows" ]; then
        # For Windows/WSL, place it in /usr/local/bin (WSL supports this structure)
        sudo mv devenv.exe /usr/local/bin/devenv.exe
    else
        echo "Unsupported platform: $platform"
        exit 1
    fi

    echo "devenv installed successfully. You can now run 'devenv' globally."
}

# Check if the binary is already compiled, otherwise compile it
if [ ! -f "./devenv" ] && [ ! -f "./devenv.exe" ]; then
    compile_devenv
fi

# Install the binary
install_devenv