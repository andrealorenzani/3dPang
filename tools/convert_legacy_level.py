#!/usr/bin/env python3
"""Convert 3dPang levels/campaigns from the 2005 (v1, Italian) format to v2.

The 2005 editor saved levels as XML with Italian tag names (``GrPalla``,
``Pavimento``, ``Rimbalzo``...), environment kinds stored as raw C++ enum
numbers (which changed between builds, so some old files are unreadable) and
paths relative to the old ``PangPlayer`` working directory (``../data/...``).

Version 2 uses English names, symbolic kinds (``floor``/``wall``/``ladder``)
and paths relative to the game data directory.

Usage:
    convert_legacy_level.py OLD_LEVEL.xml NEW_LEVEL.xml
    convert_legacy_level.py OLD_SCENARIO.sce NEW.campaign
"""
import os
import sys
import xml.etree.ElementTree as ET

SKYBOX_BASES = {"flood", "haze", "mount", "snow", "uffizi"}


def remap_path(path, kind):
    """Map an old "../data/foo.jpg" path to the v2 data-relative layout."""
    if not path:
        return ""
    name = os.path.basename(path.replace("\\", "/"))
    if kind == "skybox":
        return "skyboxes/" + name.lower()
    if kind == "model":
        # The 2005 tris.MD2 was in fact the Hobgoblin mesh (see CHANGELOG),
        # but levels always meant the default player: use the default.
        return ""
    if kind == "skin":
        return "models/md2/lbuu/" + os.path.splitext(name)[0].lower() + ".png"
    stem, ext = os.path.splitext(name)
    stem_l = stem.lower()
    # Textures that were faces of a skybox are still found in skyboxes/
    for base in SKYBOX_BASES:
        if stem_l.startswith(base + "_"):
            return "skyboxes/" + stem_l + ext.lower()
    return "textures/" + name[: -len(ext)] + ext.lower() if ext else "textures/" + name


def sub(parent, tag, **attrs):
    return ET.SubElement(parent, tag, {k: str(v) for k, v in attrs.items()})


def copy_xyz(src, dst_parent, tag):
    if src is None:
        return sub(dst_parent, tag, x=0, y=0, z=0)
    return sub(dst_parent, tag, x=src.get("x", "0"), y=src.get("y", "0"), z=src.get("z", "0"))


def convert_level(root):
    if root.tag != "SceneGraph":
        raise ValueError("not a 2005 3dPang level (root is <%s>)" % root.tag)
    lvl = ET.Element("Level", {
        "version": "2",
        "powerUpChance": root.get("pu", "0"),
        "bonusChance": root.get("pp", "0"),
        "weaponWeight": root.get("barmi", "0"),
        "lifeWeight": root.get("bvite", "0"),
        "fruitWeight": root.get("bfrutta", "0"),
        "bombWeight": root.get("bbombe", "0"),
        "tornadoWeight": root.get("btornado", "0"),
        "skybox": remap_path(root.get("cubeMapTexture", ""), "skybox"),
    })
    for child in root:
        if child.tag == "Viste":
            views = sub(lvl, "Views")
            for v in child.findall("Vista"):
                sub(views, "View", index=v.get("numVista", "0"), x=v.get("x", "0"),
                    y=v.get("y", "0"), z=v.get("z", "0"))
        elif child.tag == "GrPalla":
            ball = sub(lvl, "Ball", texture=remap_path(child.get("texture", ""), "texture"),
                       children=child.get("numballs", "0"), alpha=child.get("alpha", "1"))
            old_ball = child.find("Palla")
            sphere = old_ball.find("Sphere") if old_ball is not None else None
            if sphere is not None:
                sub(ball, "Sphere", radius=sphere.get("raggio", "1"), r=sphere.get("r", "1"),
                    g=sphere.get("g", "0"), b=sphere.get("b", "0"), alpha=sphere.get("alpha", "1"),
                    textured=old_ball.get("textured", "0"))
            bounce = child.find("Rimbalzo")
            if bounce is not None:
                sub(ball, "Bounce", maxHeight=bounce.get("hmax", "0"), gravity=bounce.get("grav", "0"))
            move = child.find("Spostamento")
            if move is not None:
                sub(ball, "Movement", dx=move.get("dx", "0"), dz=move.get("dz", "0"))
            copy_xyz(child.find("Traslazione"), ball, "Position")
        elif child.tag == "GrAmb":
            if child.find("Pavimento") is not None:
                kind = "floor"
            elif child.find("Muro") is not None:
                kind = "wall"
            elif child.find("Scala") is not None:
                kind = "ladder"
            else:
                print("warning: skipping unknown environment object", file=sys.stderr)
                continue
            env = sub(lvl, "Environment", kind=kind,
                      texture=remap_path(child.get("texture", ""), "texture"),
                      main=child.get("principale", "0"), destructible=child.get("deletable", "0"))
            if kind == "floor":
                p = child.find("Pavimento")
                floor = sub(env, "Floor", cellsX=p.get("ncellor", "1"), cellsZ=p.get("ncellvert", "1"),
                            cellSize=p.get("lato", "5"), thickness=p.get("spessore", "1"),
                            textured=p.get("textured", "0"), alpha=p.get("alpha", "1"))
                box = p.find("Paral")
            elif kind == "wall":
                m = child.find("Muro")
                floor = sub(env, "Wall", textured=m.get("textured", "0"), alpha=m.get("alpha", "1"))
                box = m.find("Paral")
            else:
                s = child.find("Scala")
                sub(env, "Ladder", rungs=s.get("npioli", "1"))
                box = None
            if box is not None:
                sub(floor, "Box", dx=box.get("dx", "1"), dy=box.get("dy", "1"), dz=box.get("dz", "1"),
                    r=box.get("r", "1"), g=box.get("g", "1"), b=box.get("b", "1"))
            copy_xyz(child.find("Traslazione"), env, "Position")
            rot = child.find("Rotazione")
            if rot is not None:
                sub(env, "Rotation", x=rot.get("x", "0"), y=rot.get("y", "1"), z=rot.get("z", "0"),
                    angle=rot.get("angle", "0"))
        elif child.tag == "GrGiocat":
            player = sub(lvl, "Player")
            g = child.find("Giocatore")
            if g is not None:
                sub(player, "Model", file=remap_path(g.get("model", ""), "model"),
                    skin=remap_path(g.get("texture", ""), "skin") if g.get("model") else "",
                    lives=g.get("nvite", "3"))
            copy_xyz(child.find("Traslazione"), player, "Position")
    return lvl


def convert_campaign(src, dst):
    with open(src, encoding="latin-1") as f:
        lines = [l.strip() for l in f if l.strip()]
    renames = {"armi.xml": "weapons.xml", "bombe.xml": "bombs.xml", "livello misto.xml": "mixed.xml",
               "lvl1.xml": "level1.xml", "lvl2.xml": "level2.xml", "testbed.xml": "testbed.xml"}
    with open(dst, "w", encoding="utf-8") as f:
        f.write("# 3dPang campaign: one level file per line, relative to data/levels\n")
        for l in lines:
            name = os.path.basename(l.replace("\\", "/"))
            f.write(renames.get(name.lower(), name.lower()) + "\n")


def indent(elem, level=0):
    pad = "\n" + "  " * level
    if len(elem):
        elem.text = pad + "  "
        for child in elem:
            indent(child, level + 1)
        child.tail = pad
    if level and not elem.tail:
        elem.tail = pad


def main(argv):
    if len(argv) != 3:
        print(__doc__)
        return 2
    src, dst = argv[1], argv[2]
    if src.lower().endswith(".sce"):
        convert_campaign(src, dst)
        return 0
    with open(src, "rb") as f:
        raw = f.read()
    try:
        root = ET.fromstring(raw)
    except ET.ParseError:
        root = ET.fromstring(raw.decode("latin-1"))
    lvl = convert_level(root)
    indent(lvl)
    ET.ElementTree(lvl).write(dst, encoding="utf-8", xml_declaration=True)
    with open(dst, "a", encoding="utf-8") as f:
        f.write("\n")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
