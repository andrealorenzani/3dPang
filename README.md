# 3dPang

A 3D game inspired by *Pang* (Mitchell, 1989), with its own level editor.

Balls bounce around the player, and every ball you hit splits into smaller
ones until they pop. You fight them with harpoons, a quadruple harpoon and a
gun, across floors, walls and ladders. Popping balls drop bonuses (fruit,
extra lives, weapons) and maluses (tornadoes, blinding bombs). One or two
players (split screen) can play, at five difficulty levels.

![3dPang in 2005](docs/images/gameplay.jpg)

Andrea Lorenzani and Eva Tamburini wrote it in 2004-2005, as the project for
the *Costruzione di Interfacce* course (teacher Paolo Cignoni) at the
University of Pisa. Version 2.0 (2026) ports it to current libraries and
tools and translates it into English (the code was originally in Italian).
Gameplay, levels and look are the same as in 2005.

| | |
|---|---|
| ![Bubble](docs/images/bubble.png) | ![Build](docs/images/build.png) |
| ![Fruits](docs/images/fruits.png) | ![Woods](docs/images/woods.png) |

## What is in the box

* **`3dpang`**, the game (SDL3 window, OpenGL).
* **`3dpang-editor`**, the level editor (Qt 6 widgets, OpenGL).
* `data/`: 11 levels, a campaign, textures, sky boxes, two player models and
  the bonus models.
* `tools/convert_legacy_level.py` converts levels made with the 2005 editor.

## Quick start

You need CMake ≥ 3.21, a C++17 compiler and Qt ≥ 6.4 (Core, Gui, Xml, Widgets,
OpenGL, OpenGLWidgets). SDL3 and [vcglib](https://github.com/cnr-isti-vclab/vcglib)
are downloaded automatically if they are not installed.

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/<compiler>
cmake --build build --parallel
./build/3dpang            # the game
./build/3dpang-editor     # the editor
```

With CLion, Visual Studio, Qt Creator or VS Code, just open the folder: the
`CMakePresets.json` provides `debug` and `release` presets (set `QT_DIR`
to your Qt installation). [docs/BUILDING.md](docs/BUILDING.md) has the
details for every platform, the options, and how to run the tests.

## Playing

| Action | Player 1 | Player 2 |
|---|---|---|
| Move | arrow keys | W A S D |
| Fire | M | Q |
| Climb up / down a ladder | J / N | R / F |
| Fixed view | F2 | F9 |
| Views following the player | F3, F4, F5 | F6, F7, F8 |

Esc leaves a game. See [docs/PLAYER.md](docs/PLAYER.md) for the menu, the
difficulty levels and the power-ups, and [docs/EDITOR.md](docs/EDITOR.md) to
make your own levels ([docs/LEVEL_FORMAT.md](docs/LEVEL_FORMAT.md) documents
the file format).

## History: the 2005 version

The code exactly as it was in 2005 is preserved in the tag and in the branch
**`original_with_Eva`** (a Visual Studio .NET 2003 solution using Qt 3.2,
SDL 1.2, GLEW and lib3ds). Both refs have the same name, so name them
explicitly:

```sh
git switch original_with_Eva                 # the branch (git switch only takes branches)
git checkout refs/tags/original_with_Eva     # the tag, as a detached snapshot
```

The original web page is on the Wayback Machine:
<https://web.archive.org/web/20050623004727/http://www.supermaestro.org/download/3dPang/>

[CHANGELOG.md](CHANGELOG.md) lists what changed in version 2.0, including the
bugs from 2005 fixed along the way.

## Credits and licences

* Code: Andrea Lorenzani and Eva Tamburini, released under the
  [GNU GPL v3](LICENSE).
* `src/course/` (cube map, MD2 loader, trackball) comes from the library of
  the *Costruzione di Interfacce* course by Paolo Cignoni and Claudio Rocchini
  (ISTI-CNR Pisa).
* [vcglib](https://github.com/cnr-isti-vclab/vcglib) (GPL v3), by the Visual
  Computing Lab of ISTI-CNR, provides the math and OpenGL helpers.
* `src/common/BMF_font.*`: BMF_font, an OpenGL bitmap font library by Markus
  Trenkwalder (LGPL 2.1).
* Player models (Quake II fan models; see the `readme.txt` next to each):
  *Little Buu* by Wetflame Gyrochaos, *Hobgoblin* by Hunter, Burnt Kona,
  Fafner and Deranged.
* Textures and sky boxes are the ones shipped with the 2005 game.

*Pang* is © Mitchell Corporation, 1989. 3dPang is an independent student
project, not affiliated with or endorsed by Mitchell.
