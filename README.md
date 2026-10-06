# 3D Game Engine (SFML 3 + OpenGL)

A modern 64-bit 3D Game Engine starter project built with **SFML 3.1.0** and **OpenGL** using C++20 and MinGW-w64 (GCC 14.2.0).

---

## 🚀 How to Run in VS Code

You can run this project directly in VS Code using any of these simple methods:

### Method 1: One-Click Run / Debug (Recommended)
1. Open this folder in VS Code (`File` -> `Open Folder...`).
2. Press **`F5`** (or go to `Run` -> `Start Debugging`).
   - VS Code will automatically compile `src/main.cpp` using the default build task and launch the 3D Game Engine with GDB debugger attached.

### Method 2: Build with Shortcut
- Press **`Ctrl + Shift + B`** to trigger the default build task (`Build (Debug)`).
- Go to `Terminal` -> `Run Task...` -> `Run 3D Game Engine` to execute it.

### Method 3: Run the Executable Directly
Double-click `bin/3D_Game_Engine.exe` or run it from any terminal:
```powershell
.\bin\3D_Game_Engine.exe
```
*(All required SFML and MinGW runtime DLLs are pre-bundled in the `bin/` folder).*

---

## 🎮 Controls

| Input | Action |
|---|---|
| **Left Mouse Click + Drag** | Orbit / Rotate Camera around target |
| **Mouse Scroll Wheel** | Zoom in / Zoom out |
| **Arrow Keys** | Orbit / Rotate Camera |
| **W / A / S / D** | Pan / Move Camera position |
| **Spacebar** | Toggle cube auto-rotation pause/play |
| **R** | Reset camera to initial position |
| **Escape** | Exit Engine |

---

## 📁 Project Structure

```text
SFML/
├── .vscode/
│   ├── tasks.json               # VS Code build and run tasks
│   ├── launch.json              # F5 debugging setup (GDB)
│   ├── c_cpp_properties.json    # IntelliSense configuration (C++20)
│   └── settings.json            # Terminal environment configuration
├── bin/
│   ├── 3D_Game_Engine.exe       # Compiled binary
│   └── *.dll                    # 64-bit SFML and MinGW runtime DLLs
├── external/
│   └── SFML/                    # Official SFML 3.1.0 64-bit release
│       ├── include/             # SFML header files
│       ├── lib/                 # Pre-built SFML import & static libraries
│       └── bin/                 # SFML DLLs
├── src/
│   └── main.cpp                 # 3D Game Engine core loop & rendering
├── CMakeLists.txt               # CMake configuration
├── Makefile                     # Make build script
└── README.md                    # Project documentation
```

---

## 🛠 Toolchain Information
- **SFML Version**: 3.1.0 (64-bit MinGW)
- **Compiler**: GCC 14.2.0 (MinGW-w64 UCRT 64-bit)
- **Language Standard**: C++20
- **Rendering API**: OpenGL 3D with 24-bit depth buffer
