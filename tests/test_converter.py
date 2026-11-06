#!/usr/bin/env python3
"""Converts a 2005 level and checks the result has the v2 structure."""
import os
import subprocess
import sys
import tempfile
import xml.etree.ElementTree as ET

here = os.path.dirname(os.path.abspath(__file__))
tool = os.path.join(here, "..", "tools", "convert_legacy_level.py")
with tempfile.TemporaryDirectory() as tmp:
    out = os.path.join(tmp, "level.xml")
    subprocess.run([sys.executable, "-I", tool, os.path.join(here, "fixtures", "legacy_level.xml"), out], check=True)
    root = ET.parse(out).getroot()
    assert root.tag == "Level" and root.get("version") == "2"
    assert root.get("powerUpChance") == "50" and root.get("skybox") == "skyboxes/mount.jpg"
    ball = root.find("Ball")
    assert ball.get("texture") == "textures/pollonina.jpg" and ball.get("children") == "2"
    assert ball.find("Sphere").get("radius") == "4"
    assert ball.find("Bounce").get("maxHeight") == "23.4375"
    kinds = [e.get("kind") for e in root.findall("Environment")]
    assert kinds == ["floor", "ladder"], kinds
    assert root.find("Environment/Floor").get("cellsX") == "15"
    assert root.find("Environment/Ladder").get("rungs") == "6"
    assert root.find("Player/Model").get("lives") == "5"
print("converter ok")
