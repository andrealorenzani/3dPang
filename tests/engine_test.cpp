// Engine tests: loaders, level files and a stress run of the rules (tornadoes,
// bombs, every weapon, two players). Needs an OpenGL context (hidden window).
// Exit code 0 = everything passed.

#include "DataPaths.h"
#include "Model3ds.h"
#include "SceneGraph.h"

#include <SDL3/SDL.h>

#include <QCoreApplication>
#include <QDir>

#include <cstdio>

using namespace pang;

static int failures = 0;
#define CHECK(cond)                                                                  \
    do {                                                                             \
        if (!(cond)) {                                                               \
            std::fprintf(stderr, "FAILED %s:%d: %s\n", __FILE__, __LINE__, #cond);   \
            ++failures;                                                              \
        }                                                                            \
    } while (0)

static void testModels()
{
    for (const char* name : {"heart", "apple", "banana", "pear"}) {
        Model3ds m;
        CHECK(m.open(QStringLiteral("models/3ds/%1.3ds").arg(QLatin1String(name))));
        CHECK(m.radius() > 0.0f);
    }
    Model3ds missing;
    CHECK(!missing.open(QStringLiteral("models/3ds/does-not-exist.3ds")));
}

static void testLevels()
{
    const QDir dir(paths::resolve(QStringLiteral("levels")));
    const QStringList levels = dir.entryList({QStringLiteral("*.xml")}, QDir::Files);
    CHECK(levels.size() >= 11);
    for (const QString& file : levels) {
        SceneGraph sg;
        CHECK(sg.loadLevel(dir.filePath(file)));
        CHECK(sg.player1 != nullptr && sg.player2 != nullptr);
        CHECK(sg.numCellsX > 0 && sg.numCellsZ > 0);
        CHECK(!sg.environment.empty());
        // Four invisible outer walls come with every main floor
        int mainObjects = 0;
        for (const EnvGroup* e : sg.environment)
            mainObjects += e->isMain ? 1 : 0;
        CHECK(mainObjects >= 5);
    }
    SceneGraph bad;
    CHECK(!bad.loadLevel(QStringLiteral("/nonexistent/level.xml")));
    // A 2005 level is refused with a hint, not loaded half way
    CHECK(!bad.loadLevel(QStringLiteral(PANG3D_TEST_FIXTURES "/legacy_level.xml")));
}

static void testCampaign()
{
    SceneGraph sg;
    sg.selectedCampaign = QStringLiteral("classic.campaign");
    sg.buildLevelQueue();
    CHECK(sg.levelQueue.size() == 4);
    sg.selectedCampaign.clear();
    sg.selectedLevel = QStringLiteral("level1.xml");
    sg.buildLevelQueue();
    CHECK(sg.levelQueue.size() == 1);
}

static void testDifficultyScaling()
{
    // The same ball is faster, heavier and bounces lower at higher difficulty
    float vx[5], gravity[5], height[5];
    for (int d = 0; d < 5; ++d) {
        SceneGraph sg;
        sg.difficulty = d;
        CHECK(sg.loadLevel(paths::resolve(QStringLiteral("levels/level1.xml"))));
        BallGroup* b = sg.balls.front();
        vx[d] = std::abs(b->movement->vx);
        gravity[d] = std::abs(b->bounce->gravity);
        height[d] = b->bounce->maxHeight;
    }
    for (int d = 1; d < 5; ++d) {
        CHECK(vx[d] > vx[d - 1]);
        CHECK(gravity[d] > gravity[d - 1]);
        CHECK(height[d] <= height[d - 1]);
    }
}

static void testCollisionRules()
{
    SceneGraph sg;
    CHECK(sg.loadLevel(paths::resolve(QStringLiteral("levels/level1.xml"))));
    // A ball dropped on the main floor never falls through it
    BallGroup* b = sg.balls.front();
    for (int i = 0; i < 240 * 20; ++i) {
        sg.update(1.0f / 240.0f);
        sg.collide();
        CHECK(b->posy > -1.0f);
    }
    // Hitting a player kills it (and only once: dying players are ignored)
    PlayerGroup* p = sg.player1;
    CHECK(!p->dying);
    b->posx = p->posx;
    b->posy = p->posy + 1.0f;
    b->posz = p->posz;
    sg.collide(b, p);
    CHECK(p->dying);
}

static void stress()
{
    SceneGraph sg;
    sg.selectedLevel = QStringLiteral("level1.xml");
    sg.numPlayers = 2;
    CHECK(sg.startGame());
    for (int f = 0; f < 6000; f++) {
        if (f % 600 == 0) {
            TornadoBonus t;
            sg.apply(&t, sg.player1);
        }
        if (f % 700 == 350) {
            BombBonus bomb(1);
            sg.apply(&bomb, sg.player2 != nullptr ? sg.player2 : sg.player1);
        }
        if (f % 500 == 100) {
            GunBonus g;
            sg.apply(&g, sg.player1);
            ExtraLife l;
            sg.apply(&l, sg.player1);
            QuadHarpoonBonus q;
            sg.apply(&q, sg.player1);
        }
        for (int s = 0; s < 4; s++) {
            sg.update(1 / 240.f);
            sg.collide();
        }
        if (f % 10 == 0) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            sg.glDraw();
        }
        if (sg.isGameOver()) {
            sg.finishGame();
            break;
        }
    }
    CHECK(sg.twister != nullptr || sg.player1 != nullptr);
    sg.clearLevel();
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 2;
    }
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_Window* w = SDL_CreateWindow("tests", 320, 240, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    SDL_GLContext c = w ? SDL_GL_CreateContext(w) : nullptr;
    if (c == nullptr) {
        std::fprintf(stderr, "no OpenGL context: %s\n", SDL_GetError());
        return 2;
    }
    SDL_GL_MakeCurrent(w, c);

    testModels();
    testLevels();
    testCampaign();
    testDifficultyScaling();
    testCollisionRules();
    stress();

    SDL_GL_DestroyContext(c);
    SDL_DestroyWindow(w);
    SDL_Quit();
    std::printf(failures == 0 ? "engine tests passed\n" : "%d check(s) FAILED\n", failures);
    return failures == 0 ? 0 : 1;
}
