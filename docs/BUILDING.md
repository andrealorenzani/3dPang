# Building 3dPang

3dPang is a CMake project. It builds from the command line and from any IDE
that understands CMake (CLion, Visual Studio, Qt Creator, VS Code...).

## Dependencies

| Library | Version | Used for | How CMake finds it |
|---|---|---|---|
| Qt | ≥ 6.4: Core, Gui, Xml, Widgets, OpenGL, OpenGLWidgets | images, XML, the editor | `find_package`; set `CMAKE_PREFIX_PATH` |
| SDL | 3 | the game window and keyboard | `find_package(SDL3)`, otherwise downloaded and built (`release-3.4.20`) |
| vcglib | 2025.07 | math (points, matrices), OpenGL helpers | downloaded (header only), or `PANG3D_VCGLIB_DIR` |
| OpenGL | 1.3+ compatibility profile | rendering | the system |

GLEW, GLU and lib3ds, needed in 2005, are no longer used.

Downloads need `git` and an internet connection on the first configure.
To build offline, install SDL3 and point `PANG3D_VCGLIB_DIR` at a vcglib
checkout.

## Command line

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH=$HOME/Qt/6.8.3/gcc_64
cmake --build build --parallel
```

Or, with the presets (`QT_DIR` must point to the Qt installation):

```sh
export QT_DIR=$HOME/Qt/6.8.3/gcc_64
cmake --preset release
cmake --build --preset release
ctest --preset release
```

The programs are `build/3dpang` and `build/3dpang-editor`. The build copies
`data/` next to them, so they run from anywhere.

### Linux

Ubuntu 24.04 packages for Qt and the libraries SDL3 needs to build:

```sh
sudo apt install cmake g++ git qt6-base-dev libgl1-mesa-dev libegl1-mesa-dev \
    libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev libxss-dev \
    libxfixes-dev libxtst-dev libxkbcommon-dev libwayland-dev wayland-protocols libdecor-0-dev
```

On distributions that package SDL3 (`libsdl3-dev`), it is used instead of
building it.

### Windows

Install Visual Studio 2022 (C++ workload) and Qt 6 for MSVC with the Qt
online installer, then:

```bat
cmake -S . -B build -DCMAKE_PREFIX_PATH=C:\Qt\6.8.3\msvc2022_64
cmake --build build --config Release
C:\Qt\6.8.3\msvc2022_64\bin\windeployqt build\Release\3dpang-editor.exe
```

`SDL3.dll` is copied next to `3dpang.exe` automatically. `windeployqt`
copies the Qt DLLs.

### macOS

```sh
brew install cmake qt sdl3
cmake -S . -B build -DCMAKE_PREFIX_PATH="$(brew --prefix qt)"
cmake --build build --parallel
```

macOS still provides the legacy OpenGL 2.1 the game uses (it is deprecated,
but it works).

### vcpkg

```sh
vcpkg install qtbase sdl3
cmake --preset vcpkg        # needs VCPKG_ROOT
```

## IDEs

* **CLion**: *File → Open* the folder. CLion reads `CMakePresets.json`:
  enable the `debug`/`release` profiles in *Settings → Build → CMake* and set
  `QT_DIR` in the environment of the profile (or add
  `-DCMAKE_PREFIX_PATH=...` to its CMake options). Run configurations
  `3dpang` and `3dpang-editor` appear automatically.
* **Visual Studio 2022**: *File → Open → Folder*. Set `QT_DIR` in
  `CMakeUserPresets.json`, or in the environment before starting VS.
* **Qt Creator**: open `CMakeLists.txt` and pick a Qt 6 kit.

## Options

| Option | Default | |
|---|---|---|
| `PANG3D_BUILD_PLAYER` | ON | build the game |
| `PANG3D_BUILD_EDITOR` | ON | build the editor |
| `PANG3D_FETCH_DEPENDENCIES` | ON | download SDL3 if it is not found |
| `PANG3D_VCGLIB_DIR` | empty | use this vcglib checkout instead of downloading it |
| `PANG3D_VCGLIB_TAG`, `PANG3D_SDL3_TAG` | `2025.07`, `release-3.4.20` | versions to download |
| `PANG3D_TEST_VIDEO_DRIVER` | `offscreen` | SDL video driver for the tests |

## Tests

`ctest` plays every level for 20 seconds without a window, with player 1
walking around and shooting (`3dpang --demo LEVEL --frames 1200 --autofire`).
The `offscreen` driver needs EGL. Where it is not available, use a real
display: `-DPANG3D_TEST_VIDEO_DRIVER=x11` (or `wayland`, `windows`, `cocoa`),
or `xvfb-run ctest` on a headless Linux box.

To look at a level without playing:

```sh
3dpang --demo woods.xml --frames 120 --screenshot woods.png
3dpang-editor data/levels/woods.xml --screenshot editor.png
```

## Installing

```sh
cmake --install build --prefix /opt/3dpang
```

On Linux the data goes to `share/3dpang/data`. On Windows and macOS it goes to
`bin/data`. The programs look for the data in this order:

1. the `PANG3D_DATA_DIR` environment variable;
2. `data/` next to the executable;
3. `../share/3dpang/data` and `../Resources/data` relative to it;
4. the `data/` folder of the source tree they were built from.

High scores are written to the user's application data folder
(`~/.local/share/3dPang/3dPang/highscores.xml` on Linux).
