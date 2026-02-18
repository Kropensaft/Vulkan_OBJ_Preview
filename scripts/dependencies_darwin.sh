#!/bin/bash

# 1. Check for Homebrew, install if missing
if ! command -v brew &> /dev/null; then
    echo "Homebrew not found. Installing..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
fi

# 2. Update Homebrew
brew update

# 3. Install build tools and libraries
echo "Installing dependencies..."
brew install cmake glfw glm 

# 4. Install Vulkan SDK (via LunarG)
brew install --cask vulkan-sdk


echo "dependencies installed"
