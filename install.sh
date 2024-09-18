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
compile_runindock() {
    echo "Compiling runindock for $platform..."
    
    # Depending on the platform, compile the correct binary
    if [ "$platform" == "Linux" ]; then
        g++ main.cpp -o runindock
    elif [ "$platform" == "Mac" ]; then
        g++ main.cpp -o runindock
    elif [ "$platform" == "Windows" ]; then
        g++ main.cpp -o runindock.exe
    else
        echo "Unsupported platform: $platform"
        exit 1
    fi
}

# Function to install the executable
install_runindock() {
    echo "Installing runindock globally..."

    if [ "$platform" == "Linux" ] || [ "$platform" == "Mac" ]; then
        # Move the compiled binary to /usr/local/bin to make it globally accessible
        sudo mv runindock /usr/local/bin/runindock
        sudo chmod +x /usr/local/bin/runindock
    elif [ "$platform" == "Windows" ]; then
        # For Windows/WSL, place it in /usr/local/bin (WSL supports this structure)
        sudo mv runindock.exe /usr/local/bin/runindock.exe
    else
        echo "Unsupported platform: $platform"
        exit 1
    fi

    echo "runindock installed successfully. You can now run 'runindock' globally."
}

# Check if the binary is already compiled, otherwise compile it
if [ ! -f "./runindock" ] && [ ! -f "./runindock.exe" ]; then
    compile_runindock
fi

# Install the binary
install_runindock