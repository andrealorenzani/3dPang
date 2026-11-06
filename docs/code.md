# Code guide

> Audience: whoever edits the C++. Read [architecture.md](architecture.md) first for the big
> picture. Paths are relative to the repository root. Line counts: about 11 000 lines in `src/`.

## 1. Where is what

```
src/
  common/                 shared helpers, no game rules
    GlInclude.h           the only place that includes <GL/gl.h> (+ missing GL 1.2-1.4 enums)
    GlUtil.{h,cpp}        gluPerspective/LookAt/UnProject/Sphere/Cylinder replacements,
                          QImage->GL texture upload, loadTexture2D, cube map capability test
    VcgMath.h             includes vcglib in the right order; glGetModelViewMatrix shims
    Definitions.h         constants, enums (Weapon, BonusType, NodeType, Difficulty, Detail),
                          QDom helpers (addChild, setAttr)
    DataPaths.{h,cpp}     data directory discovery; resolve()/relativize(); userDir()
    Geometry.{h,cpp}      Box, Sphere, Spiral, Hook, Harpoon, ShadowRect, ShadowCircle,
                          (editor) PlayerMarker/MarkerOne/MarkerTwo
    Model3ds.{h,cpp}      3DS file parser + display-list renderer (bonus models)
    TextWriter.{h,cpp}    2D text via BMF; BMF_font.{h,c} = vendored library
  course/                 vendored course library (CICubeMap, CIMd2(+Normals), CITrackBall)
  scenegraph/             the engine
    Nodes.{h,cpp}         Node, Movement, TwisterMove, Bounce, Fall, Climb, Translation,
                          Rotation, Scaling, View, CubeMap, Attraction
    Environment.{h,cpp}   EnvObject, Floor, Wall, Ladder, (editor) Cell
    Actors.{h,cpp}        Ball, Player, Twister
    Projectiles.{h,cpp}   Projectile, HarpoonShot, QuadHarpoonShot, GunShot, BombBlast
    Bonus.{h,cpp}         Bonus + all power-ups
    Groups.{h,cpp}        Group and its subclasses (see below), load()/save()
    SceneGraph.{h,cpp}    lists, rules, collisions, shadows, level I/O, game flow, editing ops
  player/                 main.cpp (SDL loop, camera, HUD, --demo), Game (keys), Menu
  editor/                 main.cpp, MainWindow, EditorView, ViewPreview, Dialogs.h (+ 5 dialogs),
                          icons/ + editor.qrc
tests/                    engine_test.cpp, test_converter.py, fixtures/
tools/                    convert_legacy_level.py, coverage.sh
data/                     levels/ campaigns/ textures/ skyboxes/ models/ fonts/
```

## 2. Naming: the 2005 Italian → English map
Useful when reading old commits (`git log --follow`) or the `original_with_Eva` tag.

| 2005 | v2 | | 2005 | v2 |
|---|---|---|---|---|
| Gioco | Game | | Pavimento | Floor |
| Scrittore | TextWriter | | Muro | Wall |
| Nodo | Node | | Scala | Ladder |
| Spostamento | Movement | | Cella | Cell |
| Rimbalzo | Bounce | | Palla | Ball |
| Caduta | Fall | | Giocatore | Player |
| Arrampicata | Climb | | Proiettile | Projectile |
| Traslazione/Rotazione/Scalatura | Translation/Rotation/Scaling | | ArpioneA, QuadrArpione, Bomba, Pistola | HarpoonShot, QuadHarpoonShot, BombBlast, GunShot |
| Attrazione | Attraction | | Gr* (GrPalla...) | *Group (BallGroup...) |
| Paral, Sfera, Cerchio, Spirale, Gancio | Box, Sphere, ShadowCircle, Spiral, Hook | | difficolta, dettaglio | difficulty, detail |
| PLAYERCODE / EDITORCODE | PANG3D_PLAYER / PANG3D_EDITOR | | Vita, Frutta, Arma, ... | ExtraLife, Fruit, WeaponBonus... |

Code style: 4 spaces, K&R braces for blocks and Allman for function bodies, `camelCase` methods and
members, `PascalCase` types, `SCREAMING_CASE` constants/enums, namespace `pang` (course code in
`vcg`). Comments explain *why*; the 2005 comments were translated and condensed.

## 3. Patterns and strategy

| Pattern | Where | Notes |
|---|---|---|
| **Composition of behaviour nodes** | `Group` + `Node` subclasses | position floats live in the group; nodes bind to them by pointer |
| **Overloaded double dispatch (manual)** | `SceneGraph::collide(A*, B*)`, `apply(Bonus*, Player*)` | one overload per pair; `apply(Bonus*)` switches on `kind` then casts |
| **Conditional compilation as variants** | `PANG3D_PLAYER/EDITOR` | see architecture §3 |
| **Display-list caching** | `EnvObject::genList`, `Ball::genList`, `Model3ds::draw` | regenerate after resize/context loss |
| **Procedural geometry through the GL matrix stack** | `Spiral::generate`, `Twister::generate`, `ShadowCircle::generate` | points are computed by `glRotate/Translate` then `glGetModelViewMatrix` (2005 trick, kept) |
| **Lazy shared resources** | `Bonus::sharedModel` (static cache), `SceneGraph::loadTexture` | 3DS models and textures are loaded once |
| **Serialization by member functions** | `load(const QDomElement&)` / `save(QDomElement&)` | every saved class has both; difficulty scaling happens in `load` only |
| **State machine** | `Menu` (`state`, `pos`, `evaluate`) | add a state = new enum value + draw + evaluate case |
| **Deferred actions** | `SceneGraph::levelCompleted`, `EditorView::placeAt/pickAt` | never mutate lists while iterating them; never pick outside `paintGL` |

### Important conventions
* **Ownership is by raw owning pointer.** `delete` happens in destructors. Lists store raw
  pointers and the *removal site* calls `delete`. Never `list.erase` without `delete`.
* **Always unscaled vs scaled values**: `Bounce::baseMaxHeight/baseGravity`,
  `Movement::baseVx/baseVz`, `PlayerGroup::baseLives` hold level-file values (what the editor
  saves); the live ones are scaled by difficulty.
* **Player 2 always exists** after `loadLevel`; the player program deletes it in single-player.
  Many places check `player2 != nullptr`.
* **GL context must be current** when creating/deleting GL objects. Destructors of GL-owning
  objects (`Floor`, `Wall`, `Ladder`, `Ball`, `Twister`) call `glDelete*`. Free scenes before
  destroying the context (`clearLevel()` in `main`).
* **Collision reports sides in the order** `(right(+x), left(-x), up(+y), down(-y), near(+z), far(-z))`.
  `up/down` are about the *object's* motion; ladders never block players.
* **Coordinates**: Y is up; floors extend toward +X and +Z from the origin; the default camera
  looks at `(2.5·cellsX, 0, 2.5·cellsZ)` (the centre, as `cellSize = 5`).
* **Time** is in seconds (float). Gameplay constants are literals in context; balance formulas are
  listed in [product.md](product.md) §3.6, keep them in sync.

## 4. Key classes in more detail

### SceneGraph (`scenegraph/SceneGraph.cpp`, ~1600 lines)
Sections, in file order: shadows → drawing → collisions → update → game flow (player) → power-ups
(player) → editing (editor) → files. The `collide()` no-arg method is the collision driver.
`spawnPowerUp` implements the power-up probabilities. `loadLevel`/`saveLevel` are the only level
I/O. `startGame/nextLevel/victory/finishGame` drive campaigns and scores.

### Group hierarchy (`scenegraph/Groups.cpp`)
`Group::onCollision` forwards to whichever movement nodes exist (null checks). `PlayerGroup`
overrides it to stop only the component moving into the obstacle. `PlayerGroup::update` contains the
dying/respawn state machine. `EnvGroup::load` builds Floor/Wall/Ladder from XML. `ProjectileGroup`
does not move: the projectile object grows from the firing position.

### Nodes (`scenegraph/Nodes.cpp`)
Physics is explicit Euler with the `x += v·dt − ½·g·dt²` form. `Bounce` computes the launch speed
from the maximum height: `v = sqrt(2·|g|·(maxHeight − y))`. `Attraction` rotates the victim around
the tornado, pulls it to the centre and temporarily sets its gravity to 0.

### Player program (`player/main.cpp`)
`App` holds scene, text writer, menu, game. `drawScene` draws one or two viewports. `simulate`
splits the frame into ≤ 1/240 s steps. CLI: `--demo LEVEL --frames N [--autofire] [--screenshot F]`
is a headless harness used by tests.

### Editor (`editor/`)
`EditorView::paintGL` first updates the scene (only if animating), draws, then resolves queued
mouse requests. Dialog classes only *collect values* and call `EditorView::new*`. Dialogs are built
in code (no `.ui` files).

## 5. Recipes (how to change common things)

**Add a bonus** (e.g. a shield):
1. `Definitions.h`: add to `BonusType` if it needs a new `kind`.
2. `Bonus.h/.cpp`: subclass `Bonus` (`kind`, `points`, `getPos/getDim`, `glDraw`).
3. `SceneGraph.cpp`: add a case in `apply(Bonus*, ...)` and an `apply(Shield*, ...)` overload;
   add it to `spawnPowerUp` (weights).
4. Level format: if it needs a new weight attribute, add to `loadLevel`/`saveLevel`/editor tab,
   bump format notes in `LEVEL_FORMAT.md`.
5. Tests: extend `tests/engine_test.cpp` `stress()`; update `product.md` §3.4.

**Add a weapon**: enum `Weapon`; `Projectile` subclass; `Game::fire` switch; `WeaponBonus` subclass;
`product.md` §3.3.

**Add a level attribute**: parse in the relevant `load`, write in `save`, keep the unscaled
value if difficulty scales it, document in `LEVEL_FORMAT.md`, add a field to the editor if
user-editable, fixture in `tests/`.

**Add a menu entry**: `Menu::State`, a draw function, `evaluate()` case, `docs/PLAYER.md`.

**Add an editor dialog**: new class in `Dialogs.h` + `.cpp` (use `ColorSliders`, `TexturePicker`),
add to `CMakeLists.txt` editor sources, open it from `MainWindow`.

**Change balance**: edit the formula in the node/group *and* `product.md` §3.6.

**Add a dependency**: see architecture §11.

## 6. Pitfalls found while porting (read before debugging)
* `glBegin/glEnd` blocks ignore state changes (`glEnable/glDisable`): never toggle state inside.
* `Matrix44` in vcglib is column-major storage but `operator*(Point3)` applies translation;
  GL matrices are read with `glGetv(GL_MODELVIEW_MATRIX, m)`.
* Qt 6: `QImage` rows are top-down; GL wants bottom-up, hence `gl::toGlImage` flips.
* High-DPI: the editor uses `devicePixelRatioF()` for viewport/pick coordinates.
* `std::list::remove(ptr)` in 2005 code was replaced by `erase` + `delete`; do not reintroduce.
* Iterating `balls` while `collide(BallGroup*, ProjectileGroup*)` pushes new balls is safe only because
  `push_front` precedes the current iterator (kept deliberately; see `SceneGraph::collide`).
* A power-up `BombBonus` flash value is stored but the visible flash comes from `BombBlast`.

## 7. Future work (not done; candidates)
Smart pointers; replacing the player/editor `#ifdef` with two scene subclasses; a proper physics
step with swept collisions; shaders; sound; unit tests for collision overloads; editor tests;
undo in the editor; making UI strings translatable (`tr`).
