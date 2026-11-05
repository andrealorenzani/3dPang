#pragma once
// The moving actors: balls, players and the tornado.

#include "CIMd2.h"
#include "Definitions.h"
#include "Geometry.h"

#include <vector>

namespace pang {

// Default player model and skin (data-relative)
inline const QString DEFAULT_PLAYER_MODEL = QStringLiteral("models/md2/lbuu/tris.md2");
inline const QString DEFAULT_PLAYER_SKIN = QStringLiteral("models/md2/lbuu/buu4.png");

// A tornado made of points, spinning around its Y axis. It "grows" from
// the ground during its first seconds of life.
class Twister {
public:
    bool useDisplayList = true;
    float angle = 0;
    int intensity;           // points generated per step
    int verticalSteps;
    int horizontalSteps;
    float creationTime;      // seconds needed to grow
    float power;             // pull towards the centre
    float height;
    float width;
    float duration;          // seconds of life
    int angularSpeed;        // degrees per second

    explicit Twister(int intens = 10, int vSteps = 30, int hSteps = 40, float timeForCreation = 3,
                     float lifeTime = 10, int angularSpeedDeg = 720, float pwr = 5, float h = 15, float w = 4)
        : intensity(intens), verticalSteps(vSteps), horizontalSteps(hSteps), creationTime(timeForCreation),
          power(pwr), height(h), width(w), duration(lifeTime), angularSpeed(angularSpeedDeg)
    {
    }
    ~Twister();
    void glDraw();
    // Grows the tornado by dt seconds
    void generate(float dt);
    void generateDL();
    Point3f getPos() const { return Point3f(0, 0, 0); }
    // [0] = width, [1] = height
    Point3f getDim() const { return Point3f(width, height, 0); }
    bool exists() const { return internalTime < duration; }
    bool generating() const { return internalTime < creationTime; }

private:
    std::vector<Point3d> mesh;  // normal, point, normal, point...
    int generatedTurns = 0;
    GLuint displayList = 0;
    float internalTime = 0.0f;
    bool generated = false;
};

class Ball {
public:
    Sphere* sphere;
    bool useDisplayList = false;
    GLuint displayList = 0;
    GLuint texture = 0;
    bool textured = false;

    explicit Ball(float radius = 1.0f, int slices = BALL_DETAIL_MEDIUM, int stacks = BALL_DETAIL_MEDIUM)
        // Small balls get fewer polygons
        : sphere(new Sphere(radius, slices + int((radius - 2) * slices / 4), stacks + int((radius - 2) * slices / 4)))
    {
    }
    ~Ball();
    Ball(const Ball&) = delete;
    Ball& operator=(const Ball&) = delete;
    NodeType getType() const { return BALL; }
    void glDraw();
    void genList();
    // Replaces the sphere (keeping colour and alpha unless told otherwise)
    void setSphere(Sphere* s)
    {
        delete sphere;
        sphere = s;
    }
};

class Player {
public:
    float animTime = 0.0f;  // clock of the animation
    bool dead = false;
#ifdef PANG3D_EDITOR
    PlayerMarker* marker;   // the "1"/"2" drawn on the floor
    bool showModel = false; // draw the model instead of the marker
#endif
    int lives;
    QString modelFile;      // data-relative, empty = default
    QString skinFile;
    CIMd2* model;

    explicit Player(int numLives = 5, const QString& model3d = QString(), const QString& skin = QString()
#ifdef PANG3D_EDITOR
                    , bool first = true
#endif
    );
    ~Player();
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    // Changes skin and, if given, model
    void setModel(const QString& newSkin, const QString& newModel = QString());

    // Animations (frame ranges of the standard Quake II player models)
    void setStand() { model->SetAnim(0, 39); }
    void setRunning() { model->SetAnim(40, 45); }
    void setAttack() { model->SetAnim(46, 53); }
    void setPain() { model->SetAnim(54, 57); }
    void setJump() { model->SetAnim(66, 71); }
    void setFlip() { model->SetAnim(72, 83); }
    void setSalute() { model->SetAnim(84, 94); }
    void setTaunt() { model->SetAnim(95, 111); }
    void setWave() { model->SetAnim(112, 122); }
    void setPoint() { model->SetAnim(123, 134); }
    void setCrouchStand() { model->SetAnim(135, 153); }
    void setCrouchWalk() { model->SetAnim(154, 159); }
    void setCrouchAttack() { model->SetAnim(160, 168); }
    void setCrouchPain() { model->SetAnim(169, 172); }
    void setCrouchDeath() { model->SetAnim(173, 177); }
    void setDeath() { model->SetAnim(178, 183); }

    // By convention the player is at the origin of its group...
    Point3f getPos() const { return Point3f(0.0f, 0.0f, 0.0f); }
    // ...and is a cylinder: [0] radius, [1] height
    Point3f getDim() const { return Point3f(1.5f, 3.8f, 0.0f); }
    NodeType getType() const { return PLAYER; }
    void glDraw();
#ifdef PANG3D_EDITOR
    void setMarker(PlayerMarker* m)
    {
        delete marker;
        marker = m;
    }
#endif
};

} // namespace pang
