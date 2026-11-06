# Product: what 3dPang is

> Audience: anyone (human or AI agent) who must decide *what* the game should do.
> For *how* it is built see [architecture.md](architecture.md) and [code.md](code.md).
> Numbers quoted here are taken from the code; the file in brackets is where to verify them.

## 1. In one paragraph

3dPang is a 3D take on the arcade classic *Pang* (Mitchell, 1989). Balls bounce
around a level made of floors, walls and ladders. The player shoots them: a hit
ball shrinks and splits into smaller balls until they pop. Popped balls can drop
**power-ups** (bonuses and maluses). The level is cleared when no ball is left.
One or two players (split screen) play levels one by one in a **campaign**, at one of
five **difficulties** and three **detail levels**. A separate **editor** creates levels.
It was written in 2004-2005 by Andrea Lorenzani and Eva Tamburini for the
*Costruzione di Interfacce* course (University of Pisa); version 2.0 (2026) is a port with
the same gameplay (see [CHANGELOG.md](../CHANGELOG.md)).

## 2. The two programs

| Program | Purpose | Technology |
|---|---|---|
| `3dpang` (player) | menu, game, split screen, high scores | SDL3 window, legacy OpenGL |
| `3dpang-editor` | create and edit levels and campaigns | Qt 6 widgets, legacy OpenGL |

They share the engine (`src/scenegraph`, `src/common`, `src/course`), compiled twice with
different feature switches (see architecture).

## 3. Game rules

### 3.1 Goal and flow
1. Menu: choose level or campaign, difficulty, detail, number of players.
2. Play. A level is **won** when the last ball is popped. In a campaign the next level
   loads immediately; lives are kept between levels.
3. A player **dies** when touched by a ball (a dying player is invulnerable for the
   death animation). After 1 s the corpse lies down; after 3 s the player respawns if lives
   are left (one life is consumed). Players cannot be hit while dying.
4. **Game over** when every player is dead with 0 lives. Winning the last level also
   ends the game ("victory"): the remaining lives are converted to points.
5. Scores enter the **Hall of Fame** (top 10, stored per user, see §8).

### 3.2 Balls
* A ball has a radius, colour, optional texture, a horizontal speed, a bounce height and gravity.
* Hit by a shot: radius −1, it splits into `children + difficulty − 1` balls that fly
  apart like a star; every new ball inherits colour, texture, bounce height and gravity.
  A ball with radius < 1 pops.
* Balls collide elastically with each other (mass ∝ r³), bounce off floors/walls, and are
  pulled by tornadoes.
* Bounce height after a hit is reduced proportionally to the radius, never below
  `5 + radius` (so the player can always jump under/over it).

### 3.3 Weapons
| Weapon | Behaviour | Shots in play |
|---|---|---|
| Harpoon (start) | a rope with a hook climbing straight up; lasts `2 + 0.3·difficulty` s | 1 at the start |
| Quad harpoon | four harpoons, 3× wider | 1 at the start |
| Gun | fast, thin bullets; lasts `2.5 + 0.5·difficulty` s | 5 on pickup |

A shot stops at the first floor/wall/ball it hits. Hitting a *destructible* floor or
wall destroys it. Picking up the weapon you already have adds shots (+1 harpoons, +3 gun).
At *Easy* and *Medium* a dead player can still fire.

### 3.4 Power-ups
A popped ball drops one with probability `powerUpChance` %. It is a **bonus** with
probability `bonusChance − 5·difficulty` %, otherwise a **malus**. It falls to the ground and
disappears after `10 − difficulty` seconds (blinking during the last 5).

| Kind | Type | Effect | Points |
|---|---|---|---|
| Heart | bonus | +1 life | 200 |
| Apple / Pear / Banana | bonus (fruit) | points only | 50 / 150 / 100 |
| Harpoon / Quad harpoon / Gun | bonus (weapon) | see 3.3 | 100 / 150 / 150 |
| Tornado | malus | a roaming tornado: catches balls and players, spins and drags them; stronger if one already exists (+180°/s, +10 s, +3 power) | 100 |
| Bomb | malus | a flash that hits every ball (radius > 1) once, then the screen turns red for a few seconds (colour mask) | 170 |

Which bonus: heart, fruit or weapon are drawn by `lifeWeight`, `fruitWeight`,
`weaponWeight` (level attributes); bomb vs. tornado by `bombWeight`/`tornadoWeight`.
Points are reduced by `5·difficulty` when picked up. Points for the tornado and bomb go to the
player who picked them up.

### 3.5 Environment
* **Floor**: a grid of cells; can be fixed or destructible (half transparent in the editor).
* **Wall**: a box; fixed or destructible. Four invisible walls close the main floor.
* **Ladder**: walk to it and press the climb keys; gravity is off while on it. Rungs and
  orientation (0° or 90°) are editable.
* **Shadows**: floors cast soft shadows on what is below, balls cast circular shadows (the
  aiming aid). Stencil-buffer based.

### 3.6 Difficulty (menu order 0-4: Easy, Medium, Hard, Nightmare, Superhuman)
Let *d* be the number. All formulas are in `Nodes.cpp`, `Groups.cpp`, `SceneGraph.cpp`, `Game.cpp`.

| Aspect | Formula |
|---|---|
| Ball speed (x, z) | `base · (1 + 0.1·d)` |
| Ball gravity | `base · (1 + 0.7·d)` |
| Ball bounce height | `base · (1.5 − 0.25·d)`, minimum 5 |
| Balls after a hit | `children + d − 1` |
| Points per hit | `radius · (100 − 15·d)` |
| Player speed | `12 − 1.2·d` |
| Player lives | `levelLives − d/2` (integer division) |
| Climb speed | `6.5 − 0.5·d` |
| Fall gravity | `−9.8 + d/2` |
| Power-up lifetime | `10 − d` s |
| Bonus chance | `bonusChance − 5·d` % |
| Harpoon duration / gun duration | `2 + 0.3·d` / `2.5 + 0.5·d` s |
| Bomb flash | `1 + 0.2·d` s |
| Tornado | stronger and spinning faster with d |
| Ball shadows | disabled at Superhuman (d = 4) |
| Victory bonus | remaining lives × `(1100 − 100·d)` |

### 3.7 Detail levels
| Detail | Textures | Sky box | Shadows | Ball polygons |
|---|---|---|---|---|
| Low | no | no | no | 6 |
| Medium | yes | no | simple | 12 |
| High | yes | yes | full | 24 |

## 4. Controls

| Action | Player 1 | Player 2 |
|---|---|---|
| Move | arrows | W A S D |
| Fire | M | Q |
| Ladder up / down | J / N | R / F |
| Fixed view | F2 | F9 |
| Following views (set by the level) | F3 F4 F5 | F6 F7 F8 |
| Back / leave game | Esc | |

Menu: up/down, Enter, Esc. Single-player uses player 1 and removes player 2.

## 5. Content shipped (`data/`)
* **Levels** (`data/levels/*.xml`): bombs, bubble, build, fruits, level1, level2, mixed,
  testbed, tornado, weapons, woods.
* **Campaign** `data/campaigns/classic.campaign`: bombs → build → woods → bubble.
* **Textures**, **sky boxes** (flood, haze, mount, snow, uffizi), **player models** (Little Buu
  default, Hobgoblin; Quake II MD2 + PNG skins), **bonus models** (heart, apple, banana,
  pear; 3DS), **fonts** (BMF bitmap).
* Format reference: [LEVEL_FORMAT.md](LEVEL_FORMAT.md).

## 6. The editor (summary)
Tabs: Balls, Floors (and ladders), Walls, Players, Views, Power-ups. Mouse: left-click selects
(floor cells, balls, walls, ladders), drag moves on X/Z, wheel moves on Y (or changes ladder
rungs), right-click turns a ladder, double-click re-centres the camera, trackball mode rotates.
Toolbar: new/open/save, pause/rewind the preview physics, shadows, zoom, snapshot,
sky box. *File → Add to Campaign*. Full guide: [EDITOR.md](EDITOR.md).

## 7. Non-goals and known limits
* No sound, no network, no gamepad. Keyboard only.
* Fixed-function OpenGL only (compatibility profile); no shaders.
* Collision is simple (spheres, boxes, cylinders): fast bouncing balls on thin floors can
  tunnel; the player loop uses 1/240 s sub-steps to limit it.
* The 2005 level files are not read directly: use `tools/convert_legacy_level.py`.
* The editor has no undo.
* English only; all UI strings are literals in `src/player/Menu.cpp` and `src/editor/`.

## 8. Persistence
* Levels and campaigns: text files in `data/` (read-only for the player).
* High scores: `highscores.xml` in the user's app-data folder
  (`~/.local/share/3dPang/3dPang/` on Linux). Not in `data/`.

## 9. Glossary
**Group**: a scene object (ball, floor, player...) with its transformation nodes.
**Malus / bonus**: harmful / helpful power-up. **Main floor**: the base floor of a level
(never deleted). **Skybox**: background cube map. **Campaign**: ordered list of levels.
