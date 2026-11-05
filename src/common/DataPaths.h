#pragma once
// Locating the game data.
//
// In 2005 every path was relative to the PangPlayer/PangEditor folder
// ("../data/..."), so the programs only worked when started from there.
// Version 2 looks for the data directory and keeps all paths in level files
// relative to it (e.g. "textures/gras.jpg").

#include <QByteArray>
#include <QString>

namespace pang::paths {

// The data directory, searched in this order:
//  1. $PANG3D_DATA_DIR
//  2. <executable dir>/data            (build tree, portable zip)
//  3. <executable dir>/../share/3dpang/data   (Linux install)
//  4. <executable dir>/../Resources/data      (macOS bundle)
//  5. the source tree data directory known at compile time
QString dataDir();

// Absolute path of a data file. Accepts data-relative paths, absolute paths
// and the legacy "../data/..." form. Returns an empty string for empty input.
QString resolve(const QString& relativePath);

// Inverse of resolve(): turns a path inside the data directory into a
// data-relative one, used when the editor saves a level.
QString relativize(const QString& path);

// Directory for files the game writes (high scores).
QString userDir();

// Local 8-bit encoding of a path, for C APIs such as fopen().
QByteArray native(const QString& path);

} // namespace pang::paths
