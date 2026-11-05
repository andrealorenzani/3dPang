#include "Nodes.h"

#include "DataPaths.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace pang {

// ---------------------------------------------------------------- Fall

Fall::Fall(float grav) : gravity(grav) {}

void Fall::onCollision(bool, bool, bool, bool down, bool, bool)
{
    // Many collisions are tested per update: remember if at least one was downwards
    collDown = collDown || down;
}

void Fall::update(float dt)
{
    if (disabled || posy == nullptr)
        return;
    if (collDown) {
        vy = 0;
        collDown = false;
    } else {
        *posy += vy * dt - (0.5f * gravity * dt * dt);
        vy += gravity * dt;
    }
}

// ---------------------------------------------------------------- Attraction

Attraction::Attraction(TwisterMove* twisterMove, Movement* victimMovement, float* pwr)
    : twister(twisterMove), victim(victimMovement), power(pwr)
{
}

void Attraction::update(float dt)
{
    // The victim follows the tornado, spins around it and is pulled to its centre
    const Point3f twisterPos(*(twister->posx), 0, *(twister->posz));
    Point3f victimPos(*(victim->posx), 0.0f, *(victim->posz));
    victimPos[0] += twister->vx * dt;
    victimPos[2] += twister->vz * dt;
    const Point3f relative = victimPos - twisterPos;

    // Pull towards the centre at `power` units per second
    const float distance = std::sqrt((relative[0] * relative[0]) + (relative[2] * relative[2]));
    float pullX = 0.0f;
    float pullZ = 0.0f;
    if (distance > 0) {
        const float a = std::acos(relative[0] / distance);
        pullX = (*power) * std::cos(a);
        pullZ = (*power) * std::sin(a);
    }
    // Balls and players are big: they spin at a third of the tornado's speed
    const float angle = float((*angularSpeed) / 3) * dt;
    glPushMatrix();
    glLoadIdentity();
    glRotatef(angle, 0, 1, 0);
    Matrix44f mm;
    glGetModelViewMatrix(mm);
    const Point3f rotated = mm * Point3f(relative[0], 0.0f, relative[2]);
    glPopMatrix();

    victimPos = rotated + twisterPos;
    victimPos[0] -= pullX * dt;
    victimPos[2] -= pullZ * dt;
    *(victim->posx) = victimPos[0];
    *(victim->posz) = victimPos[2];
}

// ---------------------------------------------------------------- CubeMap

void CubeMap::setTexture(const QString& baseName)
{
    textureBaseName = baseName;
    if (!baseName.isEmpty())
        Load(paths::resolve(baseName));
}

void CubeMap::glDraw()
{
    // Keep only the rotation of the modelview matrix (normalised by its scale)
    Matrix44f mm;
    glGetModelViewMatrix(mm);
    mm[0][3] = 0;
    mm[1][3] = 0;
    mm[2][3] = 0;
    mm[3][3] = 1;
    const float det = mm.Determinant();
    const float dt = det > 0 ? 1.0f / float(std::pow(det, 1.0 / 3.0)) : 1.0f;
    Matrix44f ss;
    ss.SetScale(dt, dt, dt);
    rr = mm * ss;
    DrawEnvCube(rr);
}

// ---------------------------------------------------------------- Bounce

Bounce::Bounce(float maxH, float grav)
    : gravity(grav), maxHeight(maxH), baseMaxHeight(maxH), baseGravity(grav)
{
}

void Bounce::onCollision(bool, bool, bool up, bool down, bool, bool)
{
    collUp = collUp || up;
    collDown = collDown || down;
}

void Bounce::update(float dt)
{
    if (posy == nullptr)
        return;
    if (collUp) {
        if (vy > 0)
            vy = -vy;
        collUp = false;
    }
    if (collDown) {
        // Speed needed to climb back to the maximum height
        float v2 = 2.0f * (-gravity) * (maxHeight - (*posy));
        if (v2 < 0)
            v2 = 0;
        vy = std::sqrt(v2);
        collDown = false;
    }
    *posy += vy * dt - (0.5f * gravity * dt * dt);
    vy += gravity * dt;
}

void Bounce::load(const QDomElement& e, int difficulty)
{
    baseMaxHeight = e.attribute(QStringLiteral("maxHeight")).toFloat();
    baseGravity = e.attribute(QStringLiteral("gravity")).toFloat();
    // Lower bounces and stronger gravity with higher difficulty
    // (gravity is 3.8 times stronger at "superhuman")
    maxHeight = baseMaxHeight * (1.5f - (0.25f * difficulty));
    if (maxHeight < 5.0f)
        maxHeight = 5.0f;
    gravity = (1.0f + (0.7f * difficulty)) * baseGravity;
}

#ifdef PANG3D_EDITOR
void Bounce::save(QDomElement& parent) const
{
    QDomElement e = addChild(parent, QStringLiteral("Bounce"));
    setAttr(e, "maxHeight", baseMaxHeight);
    setAttr(e, "gravity", baseGravity);
}
#endif

// ---------------------------------------------------------------- Rotation

void Rotation::load(const QDomElement& e, int)
{
    if (e.isNull())
        return;
    x = e.attribute(QStringLiteral("x"), QStringLiteral("0")).toFloat();
    y = e.attribute(QStringLiteral("y"), QStringLiteral("1")).toFloat();
    z = e.attribute(QStringLiteral("z"), QStringLiteral("0")).toFloat();
    angle = e.attribute(QStringLiteral("angle"), QStringLiteral("0")).toFloat();
}

#ifdef PANG3D_EDITOR
void Rotation::save(QDomElement& parent) const
{
    QDomElement e = addChild(parent, QStringLiteral("Rotation"));
    setAttr(e, "x", x);
    setAttr(e, "y", y);
    setAttr(e, "z", z);
    setAttr(e, "angle", angle);
}
#endif

// ---------------------------------------------------------------- Movement

Movement::Movement(float velX, float velZ)
    : vx(velX), vz(velZ), startVx(velX), startVz(velZ), baseVx(velX), baseVz(velZ)
{
}

void Movement::onCollision(bool right, bool left, bool, bool, bool nearSide, bool farSide)
{
    collRight = collRight || right;
    collLeft = collLeft || left;
    collNear = collNear || nearSide;
    collFar = collFar || farSide;
}

void Movement::update(float dt)
{
    // On a collision, turn back if moving towards the obstacle
    if (collRight) {
        if (vx > 0)
            vx = -vx;
        collRight = false;
    }
    if (collLeft) {
        if (vx < 0)
            vx = -vx;
        collLeft = false;
    }
    if (collNear) {
        if (vz > 0)
            vz = -vz;
        collNear = false;
    }
    if (collFar) {
        if (vz < 0)
            vz = -vz;
        collFar = false;
    }
    if (posx == nullptr || posz == nullptr)
        return;
    *posx += vx * dt;
    *posz += vz * dt;
}

void Movement::load(const QDomElement& e, int difficulty)
{
    baseVx = e.attribute(QStringLiteral("dx")).toFloat();
    baseVz = e.attribute(QStringLiteral("dz")).toFloat();
    vx = startVx = (1.0f + (0.1f * difficulty)) * baseVx;
    vz = startVz = (1.0f + (0.1f * difficulty)) * baseVz;
}

#ifdef PANG3D_EDITOR
void Movement::save(QDomElement& parent) const
{
    QDomElement e = addChild(parent, QStringLiteral("Movement"));
    setAttr(e, "dx", baseVx);
    setAttr(e, "dz", baseVz);
}
#endif

// ---------------------------------------------------------------- Climb

#ifdef PANG3D_PLAYER
void Climb::update(float dt)
{
    *posY = *posY + (dt * dir * speed);
}
#endif

// ---------------------------------------------------------------- TwisterMove

TwisterMove::TwisterMove(float maxX, float maxZ, int changeEvery)
    : changeInterval(changeEvery), maxSpeedX(maxX), maxSpeedZ(maxZ)
{
}

void TwisterMove::onCollision(bool right, bool left, bool, bool, bool nearSide, bool farSide)
{
    collRight = collRight || right;
    collLeft = collLeft || left;
    collNear = collNear || nearSide;
    collFar = collFar || farSide;
}

void TwisterMove::update(float dt)
{
    internalTime += dt;
    // Never change direction while colliding
    const bool change = !(collRight || collLeft || collNear || collFar) && internalTime > changeInterval;
    if (collRight) {
        if (vx > 0)
            vx = -vx;
        collRight = false;
    }
    if (collLeft) {
        if (vx < 0)
            vx = -vx;
        collLeft = false;
    }
    if (collNear) {
        if (vz > 0)
            vz = -vz;
        collNear = false;
    }
    if (collFar) {
        if (vz < 0)
            vz = -vz;
        collFar = false;
    }
    if (change) {
        // New random direction: each component between -max and +max
        const int sign1 = (1 + std::rand() % 2) * 2 - 3;
        const int sign2 = (1 + std::rand() % 2) * 2 - 3;
        const int mx = std::max(1, int(maxSpeedX));
        const int mz = std::max(1, int(maxSpeedZ));
        vx = float(sign1 * (std::rand() % mx));
        vz = float(sign2 * (std::rand() % mz));
    }
    if (internalTime > changeInterval)
        internalTime -= changeInterval;
    if (posx == nullptr || posz == nullptr)
        return;
    *posx += vx * dt;
    *posz += vz * dt;
}

// ---------------------------------------------------------------- Translation

void Translation::load(const QDomElement& e, int)
{
    if (e.isNull())
        return;
    move(e.attribute(QStringLiteral("x")).toFloat(), e.attribute(QStringLiteral("y")).toFloat(),
         e.attribute(QStringLiteral("z")).toFloat());
}

} // namespace pang
