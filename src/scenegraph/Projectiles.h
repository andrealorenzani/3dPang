#pragma once
// Shots fired by the players: harpoon, quadruple harpoon, gun and the
// blast of a bomb power-up.

#include "Definitions.h"
#include "Geometry.h"

namespace pang {

class Projectile {
public:
    virtual ~Projectile() = default;
    Weapon weapon = HARPOON;
    bool active = true;
    float timeToLive = 0.0f;

    virtual Point3f getPos() = 0;
    virtual Point3f getDim() = 0;
    virtual void glDraw() = 0;
    virtual void update(float dt) = 0;

protected:
    float internalTime = 0.0f;
};

// The basic weapon: a rope with a hook that climbs straight up.
class HarpoonShot : public Projectile {
public:
    HarpoonShot(float ttl, int numSteps)
        : harpoon(0.75f, numSteps, 22.5f, float(numSteps) / 10, 0.4f, 4.0f)
    {
        timeToLive = ttl;
        weapon = HARPOON;
    }
    Point3f getPos() override { return harpoon.getPos(); }
    Point3f getDim() override { return harpoon.getDim(); }
    void glDraw() override;
    void update(float dt) override { harpoon.update(dt); }

private:
    Harpoon harpoon;
};

// Four harpoons fired together: in practice a three times wider harpoon.
class QuadHarpoonShot : public Projectile {
public:
    QuadHarpoonShot(float ttl, int numSteps)
        : harpoon(0.75f, numSteps, 22.5f, float(numSteps) / 10, 0.4f, 4.0f)
    {
        timeToLive = ttl;
        weapon = QUAD_HARPOON;
    }
    Point3f getPos() override { return harpoon.getPos(); }
    Point3f getDim() override
    {
        Point3f d = harpoon.getDim();
        d[0] *= 3;
        d[2] *= 3;
        return d;
    }
    void glDraw() override;
    void update(float dt) override { harpoon.update(dt); }

private:
    Harpoon harpoon;
};

// The bomb power-up: a flash that hits every ball once, then a red tint
// that fades away.
class BombBlast : public Projectile {
public:
    bool firstHit = true;  // collides only the first time
    bool expired = false;  // can be removed

    explicit BombBlast(float flashDuration);
    // While hitting, the blast covers the whole level; afterwards it is nowhere
    Point3f getPos() override
    {
        return firstHit ? Point3f(-10000.0f, -10000.0f, -1000.0f) : Point3f(0.0f, 0.0f, 0.0f);
    }
    Point3f getDim() override
    {
        return firstHit ? Point3f(99999.0f, 99999.0f, 99999.0f) : Point3f(0.0f, 0.0f, 0.0f);
    }
    void update(float dt) override;
    void glDraw() override;

private:
    float flashDuration;
    float alpha = 1.0f;
};

// Fast, thin bullets.
class GunShot : public Projectile {
public:
    GunShot(float ttl, float heightPerSecond) : bullet(0.2f, 0.6f, 0.2f), heightPerSec(heightPerSecond)
    {
        timeToLive = ttl;
        weapon = GUN;
    }
    Point3f getPos() override { return Point3f(0.25f, heightPerSec * internalTime, 0.1f); }
    Point3f getDim() override { return Point3f(0.25f, 0.6f, 0.25f); }
    void update(float dt) override
    {
        internalTime += dt;
        active = internalTime < timeToLive;
    }
    void glDraw() override;

private:
    Box bullet;
    float heightPerSec;
};

} // namespace pang
