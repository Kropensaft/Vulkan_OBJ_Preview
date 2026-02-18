# 1. Check for Administrative privileges
if (-NOT ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Write-Warning "Please run this script as Administrator!"
    break
}

# 2. Install Build Tools (Visual Studio Build Tools)
Write-Host "Installing C++ Build Tools..."
winget install Microsoft.VisualStudio.2022.BuildTools --override "--add Microsoft.VisualStudio.Workload.VCTools" --silent

# 3. Install CMake and Git
winget install Kitware.CMake Git.Git --silent

# 4. Install Vulkan SDK
Write-Host "Installing Vulkan SDK..."
winget install LunarG.VulkanSDK --silent

Write-Host "Done! Your .exe is in the build folder."
