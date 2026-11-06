# Changelog

## 2.0 - 2026-11-06

The 2005 game, brought to current libraries and tools, and translated into
English. Gameplay, levels and graphics are unchanged. The 2005 code is kept in
the `original_with_Eva` tag and branch.

### Build and libraries

* **CMake** build (≥ 3.21) with presets, replacing the Visual Studio .NET 2003
  solution, the qmake `.pro` files and the `clean*.bat` scripts. Works from the
  command line and in CLion, Visual Studio, Qt Creator and VS Code, on Linux,
  Windows and macOS.
* **Qt 3.2 → Qt 6** (≥ 6.4). `QGLWidget` became `QOpenGLWidget`. The Qt 3
  Designer forms, which no current tool can read, were rewritten as Qt 6
  widgets.
* **SDL 1.2 → SDL3** for the game window, with a fixed-step game loop. The
  2005 loop spun at 100% CPU and timed itself with `clock()`, which on Linux
  measures CPU time, not real time.
* The 2002 copy of the course **vcglib** was replaced by the current
  [vcglib](https://github.com/cnr-isti-vclab/vcglib) release, downloaded by
  CMake.
* **GLEW and GLU removed.** The few GLU functions in use are reimplemented,
  and the OpenGL 1.3+ features in use need no extension loader.
* **lib3ds 1.2 removed**: a small built-in 3DS loader reads the bonus models.
* GitHub Actions build Linux, Windows and macOS, run the tests, and publish
  the release packages.

### English everywhere

* Every class, file, variable and comment was translated:
  `Gioco` → `Game`, `Scrittore` → `TextWriter`, `GrPalla` → `BallGroup`,
  `Pavimento` → `Floor`, `Rimbalzo` → `Bounce`, and so on.
* The game menu, HUD and editor interface are in English.
* The HTML documentation was rewritten as Markdown in `docs/`.
* New **level format** with English names and data-relative paths. All
  levels were converted, and `tools/convert_legacy_level.py` converts more.

### Data

* Organised into `levels/`, `campaigns/`, `textures/`, `skyboxes/`,
  `models/` and `fonts/`. Paths no longer depend on the working directory.
* The player model shipped in 2005 was the Hobgoblin mesh wearing a Little
  Buu skin: the two Quake II archives both contain a `tris.MD2`, and one
  overwrote the other. Both models are now included, each with its own skins
  converted from PCX to PNG (Qt cannot read PCX). Little Buu is the default.
* High scores are saved in the user's data folder instead of the game folder.
* Removed: build products (`.pdb`, `.ilk`, `.ncb`, generated `moc_*`/`uic`
  files...), `Thumbs.db`, duplicate `.rar` archives of `data/` and `doc/`,
  test levels and scenarios pointing to missing levels, unused fonts and 3D
  models, and duplicated screenshots.

### Bugs from 2005, fixed

* Saving a level in the editor stored the values already scaled by the
  difficulty, so every load/save made balls faster and heavier.
* Game over was handled twice, so every score entered the hall of fame twice.
* Campaign files skipped every other level.
* Clearing a level from inside the collision loop invalidated an iterator,
  and could crash on the next level.
* A tornado catching a player set a pointer of the player's gravity node to
  NULL instead of its speed: a crash.
* The four walls around the main floor shared two wall objects between four
  groups (a double free as soon as objects were deleted).
* The second player's climbing keys (R/F) moved the first player.
* Ladders turned by 90 degrees kept their unturned collision box after
  loading.
* State changes inside `glBegin`/`glEnd` were ignored, which left face
  culling disabled after drawing any box.
* Balls pushed apart after a collision always moved along +X.
* Two levels used enum numbers from an older build and could not be loaded.
* Many memory leaks: removed objects were never deleted.

### New

* `3dpang --demo LEVEL --frames N [--autofire] [--screenshot FILE]` plays a
  level without input: used by the `ctest` smoke tests.
* `3dpang-editor LEVEL --screenshot FILE` / `--resave FILE`.
* The editor shows the settings of the selected view and of the loaded
  power-up probabilities.
* **Tests**: an engine test (loaders, levels, campaigns, difficulty scaling,
  collision rules, a two-player stress run) and a converter test, besides the
  per-level smoke tests: 13 in total, 77% line coverage of the game and engine
  (`tools/coverage.sh`).
* **Documentation for people and AI agents** in `docs/`: product, architecture,
  code, developer manual and quality assurance.
* **Agentic workflow**: the `/new_feature` Claude Code skill with the
  `analyser`, `coder`, `qa` and `documenter` agents (`.claude/`).
* README with badges, project history, install/play/developer sections, the
  list of libraries with versions and credits.

## 1.0 - 2005-03-20

The project for the *Costruzione di Interfacce* course, University of Pisa
(tag `original_with_Eva`).
