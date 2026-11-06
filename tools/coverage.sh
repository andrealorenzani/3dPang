#!/usr/bin/env bash
# Measures line coverage of the game and engine (src/) by running the ctest
# smoke tests, and prints a table plus the total. Needs gcc + gcov, CMake and Qt 6.
#
#   QT_DIR=$HOME/Qt/6.8.3/gcc_64 tools/coverage.sh [build-dir]
#
# The editor is not built here: it has no automated tests (see docs/quality_assurance.md).
set -euo pipefail
root="$(cd "$(dirname "$0")/.." && pwd)"
build="${1:-$root/build/coverage}"
cmake -S "$root" -B "$build" -DCMAKE_BUILD_TYPE=Debug -DPANG3D_BUILD_EDITOR=OFF \
      ${QT_DIR:+-DCMAKE_PREFIX_PATH="$QT_DIR"} \
      -DCMAKE_CXX_FLAGS=--coverage -DCMAKE_C_FLAGS=--coverage -DCMAKE_EXE_LINKER_FLAGS=--coverage >/dev/null
cmake --build "$build" --target 3dpang 3dpang-tests -j"$(nproc 2>/dev/null || echo 4)" >/dev/null
find "$build" -name '*.gcda' -delete
# Serial on purpose: parallel runs corrupt the shared .gcda counters
(cd "$build" && ctest -j1 --output-on-failure >/dev/null)
mkdir -p "$build/gcov" && cd "$build/gcov" && rm -f ./*.gcov
for gcda in $(find "$build/CMakeFiles" -name '*.gcda'); do
  gcov -o "$(dirname "$gcda")" "$gcda" >/dev/null 2>&1 || true
done
python3 -I - "$root" <<'PY'
import glob, re, sys, collections
root = sys.argv[1] + "/src/"
files = collections.defaultdict(lambda: [0, 0])
for g in glob.glob("*.gcov"):
    src, executable, covered = None, 0, 0
    for line in open(g, errors="replace"):
        m = re.match(r"\s*([^:]+):\s*(\d+):(.*)", line)
        if not m:
            continue
        if m.group(2) == "0" and m.group(3).startswith("Source:"):
            src = m.group(3)[7:]
        count = m.group(1).strip()
        if m.group(2) == "0" or count == "-":
            continue
        executable += 1
        covered += count not in ("#####", "=====")
    if src and src.startswith(root):
        f = files[src[len(root):]]
        f[0], f[1] = max(f[0], executable), max(f[1], covered)
total_e = total_c = 0
for name, (e, c) in sorted(files.items()):
    print(f"{name:34} {c:5}/{e:5} {100 * c / max(e, 1):5.1f}%")
    total_e += e
    total_c += c
print(f"{'TOTAL':34} {total_c:5}/{total_e:5} {100 * total_c / max(total_e, 1):5.1f}%")
PY
