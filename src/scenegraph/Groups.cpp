#include "Groups.h"

#include <cmath>

namespace pang {

namespace {
QDomElement child(const QDomElement& e, const char* tag)
{
    return e.firstChildElement(QLatin1String(tag));
}
float fattr(const QDomElement& e, const char* name, float def = 0.0f)
{
    bool ok = false;
    const float v = e.attribute(QLatin1String(name)).toFloat(&ok);
    return ok ? v : def;
}
int iattr(const QDomElement& e, const char* name, int def = 0)
{
    bool ok = false;
    const int v = e.attribute(QLatin1String(name)).toInt(&ok);
    return ok ? v : def;
}
bool battr(const QDomElement& e, const char* name)
{
    return iattr(e, name, 0) != 0;
}
Box* loadBox(const QDomElement& e)
{
    auto* box = new Box(fattr(e, "dx", 1), fattr(e, "dy", 1), fattr(e, "dz", 1));
    box->setColor(fattr(e, "r", 1), fattr(e, "g", 1), fattr(e, "b", 1));
    return box;
}
#ifdef PANG3D_EDITOR
void saveBox(QDomElement& parent, const Box& b)
{
    QDomElement e = addChild(parent, QStringLiteral("Box"));
    setAttr(e, "dx", b.dx);
    setAttr(e, "dy", b.dy);
    setAttr(e, "dz", b.dz);
    setAttr(e, "r", b.color[0]);
    setAttr(e, "g", b.color[1]);
    setAttr(e, "b", b.color[2]);
}
void savePosition(QDomElement& parent, float x, float y, float z)
{
    QDomElement e = addChild(parent, QStringLiteral("Position"));
    setAttr(e, "x", x);
    setAttr(e, "y", y);
    setAttr(e, "z", z);
}
#endif
} // namespace

// ---------------------------------------------------------------- Group

Group::Group() : translation(new Translation()), rotation(new Rotation()), scaling(new Scaling())
{
    nowVel.SetZero();
    translation->bindCoord(&posx, &posy, &posz);
}

Group::~Group()
{
    // The attraction first: it gives the gravity back to the fall/bounce node
    delete attraction;
    delete translation;
    delete rotation;
    delete scaling;
    delete movement;
    delete bounce;
    delete twisterMove;
    delete fall;
}

void Group::setAttraction(Attraction* a)
{
    delete attraction;
    attraction = a;
}

void Group::setFall(Fall* f)
{
    delete fall;
    fall = f;
    fall->bindCoord(&posx, &posy, &posz);
}

void Group::onCollision(bool right, bool left, bool up, bool down, bool nearSide, bool farSide)
{
    if (!(right || left || up || down || nearSide || farSide))
        return;
    if (bounce != nullptr)
        bounce->onCollision(right, left, up, down, nearSide, farSide);
    if (fall != nullptr)
        fall->onCollision(right, left, up, down, nearSide, farSide);
    if (movement != nullptr)
        movement->onCollision(right, left, up, down, nearSide, farSide);
    if (twisterMove != nullptr)
        twisterMove->onCollision(right, left, up, down, nearSide, farSide);
}

#ifdef PANG3D_EDITOR
void Group::pick(Point3d p)
{
    // A generic group is a box between getPos() and getDim()
    const Point3f pos = getPos();
    const Point3f dim = getDim();
    if (pos[1] < p[1] && dim[1] > p[1] && pos[2] < p[2] && dim[2] > p[2] && pos[0] < p[0] && dim[0] > p[0])
        selected = !selected;
}
#endif

// ---------------------------------------------------------------- EnvGroup

EnvGroup::~EnvGroup()
{
    clearFixedShadows();
    delete env;
}

void EnvGroup::clearFixedShadows()
{
    for (ShadowRect* s : fixedShadows)
        delete s;
    fixedShadows.clear();
}

Point3f EnvGroup::getPos()
{
    // Lowest corner of the object, in world coordinates
    return env->getPos() + Point3f(posx, posy, posz);
}

Point3f EnvGroup::getDim()
{
    // Highest corner of the object, in world coordinates
    return env->getDim() + Point3f(posx, posy, posz);
}

void EnvGroup::glDraw()
{
    glPushMatrix();
    translation->glDraw();
    rotation->glDraw();
    scaling->glDraw();
    // The outer walls are invisible
    if (!(env->getType() == WALL && isMain))
        env->glDraw();
    glPopMatrix();
}

void EnvGroup::glDrawShadows()
{
    for (ShadowRect* s : fixedShadows)
        s->glDraw();
}

void EnvGroup::setTextureName(GLuint tex)
{
    if (env != nullptr && env->mainBox != nullptr) {
        env->mainBox->texTop = tex;
        env->mainBox->texSide = tex;
    }
}

void EnvGroup::load(const QDomElement& e, GLuint tex, int difficulty)
{
    isMain = battr(e, "main");
    destructible = battr(e, "destructible");
    const QString kind = e.attribute(QStringLiteral("kind"));
    if (kind == QLatin1String("floor")) {
        const QDomElement f = child(e, "Floor");
        auto* floor = new Floor(iattr(f, "cellsX", 1), iattr(f, "cellsZ", 1), fattr(f, "cellSize", 5),
                                fattr(f, "thickness", 1));
        floor->alpha = fattr(f, "alpha", 1);
        const QDomElement box = child(f, "Box");
        if (!box.isNull())
            floor->setSlab(loadBox(box));
        floor->textured = detail > DETAIL_LOW && battr(f, "textured");
        env = floor;
    } else if (kind == QLatin1String("wall")) {
        const QDomElement w = child(e, "Wall");
        const QDomElement box = child(w, "Box");
        auto* wall = new Wall(fattr(box, "dx", 1), fattr(box, "dy", 1), fattr(box, "dz", 1));
        wall->box->setColor(fattr(box, "r", 1), fattr(box, "g", 1), fattr(box, "b", 1));
        wall->alpha = fattr(w, "alpha", 1);
        wall->textured = detail > DETAIL_LOW && battr(w, "textured");
        env = wall;
    } else {
        // "ladder"
        const QDomElement l = child(e, "Ladder");
        auto* ladder = new Ladder();
        ladder->addRungs(iattr(l, "rungs", 1) - 1);
        env = ladder;
    }
    if (detail > DETAIL_LOW)
        setTextureName(tex);

    translation->load(child(e, "Position"), difficulty);
    rotation->load(child(e, "Rotation"), difficulty);
    // A ladder turned by 90 degrees also swaps its collision box
    // (the 2005 loader forgot this, so turned ladders could not be climbed)
    if (env->getType() == LADDER && std::fabs(rotation->angle - 90.0f) < 0.5f)
        static_cast<Ladder*>(env)->rotate90();
#ifdef PANG3D_EDITOR
    storeStartPosition();
#endif
}

#ifdef PANG3D_EDITOR
void EnvGroup::pick(Point3d p)
{
    const Point3f pos = getPos();
    const Point3f dim = getDim();
    if (pos[1] < p[1] - 0.2 && dim[1] > p[1] - 0.2 && pos[2] < p[2] && dim[2] > p[2] && pos[0] < p[0] && dim[0] > p[0]) {
        // Point relative to the object
        const Point3f envPos = env->getPos();
        p = p + Point3d(-(pos[0] - envPos[0]), -(pos[1] - envPos[1]), -(pos[2] - envPos[2]));
        env->pick(p);
        if (env->getType() == FLOOR)
            selected = static_cast<Floor*>(env)->selectedCells > 0;
        else
            selected = env->selected;
    }
}

void EnvGroup::save(QDomElement& parent) const
{
    QDomElement g = addChild(parent, QStringLiteral("Environment"));
    switch (env->getType()) {
    case FLOOR: {
        setAttr(g, "kind", "floor");
        setAttr(g, "texture", texture);
        setAttr(g, "main", isMain);
        setAttr(g, "destructible", destructible);
        const auto* f = static_cast<const Floor*>(env);
        QDomElement fe = addChild(g, QStringLiteral("Floor"));
        setAttr(fe, "cellsX", f->cellsX);
        setAttr(fe, "cellsZ", f->cellsZ);
        setAttr(fe, "cellSize", f->cellSize);
        setAttr(fe, "thickness", f->thickness);
        setAttr(fe, "textured", f->textured);
        setAttr(fe, "alpha", f->alpha);
        saveBox(fe, *f->slab);
        break;
    }
    case WALL: {
        setAttr(g, "kind", "wall");
        setAttr(g, "texture", texture);
        setAttr(g, "main", isMain);
        setAttr(g, "destructible", destructible);
        const auto* w = static_cast<const Wall*>(env);
        QDomElement we = addChild(g, QStringLiteral("Wall"));
        setAttr(we, "textured", w->textured);
        setAttr(we, "alpha", w->alpha);
        saveBox(we, *w->box);
        break;
    }
    default: {
        setAttr(g, "kind", "ladder");
        setAttr(g, "texture", texture);
        setAttr(g, "main", isMain);
        setAttr(g, "destructible", destructible);
        QDomElement le = addChild(g, QStringLiteral("Ladder"));
        setAttr(le, "rungs", static_cast<const Ladder*>(env)->rungs);
        break;
    }
    }
    savePosition(g, startX, startY, startZ);
    rotation->save(g);
}
#endif

// ---------------------------------------------------------------- PlayerGroup

PlayerGroup::PlayerGroup(bool isFirst)
    : player(new Player(5, QString(), QString()
#ifdef PANG3D_EDITOR
                        , isFirst
#endif
                        )),
      first(isFirst)
{
    movement = new Movement(0.0f, 0.0f);
    movement->bindCoord(&posx, &posy, &posz);
    fall = new Fall(0.0f);
    fall->bindCoord(&posx, &posy, &posz);
#ifdef PANG3D_PLAYER
    climb = new Climb(5.0f, &posy);
#endif
}

PlayerGroup::~PlayerGroup()
{
    delete player;
#ifdef PANG3D_PLAYER
    delete climb;
#endif
}

void PlayerGroup::glDraw()
{
    glPushMatrix();
    translation->glDraw();
    rotation->glDraw();
    scaling->glDraw();
    player->glDraw();
    glPopMatrix();
}

Point3f PlayerGroup::getPos()
{
    return player->getPos() + Point3f(posx, posy, posz);
}

Point3f PlayerGroup::getDim()
{
    return player->getDim();
}

void PlayerGroup::onCollision(bool right, bool left, bool up, bool down, bool nearSide, bool farSide)
{
    // Stop only the component moving towards the obstacle: walking along a
    // wall does not get stuck on it
    if (right && movement->vx > 0)
        movement->vx = 0;
    else if (left && movement->vx < 0)
        movement->vx = 0;
    if (down)
        fall->onCollision(right, left, up, down, nearSide, farSide);
    if (nearSide && movement->vz > 0)
        movement->vz = 0;
    else if (farSide && movement->vz < 0)
        movement->vz = 0;
}

void PlayerGroup::die()
{
    dying = true;
    player->setDeath();
}

void PlayerGroup::update(float dt)
{
    player->animTime += dt;
    if (attraction != nullptr)
        attraction->update(dt);
    if (dying) {
        dyingTime += dt;
        // The death animation lasts one second...
        if (dyingTime > 1.0f && !player->dead) {
            player->dead = true;
        } else if (dyingTime > 3.0f) {
            // ...and after three the player is back, if lives are left
#ifdef PANG3D_PLAYER
            if (player->lives > 0) {
#endif
                dying = false;
                player->dead = false;
                if (movement->vx == 0 && movement->vz == 0)
                    player->setStand();
                else
                    player->setRunning();
                dyingTime = 0.0f;
#ifdef PANG3D_PLAYER
                player->lives--;
            }
#endif
        }
    } else {
        movement->update(dt);
    }
    fall->update(dt);
    translation->update(dt);
    rotation->update(dt);
    scaling->update(dt);
#ifdef PANG3D_PLAYER
    if (!dying)
        climb->update(dt);
#endif
}

void PlayerGroup::load(const QDomElement& e, GLuint, int difficulty)
{
    texture = e.attribute(QStringLiteral("texture"));
    const QDomElement m = child(e, "Model");
    baseLives = iattr(m, "lives", 3);
    delete player;
    // One life less every two difficulty levels
    player = new Player(baseLives - (difficulty / 2), m.attribute(QStringLiteral("file")),
                        m.attribute(QStringLiteral("skin"))
#ifdef PANG3D_EDITOR
                        , first
#endif
    );
    translation->load(child(e, "Position"), difficulty);
#ifdef PANG3D_PLAYER
    // Slower and heavier players at higher difficulty
    delete movement;
    movement = new Movement(12 - (1.2f * difficulty), 12 - (1.2f * difficulty));
    movement->bindCoord(&posx, &posy, &posz);
    movement->vx = 0;
    movement->vz = 0;
    setFall(new Fall(-9.8f + float(difficulty / 2)));
    delete climb;
    climb = new Climb(6.5f - 0.5f * difficulty, &posy);
#endif
#ifdef PANG3D_EDITOR
    storeStartPosition();
#endif
}

#ifdef PANG3D_EDITOR
void PlayerGroup::save(QDomElement& parent) const
{
    QDomElement g = addChild(parent, QStringLiteral("Player"));
    QDomElement m = addChild(g, QStringLiteral("Model"));
    setAttr(m, "file", player->modelFile);
    setAttr(m, "skin", player->skinFile);
    setAttr(m, "lives", baseLives);
    savePosition(g, startX, startY, startZ);
}
#endif

// ---------------------------------------------------------------- BallGroup

namespace {
int slicesFor(int detail)
{
    return detail == DETAIL_LOW ? BALL_DETAIL_LOW : detail == DETAIL_MEDIUM ? BALL_DETAIL_MEDIUM : BALL_DETAIL_HIGH;
}
} // namespace

BallGroup::BallGroup(int detailLevel)
    : ball(new Ball(1.0f, slicesFor(detailLevel), slicesFor(detailLevel))), detail(detailLevel)
{
    alpha = ball->sphere->alpha;
    movement = new Movement(0.0f, 0.0f);
    movement->bindCoord(&posx, &posy, &posz);
    bounce = new Bounce(0.0f, 0.0f);
    bounce->bindCoord(&posx, &posy, &posz);
}

BallGroup::~BallGroup()
{
    delete ball;
}

void BallGroup::glDraw()
{
    glPushMatrix();
    translation->glDraw();
    rotation->glDraw();
    scaling->glDraw();
#ifdef PANG3D_EDITOR
    // Selected balls are half transparent
    ball->sphere->alpha = selected ? alpha / 2 : alpha;
#endif
    ball->glDraw();
    glPopMatrix();
}

Point3f BallGroup::getDim()
{
    const float r = ball->sphere->radius;
    return Point3f(r, r, r);
}

void BallGroup::setVel(const Point3f& v)
{
    bounce->vy = v[1];
    movement->vx = v[0];
    movement->vz = v[2];
}

void BallGroup::update(float dt)
{
    if (attraction != nullptr)
        attraction->update(dt);
    bounce->update(dt);
    movement->update(dt);
}

void BallGroup::load(const QDomElement& e, GLuint tex, int difficulty)
{
    children = iattr(e, "children", 0);
    alpha = fattr(e, "alpha", 1);
    const QDomElement s = child(e, "Sphere");
    const float radius = fattr(s, "radius", 1);
    setBall(new Ball(radius, slicesFor(detail), slicesFor(detail)));
    ball->texture = tex;
    ball->textured = detail > DETAIL_LOW && battr(s, "textured");
    ball->sphere->alpha = fattr(s, "alpha", 1);
    ball->sphere->setColor(fattr(s, "r", 1), fattr(s, "g", 0), fattr(s, "b", 0));

    bounce->load(child(e, "Bounce"), difficulty);
    // Always bounce high enough to jump over the player
    if (bounce->maxHeight < 5.0f + radius)
        bounce->maxHeight = 5.0f + radius;
    movement->load(child(e, "Movement"), difficulty);
    translation->load(child(e, "Position"), difficulty);
#ifdef PANG3D_EDITOR
    storeStartPosition();
#endif
}

#ifdef PANG3D_EDITOR
void BallGroup::pick(Point3d p)
{
    const Point3f pos = getPos();
    const double dx = pos[0] - p[0];
    const double dy = pos[1] - p[1];
    const double dz = pos[2] - p[2];
    if (std::sqrt(dx * dx + dy * dy + dz * dz) < getDim()[0])
        selected = !selected;
}

void BallGroup::save(QDomElement& parent) const
{
    QDomElement g = addChild(parent, QStringLiteral("Ball"));
    setAttr(g, "texture", texture);
    setAttr(g, "children", children);
    setAttr(g, "alpha", alpha);
    QDomElement s = addChild(g, QStringLiteral("Sphere"));
    setAttr(s, "radius", ball->sphere->radius);
    setAttr(s, "r", ball->sphere->color[0]);
    setAttr(s, "g", ball->sphere->color[1]);
    setAttr(s, "b", ball->sphere->color[2]);
    setAttr(s, "alpha", ball->sphere->alpha);
    setAttr(s, "textured", ball->textured);
    bounce->save(g);
    movement->save(g);
    savePosition(g, startX, startY, startZ);
}
#endif

// ---------------------------------------------------------------- TwisterGroup

TwisterGroup::TwisterGroup(Twister* t) : twister(t)
{
    twisterMove = new TwisterMove(5.0f, 5.0f, 5);
    twisterMove->bindCoord(&posx, &posy, &posz);
}

void TwisterGroup::glDraw()
{
    glPushMatrix();
    translation->glDraw();
    twister->glDraw();
    glPopMatrix();
}

void TwisterGroup::update(float dt)
{
    twisterMove->update(dt);
    twister->generate(dt);
    twister->angle = float(int((twister->angularSpeed * dt) + twister->angle + 0.5f) % 360);
}

// ---------------------------------------------------------------- ProjectileGroup

ProjectileGroup::ProjectileGroup(PlayerGroup* player) : shooter(player)
{
    const Point3f pos = player->getPos();
    translation->move(pos[0], pos[1], pos[2]);
}

void ProjectileGroup::glDraw()
{
    glPushMatrix();
    translation->glDraw();
    projectile->glDraw();
    glPopMatrix();
}

Point3f ProjectileGroup::getPos()
{
    return projectile->getPos() + Point3f(posx, posy, posz);
}

// ---------------------------------------------------------------- BonusGroup

BonusGroup::BonusGroup(float lifeTime) : timeToLive(lifeTime)
{
    setFall(new Fall(-5.0f));
}

void BonusGroup::glDraw()
{
    // In the last five seconds the bonus blinks every half second
    bool visible = true;
    if (timeToLive - elapsed < 5 && int((timeToLive - elapsed) * 2) % 2 == 0)
        visible = false;
    if (!visible)
        return;
    glPushMatrix();
    translation->glDraw();
    bonus->glDraw();
    glPopMatrix();
}

Point3f BonusGroup::getPos()
{
    return bonus->getPos() + Point3f(posx, posy, posz);
}

void BonusGroup::update(float dt)
{
    elapsed += dt;
    if (timeToLive - elapsed < 0)
        remove = true;
    fall->update(dt);
    bonus->update(dt);
}

} // namespace pang
