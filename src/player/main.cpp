// 3dPang player: SDL3 window and main loop.
//
//   3dpang                      play
//   3dpang --demo LEVEL.xml --frames N --screenshot out.png
//                               run a level for N frames (1/60 s each, no
//                               input), save the last frame and quit: used to
//                               check builds without playing. With --autofire
//                               player 1 walks around and shoots.

#include "DataPaths.h"
#include "Definitions.h"
#include "Game.h"
#include "GlUtil.h"
#include "Menu.h"
#include "SceneGraph.h"
#include "TextWriter.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <QCoreApplication>
#include <QFileInfo>
#include <QImage>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace pang;

namespace {

struct App {
    SceneGraph scene;
    TextWriter writer;
    Menu menu{&scene};
    Game game{&scene};
    int width = 800;
    int height = 600;
};

void initGl()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // glColor sets the ambient and diffuse material
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // A directional light (w = 0)
    const float light[4] = {0, 30, 30, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, light);
    // Normals go through the modelview matrix too: renormalise them
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);
    glClearStencil(0);
}

// Viewport and projection: the whole window for one player, the top half
// for player 1 and the bottom half for player 2 in two player games
void setViewport(App& app, bool first, bool single)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const int h = single ? app.height : app.height / 2;
    const int start = single ? 0 : h;
    const float ratio = float(app.width) / float(std::max(1, h));
    gl::perspective(45, ratio, 10, 100000);
    if (first)
        glViewport(0, start, app.width, h);
    else
        glViewport(0, 0, app.width, h);
    // The cube map must not cut the scene
    app.scene.cubeMap->radius = 10 * ratio;
    glMatrixMode(GL_MODELVIEW);
}

// Camera position and target for a player
void camera(const App& app, const PlayerGroup* g, Point3i eye, Point3i& outEye, Point3f& outTarget)
{
    const SceneGraph& sg = app.scene;
    // The default view is fixed and looks at the centre of the level
    outTarget = Point3f(2.5f * sg.numCellsX, 0, 2.5f * sg.numCellsZ);
    outEye = eye;
    if (g != nullptr && eye != sg.defaultView) {
        // The other views follow the player
        outTarget = const_cast<PlayerGroup*>(g)->getPos();
        outEye = Point3i(int(outTarget[0] + eye[0]), int(outTarget[1] + eye[1]), int(outTarget[2] + eye[2]));
        if (outEye == Point3i(0, 0, 0))
            outEye = sg.defaultView;
    }
}

void drawHud(App& app, int score, int lives)
{
    app.writer.prepare(0.0f, 0.0f, 0.0f, 1.0f, 10, app.height - 20);
    app.writer.write(QStringLiteral("Score: %1").arg(score));
    app.writer.prepare(0.0f, 0.0f, 0.0f, 1.0f, app.width - 100, app.height - 20);
    app.writer.write(QStringLiteral("Lives: %1").arg(lives));
}

void drawScene(App& app, int fps)
{
    SceneGraph& sg = app.scene;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    Point3i eye;
    Point3f target;
    const bool single = sg.player2 == nullptr;
    glLoadIdentity();
    setViewport(app, true, single);
    camera(app, sg.player1, app.game.eye1, eye, target);
    gl::lookAt(eye[0], eye[1], eye[2], target[0], target[1], target[2], 0, 1, 0);
    sg.glDraw();
    if (sg.player1 != nullptr)
        drawHud(app, sg.score1, sg.player1->player->lives);

    if (!single) {
        glLoadIdentity();
        setViewport(app, false, false);
        camera(app, sg.player2, app.game.eye2, eye, target);
        gl::lookAt(eye[0], eye[1], eye[2], target[0], target[1], target[2], 0, 1, 0);
        sg.glDraw();
        drawHud(app, sg.score2, sg.player2->player->lives);
    }
    glViewport(0, 0, app.width, app.height);
    app.writer.prepare(0.0f, 0.0f, 0.0f, 1.0f, 0, 0, 0.0f, 1.0f, 1.0f);
    app.writer.write(QStringLiteral("FPS: %1").arg(fps));
    glDisable(GL_BLEND);
}

// Advances the game by dt seconds, in small steps so that fast balls do not
// pass through thin floors
void simulate(SceneGraph& sg, float dt)
{
    constexpr float kStep = 1.0f / 240.0f;
    dt = std::min(dt, 0.25f);  // e.g. after the window was dragged
    while (dt > 0.0f) {
        const float step = std::min(dt, kStep);
        sg.update(step);
        sg.collide();
        dt -= step;
    }
}

void resize(App& app, int w, int h)
{
    app.width = std::max(1, w);
    app.height = std::max(1, h);
    app.writer.setVideoSize(app.width, app.height);
    app.menu.setVideoSize(app.width, app.height);
    glViewport(0, 0, app.width, app.height);
}

bool saveScreenshot(int w, int h, const QString& file)
{
    QImage image(w, h, QImage::Format_RGBA8888);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
    return image.flipped(Qt::Vertical).save(file);
#else
    return image.mirrored(false, true).save(file);
#endif
}

} // namespace

int main(int argc, char** argv)
{
    // Qt reads images and XML (no Qt GUI: the window is SDL's)
    QCoreApplication qtApp(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("3dPang"));
    QCoreApplication::setApplicationName(QStringLiteral("3dPang"));
    std::srand(unsigned(std::time(nullptr)));

    QString demoLevel;
    QString screenshot;
    int demoFrames = 0;
    bool autofire = false;
    const QStringList args = QCoreApplication::arguments();
    for (int i = 1; i < args.size(); ++i) {
        if (args[i] == QLatin1String("--demo") && i + 1 < args.size())
            demoLevel = args[++i];
        else if (args[i] == QLatin1String("--frames") && i + 1 < args.size())
            demoFrames = args[++i].toInt();
        else if (args[i] == QLatin1String("--autofire"))
            autofire = true;
        else if (args[i] == QLatin1String("--screenshot") && i + 1 < args.size())
            screenshot = args[++i];
        else if (args[i] == QLatin1String("--help") || args[i] == QLatin1String("-h")) {
            std::printf("usage: 3dpang [--demo LEVEL.xml [--frames N] [--autofire] [--screenshot FILE.png]]\n"
                        "data directory: %s\n",
                        qPrintable(paths::dataDir()));
            return 0;
        }
    }
    const bool demo = !demoLevel.isEmpty();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::fprintf(stderr, "3dPang: SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    // Legacy OpenGL (fixed pipeline) with a stencil buffer for the shadows
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    App app;
    SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    if (demo)
        flags |= SDL_WINDOW_HIDDEN;
    SDL_Window* window = SDL_CreateWindow("3dPang", app.width, app.height, flags);
    if (window == nullptr) {
        std::fprintf(stderr, "3dPang: cannot create the window: %s\n", SDL_GetError());
        SDL_Quit();
        return 2;
    }
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        std::fprintf(stderr, "3dPang: cannot create an OpenGL context: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 2;
    }
    SDL_GL_MakeCurrent(window, context);
    SDL_GL_SetSwapInterval(1);

    initGl();
    int pw = app.width;
    int ph = app.height;
    SDL_GetWindowSizeInPixels(window, &pw, &ph);
    resize(app, pw, ph);
    app.writer.init();
    app.menu.init();

    int exitCode = 0;
    if (demo) {
        app.scene.selectedLevel = demoLevel;
        app.scene.selectedCampaign.clear();
        app.scene.numPlayers = 1;
        if (!app.scene.startGame()) {
            std::fprintf(stderr, "3dPang: cannot load level '%s'\n", qPrintable(demoLevel));
            exitCode = 3;
        } else {
            app.game.resetViews();
            const SDL_Keycode walk[4] = {SDLK_LEFT, SDLK_UP, SDLK_RIGHT, SDLK_DOWN};
            for (int f = 0; f < std::max(1, demoFrames); ++f) {
                if (autofire) {
                    // Fire twice a second, change direction every two seconds
                    app.game.onKey(SDLK_M, f % 30 == 0);
                    if (f % 120 == 0) {
                        app.game.onKey(walk[(f / 120 + 3) % 4], false);
                        app.game.onKey(walk[(f / 120) % 4], true);
                    }
                }
                simulate(app.scene, 1.0f / 60.0f);
                if (app.scene.player1 == nullptr)
                    break;
                drawScene(app, 60);
            }
            glFinish();
            if (!screenshot.isEmpty() && !saveScreenshot(app.width, app.height, screenshot)) {
                std::fprintf(stderr, "3dPang: cannot save '%s'\n", qPrintable(screenshot));
                exitCode = 4;
            }
            std::printf("demo: %d balls, %d environment objects, %d bonuses, score %d, lives %d, level %s\n",
                        int(app.scene.balls.size()), int(app.scene.environment.size()),
                        int(app.scene.bonuses.size()), app.scene.score1,
                        app.scene.player1 != nullptr ? app.scene.player1->player->lives : -1,
                        qPrintable(QFileInfo(app.scene.currentLevel).fileName()));
        }
    }

    bool running = !demo;
    int frames = 0;
    int fps = 0;
    float fpsTime = 0.0f;
    auto lastFrame = SDL_GetTicksNS();
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                // SDL keeps the OpenGL context: no need to reload textures
                resize(app, event.window.data1, event.window.data2);
                break;
            case SDL_EVENT_KEY_DOWN:
                if (app.menu.active) {
                    running = app.menu.onKey(event.key.key);
                    if (app.menu.takeGameStarted())
                        app.game.resetViews();
                } else if (!event.key.repeat) {
                    // Esc leaves the game
                    if (event.key.key == SDLK_ESCAPE) {
                        app.scene.finishGame();
                        app.menu.showHallOfFame();
                    } else {
                        app.game.onKey(event.key.key, true);
                    }
                }
                break;
            case SDL_EVENT_KEY_UP:
                if (!app.menu.active)
                    app.game.onKey(event.key.key, false);
                break;
            default:
                break;
            }
        }
        if (!running)
            break;

        const auto now = SDL_GetTicksNS();
        const float frameTime = float(now - lastFrame) / 1e9f;
        lastFrame = now;
        fpsTime += frameTime;
        if (fpsTime > 1.0f) {
            fps = frames;
            frames = 0;
            fpsTime = 0.0f;
        }

        if (app.menu.active) {
            app.menu.draw();
        } else if (app.scene.isGameOver()) {
            app.scene.finishGame();
            app.menu.showHallOfFame();
            app.menu.draw();
        } else {
            simulate(app.scene, app.scene.tick());
            drawScene(app, fps);
            frames++;
        }
        SDL_GL_SwapWindow(window);
    }

    // Free the scene while the OpenGL context still exists
    app.scene.clearLevel();
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return exitCode;
}
