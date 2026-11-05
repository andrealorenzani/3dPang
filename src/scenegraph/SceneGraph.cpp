#include "SceneGraph.h"

#include "DataPaths.h"
#include "GlUtil.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace pang {

namespace {
template <class T>
void deleteAll(std::list<T*>& items)
{
    for (T* item : items)
        delete item;
    items.clear();
}
} // namespace

SceneGraph::SceneGraph()
    : defaultView(0, 0, 0), cubeMap(new CubeMap()),
      shadowLow(new ShadowCircle(SHADOW_DETAIL_LOW)), shadowMedium(new ShadowCircle(SHADOW_DETAIL_MEDIUM)),
      shadowHigh(new ShadowCircle(SHADOW_DETAIL_HIGH))
{
#ifdef PANG3D_EDITOR
    // Only the main floor exists at the start: it casts no shadow
    recomputeFixedShadows = false;
#endif
    resetClock();
}

SceneGraph::~SceneGraph()
{
    clearLevel();
    delete shadowLow;
    delete shadowMedium;
    delete shadowHigh;
    delete cubeMap;
}

float SceneGraph::tick()
{
    const auto now = std::chrono::steady_clock::now();
    const float dt = std::chrono::duration<float>(now - lastTime).count();
    lastTime = now;
    return dt;
}

void SceneGraph::clearLevel()
{
    deleteAll(balls);
    deleteAll(environment);
    deleteAll(shots);
    deleteAll(bonuses);
    delete twister;
    twister = nullptr;
    delete player1;
    player1 = nullptr;
    delete player2;
    player2 = nullptr;
    recomputeFixedShadows = true;
}

// ================================================================ shadows

void SceneGraph::calculateShadow(BallGroup* ball)
{
    // A ball casts a shadow on every object below its centre. The shadow is
    // stored as: x and z of the centre, distance between the bottom of the
    // ball and the surface, radius.
    const Point3f posBall = ball->getPos();
    const float r = ball->getDim()[0];
    for (EnvGroup* env : environment) {
        const Point3f posEnv = env->getPos();
        const Point3f dimEnv = env->getDim();
        if (posBall[0] - r <= dimEnv[0] && posBall[0] + r >= posEnv[0] && posBall[2] - r <= dimEnv[2] &&
            posBall[2] + r >= posEnv[2] && dimEnv[1] < posBall[1])
            env->ballShadows.push_back(Point4f(posBall[0], posBall[2], (posBall[1] - r) - dimEnv[1], r));
    }
}

void SceneGraph::generateFixedShadows()
{
    for (EnvGroup* e : environment)
        e->clearFixedShadows();
    for (auto a1 = environment.begin(); a1 != environment.end(); ++a1) {
        const Point3f pos1 = (*a1)->getPos();
        const Point3f dim1 = (*a1)->getDim();
        auto a2 = a1;
        for (++a2; a2 != environment.end(); ++a2) {
            const Point3f pos2 = (*a2)->getPos();
            const Point3f dim2 = (*a2)->getDim();
            // Which of the two objects is above the other: the upper one
            // ("caster") projects a rectangle of shadow on the lower one
            EnvGroup* receiver = nullptr;
            EnvGroup* caster = nullptr;
            float deltaH = 0, halfX = 0, halfZ = 0;
            Point3f center;
            if (pos1[1] > dim2[1]) {
                deltaH = pos1[1] - dim2[1];
                receiver = *a2;
                caster = *a1;
                halfX = (dim1[0] - pos1[0]) / 2;
                halfZ = (dim1[2] - pos1[2]) / 2;
                center = Point3f(pos1[0] + halfX, dim2[1], pos1[2] + halfZ);
            } else if (pos2[1] > dim1[1]) {
                deltaH = pos2[1] - dim1[1];
                receiver = *a1;
                caster = *a2;
                halfX = (dim2[0] - pos2[0]) / 2;
                halfZ = (dim2[2] - pos2[2]) / 2;
                center = Point3f(pos2[0] + halfX, dim1[1], pos2[2] + halfZ);
            } else {
                continue;
            }
            // Transparent floors cast lighter shadows, and lighter on glass
            float alpha = 1;
            if (caster->env->getType() == FLOOR)
                alpha = static_cast<Floor*>(caster->env)->alpha;
            if (receiver->env->getType() == FLOOR)
                alpha *= static_cast<Floor*>(receiver->env)->alpha;
            // The higher the caster, the smaller the shadow; too high, no shadow
            const float smaller = std::min(halfX, halfZ);
            if (deltaH > smaller * SHADOW_LOW_MAX_HEIGHT)
                continue;
            float scale = 1 - (deltaH / (smaller * SHADOW_LOW_MAX_HEIGHT));
            if (detail == DETAIL_MEDIUM)
                scale = 1;
            auto* shadow = new ShadowRect(detail);
            shadow->alpha = alpha;
            shadow->halfZ = halfZ * scale;
            shadow->halfX = halfX * scale;
            shadow->pos = center;
            receiver->fixedShadows.push_back(shadow);
        }
    }
    recomputeFixedShadows = false;
}

void SceneGraph::glDrawBallShadows(EnvGroup* env)
{
    // Shadows are drawn on the top face of the object. The circle detail and
    // the size depend on how high the ball is, in multiples of its radius.
    const float h = env->getDim()[1];
    for (const Point4f& s : env->ballShadows) {
        ShadowCircle* c = shadowLow;
        float scale = 0;
        if (s[2] < SHADOW_HIGH_MAX_HEIGHT * s[3]) {
            c = detail == DETAIL_LOW ? shadowLow : detail == DETAIL_MEDIUM ? shadowMedium : shadowHigh;
            // From 1 on the ground to 0.5
            scale = 1 - (s[2] / (2 * (SHADOW_HIGH_MAX_HEIGHT * s[3])));
        } else if (s[2] < SHADOW_MEDIUM_MAX_HEIGHT * s[3]) {
            c = detail >= DETAIL_MEDIUM ? shadowMedium : shadowLow;
            // From 0.5 to 0.25
            scale = 0.5f - ((s[2] - (SHADOW_HIGH_MAX_HEIGHT * s[3])) /
                            (4 * ((SHADOW_MEDIUM_MAX_HEIGHT * s[3]) - (SHADOW_HIGH_MAX_HEIGHT * s[3]))));
        } else if (s[2] < SHADOW_LOW_MAX_HEIGHT * s[3]) {
            c = shadowLow;
            // From 0.25 to 0
            scale = 0.25f - ((s[2] - (SHADOW_MEDIUM_MAX_HEIGHT * s[3])) /
                             (4 * ((SHADOW_LOW_MAX_HEIGHT * s[3]) - (SHADOW_MEDIUM_MAX_HEIGHT * s[3]))));
        }
        if (scale <= 0)
            continue;
        // The circles have radius 1
        scale *= s[3];
        glPushMatrix();
        glTranslatef(s[0], h + 0.05f, s[1]);
        glScalef(scale, 0, scale);
        c->glDraw();
        glPopMatrix();
    }
}

// ================================================================ drawing

void SceneGraph::glDraw()
{
    if (recomputeFixedShadows && shadows && detail > DETAIL_LOW)
        generateFixedShadows();
    for (EnvGroup* e : environment)
        e->ballShadows.clear();

    glPushMatrix();
    if (detail == DETAIL_HIGH)
        cubeMap->glDraw();
    // Ball shadows help aiming: at "superhuman" there are none
    if (shadows && difficulty < SUPERHUMAN)
        for (BallGroup* b : balls)
            calculateShadow(b);

    const bool drawShadows = shadows && detail > DETAIL_LOW;
    for (EnvGroup* e : environment) {
        if (drawShadows) {
            // The object writes 1 in the stencil buffer where its top face
            // is visible (see Box::glDraw)...
            glClear(GL_STENCIL_BUFFER_BIT);
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_ALWAYS, 1, 1);
            glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
        }
        e->glDraw();
        if (drawShadows) {
            // ...and the shadows are drawn only there, without depth test
            // (the stencil already did it, and there is no z-fighting)
            glStencilFunc(GL_EQUAL, 1, 1);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glDisable(GL_DEPTH_TEST);
            glDrawBallShadows(e);
            e->glDrawShadows();
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_STENCIL_TEST);
        }
    }
    for (BallGroup* b : balls)
        b->glDraw();
    for (BonusGroup* b : bonuses)
        b->glDraw();
    if (player1 != nullptr)
        player1->glDraw();
    if (player2 != nullptr)
        player2->glDraw();
#ifdef PANG3D_PLAYER
    if (twister != nullptr)
        twister->glDraw();
    for (ProjectileGroup* s : shots)
        s->glDraw();
#endif
    glPopMatrix();
}

GLuint SceneGraph::loadTexture(const QString& file)
{
    if (file.isEmpty())
        return 0;
    const auto it = textures.find(file);
    if (it != textures.end())
        return it->second;
    const GLuint name = gl::loadTexture2D(paths::resolve(file));
    textures[file] = name;
    return name;
}

void SceneGraph::regenerateDisplayList()
{
    for (EnvGroup* e : environment)
        e->generateDL();
    for (BallGroup* b : balls)
        b->generateDL();
}

// ================================================================ collisions

void SceneGraph::collide(BallGroup* p1, BallGroup* p2)
{
    // Two spheres collide when their distance is less than the sum of the radii
    const Point3f pos1 = p1->getPos();
    const Point3f pos2 = p2->getPos();
    const float r1 = p1->getDim()[0];
    const float r2 = p2->getDim()[0];
    const Point3f d = pos1 - pos2;
    const float distance = std::sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    if (distance >= r1 + r2)
        return;

    // Elastic collision. The mass is proportional to the volume (all balls
    // have the same density).
    const Point3f v1 = p1->getVel();
    const Point3f v2 = p2->getVel();
    const float m1 = 4.0f / 3.0f * PI * std::pow(r1, 3.0f);
    const float m2 = 4.0f / 3.0f * PI * std::pow(r2, 3.0f);
    Point3f nv1, nv2;
    for (int i = 0; i < 3; ++i) {
        nv1[i] = (2 * m2 * v2[i] + (m1 - m2) * v1[i]) / (m1 + m2);
        nv2[i] = (2 * m1 * v1[i] + (m2 - m1) * v2[i]) / (m1 + m2);
    }
    p1->setVel(nv1);
    p2->setVel(nv2);

    // Push the balls apart so that they do not merge. (The 2005 version
    // decomposed the direction with asin and always pushed along +x.)
    if (distance > 0.0f) {
        const float push = (r1 + r2 - distance) / 2.0f;
        const Point3f n = d / distance;
        p1->posx += push * n[0];
        p1->posy += push * n[1];
        p1->posz += push * n[2];
        p2->posx -= push * n[0];
        p2->posy -= push * n[1];
        p2->posz -= push * n[2];
    }
}

void SceneGraph::collide(BallGroup* p, EnvGroup* a)
{
    // Sphere against box, taking care of the edges.
    const Point3f pos1 = p->getPos();
    const Point3f pos2 = a->getPos();
    const Point3f dim1 = p->getDim();
    const Point3f dim2 = a->getDim();
    bool collisionDetect = false;
    bool right = false, left = false, nearSide = false, farSide = false, up = false, down = false;

    // First the cube around the sphere against the box: if the sphere
    // collides, its bounding cube does too
    if ((pos1[0] + dim1[0] - pos2[0]) * (pos1[0] - dim1[0] - dim2[0]) <= 0 &&
        (pos1[2] + dim1[0] - pos2[2]) * (pos1[2] - dim1[0] - dim2[2]) <= 0 &&
        (pos1[1] + dim1[0] - pos2[1]) * (pos1[1] - dim1[0] - dim2[1]) <= 0) {
        // The cube collides but the sphere may not: check the distance from
        // the edges. These booleans tell roughly where the ball is.
        collisionDetect = true;
        const bool belowX = pos1[0] < pos2[0], belowY = pos1[1] < pos2[1], belowZ = pos1[2] < pos2[2];
        const bool aboveX = pos1[0] > dim2[0], aboveY = pos1[1] > dim2[1], aboveZ = pos1[2] > dim2[2];
        float edgeDistance = 0.0f;
        float constPart = 0.0f;
        bool hit = false;
        bool onFace = false;

        // Projection on the XZ plane. The box becomes a rectangle whose
        // corners are real corners of the box if the ball is above or below
        // it, or points inside a vertical edge otherwise (constPart = 0).
        if (aboveY)
            constPart = (pos1[1] - dim2[1]) * (pos1[1] - dim2[1]);
        else if (belowY)
            constPart = (pos1[1] - pos2[1]) * (pos1[1] - pos2[1]);
        if (belowX && belowZ)
            edgeDistance = std::sqrt((pos1[0] - pos2[0]) * (pos1[0] - pos2[0]) + (pos1[2] - pos2[2]) * (pos1[2] - pos2[2]) + constPart);
        else if (aboveX && belowZ)
            edgeDistance = std::sqrt((pos1[0] - dim2[0]) * (pos1[0] - dim2[0]) + (pos1[2] - pos2[2]) * (pos1[2] - pos2[2]) + constPart);
        else if (belowX && aboveZ)
            edgeDistance = std::sqrt((pos1[0] - pos2[0]) * (pos1[0] - pos2[0]) + (pos1[2] - dim2[2]) * (pos1[2] - dim2[2]) + constPart);
        else if (aboveX && aboveZ)
            edgeDistance = std::sqrt((pos1[0] - pos2[0]) * (pos1[0] - pos2[0]) + (pos1[2] - pos2[2]) * (pos1[2] - pos2[2]) + constPart);
        hit = edgeDistance < dim1[0];
        // Distance 0: no edge test applies on this projection
        onFace = edgeDistance == 0.0f;
        if (aboveX)
            right = hit && !onFace;
        else if (belowX)
            left = hit && !onFace;
        if (aboveZ)
            farSide = hit && !onFace;
        else if (belowZ)
            nearSide = hit && !onFace;
        if (!onFace)
            collisionDetect = false;

        // Projection on the XY plane
        constPart = 0.0f;
        if (aboveZ)
            constPart = (pos1[2] - dim2[2]) * (pos1[2] - dim2[2]);
        else if (belowZ)
            constPart = (pos1[2] - pos2[2]) * (pos1[2] - pos2[2]);
        if (belowX && belowY)
            edgeDistance = std::sqrt((pos1[0] - pos2[0]) * (pos1[0] - pos2[0]) + (pos1[1] - pos2[1]) * (pos1[1] - pos2[1]) + constPart);
        else if (aboveX && belowY)
            edgeDistance = std::sqrt((pos1[0] - dim2[0]) * (pos1[0] - dim2[0]) + (pos1[1] - pos2[1]) * (pos1[1] - pos2[1]) + constPart);
        else if (belowX && aboveY)
            edgeDistance = std::sqrt((pos1[0] - pos2[0]) * (pos1[0] - pos2[0]) + (pos1[1] - dim2[1]) * (pos1[1] - dim2[1]) + constPart);
        else if (aboveX && aboveY)
            edgeDistance = std::sqrt((pos1[0] - pos2[0]) * (pos1[0] - pos2[0]) + (pos1[1] - pos2[1]) * (pos1[1] - pos2[1]) + constPart);
        hit = edgeDistance < dim1[0];
        onFace = edgeDistance == 0.0f;
        if (aboveX)
            left = left || (hit && !onFace);
        else if (belowX)
            right = right || (hit && !onFace);
        if (aboveY)
            down = hit && !onFace;
        else if (belowY)
            up = hit && !onFace;
        if (!onFace)
            collisionDetect = false;

        // Projection on the ZY plane
        constPart = 0.0f;
        if (aboveX)
            constPart = (pos1[0] - dim2[0]) * (pos1[0] - dim2[0]);
        else if (belowX)
            constPart = (pos1[0] - pos2[0]) * (pos1[0] - pos2[0]);
        if (belowZ && belowY)
            edgeDistance = std::sqrt((pos1[2] - pos2[2]) * (pos1[2] - pos2[2]) + (pos1[1] - pos2[1]) * (pos1[1] - pos2[1]) + constPart);
        else if (aboveZ && belowY)
            edgeDistance = std::sqrt((pos1[2] - dim2[2]) * (pos1[2] - dim2[2]) + (pos1[1] - pos2[1]) * (pos1[1] - pos2[1]) + constPart);
        else if (belowZ && aboveY)
            edgeDistance = std::sqrt((pos1[2] - pos2[2]) * (pos1[2] - pos2[2]) + (pos1[1] - dim2[1]) * (pos1[1] - dim2[1]) + constPart);
        else if (aboveZ && aboveY)
            edgeDistance = std::sqrt((pos1[2] - pos2[2]) * (pos1[2] - pos2[2]) + (pos1[1] - pos2[1]) * (pos1[1] - pos2[1]) + constPart);
        hit = edgeDistance < dim1[0];
        onFace = edgeDistance == 0.0f;
        if (aboveZ)
            farSide = farSide || (hit && !onFace);
        else if (belowZ)
            nearSide = nearSide || (hit && !onFace);
        if (aboveY)
            down = down || (hit && !onFace);
        else if (belowY)
            up = up || (hit && !onFace);
        if (!onFace)
            collisionDetect = false;
    }

    if (collisionDetect) {
        // No edge involved: a plain collision with a face
        if (pos1[0] < pos2[0])
            right = true;
        else if (pos1[0] > dim2[0])
            left = true;
        else if (pos1[1] < pos2[1])
            up = true;
        else if (pos1[1] > dim2[1])
            down = true;
        else if (pos1[2] < pos2[2])
            nearSide = true;
        else if (pos1[2] > dim2[2])
            farSide = true;
    }
    p->onCollision(right, left, up, down, nearSide, farSide);
}

void SceneGraph::collide(BallGroup* p, PlayerGroup* g)
{
    // Sphere against the cylinder of the player. A dying player is left alone.
    if (g->dying)
        return;
    const Point3f pos1 = p->getPos();
    const Point3f dim1 = p->getDim();
    const Point3f pos2 = g->getPos();
    const Point3f dim2 = g->getDim();
    const Point3f d = pos1 - pos2;
    const float dist = std::sqrt(d[0] * d[0] + d[2] * d[2]);
    if (dist < dim2[0] + dim1[0]) {
        // Above the feet: closer than height + radius; below: closer than the radius
        if ((d[1] > 0 && d[1] < (dim2[0] + dim1[1])) || (d[1] < 0 && -d[1] < dim1[0]))
            g->die();
    }
}

void SceneGraph::collide(EnvGroup* a, PlayerGroup* g)
{
    // The player is a box too here
    bool right = false, left = false, nearSide = false, farSide = false, up = false, down = false;
    const Point3f pos1 = g->getPos();
    const Point3f dim1 = g->getDim();
    const Point3f pos2 = a->getPos();
    const Point3f dim2 = a->getDim();
    if ((pos1[0] + dim1[0] - pos2[0]) * (pos1[0] - dim1[0] - dim2[0]) <= 0 &&
        (pos1[2] + dim1[0] - pos2[2]) * (pos1[2] - dim1[0] - dim2[2]) <= 0 &&
        ((pos1[1] - pos2[1] > 0 && pos1[1] < dim2[1]) || (pos1[1] - pos2[1] < 0 && pos2[1] < dim1[1]))) {
        if (a->env->getType() == LADDER) {
            // Ladders do not stop the player: they can be climbed, and catch
            // a falling player
            g->nearLadder = true;
            g->fall->disabled = true;
            g->fall->vy = 0.0f;
        } else {
            if (pos1[1] < dim2[1] && pos1[1] > pos2[1])
                down = true;
            const float x1 = pos1[0] + dim1[0] - dim2[0];
            const float z1 = pos1[2] + dim1[0] - dim2[2];
            // Only collisions in the walking direction count (no getting
            // stuck on walls), and the player is pushed out of the object
            if (x1 * (pos1[0] - dim1[0] - pos2[0]) > 0) {
                if (x1 > 0 && g->movement->vx < 0) {
                    left = true;
                    g->translation->move(dim2[0] - pos1[0] + dim1[0], 0, 0);
                } else if (x1 < 0 && g->movement->vx > 0) {
                    right = true;
                    g->translation->move(pos2[0] - pos1[0] - dim1[0], 0, 0);
                }
            }
            if (z1 * (pos1[2] - dim1[0] - pos2[2]) > 0) {
                if (z1 > 0 && g->movement->vz < 0) {
                    farSide = true;
                    g->translation->move(0, 0, -(dim2[2] - pos1[2] + dim1[2]));
                } else if (z1 < 0 && g->movement->vz > 0) {
                    nearSide = true;
                    g->translation->move(0, 0, -(pos2[2] - pos1[2] - dim1[2]));
                }
            }
        }
    }
    g->onCollision(right, left, up, down, nearSide, farSide);
}

void SceneGraph::collide(EnvGroup* a, BonusGroup* b)
{
    // Bonuses only fall: they can only hit something below them
    const Point3f pos1 = b->getPos();
    const Point3f dim1 = b->getDim();
    const Point3f pos2 = a->getPos();
    const Point3f dim2 = a->getDim();
    if ((pos1[0] + dim1[0] - pos2[0]) * (pos1[0] - dim1[0] - dim2[0]) <= 0 &&
        (pos1[2] + dim1[0] - pos2[2]) * (pos1[2] - dim1[0] - dim2[2]) <= 0 &&
        (pos1[1] + dim1[0] - pos2[1]) * (pos1[1] - dim1[0] - dim2[1]) <= 0)
        b->onCollision(false, false, false, true, false, false);
}

#ifdef PANG3D_PLAYER
void SceneGraph::collide(BonusGroup* b, PlayerGroup* g)
{
    // Sphere against cylinder, as for the balls
    const Point3f pos1 = b->getPos();
    const Point3f dim1 = b->getDim();
    const Point3f pos2 = g->getPos();
    const Point3f dim2 = g->getDim();
    const Point3f d = pos1 - pos2;
    const float dist = std::sqrt(d[0] * d[0] + d[2] * d[2]);
    if (dist < dim1[0] + dim2[0] &&
        ((d[1] > 0 && d[1] < (dim2[1] - pos2[1]) + dim1[0]) || (d[1] < 0 && -d[1] < dim1[0]))) {
        apply(b->bonus, g);
        b->remove = true;
    }
}

void SceneGraph::spawnPowerUp(const Point3f& where, float gravity)
{
    if ((std::rand() % 100) >= powerUpChance)
        return;
    // Shorter life at higher difficulty
    auto* bonus = new BonusGroup(float(10 - difficulty));
    bonus->translation->move(where[0], where[1], where[2]);
    if ((std::rand() % 100) < (bonusChance - 5 * difficulty)) {
        // A bonus
        const int roll = std::rand() % 100;
        const int modifier = 5 * difficulty;
        if (roll <= lifeWeight - modifier) {
            bonus->bonus = new ExtraLife();
        } else if (roll <= lifeWeight + fruitWeight) {
            if (roll % 3 == 0)
                bonus->bonus = new Apple();
            else if (roll % 3 == 1)
                bonus->bonus = new Pear();
            else
                bonus->bonus = new Banana();
        } else {
            if (roll % 3 == 0)
                bonus->bonus = new HarpoonBonus();
            else if (roll % 3 == 1)
                bonus->bonus = new QuadHarpoonBonus();
            else
                bonus->bonus = new GunBonus();
        }
    } else {
        // A malus
        const int roll = std::rand() % 100;
        if (roll <= bombWeight) {
            // Longer flash at higher difficulty
            auto* bomb = new BombBonus(2 + 0.3f * difficulty);
            if (detail > DETAIL_LOW)
                bomb->setTexture(loadTexture(QStringLiteral("textures/bomb.jpg")));
            bonus->bonus = bomb;
        } else {
            bonus->bonus = new TornadoBonus();
        }
    }
    // It falls with the gravity of the ball
    bonus->setFall(new Fall(gravity));
    bonuses.push_back(bonus);
}

void SceneGraph::collide(BallGroup* b, ProjectileGroup* p)
{
    // Sphere against the cylinder of the shot. When a ball is hit it shrinks
    // and splits into smaller balls, flying apart like a star, and may drop
    // a power-up.
    const Point3f pos1 = b->getPos();
    const Point3f dim1 = b->getDim();
    const Point3f pos2 = p->getPos();
    const Point3f dim2 = p->getDim();
    const Point3f d = pos1 - pos2;
    const float dist = std::sqrt(d[0] * d[0] + d[2] * d[2]);
    if (dist >= dim2[0] + dim1[0])
        return;
    if (!((d[1] > 0 && d[1] < (dim1[0] + dim2[1])) || (d[1] < 0 && -d[1] < dim2[0])))
        return;

    // Points depend on the size of the ball and the difficulty
    const int points = int(b->ball->sphere->radius * (100 - 15 * difficulty));
    if (p->shooter == player1)
        score1 += points;
    else
        score2 += points;

    const float radius = b->ball->sphere->radius;
    const int slices = b->ball->sphere->slices;
    const int stacks = b->ball->sphere->stacks;
    const int newBalls = b->children + difficulty - 1;
    const Point3f velocity = b->getVel();
    const float speed = std::sqrt(velocity[0] * velocity[0] + velocity[2] * velocity[2]);
    // Direction of the ball on the XZ plane, and the angle between the new balls
    const double oldAngle = speed != 0 ? std::acos(velocity[0] / speed) : 0.0;
    const double deltaAngle = newBalls > 1 ? (360 / newBalls) / (180 / PI) : 0.0;
    double angle = oldAngle;
    const float offset = newBalls > 1 ? radius + (newBalls / 4) : 0.0f;
    const Point3f ballPos = b->getPos();

    // The ball that was hit becomes the first of the new ones
    b->translation->move(float(offset * std::cos(oldAngle)), 0.5f, float(offset * std::sin(oldAngle)));
    b->bounce->maxHeight = (b->bounce->maxHeight / radius) * (radius - 1);
    // Always high enough to jump over a player
    if (b->bounce->maxHeight < 5.0f + b->ball->sphere->radius)
        b->bounce->maxHeight = 5.0f + b->ball->sphere->radius;
    b->ball->sphere->radius -= 1;
    b->onCollision(false, false, false, true, false, false);  // bounces off the shot

    for (int i = 1; i < newBalls; i++) {
        angle += deltaAngle;
        auto* nb = new BallGroup(detail);
        nb->children = b->children;
        nb->bounce->maxHeight = b->bounce->maxHeight;
        nb->bounce->gravity = b->bounce->gravity;
        nb->bounce->vy = b->bounce->vy;
        auto* sphere = new Sphere(radius - 1, slices, stacks);
        const Point3f col = b->ball->sphere->color;
        sphere->setColor(col[0], col[1], col[2]);
        nb->ball->setSphere(sphere);
        nb->ball->textured = b->ball->textured;
        if (nb->ball->textured && detail > DETAIL_LOW) {
            nb->texture = b->texture;
            nb->ball->texture = b->ball->texture;
        }
        nb->movement->vx = float(speed * std::cos(angle));
        nb->movement->vz = float(speed * std::sin(angle));
        nb->translation->move(float(ballPos[0] + (offset * std::cos(angle))), ballPos[1],
                              float(ballPos[2] + (offset * std::sin(angle))));
        balls.push_front(nb);
    }
    spawnPowerUp(pos1, b->bounce->gravity);
    p->finished = true;
}

void SceneGraph::collide(EnvGroup* a, ProjectileGroup* p)
{
    // A shot stops on the first object it hits, and destroys it if destructible
    const Point3f pos1 = a->getPos();
    const Point3f dim1 = a->getDim();
    const Point3f pos2 = p->getPos();
    const Point3f dim2 = p->getDim();
    const Point3f d = pos2 - pos1;
    bool hitX = false, hitY = false, hitZ = false;
    if (d[0] > 0) {
        if (d[0] < (dim1[0] - pos1[0]) + dim2[0])
            hitX = true;
    } else if (-d[0] < dim2[0]) {
        hitX = true;
    }
    if (d[2] > 0) {
        if (d[2] < (dim1[2] - pos1[2]) + dim2[0])
            hitZ = true;
    } else if (-d[2] < dim2[2]) {
        hitZ = true;
    }
    // Shots go up: only objects above the base of the shot matter
    if (pos1[1] > pos2[1] && pos1[1] < dim2[1])
        hitY = true;
    if (hitX && hitY && hitZ) {
        if (a->destructible && !a->isMain)
            a->destroyed = true;
        p->finished = true;
    }
}

void SceneGraph::collide(EnvGroup* a, TwisterGroup* tw)
{
    // The tornado changes direction when it hits a wall
    const Point3f pos1 = tw->getPos();
    const Point3f dim1 = tw->getDim();
    const Point3f pos2 = a->getPos();
    const Point3f dim2 = a->getDim();
    if (!((pos1[0] + dim1[0] - pos2[0]) * (pos1[0] - dim1[0] - dim2[0]) <= 0 &&
          (pos1[2] + dim1[0] - pos2[2]) * (pos1[2] - dim1[0] - dim2[2]) <= 0 &&
          ((pos1[1] - pos2[1] > 0 && pos1[1] < dim2[1]) || (pos1[1] - pos2[1] < 0 && pos2[1] < dim1[1]))))
        return;
    bool right = false, left = false, nearSide = false, farSide = false;
    const float x1 = pos1[0] + dim1[0] - dim2[0];
    const float z1 = pos1[2] + dim1[0] - dim2[2];
    if (x1 * (pos1[0] - dim1[0] - pos2[0]) > 0) {
        if (x1 > 0 && tw->twisterMove->vx < 0) {
            left = true;
            tw->translation->move(dim2[0] - pos1[0] + dim1[0], 0, 0);
        } else if (x1 < 0 && tw->twisterMove->vx > 0) {
            right = true;
            tw->translation->move(pos2[0] - pos1[0] - dim1[0], 0, 0);
        }
    }
    if (z1 * (pos1[2] - dim1[0] - pos2[2]) > 0) {
        if (z1 > 0 && tw->twisterMove->vz < 0) {
            farSide = true;
            tw->translation->move(0, 0, -(dim2[2] - pos1[2] + dim1[2]));
        } else if (z1 < 0 && tw->twisterMove->vz > 0) {
            nearSide = true;
            tw->translation->move(0, 0, -(pos2[2] - pos1[2] - dim1[2]));
        }
    }
    tw->onCollision(right, left, false, false, nearSide, farSide);
}

void SceneGraph::collide(BallGroup* p, TwisterGroup* tw)
{
    // A ball touching the tornado is caught by it
    const Point3f pos2 = tw->getPos();
    const Point3f dim2 = tw->getDim();
    const Point3f pos1 = p->getPos();
    const Point3f dim1 = p->getDim();
    const Point3f d = pos1 - pos2;
    const float dist = std::sqrt(d[0] * d[0] + d[2] * d[2]);
    bool caught = false;
    if (dist < dim2[0] + dim1[0] &&
        ((d[1] > 0 && d[1] < (dim2[0] + dim1[1])) || (d[1] < 0 && -d[1] < dim1[0]))) {
        caught = true;
        if (p->attraction == nullptr) {
            auto* a = new Attraction(tw->twisterMove, p->movement, &tw->twister->power);
            a->bindValues(&p->bounce->gravity, &tw->twister->angularSpeed);
            p->setAttraction(a);
            p->bounce->vy = 0;
        }
    }
    if (!caught && p->attraction != nullptr)
        p->setAttraction(nullptr);
}

void SceneGraph::collide(PlayerGroup* g, TwisterGroup* tw)
{
    // Cylinder against cylinder
    const Point3f pos2 = tw->getPos();
    const Point3f dim2 = tw->getDim();
    const Point3f pos1 = g->getPos();
    const Point3f dim1 = g->getDim();
    const Point3f d = pos1 - pos2;
    const float dist = std::sqrt(d[0] * d[0] + d[2] * d[2]);
    bool caught = false;
    if (dist < dim2[0] + dim1[0] && ((d[1] >= 0 && d[1] < dim2[1]) || (d[1] < 0 && -d[1] < dim1[1]))) {
        caught = true;
        if (g->attraction == nullptr) {
            auto* a = new Attraction(tw->twisterMove, g->movement, &tw->twister->power);
            a->bindValues(&g->fall->gravity, &tw->twister->angularSpeed);
            g->setAttraction(a);
            // (2005: this line set the fall's position *pointer* to NULL)
            g->fall->vy = 0;
        }
    }
    if (!caught && g->attraction != nullptr)
        g->setAttraction(nullptr);
}

void SceneGraph::removeFinishedShots()
{
    for (auto s = shots.begin(); s != shots.end();) {
        ProjectileGroup* shot = *s;
        if (shot->projectile->weapon == BOMB_BLAST) {
            // The blast hits only in its first frame, and stays until the
            // flash has faded out
            auto* blast = static_cast<BombBlast*>(shot->projectile);
            blast->firstHit = false;
            if (blast->expired) {
                delete shot;
                s = shots.erase(s);
                continue;
            }
        } else if (shot->finished || !shot->projectile->active) {
            // One more shot available for the player
            shot->shooter->shotsInPlay--;
            delete shot;
            s = shots.erase(s);
            continue;
        }
        ++s;
    }
}
#endif

void SceneGraph::collide()
{
    // Balls against everything (each pair of balls once)
    for (auto i = balls.begin(); i != balls.end();) {
        BallGroup* ball = *i;
        // Balls smaller than 1 have been popped
        if (ball->ball->sphere->radius < 1) {
            delete ball;
            i = balls.erase(i);
#ifdef PANG3D_PLAYER
            if (balls.empty())
                levelCompleted = true;
#endif
            continue;
        }
        auto j = i;
        for (++j; j != balls.end(); ++j)
            collide(ball, *j);
        for (EnvGroup* a : environment)
            collide(ball, a);
        if (player1 != nullptr)
            collide(ball, player1);
        if (player2 != nullptr)
            collide(ball, player2);
#ifdef PANG3D_PLAYER
        for (ProjectileGroup* s : shots) {
            // The blast only hits balls that can still shrink
            if (s->projectile->weapon != BOMB_BLAST || ball->ball->sphere->radius > 1)
                collide(ball, s);
        }
        if (twister != nullptr && !twister->twister->generating())
            collide(ball, twister);
#endif
        ++i;
    }

    // The environment. Reset the ladder state first: a player who is no
    // longer on a ladder falls again
    for (PlayerGroup* g : {player1, player2}) {
        if (g != nullptr) {
            g->nearLadder = false;
            g->fall->disabled = false;
        }
    }
    for (EnvGroup* a : environment) {
        if (player1 != nullptr)
            collide(a, player1);
        if (player2 != nullptr)
            collide(a, player2);
        for (BonusGroup* b : bonuses)
            collide(a, b);
#ifdef PANG3D_PLAYER
        for (ProjectileGroup* s : shots)
            collide(a, s);
        if (twister != nullptr)
            collide(a, twister);
#endif
    }

#ifdef PANG3D_PLAYER
    // Bonuses picked up by the players
    for (auto b = bonuses.begin(); b != bonuses.end();) {
        if (player1 != nullptr)
            collide(*b, player1);
        if (player2 != nullptr)
            collide(*b, player2);
        if ((*b)->remove) {
            delete *b;
            b = bonuses.erase(b);
        } else {
            ++b;
        }
    }
    if (twister != nullptr && !twister->twister->generating()) {
        if (player1 != nullptr)
            collide(player1, twister);
        if (player2 != nullptr)
            collide(player2, twister);
    }
    removeFinishedShots();
    // Moving to the next level only now: the 2005 code did it in the middle
    // of the loop above, while iterating over lists it was clearing
    if (levelCompleted) {
        levelCompleted = false;
        nextLevel();
    }
#endif
}

// ================================================================ update

void SceneGraph::update(float dt)
{
    // Destroyed floors and walls disappear (and the shadows change)
    for (auto a = environment.begin(); a != environment.end();) {
        if ((*a)->destroyed) {
            delete *a;
            a = environment.erase(a);
            recomputeFixedShadows = true;
        } else {
            ++a;
        }
    }
    for (BallGroup* b : balls) {
        b->update(dt);
        // Balls should never leave the main floor, but just in case put
        // them back and turn them around
        const Point3f pos = b->getPos();
        const float r = b->ball->sphere->radius;
        if (pos[0] < 0.0f) {
            b->posx = r;
            if (b->movement->vx < 0)
                b->movement->vx = -b->movement->vx;
            b->movement->update(0.01f);
        } else if (pos[0] > numCellsX * cellSize) {
            b->posx = numCellsX * cellSize - r;
            if (b->movement->vx > 0)
                b->movement->vx = -b->movement->vx;
            b->movement->update(0.01f);
        }
        if (pos[1] < 0.0f) {
            b->posy = 2 * r + floorThickness;
            if (b->bounce->vy < 0)
                b->bounce->onCollision(false, false, false, true, false, false);
            b->bounce->update(0.01f);
        }
        if (pos[2] < 0.0f) {
            b->posz += r;
            if (b->movement->vz < 0)
                b->movement->vz = -b->movement->vz;
            b->movement->update(0.01f);
        } else if (pos[2] > numCellsZ * cellSize) {
            b->posz = numCellsZ * cellSize - r;
            if (b->movement->vz > 0)
                b->movement->vz = -(b->movement->vz / 2.0f);
            b->movement->update(0.01f);
        }
    }
    for (BonusGroup* b : bonuses)
        b->update(dt);
    for (ProjectileGroup* s : shots)
        s->update(dt);
    if (player1 != nullptr)
        player1->update(dt);
    if (player2 != nullptr)
        player2->update(dt);
#ifdef PANG3D_PLAYER
    if (twister != nullptr) {
        if (!twister->twister->exists()) {
            // The tornado is over: release everything it caught
            delete twister;
            twister = nullptr;
            for (BallGroup* b : balls)
                b->setAttraction(nullptr);
            if (player1 != nullptr)
                player1->setAttraction(nullptr);
            if (player2 != nullptr)
                player2->setAttraction(nullptr);
        } else {
            twister->update(dt);
        }
    }
#endif
}

// ================================================================ game flow

#ifdef PANG3D_PLAYER
void SceneGraph::buildLevelQueue()
{
    levelQueue.clear();
    if (!selectedCampaign.isEmpty()) {
        // A campaign is a text file with one level per line ('#' starts a comment).
        // (The 2005 reader skipped every other line.)
        QFile file(paths::resolve(QStringLiteral("campaigns/") + selectedCampaign));
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                const QString line = in.readLine().trimmed();
                if (line.isEmpty() || line.startsWith(QLatin1Char('#')))
                    continue;
                const QString path = paths::resolve(QStringLiteral("levels/") + QFileInfo(line).fileName());
                if (QFileInfo::exists(path))
                    levelQueue << path;
                else
                    qWarning("3dPang: campaign level '%s' not found", qPrintable(line));
            }
        }
    }
    if (levelQueue.isEmpty() && !selectedLevel.isEmpty())
        levelQueue << paths::resolve(QStringLiteral("levels/") + selectedLevel);
}

bool SceneGraph::startGame()
{
    clearLevel();
    buildLevelQueue();
    if (levelQueue.isEmpty())
        return false;
    currentLevel = levelQueue.takeFirst();
    score1 = 0;
    score2 = 0;
    scoresSaved = false;
    levelCompleted = false;
    if (!loadLevel(currentLevel))
        return false;
    if (numPlayers < 2) {
        delete player2;
        player2 = nullptr;
    }
    resetClock();
    return true;
}

void SceneGraph::nextLevel()
{
    if (levelQueue.isEmpty()) {
        victory();
        return;
    }
    // The players keep their lives
    const int lives1 = player1 != nullptr ? player1->player->lives : 0;
    const int lives2 = player2 != nullptr ? player2->player->lives : 0;
    const bool hadPlayer2 = player2 != nullptr;
    currentLevel = levelQueue.takeFirst();
    loadLevel(currentLevel);
    player1->player->lives = lives1;
    if (hadPlayer2 && lives2 != 0) {
        player2->player->lives = lives2;
    } else {
        delete player2;
        player2 = nullptr;
    }
    resetClock();
}

bool SceneGraph::isGameOver() const
{
    const auto out = [](const PlayerGroup* g) {
        return g == nullptr || (g->player->dead && g->player->lives == 0);
    };
    return out(player1) && out(player2);
}

void SceneGraph::victory()
{
    // Bonus for the lives left, then the players "die" to end the game
    if (player1 != nullptr) {
        score1 += player1->player->lives * (1100 - 100 * difficulty);
        player1->player->dead = true;
        player1->player->lives = 0;
    }
    if (player2 != nullptr) {
        score2 += player2->player->lives * (1100 - 100 * difficulty);
        delete player2;
        player2 = nullptr;
    }
}

QList<HighScore> SceneGraph::loadHighScores()
{
    QList<HighScore> list;
    QFile file(paths::userDir() + QStringLiteral("/highscores.xml"));
    QDomDocument doc;
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return list;
    const QDomElement root = doc.documentElement();
    if (root.tagName() != QLatin1String("HighScores"))
        return list;
    for (QDomElement e = root.firstChildElement(QStringLiteral("Score")); !e.isNull();
         e = e.nextSiblingElement(QStringLiteral("Score")))
        list.append({e.attribute(QStringLiteral("player")).toInt(), e.attribute(QStringLiteral("points")).toInt(),
                     e.attribute(QStringLiteral("difficulty")).toInt()});
    return list;
}

void SceneGraph::finishGame()
{
    if (scoresSaved)
        return;
    scoresSaved = true;
    QList<HighScore> list = loadHighScores();
    if (score1 > 0)
        list.append({1, score1, difficulty});
    if (score2 > 0)
        list.append({2, score2, difficulty});
    std::stable_sort(list.begin(), list.end(),
                     [](const HighScore& a, const HighScore& b) { return a.points > b.points; });
    while (list.size() > 10)
        list.removeLast();

    QDomDocument doc;
    QDomElement root = doc.createElement(QStringLiteral("HighScores"));
    doc.appendChild(root);
    for (const HighScore& h : list) {
        QDomElement e = addChild(root, QStringLiteral("Score"));
        setAttr(e, "player", h.player);
        setAttr(e, "points", h.points);
        setAttr(e, "difficulty", h.difficulty);
    }
    QFile file(paths::userDir() + QStringLiteral("/highscores.xml"));
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        file.write(doc.toByteArray(2));
}

// ================================================================ power-ups

void SceneGraph::apply(Bonus* b, PlayerGroup* g)
{
    if (g == player1)
        score1 += b->points - 5 * difficulty;
    else
        score2 += b->points - 5 * difficulty;
    switch (b->kind) {
    case BONUS_LIFE:
        apply(static_cast<ExtraLife*>(b), g);
        break;
    case BONUS_WEAPON:
        apply(static_cast<WeaponBonus*>(b), g);
        break;
    case BONUS_TORNADO:
        apply(static_cast<TornadoBonus*>(b), g);
        break;
    case BONUS_BOMB:
        apply(static_cast<BombBonus*>(b), g);
        break;
    case BONUS_FRUIT:  // points only
        break;
    }
}

void SceneGraph::apply(ExtraLife*, PlayerGroup* g)
{
    g->player->lives++;
}

void SceneGraph::apply(WeaponBonus* w, PlayerGroup* g)
{
    if (g->weapon == w->weapon) {
        g->maxShots += w->extraShots;
    } else {
        g->weapon = w->weapon;
        g->maxShots = w->startingShots;
    }
}

void SceneGraph::apply(TornadoBonus*, PlayerGroup* g)
{
    if (twister != nullptr) {
        // A tornado is already there: it gets stronger and lasts longer
        twister->twister->angularSpeed += 180;
        twister->twister->duration += 10;
        twister->twister->power += 3;
        return;
    }
    twister = new TwisterGroup(new Twister(5 + 2 * detail, 15 + 5 * detail, 20 + 10 * detail,
                                           5 - 0.5f * difficulty, float(20 + 5 * difficulty),
                                           540 + 180 * difficulty, 3 + 0.6f * difficulty));
    const Point3f pos = g->getPos();
    twister->posx = pos[0];
    twister->posy = pos[1];
    twister->posz = pos[2];
}

void SceneGraph::apply(BombBonus*, PlayerGroup* g)
{
    // A blast hitting every ball with radius > 1 and the environment, once.
    // The points go to the player who picked up the bomb.
    auto* blast = new ProjectileGroup(g);
    blast->projectile = new BombBlast(1 + (0.2f * difficulty));
    shots.push_back(blast);
}
#endif

// ================================================================ editing

#ifdef PANG3D_EDITOR
void SceneGraph::pick(Point2i in)
{
    const Point3d p = get3dPos(in);
    for (BallGroup* b : balls)
        b->pick(p);
    for (EnvGroup* e : environment)
        e->pick(p);
}

void SceneGraph::moveUp(float h)
{
    for (BallGroup* b : balls) {
        if (!b->selected)
            continue;
        const float r = b->ball->sphere->radius;
        // Never below the top of the main floor
        if (b->posy + h - r < floorThickness)
            b->posy = floorThickness + r;
        else
            b->translation->move(0.0f, h, 0.0f);
        b->startY = b->posy;
    }
    for (EnvGroup* e : environment) {
        if (!e->selected || e->isMain)
            continue;
        if (e->env->getType() == LADDER) {
            // The wheel changes the number of rungs of ladders
            static_cast<Ladder*>(e->env)->addRungs(h > 0 ? 1 : -1);
            continue;
        }
        if (e->posy + h < floorThickness)
            e->posy = float(floorThickness);
        else
            e->translation->move(0.0f, h, 0.0f);
        e->startY = e->posy;
    }
    recomputeFixedShadows = true;
}

void SceneGraph::deleteSelectedBalls()
{
    for (auto i = balls.begin(); i != balls.end();) {
        if ((*i)->selected) {
            delete *i;
            i = balls.erase(i);
        } else {
            ++i;
        }
    }
}

void SceneGraph::deleteSelectedFloors()
{
    for (auto i = environment.begin(); i != environment.end();) {
        EnvGroup* e = *i;
        if (e->env->getType() == FLOOR && !e->isMain && static_cast<Floor*>(e->env)->selectedCells > 0) {
            delete e;
            i = environment.erase(i);
        } else {
            ++i;
        }
    }
    recomputeFixedShadows = true;
}

void SceneGraph::deleteSelectedWalls()
{
    for (auto i = environment.begin(); i != environment.end();) {
        EnvGroup* e = *i;
        if (e->env->getType() == WALL && !e->isMain && e->selected) {
            delete e;
            i = environment.erase(i);
        } else {
            ++i;
        }
    }
    recomputeFixedShadows = true;
}

void SceneGraph::deleteSelectedLadders()
{
    for (auto i = environment.begin(); i != environment.end();) {
        EnvGroup* e = *i;
        if (e->env->getType() == LADDER && e->env->selected) {
            delete e;
            i = environment.erase(i);
        } else {
            ++i;
        }
    }
    recomputeFixedShadows = true;
}

Point3d SceneGraph::get3dPos(Point2i in)
{
    double model[16];
    double proj[16];
    int viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    glGetIntegerv(GL_VIEWPORT, viewport);
    // Window coordinates have the origin at the bottom in OpenGL
    float z = 1.0f;
    const int winY = viewport[3] - in[1];
    glReadPixels(in[0], winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
    Point3d o(in[0], winY, z);
    gl::unProject(o[0], o[1], o[2], model, proj, viewport, &o[0], &o[1], &o[2]);
    return o;
}

void SceneGraph::newBalls(int radius, int children, int startHeight, int maxHeight, float gravity, float dirX,
                          float dirZ, float red, float green, float blue, bool textured, const QString& texture)
{
    // One new ball above each selected cell of each floor
    const GLuint tex = textured ? loadTexture(texture) : 0;
    const int slices = detail == DETAIL_LOW ? BALL_DETAIL_LOW : detail == DETAIL_MEDIUM ? BALL_DETAIL_MEDIUM : BALL_DETAIL_HIGH;
    for (EnvGroup* e : environment) {
        if (e->env->getType() != FLOOR)
            continue;
        auto* floor = static_cast<Floor*>(e->env);
        if (floor->selectedCells <= 0)
            continue;
        const Point3f floorPos = e->getPos();
        for (Cell* cell : floor->cells) {
            if (!cell->selected)
                continue;
            const Point3f cellPos = cell->getPos();
            const Point3f cellDim = cell->getDim();
            const float cx = ((cellPos[0] + cellDim[0]) / 2) + floorPos[0];
            const float cz = ((cellPos[2] + cellDim[2]) / 2) + floorPos[2];
            const float cy = (cellPos[1] + startHeight) + floorPos[1];

            auto* g = new BallGroup(detail);
            g->children = children;
            g->texture = texture;
            auto* ball = new Ball(float(radius), slices, slices);
            ball->texture = tex;
            ball->textured = textured;
            ball->sphere->setColor(red, green, blue);
            g->setBall(ball);
            g->alpha = 1.0f;
            g->translation->move(cx, cy, cz);
            delete g->movement;
            g->movement = new Movement(dirX, dirZ);
            g->movement->bindCoord(&g->posx, &g->posy, &g->posz);
            delete g->bounce;
            g->bounce = new Bounce(float(maxHeight), -gravity);
            g->bounce->bindCoord(&g->posx, &g->posy, &g->posz);
            // Always bounce high enough to jump over the player
            if (g->bounce->maxHeight < 5.0f + radius)
                g->bounce->maxHeight = 5.0f + radius;
            g->storeStartPosition();
            balls.push_back(g);
        }
    }
}

void SceneGraph::newFloor(bool destructible, bool isMain, int cellsX, int cellsZ, int h, int thickness, float red,
                          float green, float blue, float alpha, bool textured, const QString& texture)
{
    const GLuint tex = textured ? loadTexture(texture) : 0;
    if (isMain) {
        // The main floor: players start in the middle, three cells apart,
        // and four invisible walls close the level
        if (player1 != nullptr) {
            player1->translation->move(cellSize * (cellsX - 3) / 2, h + thickness + 0.5f, cellSize * cellsZ / 2);
            player1->storeStartPosition();
        }
        if (player2 != nullptr) {
            player2->translation->move(cellSize * (cellsX + 3) / 2, h + thickness + 0.5f, cellSize * cellsZ / 2);
            player2->storeStartPosition();
            player2->player->setMarker(new MarkerTwo());
        }
        for (int i = 0; i < 4; i++) {
            auto* g = new EnvGroup(DETAIL_MEDIUM);
            g->isMain = true;
            g->destructible = false;
            // Two walls as wide as the floor, two as deep. (In 2005 pairs of
            // groups shared one wall object.)
            if (i % 2 == 0)
                g->env = new Wall(numCellsX * cellSize, 1000, 1);
            else
                g->env = new Wall(1, 1000, numCellsZ * cellSize);
            if (i == 2)
                g->translation->move(0, 0, numCellsZ * cellSize);
            if (i == 3)
                g->translation->move(numCellsX * cellSize, 0, 0);
            g->storeStartPosition();
            environment.push_back(g);
        }
    }
    auto* g = new EnvGroup(DETAIL_MEDIUM);
    g->texture = texture;
    g->isMain = isMain;
    g->destructible = destructible;
    auto* floor = new Floor(cellsX, cellsZ, cellSize, float(thickness));
    floor->slab->setColor(red, green, blue);
    floor->alpha = alpha;
    floor->textured = textured;
    g->env = floor;
    g->setTextureName(tex);
    g->translation->move(0, float(h), 0);
    g->storeStartPosition();
    environment.push_back(g);
    recomputeFixedShadows = true;
}

void SceneGraph::newWall(bool destructible, bool isMain, int length, int width, int h, int elevation, float red,
                         float green, float blue, float alpha, bool textured, const QString& texture)
{
    const GLuint tex = textured ? loadTexture(texture) : 0;
    auto* g = new EnvGroup(DETAIL_MEDIUM);
    g->texture = texture;
    g->isMain = isMain;
    g->destructible = destructible;
    auto* wall = new Wall(width * cellSize, float(h), length * cellSize);
    wall->box->setColor(red, green, blue);
    wall->alpha = alpha;
    wall->textured = textured;
    g->env = wall;
    g->setTextureName(tex);
    g->translation->move(0, float(elevation), 0);
    g->storeStartPosition();
    environment.push_back(g);
    recomputeFixedShadows = true;
}

void SceneGraph::rewind()
{
    for (BallGroup* b : balls) {
        b->posx = b->startX;
        b->posy = b->startY;
        b->posz = b->startZ;
        b->bounce->vy = 0;
    }
}

void SceneGraph::setBallColor(float r, float g, float b)
{
    for (BallGroup* ball : balls) {
        if (ball->selected) {
            ball->ball->sphere->setColor(r, g, b);
            ball->ball->sphere->alpha = 1.0f;
            ball->alpha = 1.0f;
        }
    }
}

void SceneGraph::setBallTexture(bool textured, const QString& file)
{
    for (BallGroup* b : balls) {
        if (!b->selected)
            continue;
        if (textured) {
            b->ball->texture = loadTexture(file);
            b->texture = file;
        } else {
            b->ball->texture = 0;
            b->texture.clear();
        }
        b->ball->textured = textured;
    }
}

void SceneGraph::setFloorColor(float r, float g, float b)
{
    for (EnvGroup* e : environment) {
        if (e->env->getType() == FLOOR && static_cast<Floor*>(e->env)->selectedCells > 0) {
            auto* f = static_cast<Floor*>(e->env);
            f->slab->setColor(r, g, b);
            f->slab->alpha = 1.0f;
            f->alpha = 1.0f;
        }
    }
}

void SceneGraph::setFloorTexture(bool textured, const QString& file)
{
    for (EnvGroup* e : environment) {
        if (e->env->getType() != FLOOR || static_cast<Floor*>(e->env)->selectedCells <= 0)
            continue;
        e->env->textured = textured;
        e->setTextureName(textured ? loadTexture(file) : 0);
        e->texture = textured ? file : QString();
    }
}

void SceneGraph::setWallColor(float r, float g, float b)
{
    for (EnvGroup* e : environment) {
        if (e->env->getType() == WALL && e->env->selected) {
            auto* w = static_cast<Wall*>(e->env);
            w->box->setColor(r, g, b);
            w->box->alpha = 1.0f;
            w->alpha = 1.0f;
        }
    }
}

void SceneGraph::setWallTexture(bool textured, const QString& file)
{
    for (EnvGroup* e : environment) {
        if (e->env->getType() != WALL || !e->env->selected)
            continue;
        e->env->textured = textured;
        e->setTextureName(textured ? loadTexture(file) : 0);
        e->texture = textured ? file : QString();
    }
}

void SceneGraph::translateSelected(Point2i mouseDelta)
{
    // Objects move by a tenth of the mouse movement and stay on the main floor
    const Point3f mainPos(0, 0, 0);
    const Point3f mainDim(cellSize * numCellsX, float(floorThickness), cellSize * numCellsZ);
    const float dx = float(mouseDelta[0] / 10);
    const float dz = float(mouseDelta[1] / 10);
    for (BallGroup* b : balls) {
        if (!b->selected)
            continue;
        const Point3f dim = b->getDim();
        float x = b->posx + dx;
        float z = b->posz + dz;
        if (x - dim[0] < mainPos[0])
            x = mainPos[0] + dim[0];
        else if (x + dim[0] > mainDim[0])
            x = mainDim[0] - dim[0];
        if (z - dim[0] < mainPos[2])
            z = mainPos[2] + dim[0];
        else if (z + dim[2] > mainDim[2])
            z = mainDim[2] - dim[2];
        b->translation->move(x - b->posx, 0.0f, z - b->posz);
        b->startX = x;
        b->startZ = z;
    }
    for (EnvGroup* e : environment) {
        if (e->isMain || !e->selected)
            continue;
        const Point3f pos = e->getPos();
        const Point3f dim = e->getDim();
        float x = e->posx + dx;
        float z = e->posz + dz;
        // posx/posz is the corner of the object, getDim() its far corner
        if (x < mainPos[0])
            x = mainPos[0];
        else if (dim[0] + dx > mainDim[0])
            x = mainDim[0] - (dim[0] - pos[0]);
        if (z < mainPos[2])
            z = mainPos[2];
        else if (dim[2] + dz > mainDim[2])
            z = mainDim[2] - (dim[2] - pos[2]);
        e->translation->move(x - e->posx, 0.0f, z - e->posz);
        e->startX = x;
        e->startZ = z;
        recomputeFixedShadows = true;
    }
}
#endif

// ================================================================ files

bool SceneGraph::loadLevel(const QString& fileName)
{
    QFile file(fileName);
    QDomDocument doc;
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file)) {
        qWarning("3dPang: cannot read level '%s'", qPrintable(fileName));
        return false;
    }
    const QDomElement root = doc.documentElement();
    if (root.tagName() != QLatin1String("Level")) {
        qWarning("3dPang: '%s' is not a 3dPang v2 level (convert 2005 levels with "
                 "tools/convert_legacy_level.py)",
                 qPrintable(fileName));
        return false;
    }
    clearLevel();
    powerUpChance = root.attribute(QStringLiteral("powerUpChance")).toInt();
    bonusChance = root.attribute(QStringLiteral("bonusChance")).toInt();
    weaponWeight = root.attribute(QStringLiteral("weaponWeight")).toInt();
    lifeWeight = root.attribute(QStringLiteral("lifeWeight")).toInt();
    fruitWeight = root.attribute(QStringLiteral("fruitWeight")).toInt();
    bombWeight = root.attribute(QStringLiteral("bombWeight")).toInt();
    tornadoWeight = root.attribute(QStringLiteral("tornadoWeight")).toInt();
    cubeMap->setTexture(root.attribute(QStringLiteral("skybox")));

    for (QDomElement e = root.firstChildElement(); !e.isNull(); e = e.nextSiblingElement()) {
        const QString tag = e.tagName();
        const QString textureFile = e.attribute(QStringLiteral("texture"));
        if (tag == QLatin1String("Views")) {
            for (QDomElement v = e.firstChildElement(QStringLiteral("View")); !v.isNull();
                 v = v.nextSiblingElement(QStringLiteral("View"))) {
                const int index = v.attribute(QStringLiteral("index")).toInt();
                if (index >= 0 && index < 6)
                    views[index].eye = Point3i(v.attribute(QStringLiteral("x")).toInt(),
                                               v.attribute(QStringLiteral("y")).toInt(),
                                               v.attribute(QStringLiteral("z")).toInt());
            }
        } else if (tag == QLatin1String("Ball")) {
            auto* g = new BallGroup(detail);
            g->load(e, loadTexture(textureFile), difficulty);
            g->texture = textureFile;
            balls.push_back(g);
        } else if (tag == QLatin1String("Player")) {
            auto* g = new PlayerGroup(player1 == nullptr);
            g->load(e, 0, difficulty);
            if (player1 == nullptr)
                player1 = g;
            else if (player2 == nullptr)
                player2 = g;
            else
                delete g;
        } else if (tag == QLatin1String("Environment")) {
            auto* g = new EnvGroup(detail);
            g->load(e, loadTexture(textureFile), difficulty);
            g->texture = textureFile;
            if (g->isMain && g->env->getType() == FLOOR) {
                const auto* f = static_cast<Floor*>(g->env);
                numCellsX = f->cellsX;
                numCellsZ = f->cellsZ;
                floorThickness = int(f->thickness);
                defaultView = Point3i(int(2.5f * numCellsX), 35, int(40 + 5.0f * numCellsZ));
            }
            environment.push_back(g);
        }
    }
    // Every level has two players (the player program removes the second
    // one in single player games)
    if (player1 == nullptr)
        player1 = new PlayerGroup(true);
    if (player2 == nullptr)
        player2 = new PlayerGroup(false);
    regenerateDisplayList();
    resetClock();
    return true;
}

#ifdef PANG3D_EDITOR
bool SceneGraph::saveLevel(const QString& fileName) const
{
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction(QStringLiteral("xml"),
                                                    QStringLiteral("version=\"1.0\" encoding=\"UTF-8\"")));
    QDomElement root = doc.createElement(QStringLiteral("Level"));
    doc.appendChild(root);
    setAttr(root, "version", 2);
    setAttr(root, "powerUpChance", powerUpChance);
    setAttr(root, "bonusChance", bonusChance);
    setAttr(root, "weaponWeight", weaponWeight);
    setAttr(root, "lifeWeight", lifeWeight);
    setAttr(root, "fruitWeight", fruitWeight);
    setAttr(root, "bombWeight", bombWeight);
    setAttr(root, "tornadoWeight", tornadoWeight);
    setAttr(root, "skybox", cubeMap->textureBaseName);

    QDomElement viewsElement = addChild(root, QStringLiteral("Views"));
    for (int j = 0; j < 6; j++) {
        QDomElement v = addChild(viewsElement, QStringLiteral("View"));
        setAttr(v, "index", j);
        setAttr(v, "x", views[j].eye[0]);
        setAttr(v, "y", views[j].eye[1]);
        setAttr(v, "z", views[j].eye[2]);
    }
    for (BallGroup* b : balls)
        b->save(root);
    for (EnvGroup* e : environment)
        e->save(root);
    if (player1 != nullptr)
        player1->save(root);
    if (player2 != nullptr)
        player2->save(root);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;
    file.write(doc.toByteArray(2));
    return true;
}
#endif

} // namespace pang
