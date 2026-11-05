#include "Projectiles.h"

namespace pang {

void HarpoonShot::glDraw()
{
    if (harpoon.internalTime < timeToLive)
        harpoon.glDraw();
    else
        active = false;
}

void QuadHarpoonShot::glDraw()
{
    if (harpoon.internalTime >= timeToLive) {
        active = false;
        return;
    }
    // Four harpoons around the player
    const Point3f dim = harpoon.getDim();
    glPushMatrix();
    glTranslatef(dim[0] * 3 / 2, 0, 0);
    harpoon.glDraw();
    glTranslatef(-(dim[0] * 3), 0, 0);
    harpoon.glDraw();
    glTranslatef(dim[0] * 3 / 2, 0, -dim[0] * 3 / 2);
    harpoon.glDraw();
    glTranslatef(0, 0, dim[0] * 3);
    harpoon.glDraw();
    glPopMatrix();
}

BombBlast::BombBlast(float flash) : flashDuration(flash)
{
    weapon = BOMB_BLAST;
    // A red flash, then the scene stays red for a while: the colour mask
    // drops the red channel and the clear colour becomes red.
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glColorMask(0, 1, 1, 1);
}

void BombBlast::update(float dt)
{
    internalTime += dt;
    // After the flash the blinding plane fades out in three seconds
    if (internalTime > flashDuration)
        alpha -= dt / 3.0f;
    expired = internalTime > flashDuration + 3.5f;
    if (expired) {
        glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);
        glColorMask(1, 1, 1, 1);
    }
}

void BombBlast::glDraw()
{
    // The flash is a huge red plane drawn over everything
    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.0f, 0.0f, alpha);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(-99999, -99999, 99999);
    glVertex3f(-99999, 99999, 99999);
    glVertex3f(99999, 99999, -99999);
    glVertex3f(99999, -99999, -99999);
    glEnd();
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glPopMatrix();
}

void GunShot::glDraw()
{
    // Two bullets side by side, rising
    glPushMatrix();
    glTranslatef(0, heightPerSec * internalTime, 0);
    bullet.glDraw();
    glTranslatef(0.3f, 0, 0);
    bullet.glDraw();
    glPopMatrix();
}

} // namespace pang
