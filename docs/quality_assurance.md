# Quality assurance

> What is tested, how, how much, and what is *not*. Numbers were measured on 2026-11-06 with
> `tools/coverage.sh` (gcc 13, Qt 6.8.3, Linux). Re-measure after changes and update §3.

## 1. Test inventory (13 CTest tests)

| Test | Type | Source | What it checks |
|---|---|---|---|
| `engine` | in-process, needs GL context (hidden SDL window) | `tests/engine_test.cpp` | see below |
| `legacy_converter` | script | `tests/test_converter.py` + `tests/fixtures/legacy_level.xml` | 2005 → v2 level conversion structure |
| `demo_<level>.xml` ×11 | end-to-end smoke | `3dpang --demo LEVEL --frames 1200 --autofire` | each shipped level loads, simulates 20 s while shooting, renders every frame, exits 0 |

### `engine` test cases
1. **models**: the four 3DS bonus models parse (radius > 0); a missing file fails cleanly.
2. **levels**: every file in `data/levels` loads (≥ 11), has two players, a main floor and ≥ 5 main
   objects; a nonexistent file and a 2005-format file are refused.
3. **campaign**: `classic.campaign` yields 4 levels; a single selected level yields 1.
4. **difficulty scaling**: ball speed ↑, gravity ↑, bounce height ↓ monotonically from difficulty 0 to 4.
5. **collision rules**: a ball never falls through the main floor in 20 s; a ball on a player kills it.
6. **stress**: 6000 frames, two players, tornadoes, bombs, all weapons, extra lives applied repeatedly,
   with rendering → exercises tornado/attraction, bomb blast, shots, shadows, bonus drawing, game-over,
   high-score write.

## 2. How to run
```sh
ctest --test-dir build/release --output-on-failure        # everything
ctest --test-dir build/release -R demo_ -j4               # smoke only
```
Headless: `SDL_VIDEO_DRIVER=offscreen` (needs EGL; default) or `-DPANG3D_TEST_VIDEO_DRIVER=x11` +
`xvfb-run`. CI (`.github/workflows/build.yml`) runs all tests on Linux under Xvfb; Windows and macOS only
build (their runners have no GPU/EGL guarantee).
Sanitizers: build with `-fsanitize=address,undefined` and run `3dpang-tests` and a few demos; the
last full run (2026-11-05, 5 levels × 40 s + stress with 10 tornadoes/9 bombs) reported **no** errors.

## 3. Coverage (line coverage of `src/`, player + engine build)

| Area | Lines | Notes |
|---|---|---|
| `scenegraph/Groups.cpp` | 98.5 % | |
| `scenegraph/Nodes.cpp` | 96.7 % | |
| `common/Geometry.cpp` | 96.2 % | |
| `common/Model3ds.cpp` | 91.8 % | |
| `scenegraph/SceneGraph.cpp` | 85.5 % | uncovered: some edge-collision branches, campaign errors |
| `scenegraph/Bonus.cpp` | 82.8 % | |
| `scenegraph/Environment.cpp` | 81.7 % | editor-only cell code is not in this build |
| `common/BMF_font.c` | 80.6 % | |
| `scenegraph/Projectiles.cpp` | 64.1 % | |
| `player/main.cpp` | 65.0 % | window/menu/event paths not reached by `--demo` |
| `common/GlUtil.cpp` | 65.0 % | `unProject`, cylinder (editor/bomb) |
| `player/Game.cpp` | 63.2 % | only keys sent by `--autofire` |
| `course/CICubeMap.cpp` | 55.0 % | fallback six-texture path not run |
| `common/DataPaths.cpp` | 54.7 % | |
| `player/Menu.cpp` | **1.8 %** | **not tested** (needs input events) |
| **Total (player + engine)** | **77.3 %** (2964 / 3833) | |

Not measured: the **editor** (~2 100 lines: `src/editor/*`, editor-only branches of the engine) has
**no automated tests**; it was verified manually (open a level, resave: identical XML; screenshots
of the main window). Reproduce the table with `tools/coverage.sh`.

## 4. What is NOT tested (known gaps, by priority)
1. **Editor**: picking, dragging, dialogs, save → load round trip beyond the manual resave check.
   Idea: a Qt Test target using `QT_QPA_PLATFORM=offscreen` for the dialogs and `--resave` diff in CI.
2. **Menu** and keyboard handling (`Menu.cpp`, most of `Game.cpp`): simulate SDL events with
   `SDL_PushEvent`.
3. **Visual regression**: screenshots are produced (`--screenshot`) but not compared.
4. **Collision edge cases** (ball on box edges/corners, tunnelling at high speed).
5. **Windows/macOS runtime**: built in CI but not run.
6. **Balance**: formulas in `product.md` §3.6 are only partially asserted (item 4 of `engine`).
7. Performance (no benchmark; 60 FPS observed on a desktop GPU).

## 5. Manual test checklist (before a release)
Player: start → menu navigation; New Game 1 and 2 players; each difficulty; each detail; fire all
weapons (pick up each bonus); climb a ladder; die and respawn; game over → Hall of Fame entry; win a
campaign; resize/fullscreen; Esc.
Editor: new level; add balls/floor/wall/ladder; select, drag, wheel, right-click ladder; colours and
textures; place both players; set views F3-F8 and see them in game; power-up tab; save, reopen, play;
add to campaign; snapshot; sky box change.
Data: every level loads in the editor and the player.

## 6. Definition of done for a change
* Builds warning-free in both variants, on Linux at least (CI for the others).
* All 13 tests pass; new behaviour has a test (or a stated reason why not).
* Coverage does not drop by more than 1 point on files touched; update §3 if it moves.
* Sanitizer run clean for changes touching ownership or collisions.
* `docs/` and `CHANGELOG.md` updated.

## 7. Defects fixed so far (regression ideas)
See the "Bugs from 2005, fixed" list in `CHANGELOG.md`. Each is a candidate regression test; those with
tests: level scaling round-trip (`--resave`, manual), ball/player collisions, campaign parsing,
tornado victim handling (`stress`).
