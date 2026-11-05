#include "DataPaths.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>

namespace pang::paths {

namespace {
bool looksLikeDataDir(const QString& dir)
{
    return !dir.isEmpty() && QFileInfo(dir + QStringLiteral("/levels")).isDir();
}
} // namespace

QString dataDir()
{
    static const QString cached = [] {
        QStringList candidates;
        const QString env = qEnvironmentVariable("PANG3D_DATA_DIR");
        if (!env.isEmpty())
            candidates << env;
        if (QCoreApplication::instance() != nullptr) {
            const QString app = QCoreApplication::applicationDirPath();
            candidates << app + QStringLiteral("/data")
                       << app + QStringLiteral("/../share/3dpang/data")
                       << app + QStringLiteral("/../Resources/data");
        }
#ifdef PANG3D_SOURCE_DATA_DIR
        candidates << QStringLiteral(PANG3D_SOURCE_DATA_DIR);
#endif
        for (const QString& c : candidates) {
            if (looksLikeDataDir(c))
                return QDir::cleanPath(QDir(c).absolutePath());
        }
        qWarning("3dPang: data directory not found, set PANG3D_DATA_DIR");
        return QDir::currentPath() + QStringLiteral("/data");
    }();
    return cached;
}

QString resolve(const QString& relativePath)
{
    QString p = relativePath.trimmed();
    if (p.isEmpty())
        return {};
    p.replace(QLatin1Char('\\'), QLatin1Char('/'));
    if (p.startsWith(QStringLiteral("../data/")))
        p = p.mid(8);
    if (QDir::isAbsolutePath(p))
        return p;
    return dataDir() + QLatin1Char('/') + p;
}

QString relativize(const QString& path)
{
    if (path.isEmpty())
        return {};
    const QString abs = QDir::cleanPath(QFileInfo(path).absoluteFilePath());
    const QString base = dataDir() + QLatin1Char('/');
    if (abs.startsWith(base))
        return abs.mid(base.size());
    if (!QDir::isAbsolutePath(path))
        return QDir::cleanPath(path);
    return abs;
}

QString userDir()
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dir.isEmpty())
        dir = QDir::homePath() + QStringLiteral("/.3dpang");
    QDir().mkpath(dir);
    return dir;
}

QByteArray native(const QString& path)
{
    return QFile::encodeName(path);
}

} // namespace pang::paths
