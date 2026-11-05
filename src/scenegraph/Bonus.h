#pragma once
// Power-ups dropped by the balls: extra life, fruit, weapons (bonuses) and
// tornado, bomb (maluses).

#include "Actors.h"
#include "Definitions.h"
#include "Geometry.h"
#include "Model3ds.h"

namespace pang {

class Bonus {
public:
    virtual ~Bonus() = default;
    int points = 0;  // score for the player who picks it up
    BonusType kind = BONUS_FRUIT;

    // Bonuses are spheres for collisions
    virtual Point3f getPos() = 0;
    virtual Point3f getDim() = 0;
    NodeType getType() const { return BONUS; }
    virtual void glDraw() = 0;
    // Spins and floats up and down
    virtual void update(float dt);

protected:
    // 3DS models are loaded once and shared (data-relative path)
    static Model3ds* sharedModel(const QString& file);
    float angle = 0.0f;
    float height = 0.0f;
    float oscillation = 0.5f;
};

// A heart: one more life.
class ExtraLife : public Bonus {
public:
    ExtraLife()
    {
        model = sharedModel(QStringLiteral("models/3ds/heart.3ds"));
        points = 200;
        kind = BONUS_LIFE;
    }
    void glDraw() override;
    Point3f getPos() override { return Point3f(1, 1, 1); }
    Point3f getDim() override { return Point3f(1, 1, 1); }

private:
    Model3ds* model;
};

// Fruit only gives points.
class Fruit : public Bonus {
public:
    void glDraw() override;
    Point3f getPos() override { return Point3f(1, 1, 1); }
    Point3f getDim() override { return Point3f(2.0f, 2.0f, 2.0f); }

protected:
    Fruit(const QString& modelFile, int pts, int modelRotation, float modelScale)
        : model(sharedModel(modelFile)), rotation(modelRotation), scale(modelScale)
    {
        points = pts;
        kind = BONUS_FRUIT;
    }
    Model3ds* model;
    int rotation;  // degrees around X, to put the model upright
    float scale;
};

class Apple : public Fruit {
public:
    Apple() : Fruit(QStringLiteral("models/3ds/apple.3ds"), 50, -90, 0.01f) {}
};

class Banana : public Fruit {
public:
    Banana() : Fruit(QStringLiteral("models/3ds/banana.3ds"), 100, 145, 0.01f) {}
};

class Pear : public Fruit {
public:
    Pear() : Fruit(QStringLiteral("models/3ds/pear.3ds"), 150, -45, 0.01f) {}
};

// A weapon: changes the player's weapon, or adds shots if it is the same one.
class WeaponBonus : public Bonus {
public:
    Weapon weapon = HARPOON;
    int startingShots = 1;  // shots in play when switching to this weapon
    int extraShots = 1;     // shots added when the player already has it

    WeaponBonus()
    {
        kind = BONUS_WEAPON;
    }
    void glDraw() override;
    Point3f getPos() override { return Point3f(0.0f, 0.0f, 0.0f); }
    Point3f getDim() override { return Point3f(1.5f, 1.5f, 1.5f); }
    virtual void drawWeapon() = 0;
};

class HarpoonBonus : public WeaponBonus {
public:
    HarpoonBonus() : hook(0.0f, 0.5f)
    {
        points = 100;
        weapon = HARPOON;
    }
    void drawWeapon() override { hook.glDraw(); }

private:
    Hook hook;
};

class QuadHarpoonBonus : public WeaponBonus {
public:
    QuadHarpoonBonus() : hook(0.0f, 0.5f)
    {
        points = 150;
        weapon = QUAD_HARPOON;
    }
    Point3f getDim() override { return Point3f(2.5f, 2.5f, 2.5f); }
    void drawWeapon() override;

private:
    Hook hook;
};

class GunBonus : public WeaponBonus {
public:
    GunBonus();
    void drawWeapon() override;

private:
    // An L-shaped gun made of three boxes
    Box grip;
    Box barrel;
    Box barrel2;
};

// Malus: starts a tornado (or makes the current one stronger).
class TornadoBonus : public Bonus {
public:
    TornadoBonus() : miniTwister(5, 10, 10, 0.2f, 1000.0f, 1080, 0.0f, 2.0f, 2.0f)
    {
        points = 100;
        kind = BONUS_TORNADO;
    }
    Point3f getPos() override { return Point3f(0.0f, 1.5f, 0.0f); }
    Point3f getDim() override { return Point3f(2.5f, 2.5f, 2.5f); }
    void glDraw() override;

private:
    Twister miniTwister;
};

// Malus: a bomb. It hits every ball, but blinds the players for a while.
class BombBonus : public Bonus {
public:
    explicit BombBonus(float flashDuration);
    Point3f getPos() override { return Point3f(0, 0, 0); }
    Point3f getDim() override { return Point3f(1.5f, 1.5f, 1.5f); }
    void setTexture(GLuint tex)
    {
        texture = tex;
        hasTexture = tex != 0;
    }
    void glDraw() override;

private:
    float flashDuration;
    Box fuse;
    GLuint texture = 0;
    bool hasTexture = false;
};

} // namespace pang
