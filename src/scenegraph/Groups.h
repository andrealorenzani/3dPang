#pragma once
// Groups: every object in the scene is a group holding the drawable object
// plus the nodes that move it (translation, rotation, movement, gravity...).

#include "Actors.h"
#include "Bonus.h"
#include "Definitions.h"
#include "Environment.h"
#include "Geometry.h"
#include "Nodes.h"
#include "Projectiles.h"

#include <list>

namespace pang {

class Group {
public:
    // Current position, shared with the nodes through bindCoord()
    float posx = 0.0f, posy = 0.0f, posz = 0.0f;
#ifdef PANG3D_EDITOR
    // Start position: what is saved, and where "rewind" brings objects back
    float startX = 0.0f, startY = 0.0f, startZ = 0.0f;
#endif
    // Nodes, owned by the group. Only some are used by each kind of group.
    Translation* translation;
    Rotation* rotation;
    Scaling* scaling;
    Attraction* attraction = nullptr;  // set while caught by a tornado
    Movement* movement = nullptr;
    Bounce* bounce = nullptr;
    TwisterMove* twisterMove = nullptr;
    Fall* fall = nullptr;
    bool selected = false;  // editor selection
    QString texture;        // data-relative texture file
    Point3f nowVel;

    Group();
    virtual ~Group();
    Group(const Group&) = delete;
    Group& operator=(const Group&) = delete;

    // Forwards a collision to every movement node
    virtual void onCollision(bool right, bool left, bool up, bool down, bool nearSide, bool farSide);
    virtual void generateDL() {}
    virtual Point3f getPos() { return Point3f(posx, posy, posz); }
    virtual Point3f getDim() = 0;
    virtual NodeType getType() const { return GROUP; }
    virtual Point3f getVel() { return nowVel; }
    virtual void glDraw() {}
    virtual void update(float) {}
    virtual void load(const QDomElement&, GLuint /*texture*/, int /*difficulty*/) {}

    void setAttraction(Attraction* a);
    void setFall(Fall* f);
#ifdef PANG3D_EDITOR
    virtual void pick(Point3d p);
    virtual void save(QDomElement& /*parent*/) const {}
    void storeStartPosition()
    {
        startX = posx;
        startY = posy;
        startZ = posz;
    }
#endif
};

// A piece of the level: floor, wall or ladder.
class EnvGroup : public Group {
public:
    // Shadows of the balls on this object: [0] x, [1] z of the ball centre,
    // [2] distance between the bottom of the ball and the surface, [3] radius
    std::list<Point4f> ballShadows;
    // Shadows of the floors above this object
    std::list<ShadowRect*> fixedShadows;

    EnvObject* env = nullptr;
    bool isMain = false;        // main floor and outer walls: cannot be moved or deleted
    bool destructible = false;  // destroyed when hit by a shot
    bool destroyed = false;
    int detail;

    explicit EnvGroup(int detailLevel = DETAIL_MEDIUM) : detail(detailLevel) {}
    ~EnvGroup() override;
    void clearFixedShadows();
    Point3f getPos() override;
    Point3f getDim() override;
    void generateDL() override
    {
        if (env != nullptr && env->useDisplayList)
            env->genList();
    }
    NodeType getType() const override { return ENV_GROUP; }
    void glDraw() override;
    void glDrawShadows();
    void load(const QDomElement& e, GLuint texture, int difficulty) override;
    // Applies a texture to the object
    void setTextureName(GLuint tex);
#ifdef PANG3D_EDITOR
    void pick(Point3d p) override;
    void save(QDomElement& parent) const override;
#endif
};

class PlayerGroup : public Group {
public:
#ifdef PANG3D_PLAYER
    Weapon weapon = HARPOON;
    int maxShots = 1;     // shots that can be in play at the same time
    int shotsInPlay = 0;
    Climb* climb;
#endif
    // True while dying and when dead: the corpse is left alone
    bool dying = false;
    bool nearLadder = false;
    float alpha = 1.0f;
    Player* player;
    int baseLives = 3;    // lives in the level file

    explicit PlayerGroup(bool first = true);
    ~PlayerGroup() override;
    Point3f getDim() override;
    Point3f getPos() override;
    NodeType getType() const override { return PLAYER_GROUP; }
    void glDraw() override;
    // Plays the death animation; the player is dead one second later
    void die();
    void onCollision(bool right, bool left, bool up, bool down, bool nearSide, bool farSide) override;
    void update(float dt) override;
    void load(const QDomElement& e, GLuint texture, int difficulty) override;
#ifdef PANG3D_EDITOR
    void save(QDomElement& parent) const override;
#endif

private:
    bool first;
    float dyingTime = 0.0f;
};

class BallGroup : public Group {
public:
    float alpha = 1.0f;
    Ball* ball;
    int children = 0;  // balls it splits into when hit (before difficulty)
    int detail;

    explicit BallGroup(int detailLevel = DETAIL_MEDIUM);
    ~BallGroup() override;
    void generateDL() override
    {
        if (ball->useDisplayList)
            ball->genList();
    }
    Point3f getDim() override;
    Point3f getPos() override { return Point3f(posx, posy, posz); }
    NodeType getType() const override { return BALL_GROUP; }
    Point3f getVel() override { return Point3f(movement->vx, bounce->vy, movement->vz); }
    void glDraw() override;
    void setVel(const Point3f& v);
    void update(float dt) override;
    void load(const QDomElement& e, GLuint texture, int difficulty) override;
    void setBall(Ball* b)
    {
        delete ball;
        ball = b;
    }
#ifdef PANG3D_EDITOR
    void pick(Point3d p) override;
    void save(QDomElement& parent) const override;
#endif
};

// The tornado started by a tornado malus. There is at most one.
class TwisterGroup : public Group {
public:
    Twister* twister;

    explicit TwisterGroup(Twister* t);
    ~TwisterGroup() override { delete twister; }
    void generateDL() override
    {
        if (twister->useDisplayList)
            twister->generateDL();
    }
    Point3f getDim() override { return twister->getDim(); }
    Point3f getPos() override { return Point3f(posx, posy, posz); }
    NodeType getType() const override { return TWISTER; }
    void glDraw() override;
    void update(float dt) override;
};

// A shot in play. It does not move: the projectile grows from where it was fired.
class ProjectileGroup : public Group {
public:
    Projectile* projectile = nullptr;
    PlayerGroup* shooter;
    bool finished = false;  // hit something: remove it

    explicit ProjectileGroup(PlayerGroup* player);
    ~ProjectileGroup() override { delete projectile; }
    void glDraw() override;
    void update(float dt) override { projectile->update(dt); }
    Point3f getPos() override;
    Point3f getDim() override { return projectile->getDim(); }
};

// A power-up falling to the ground and blinking before it disappears.
class BonusGroup : public Group {
public:
    bool remove = false;
    Bonus* bonus = nullptr;

    explicit BonusGroup(float lifeTime);
    ~BonusGroup() override { delete bonus; }
    Point3f getPos() override;
    Point3f getDim() override { return bonus->getDim(); }
    NodeType getType() const override { return BONUS_GROUP; }
    void glDraw() override;
    void update(float dt) override;

private:
    float timeToLive;
    float elapsed = 0.0f;
};

} // namespace pang
