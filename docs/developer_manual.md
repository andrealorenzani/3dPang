# Manual: install, use, change

> Three audiences: **players** (install and play), **level designers**, **developers**.
> Platform-specific build details live in [BUILDING.md](BUILDING.md); this manual is the guided tour.

## 1. For players

### Install
* **From a release**: download the archive for your system from the
  [GitHub releases](https://github.com/andrealorenzani/3dPang/releases) (Windows `.zip`, Linux
  `.tar.gz`), unpack it, run `bin/3dpang` (game) or `bin/3dpang-editor`.
  Linux needs Qt 6 libraries (`qt6-base` / `libqt6widgets6`...), an OpenGL driver and a desktop session.
* **From source**: see §3.

### Play
Run `3dpang`. Arrows + Enter in the menu, Esc to go back. Choose *New Game* → number of players.
Controls and rules: [PLAYER.md](PLAYER.md), [product.md](product.md).

### Where things are stored
High scores: `highscores.xml` in the application data folder
(`~/.local/share/3dPang/3dPang/` on Linux, `%APPDATA%` on Windows, `~/Library/Application Support`
on macOS). Delete it to reset the Hall of Fame.

### Troubleshooting
| Symptom | Try |
|---|---|
| "data directory not found" | set `PANG3D_DATA_DIR` to the folder containing `levels/` |
| window opens but black | the driver lacks a legacy/compatibility OpenGL context; update drivers; on Wayland try `SDL_VIDEO_DRIVER=x11` |
| no levels in the menu | the `data/levels` folder is missing from the install |
| very slow | menu *Detail* → Low |
| `3dpang --help` | prints the data directory in use |

## 2. For level designers
1. Run `3dpang-editor` (or `3dpang-editor data/levels/woods.xml`).
2. Set the main floor, add floors/walls/ladders/balls, place the players, set the views and the
   power-up frequencies. Guide: [EDITOR.md](EDITOR.md). Save into `data/levels/`.
3. *File → Add to Campaign* to chain levels. A campaign is a text file in `data/campaigns/`.
4. Test: `3dpang --demo mylevel.xml --frames 600 --autofire --screenshot shot.png`, or play it from the menu.
5. Old (2005) levels: `python3 tools/convert_legacy_level.py OLD.xml data/levels/new.xml`.
Format: [LEVEL_FORMAT.md](LEVEL_FORMAT.md).

## 3. For developers

### 3.1 Get the code and build
```sh
git clone https://github.com/andrealorenzani/3dPang.git && cd 3dPang
export QT_DIR=$HOME/Qt/6.8.3/gcc_64            # your Qt 6 kit
cmake --preset release && cmake --build --preset release
ctest --preset release                         # 13 tests, about 1 minute
```
Prerequisites and OS-specific packages: [BUILDING.md](BUILDING.md). First configure downloads vcglib and, if
needed, SDL3 (internet + git). Offline: install SDL3, pass `-DPANG3D_VCGLIB_DIR=/path/to/vcglib`.

### 3.2 Import into an IDE
| IDE | Steps |
|---|---|
| **CLion** | *Open* the folder; *Settings → Build → CMake*: use the `debug` preset profile; add `QT_DIR` to the profile environment (or `-DCMAKE_PREFIX_PATH=...`). Targets `3dpang`, `3dpang-editor`, `3dpang-tests` appear. |
| **Visual Studio 2022** | *File → Open → Folder*; set `QT_DIR` (system env or `CMakeUserPresets.json`). |
| **Qt Creator** | open `CMakeLists.txt`, choose a Qt 6 kit. |
| **VS Code** | CMake Tools extension, pick the preset. |
`build/` is git-ignored.

### 3.3 Day-to-day commands
| Task | Command |
|---|---|
| Build one target | `cmake --build build/release --target 3dpang` |
| Run the player on a level | `build/release/3dpang --demo woods.xml --frames 300 --screenshot /tmp/w.png` (use `SDL_VIDEO_DRIVER=offscreen` without a display) |
| All tests | `ctest --test-dir build/release --output-on-failure` |
| One test | `ctest --test-dir build/release -R engine` |
| Coverage | `QT_DIR=... tools/coverage.sh` |
| Sanitizers | configure with `-DCMAKE_CXX_FLAGS="-fsanitize=address,undefined"` and run `3dpang-tests` and the demos |

### 3.4 Making a change (the project rules)
1. Read [architecture.md](architecture.md) §11 and the relevant recipe in [code.md](code.md) §5.
2. Keep **both** variants compiling (player and editor).
3. Add or update a test (see [quality_assurance.md](quality_assurance.md)).
4. Update the docs in `docs/` that the change touches (product rules, architecture, code map,
   QA tables) and add a line to `CHANGELOG.md`.
5. English only, comments explain *why*, no Italian identifiers.
The same flow is automated by the agentic workflow: run **`/new_feature <description>`** in Claude Code
(see [§5](#5-agentic-workflow)).

### 3.5 Releasing
1. Update `CHANGELOG.md` (new `## X.Y - date` section at the top: it becomes the release notes),
   `project(... VERSION X.Y.Z)` in `CMakeLists.txt`, and the version badge in `README.md`.
2. Commit, push, wait for CI to be green on all three platforms.
3. `git tag -a vX.Y -m "3dPang X.Y" && git push origin vX.Y`. The workflow builds the packages and
   creates the GitHub release.

### 3.6 Environment variables
| Variable | Effect |
|---|---|
| `PANG3D_DATA_DIR` | data directory override |
| `SDL_VIDEO_DRIVER` | `x11`, `wayland`, `offscreen`... for the player |
| `QT_QPA_PLATFORM` | Qt platform for the editor (`xcb`, `wayland`, ...) |

### 3.7 Command line reference
```
3dpang [--demo LEVEL.xml [--frames N] [--autofire] [--screenshot FILE.png]] [--help]
3dpang-editor [LEVEL.xml] [--screenshot FILE.png] [--resave OUT.xml]
python3 tools/convert_legacy_level.py OLD.xml|OLD.sce NEW
tools/coverage.sh [build-dir]
```

## 4. Historic versions
The 2005 sources are the tag and branch `original_with_Eva` (Visual Studio .NET 2003, Qt 3.2, SDL 1.2,
GLEW, lib3ds). `git switch original_with_Eva` or `git checkout refs/tags/original_with_Eva`.

## 5. Agentic workflow
`.claude/` contains a Claude Code workflow for changes:

| Piece | File | Role |
|---|---|---|
| Skill `/new_feature` | `.claude/skills/new_feature/SKILL.md` | orchestrates the steps below |
| Agent `analyser` | `.claude/agents/analyser.md` | reads `docs/`, plans the change (product / architecture / code / QA impact) |
| Agent `coder` | `.claude/agents/coder.md` | implements the plan and reports the diff |
| Agent `qa` | `.claude/agents/qa.md` | builds, tests and checks the result against the plan |
| Agent `documenter` | `.claude/agents/documenter.md` | cheap model; updates `docs/`, `CHANGELOG.md`, README badges |

Usage: `/new_feature make the gun shoot three bullets in a fan`. The plan is shown for approval
before code is written; the loop coder ⇄ qa repeats until QA passes or a stop condition is
reached. Keep `docs/` accurate: the agents trust it.
