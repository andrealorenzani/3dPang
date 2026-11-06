# Architecture

> Audience: whoever changes structure, dependencies or the build. For classes and files
> see [code.md](code.md); for behaviour see [product.md](product.md).

## 1. System overview

```
            +-------------------+                 +----------------------+
            |  3dpang  (player) |                 |  3dpang-editor       |
            |  SDL3 window/loop |                 |  Qt6 widgets/dialogs |
            |  Menu, Game input |                 |  EditorView (QOpenGL)|
            +---------+---------+                 +----------+-----------+
                      |  -DPANG3D_PLAYER                     |  -DPANG3D_EDITOR
                      v                                      v
            +-------------------------------------------------------------+
            |   engine = SceneGraph + Groups + Nodes + Actors + ...       |
            |   src/scenegraph   (compiled twice, see 3)                  |
            +----------+----------------------+---------------------------+
                       |                      |
            +----------v---------+  +---------v------------+  +-----------+
            | src/common         |  | src/course           |  | vcglib    |
            | GL helpers, text,  |  | skybox, MD2, trackbal|  | (header)  |
            | 3DS, paths, shapes |  | (course library)     |  | Point/Mat |
            +----------+---------+  +---------+------------+  +-----+-----+
                       +----------+-----------+---------------------+
                                  v
                       Qt6 Core/Gui/Xml (images, XML)  +  OpenGL (compat. 2.1)
```

Both programs are single-threaded. There is no network and no audio.

## 2. Libraries and versions

| Library | Version | Role | Where it enters |
|---|---|---|---|
| C++ | C++17 (`CMAKE_CXX_STANDARD 17`, extensions off) | language | all |
| Qt | ≥ 6.4 required, built/tested with **6.8.3** | `QImage` (texture loading), `QDom*` (XML), `QFile`; editor GUI | Core, Gui, Xml (engine); Widgets, OpenGL, OpenGLWidgets (editor) |
| SDL | **3.4.20** (`release-3.4.20`, system SDL3 if found) | player window, GL context, keyboard | `src/player/` only |
| vcglib | **2025.07** (tag, downloaded) | `Point2/3/4`, `Matrix44`, GL wrappers (`glVertex(p)`, `glGetv`) | `src/common/VcgMath.h` |
| Eigen | 3.4.0 (bundled in vcglib) | required by vcglib's `Matrix44` | indirect |
| OpenGL | 1.3+ compatibility profile (2.1 requested) | rendering | everywhere |
| BMF_font | 1.01 (2003, vendored, LGPL) | bitmap font rendering | `src/common/BMF_font.*` |
| Course library | 2002-2004 (vendored) | cube map, MD2 loader, trackball | `src/course/` |

Removed from 2005: GLEW, GLU, lib3ds, Qt 3, SDL 1.2. Replacements: `GlUtil` (GLU functions),
`GlInclude.h` (GL enums), `Model3ds` (3DS parser).

## 3. Two programs, one engine: conditional compilation

`SceneGraph`, `Groups`, `Nodes`, `Environment`, `Actors`, `Geometry`, `Bonus`... exist once on
disk but are compiled **twice**, as two static libraries (`CMakeLists.txt`,
`pang3d_add_engine`):

| Target | Define | Adds | Removes |
|---|---|---|---|
| `pang3d_engine_player` | `PANG3D_PLAYER` | shots, bonuses applying, tornado, climbing, high scores, level queue | picking, cells, saving |
| `pang3d_engine_editor` | `PANG3D_EDITOR` | cells/picking, `save()`, markers, rewind, `new*()` | shots, tornado, scoring |

Rule: **gameplay in `#ifdef PANG3D_PLAYER`, editing in `#ifdef PANG3D_EDITOR`, the rest is shared.**
This is the 2005 design (`PLAYERCODE`/`EDITORCODE`) kept on purpose to avoid a rewrite.
Consequence: a header change can compile in one variant and break the other. Always build both.

## 3b. Build system (CMake)
* `CMakeLists.txt`: dependency discovery (Qt, OpenGL, vcglib via `FetchContent`, SDL3 via
  `find_package` then `FetchContent`), the two engine libraries, the two executables, tests,
  install rules, CPack.
* `data/` is copied next to the executables after each build (`pang3d_copy_data`).
* `CMakePresets.json`: `debug`, `release`, `vcpkg` presets (`QT_DIR` selects Qt).
* `.github/workflows/build.yml`: Linux/Windows/macOS build; Linux runs the tests under Xvfb;
  a `v*` tag publishes a GitHub release with packages and the top `CHANGELOG.md` section.

## 4. The scene graph

Despite the name it is not a tree: it is a set of **lists of groups**, each group owning a few
**nodes** that change its position.

```
SceneGraph
 +- lists: balls<BallGroup*>, environment<EnvGroup*>, shots<ProjectileGroup*>,
 |         bonuses<BonusGroup*>
 +- player1, player2 : PlayerGroup*      twister : TwisterGroup* (0 or 1)
 +- cubeMap, views[6], defaultView, shadow circles, texture cache
 +- rules: difficulty, detail, power-up weights, level queue, scores (player)
```

```
Group (base)                     owns                     drawable object
 posx,posy,posz  <--bound-->  Translation, Rotation, Scaling
                              Movement | Bounce | Fall | TwisterMove | Climb
                              Attraction (while in a tornado)
 BallGroup      -> Ball -> Sphere
 EnvGroup       -> EnvObject: Floor(Box,Cells) | Wall(Box) | Ladder(Boxes)
 PlayerGroup    -> Player -> CIMd2 (Quake II model) [+ PlayerMarker in editor]
 ProjectileGroup-> Projectile: HarpoonShot | QuadHarpoonShot | GunShot | BombBlast
 BonusGroup     -> Bonus: ExtraLife | Apple/Pear/Banana | Harpoon/Quad/GunBonus | Tornado/BombBonus
 TwisterGroup   -> Twister (particle cloud)
```

**Binding by pointer**: a group stores its position as three floats; nodes hold pointers to them
(`Node::bindCoord`). A node therefore moves "its" object without knowing it. This is why groups
are non-copyable and why changing a node (e.g. `setFall`) must re-bind (`Group::setFall` does).

**Ownership**: groups own their nodes and drawable (raw owning pointers, deleted in destructors);
the scene owns the groups' lists. Attractions give gravity back in their destructor, so `delete
attraction` must happen before the gravity owner dies (`Group::~Group` does).

## 5. Frame loop (player)

```
SDL events -> Menu or Game::onKey
if menu active      : Menu::draw
elif game over      : SceneGraph::finishGame -> Menu hall of fame
else
   dt = SceneGraph::tick()
   repeat in steps <= 1/240 s (max dt 0.25 s):  SceneGraph::update(step); SceneGraph::collide()
   drawScene(): per player viewport: perspective + lookAt, SceneGraph::glDraw(), HUD
SDL_GL_SwapWindow
```

`update`: removes destroyed environment, updates balls (and clamps them back to the main floor),
bonuses, shots, players, the tornado. `collide`: balls vs everything, environment vs players /
bonuses / shots / tornado, bonuses vs players, removes finished shots, then (last!) moves to the next
level if cleared (never mid-iteration).

## 6. Rendering
* Fixed-function pipeline: lighting + `GL_COLOR_MATERIAL`, display lists for static geometry
  (floors, walls, ladders, balls) rebuilt by `regenerateDisplayList` after context changes.
* **Shadows** (`SceneGraph::glDraw`): for each environment object the stencil buffer marks the
  visible top face while drawing it; shadows (balls' circles, floors' rectangles) are then drawn
  only where stencil = 1, with depth test off.
* **Sky box**: cube map (`CICubeMap`) drawn first, rotation-only; falls back to six textures if
  cube maps are unavailable.
* **Text**: BMF bitmap font in a 2D ortho pass (`TextWriter`).
* **Split screen**: two viewports, scene drawn twice.
* Textures: loaded once per file into a cache (`SceneGraph::loadTexture`).
* The GL context must be current whenever GL objects are created or deleted (the editor calls
  `makeCurrent()` around such operations).

## 7. Collision
All in `SceneGraph::collide(...)` overloads, one per pair type. Shapes: ball = sphere,
player = cylinder (radius 1.5, height 3.8), bonus = sphere, shot = cylinder, floors/walls = AABB,
tornado = cylinder. Ball vs box checks the bounding cube, then edge distances on three
projections; a collision is reported as six booleans (±x, ±y, ±z) to the group's nodes
(`Group::onCollision`), which flip velocities. No broad phase: lists are short (< 100 objects).

## 8. Data and persistence
* **Paths**: every path stored in data files is relative to the data directory. `DataPaths`
  resolves them (env `PANG3D_DATA_DIR`, next to the executable, install location, source tree).
  It also accepts the legacy `../data/...` form.
* **Level files**: XML parsed with `QDomDocument` in `SceneGraph::loadLevel`, written by
  `saveLevel` (editor). Each group class has `load(QDomElement)` / `save(QDomElement&)`.
  Difficulty scaling is applied on load; the *unscaled* value is kept (`base*` members) so saving
  round-trips.
* **Campaigns**: text file, one level per line.
* **High scores**: `highscores.xml` in `QStandardPaths::AppDataLocation`.

## 9. Editor architecture
`MainWindow` (tabs, menus, toolbar) -> `EditorView : QOpenGLWidget` (owns a `SceneGraph`, camera,
trackball, mouse handling) -> dialogs (`Dialogs.h`: reusable `ColorSliders`, `TexturePicker`).
`ViewPreview` is a second small GL widget. Mouse picks are resolved *inside* `paintGL` because
they need that frame's depth buffer (`get3dPos`). Objects being placed follow the mouse until a click.

## 10. Design decisions (and why)
| Decision | Reason |
|---|---|
| Keep fixed-function OpenGL | the whole renderer relies on it; a shader port is a rewrite with no gameplay gain |
| SDL3 for the player, Qt for the editor | as in 2005 (SDL game / Qt tool), each is the lightest fit |
| Replace GLEW/GLU/lib3ds with small in-tree code | unmaintained or unnecessary; fewer dependencies for a build that must work on three OSes |
| Same engine compiled twice | preserves the 2005 design; avoids duplicating the scene model |
| Level format rewritten in English | the 2005 format stored C++ enum numbers that changed between builds |
| Raw owning pointers kept | matches the code style; migrating to smart pointers would touch every file (see code.md "future work") |
| vcglib from upstream, not vendored | stays updatable; only headers are used |

## 11. Constraints for changes
1. Keep both variants compiling (`PANG3D_PLAYER`, `PANG3D_EDITOR`).
2. No new dependency without updating `CMakeLists.txt`, `docs/BUILDING.md`, README badges and CI.
3. New data paths: data-relative only; resolve through `paths::resolve`.
4. New user-visible numbers (balance): put the formula in `product.md` §3.6.
5. Level format changes bump `version` and need a migration note in `LEVEL_FORMAT.md`.
