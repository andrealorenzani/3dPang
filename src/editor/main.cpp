// 3dPang level editor.
//
//   3dpang-editor              asks for the size of a new level
//   3dpang-editor LEVEL.xml    opens a level
//   3dpang-editor LEVEL.xml --screenshot FILE.png
//                              saves a picture of the window and quits
//   3dpang-editor LEVEL.xml --resave OUT.xml
//                              loads and saves the level again, then quits

#include "DataPaths.h"
#include "EditorView.h"
#include "MainWindow.h"

#include <QApplication>
#include <QTimer>
#include <QSurfaceFormat>

#include <cstdlib>
#include <ctime>

int main(int argc, char** argv)
{
    // Legacy OpenGL with a stencil buffer (for the shadows), for every GL widget
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setVersion(2, 1);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QApplication app(argc, argv);
    QApplication::setOrganizationName(QStringLiteral("3dPang"));
    QApplication::setApplicationName(QStringLiteral("3dPang Editor"));
    QApplication::setApplicationVersion(QStringLiteral(PANG3D_VERSION));
    std::srand(unsigned(std::time(nullptr)));

    QStringList args = QApplication::arguments();
    args.removeFirst();
    QString screenshot;
    const int shotIndex = int(args.indexOf(QStringLiteral("--screenshot")));
    if (shotIndex >= 0 && shotIndex + 1 < args.size()) {
        screenshot = args.at(shotIndex + 1);
        args.remove(shotIndex, 2);
    }

    QString resave;
    const int resaveIndex = int(args.indexOf(QStringLiteral("--resave")));
    if (resaveIndex >= 0 && resaveIndex + 1 < args.size()) {
        resave = args.at(resaveIndex + 1);
        args.remove(resaveIndex, 2);
    }

    pang::MainWindow window;
    window.show();
    if (!args.isEmpty()) {
        if (!window.openLevel(args.first()))
            return 1;
    } else if (!window.askMainFloor()) {
        return 0;
    }
    if (!resave.isEmpty()) {
        QTimer::singleShot(500, &window, [&window, resave] {
            QApplication::exit(window.view()->save(resave) ? 0 : 4);
        });
    }
    if (!screenshot.isEmpty()) {
        QTimer::singleShot(1500, &window, [&window, screenshot] {
            const bool ok = window.grab().save(screenshot);
            QApplication::exit(ok ? 0 : 4);
        });
    }
    return app.exec();
}
