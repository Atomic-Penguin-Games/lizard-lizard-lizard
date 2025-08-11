# 🚀 Release Guide

## Creating a New Release

### Option 1: Using the Script (Recommended)
```bash
# Create and push a version tag
./create-release.sh v1.0.0
```

### Option 2: Manual
```bash
# Create a version tag
git tag -a v1.0.0 -m "Release v1.0.0"

# Push the tag to GitHub
git push origin v1.0.0
```

## What Happens Automatically

When you push a version tag (like `v1.0.0`), GitHub Actions will:

1. **Build all platforms** (Linux, Windows, macOS, Web)
2. **Create a GitHub Release** 
3. **Upload downloadable packages**:
   - `lizard-meme-linux.zip` - Linux executable + resources
   - `lizard-meme-windows.zip` - Windows .exe + resources  
   - `lizard-meme-macos.zip` - macOS .app bundle
   - `lizard-meme-web.zip` - Web build files

## Download Locations

### 🎯 **For Users (Permanent Downloads)**
- Go to: `https://github.com/Derek52/lizard-meme-game/releases`
- Download the appropriate package for your platform
- Each release stays available permanently

### 🔧 **For Development (Temporary)**
- Go to: **Actions** tab → Select workflow run → **Artifacts** section
- Artifacts expire after 90 days
- Useful for testing builds from PRs

## Platform-Specific Instructions

### 🐧 **Linux** (`lizard-meme-linux.zip`)
```bash
unzip lizard-meme-linux.zip
cd lizard-meme-linux
./lizard_meme
```

### 🪟 **Windows** (`lizard-meme-windows.zip`)
1. Extract the zip file
2. Double-click `lizard_meme.exe`

### 🍎 **macOS** (`lizard-meme-macos.zip`)
1. Extract the zip file
2. Drag `LizardMeme.app` to Applications folder
3. Right-click → Open (first time only, due to Gatekeeper)

### 🌐 **Web** (`lizard-meme-web.zip`)
```bash
unzip lizard-meme-web.zip
cd lizard-meme-web
python -m http.server 8000
# Open browser to http://localhost:8000/lizard_meme.html
```

## Version Naming

Use [Semantic Versioning](https://semver.org/):
- `v1.0.0` - Major release
- `v1.1.0` - Minor update (new features)
- `v1.0.1` - Patch (bug fixes)

## Release Notes

The GitHub release will automatically include:
- Platform-specific download instructions
- System requirements
- Build information

You can edit the release afterward to add:
- Changelog
- Known issues
- Credits
