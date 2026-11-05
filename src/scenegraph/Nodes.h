#pragma once
// Scene graph nodes: movements, transformations and other behaviours that
// groups attach to their position.

#include "Definitions.h"

#include "CICubeMap.h"

namespace pang {

// Base node. A node is bound to the position of the object it moves.
class Node {
public:
    virtual ~Node() = default;
    // Reacts to collisions on the six sides: +x, -x, +y, -y, +z (near), -z (far)
    virtual void onCollision(bool /*right*/, bool /*left*/, bool /*up*/, bool /*down*/, bool /*nearSide*/,
                             bool /*farSide*/) {}
    virtual NodeType getType() const { return NODE; }
    virtual void glDraw() {}
    virtual void load(const QDomElement& /*e*/, int /*difficulty*/) {}
    virtual void update(float /*dt*/) {}
    virtual void bindCoord(float* px, float* py, float* pz) = 0;
#ifdef PANG3D_EDITOR
    virtual void save(QDomElement& /*parent*/) const {}
#endif
};

// Linear movement on the XZ plane, bouncing back on collisions.
class Movement : public Node {
public:
    bool collRight = false, collLeft = false, collFar = false, collNear = false;
    float vx, vz;              // current velocity
    float startVx, startVz;    // velocity set when created/loaded
    float baseVx, baseVz;      // value in the level file, before difficulty scaling
    float* posx = nullptr;
    float* posz = nullptr;

    Movement(float velX, float velZ);
    void onCollision(bool right, bool left, bool up, bool down, bool nearSide, bool farSide) override;
    NodeType getType() const override { return MOVEMENT; }
    void load(const QDomElement& e, int difficulty) override;
    Point3f getVelocity() const { return Point3f(vx, 0.0f, vz); }
    void update(float dt) override;
    void bindCoord(float* px, float*, float* pz) override
    {
        posx = px;
        posz = pz;
    }
#ifdef PANG3D_EDITOR
    void save(QDomElement& parent) const override;
#endif
};

// Random wandering of the tornado: every few seconds it picks a new direction.
class TwisterMove : public Node {
public:
    bool collRight = false, collLeft = false, collFar = false, collNear = false;
    float vx = 0.0f, vz = 0.0f;
    float* posx = nullptr;
    float* posz = nullptr;
    int changeInterval;  // seconds between direction changes

    TwisterMove(float maxSpeedX, float maxSpeedZ, int changeEvery);
    void onCollision(bool right, bool left, bool up, bool down, bool nearSide, bool farSide) override;
    NodeType getType() const override { return RANDOM_MOVE; }
    void update(float dt) override;
    void bindCoord(float* px, float*, float* pz) override
    {
        posx = px;
        posz = pz;
    }

private:
    float internalTime = 0.0f;
    float maxSpeedX, maxSpeedZ;
};

// Vertical bounce of a ball.
class Bounce : public Node {
public:
    bool collUp = false, collDown = false;
    float vy = 0.0f;
    float gravity;
    float* posy = nullptr;
    // Maximum height. On the ground it also sets the speed the ball bounces back with.
    float maxHeight;
    float baseMaxHeight, baseGravity;  // values in the level file

    Bounce(float maxH, float grav);
    void onCollision(bool right, bool left, bool up, bool down, bool nearSide, bool farSide) override;
    NodeType getType() const override { return BOUNCE; }
    void load(const QDomElement& e, int difficulty) override;
    Point3f getVelocity() const { return Point3f(0.0f, vy, 0.0f); }
    void update(float dt) override;
    void bindCoord(float*, float* py, float*) override { posy = py; }
#ifdef PANG3D_EDITOR
    void save(QDomElement& parent) const override;
#endif
};

// Gravity for bonuses and players.
class Fall : public Node {
public:
    bool collDown = false;
    // Disabled while a player holds on to a ladder
    bool disabled = false;
    float vy = 0.0f;
    float gravity;
    float* posy = nullptr;

    explicit Fall(float grav);
    void onCollision(bool right, bool left, bool up, bool down, bool nearSide, bool farSide) override;
    NodeType getType() const override { return FALL; }
    Point3f getVelocity() const { return Point3f(0.0f, vy, 0.0f); }
    void update(float dt) override;
    void bindCoord(float*, float* py, float*) override { posy = py; }
};

#ifdef PANG3D_PLAYER
// Climbing up or down a ladder (player only).
class Climb : public Node {
public:
    Climb(float climbSpeed, float* playerY) : speed(climbSpeed), posY(playerY) {}
    NodeType getType() const override { return CLIMB; }
    void update(float dt) override;
    void onCollision(bool, bool, bool up, bool down, bool, bool) override
    {
        // Stop when hitting something in the climbing direction
        if (up && dir == 1)
            dir = 0;
        if (down && dir == -1)
            dir = 0;
    }
    void bindCoord(float*, float*, float*) override {}
    void up() { dir = +1; }
    void down() { dir = -1; }
    void stop() { dir = 0; }

private:
    float speed;
    float* posY;
    int dir = 0;  // +1 up, -1 down, 0 still
};
#endif

// Translation to the position of the object: it also stores the start position.
class Translation : public Node {
public:
    float* x = nullptr;
    float* y = nullptr;
    float* z = nullptr;

    NodeType getType() const override { return TRANSLATION; }
    void glDraw() override { glTranslatef(*x, *y, *z); }
    void load(const QDomElement& e, int difficulty) override;
    void move(float dx, float dy, float dz)
    {
        *x += dx;
        *y += dy;
        *z += dz;
    }
    void bindCoord(float* px, float* py, float* pz) override
    {
        x = px;
        y = py;
        z = pz;
    }
};

// Rotation (used to turn ladders by 90 degrees and to orient players).
class Rotation : public Node {
public:
    float angle, x, y, z;

    explicit Rotation(float degrees = 0, float axisX = 0, float axisY = 1, float axisZ = 0)
        : angle(degrees), x(axisX), y(axisY), z(axisZ) {}
    NodeType getType() const override { return ROTATION; }
    void glDraw() override { glRotatef(angle, x, y, z); }
    void load(const QDomElement& e, int difficulty) override;
    void bindCoord(float*, float*, float*) override {}
#ifdef PANG3D_EDITOR
    void save(QDomElement& parent) const override;
#endif
};

// Scaling. Never set from levels; kept for the transformation order of groups.
class Scaling : public Node {
public:
    float x, y, z;

    explicit Scaling(float sx = 1, float sy = 1, float sz = 1) : x(sx), y(sy), z(sz) {}
    NodeType getType() const override { return SCALING; }
    void glDraw() override { glScalef(x, y, z); }
    void bindCoord(float*, float*, float*) override {}
};

// A camera. The fixed default view looks at the centre of the level; the six
// optional views (F3-F8) follow a player from `eye` units away.
class View {
public:
    Point3i eye;
    View() : eye(25, 20, 91) {}
};

// The background sky box.
class CubeMap : public CICubeMap {
public:
    CubeMap() { rr.SetIdentity(); }
    void glDraw();
    // Data-relative base name, e.g. "skyboxes/mount.jpg"
    void setTexture(const QString& baseName);
    QString textureBaseName;

private:
    Matrix44f rr;
};

// Pull of a tornado on a ball or a player that touches it.
class Attraction {
public:
    // Removes gravity and slows the victim down while it is caught
    Attraction(TwisterMove* twisterMove, Movement* victimMovement, float* power);
    // Gives the victim its gravity back
    ~Attraction() { *gravityOfVictim = savedGravity; }
    Attraction(const Attraction&) = delete;
    Attraction& operator=(const Attraction&) = delete;

    void update(float dt);
    // Binds the victim's gravity (set to zero while caught) and the tornado's angular speed
    void bindValues(float* victimGravity, int* angularSpeed)
    {
        gravityOfVictim = victimGravity;
        savedGravity = *victimGravity;
        this->angularSpeed = angularSpeed;
        *victimGravity = 0;
    }

private:
    TwisterMove* twister;
    Movement* victim;
    float savedGravity = 0.0f;
    float* gravityOfVictim = nullptr;
    float* power;
    int* angularSpeed = nullptr;
};

} // namespace pang
