# Level and campaign files

All paths in these files are relative to the data directory (`data/`).

## Levels (`data/levels/*.xml`)

The editor writes them, but they are plain XML. A trimmed example:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Level version="2" powerUpChance="90" bonusChance="78"
       weaponWeight="15" lifeWeight="5" fruitWeight="75"
       bombWeight="45" tornadoWeight="45" skybox="skyboxes/mount.jpg">
  <Views>
    <View index="0" x="25" y="20" z="91"/>   <!-- F3 ... index 5 = F8 -->
  </Views>
  <Ball texture="textures/pollonina.jpg" children="2" alpha="1">
    <Sphere radius="4" r="1" g="1" b="1" alpha="1" textured="1"/>
    <Bounce maxHeight="23.4375" gravity="-127.738"/>
    <Movement dx="-0.6655" dz="-3.3275"/>
    <Position x="13.5" y="23.4333" z="59.5"/>
  </Ball>
  <Environment kind="floor" texture="textures/skulls.jpg" main="1" destructible="0">
    <Floor cellsX="15" cellsZ="15" cellSize="5" thickness="1" textured="1" alpha="1">
      <Box dx="75" dy="1" dz="75" r="1" g="1" b="1"/>
    </Floor>
    <Position x="0" y="0" z="0"/>
    <Rotation x="0" y="1" z="0" angle="0"/>
  </Environment>
  <Environment kind="wall" texture="" main="0" destructible="1">
    <Wall textured="0" alpha="0.5"><Box dx="10" dy="5" dz="10" r="0" g="0" b="1"/></Wall>
    <Position x="30" y="4" z="30"/>
  </Environment>
  <Environment kind="ladder" texture="" main="0" destructible="0">
    <Ladder rungs="8"/>
    <Position x="12" y="1" z="40"/>
    <Rotation x="0" y="1" z="0" angle="90"/>
  </Environment>
  <Player>
    <Model file="" skin="models/md2/lbuu/buu4.png" lives="5"/>
    <Position x="30" y="1.5" z="37.5"/>
  </Player>
</Level>
```

### `Level`

| Attribute | Meaning |
|---|---|
| `powerUpChance` | % chance that a popped ball drops a power-up |
| `bonusChance` | % of those power-ups that are bonuses (the others are maluses) |
| `weaponWeight`, `lifeWeight`, `fruitWeight` | how often each bonus appears (they add up to about 100) |
| `bombWeight`, `tornadoWeight` | how often each malus appears (they add up to about 100) |
| `skybox` | base name of the sky box: the faces are `<name>_posx.jpg`, `_negx`, `_posy`, `_negy`, `_posz`, `_negz` |

### `Ball`

* `children`: how many balls it splits into when hit (difficulty adds more).
  Every hit makes the ball one unit smaller; balls smaller than 1 pop.
* `Sphere`: radius, colour (0-1), opacity; `textured` wraps `texture` around
  the ball.
* `Bounce`: `maxHeight` of the bounces and `gravity` (negative).
* `Movement`: speed on X and Z, in units per second.
* `Position`: centre of the ball at the start.

`Bounce` and `Movement` hold the values *before* the difficulty scaling:
higher difficulty makes balls faster, heavier and lower.

### `Environment`

* `kind`: `floor`, `wall` or `ladder`.
* `main`: the main floor and its four (invisible) outer walls.
* `destructible`: destroyed when a shot hits it.
* `Floor`: grid of `cellsX` × `cellsZ` cells of `cellSize` units, thickness,
  opacity; `Box` is the actual size and colour.
* `Wall`: opacity and `Box`.
* `Ladder`: number of `rungs` (0.7 units each).
* `Position`: the lowest corner of the object; `Rotation`: degrees around an
  axis (ladders turned by 90 degrees).

### `Player`

Up to two. `Model` gives the Quake II model (`.md2`, empty for the default
Little Buu), its skin (PNG) and the lives at *Medium* difficulty.

## Campaigns (`data/campaigns/*.campaign`)

A text file with one level file name per line, relative to `data/levels`,
played in order. Lines starting with `#` are comments.

```
# 3dPang campaign: one level file per line, relative to data/levels
bombs.xml
build.xml
woods.xml
bubble.xml
```

## Levels from 2005

The 2005 editor wrote a different format: Italian tag names (`GrPalla`,
`Pavimento`, `Rimbalzo`...), raw C++ enum numbers for the kind of object and
paths like `../data/gras.jpg`. Version 2 does not read it. Convert old files
with:

```sh
python3 tools/convert_legacy_level.py OldLevel.xml data/levels/new_level.xml
python3 tools/convert_legacy_level.py old_scenario.sce data/campaigns/new.campaign
```

The levels in `data/levels` were converted this way from the 2005 files
(see the `original_with_Eva` tag).
