#pragma once
// The scene graph: everything in a level, plus the game rules (collisions,
// power-ups, levels and scores) and, in the editor, the editing operations.

#include "Actors.h"
#include "Bonus.h"
#include "Definitions.h"
#include "Environment.h"
#include "Geometry.h"
#include "Groups.h"
#include "Nodes.h"

#include <chrono>
#include <list>
#include <map>

namespace pang {

#ifdef PANG3D_PLAYER
struct HighScore {
    int player;
    int points;
    int difficulty;
};
#endif

class SceneGraph {
public:
    SceneGraph();
    ~SceneGraph();
    SceneGraph(const SceneGraph&) = delete;
    SceneGraph& operator=(const SceneGraph&) = delete;

    // Seconds since the previous call (the clock is part of the scene so that
    // the time spent in the menu does not count in the next game)
    float tick();
    void resetClock() { lastTime = std::chrono::steady_clock::now(); }

    // Cameras: six optional views following the players and the fixed default one
    View views[6];
    Point3i defaultView;
    CubeMap* cubeMap;
    // At most one tornado in play
    TwisterGroup* twister = nullptr;
#ifdef PANG3D_PLAYER
    int score1 = 0, score2 = 0;
    int numPlayers = 1;
#endif

    // Options chosen in the menu
    int difficulty = MEDIUM;
    int detail = DETAIL_HIGH;
    QString selectedCampaign;  // file name in data/campaigns
    QString selectedLevel;     // file name in data/levels
    QString currentLevel;      // full path of the level being played
    QStringList levelQueue;    // levels still to play

#ifdef PANG3D_PLAYER
    // Builds the list of levels for the chosen level or campaign (does not load them)
    void buildLevelQueue();
    bool startGame();
    void nextLevel();
    // True when every player is dead (or there are none)
    bool isGameOver() const;
    // Adds the scores to the high scores, once per game
    void finishGame();
    // Ends the game after the last level (it is "won" by killing the players)
    void victory();
    static QList<HighScore> loadHighScores();
#endif

    // Power-up chances, in percent
    int powerUpChance = 0;  // a power-up when a ball pops
    int bonusChance = 0;    // the power-up is a bonus (otherwise a malus)
    // Relative weights: weapons + lives + fruit (bonuses), bombs + tornadoes (maluses)
    int weaponWeight = 0, lifeWeight = 0, fruitWeight = 0, bombWeight = 0, tornadoWeight = 0;

    // Recompute the shadows cast by floors in the next frame (ball shadows
    // are computed every frame anyway)
    bool recomputeFixedShadows = true;
    bool shadows = true;
    // Size of the main floor
    int numCellsX = 0, numCellsZ = 0;
    int floorThickness = 0;
    float cellSize = 5.0f;

    // Shadow circles for balls, at three levels of detail
    ShadowCircle* shadowLow;
    ShadowCircle* shadowMedium;
    ShadowCircle* shadowHigh;

    std::list<BallGroup*> balls;
    std::list<EnvGroup*> environment;
    std::list<ProjectileGroup*> shots;
    std::list<BonusGroup*> bonuses;
    PlayerGroup* player1 = nullptr;
    PlayerGroup* player2 = nullptr;

    // Collisions
    void collide();
    void collide(BallGroup*, EnvGroup*);
    void collide(BallGroup*, BallGroup*);
    void collide(BallGroup*, PlayerGroup*);
    void collide(EnvGroup*, PlayerGroup*);
    void collide(EnvGroup*, BonusGroup*);
#ifdef PANG3D_PLAYER
    void collide(BonusGroup*, PlayerGroup*);
    void collide(EnvGroup*, ProjectileGroup*);
    void collide(BallGroup*, ProjectileGroup*);
    void collide(EnvGroup*, TwisterGroup*);
    void collide(BallGroup*, TwisterGroup*);
    void collide(PlayerGroup*, TwisterGroup*);
#endif

    void calculateShadow(BallGroup*);
    void generateFixedShadows();
    void update(float dt);
    void glDraw();
    void glDrawBallShadows(EnvGroup*);
    // Loads a texture once (data-relative path) and returns its OpenGL name, 0 if none
    GLuint loadTexture(const QString& file);
    void regenerateDisplayList();
    bool loadLevel(const QString& fileName);
    // Deletes every object of the level
    void clearLevel();
#ifdef PANG3D_EDITOR
    bool saveLevel(const QString& fileName) const;
#endif

#ifdef PANG3D_PLAYER
    // Power-ups picked up by a player
    void apply(Bonus*, PlayerGroup*);
    void apply(ExtraLife*, PlayerGroup*);
    void apply(WeaponBonus*, PlayerGroup*);
    void apply(TornadoBonus*, PlayerGroup*);
    void apply(BombBonus*, PlayerGroup*);
#endif

#ifdef PANG3D_EDITOR
    // Editing operations, applied to the selected objects
    void deleteSelectedBalls();
    void deleteSelectedFloors();
    void deleteSelectedWalls();
    void deleteSelectedLadders();
    // Window point (pixels, origin top-left) -> world point under it, read
    // from the depth buffer. Uses the current OpenGL matrices.
    Point3d get3dPos(Point2i in);
    // Raises/lowers the selection (and adds/removes ladder rungs)
    void moveUp(float h);
    void newBalls(int radius, int children, int startHeight, int maxHeight, float gravity, float dirX, float dirZ,
                  float red, float green, float blue, bool textured, const QString& texture);
    void newFloor(bool destructible, bool isMain, int cellsX, int cellsZ, int h, int thickness, float red,
                  float green, float blue, float alpha, bool textured, const QString& texture);
    void newWall(bool destructible, bool isMain, int length, int width, int h, int elevation, float red,
                 float green, float blue, float alpha, bool textured, const QString& texture);
    void pick(Point2i in);
    // Puts the moving objects (the balls) back where they started
    void rewind();
    void setBallColor(float, float, float);
    void setFloorColor(float, float, float);
    void setWallColor(float, float, float);
    void setBallTexture(bool, const QString&);
    void setFloorTexture(bool, const QString&);
    void setWallTexture(bool, const QString&);
    // Drags the selection on the XZ plane by a mouse movement
    void translateSelected(Point2i mouseDelta);
#endif

private:
    std::map<QString, GLuint> textures;
    std::chrono::steady_clock::time_point lastTime;
#ifdef PANG3D_PLAYER
    bool levelCompleted = false;
    bool scoresSaved = false;
    void removeFinishedShots();
    void spawnPowerUp(const Point3f& where, float gravity);
#endif
};

} // namespace pang
