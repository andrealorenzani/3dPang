#include "Bonus.h"

#include "GlUtil.h"

#include <map>
#include <memory>

namespace pang {

Model3ds* Bonus::sharedModel(const QString& file)
{
    static std::map<QString, std::unique_ptr<Model3ds>> cache;
    auto& slot = cache[file];
    if (!slot) {
        slot = std::make_unique<Model3ds>();
        slot->open(file);
    }
    return slot.get();
}

void Bonus::update(float dt)
{
    // Half a turn per second...
    angle += 180 * dt;
    // ...while floating between 0 and 1
    const float h = height + (oscillation * dt);
    if (h > 1) {
        height = 2 - h;
        oscillation = -oscillation;
    } else if (h < 0) {
        height = -h;
        oscillation = -oscillation;
    } else {
        height = h;
    }
}

void ExtraLife::glDraw()
{
    glPushMatrix();
    glTranslatef(0, height, 0);
    glRotatef(angle, 0, 1, 0);
    glRotatef(-90, 1, 0, 0);  // the model lies face down
    model->draw();
    glPopMatrix();
}

void Fruit::glDraw()
{
    glPushMatrix();
    glTranslatef(0, height, 0);
    glRotatef(angle, 0, 1, 0);
    glScalef(scale, scale, scale);
    glRotatef(float(rotation), 1, 0, 0);
    model->draw();
    glPopMatrix();
}

void WeaponBonus::glDraw()
{
    glPushMatrix();
    glTranslatef(0, height, 0);
    glRotatef(angle, 0, 1, 0);
    drawWeapon();
    glPopMatrix();
}

void QuadHarpoonBonus::drawWeapon()
{
    // Four hooks on a square, slightly raised
    const float d = hook.getDim()[0];
    glPushMatrix();
    glTranslatef(0.0f, 0.3f, 1.5f * d);
    hook.glDraw();
    glTranslatef(0.0f, 0.0f, -3.0f * d);
    hook.glDraw();
    glTranslatef(-1.5f * d, 0.0f, 1.5f * d);
    hook.glDraw();
    glTranslatef(3.0f * d, 0.0f, 0.0f);
    hook.glDraw();
    glPopMatrix();
}

GunBonus::GunBonus() : grip(0.6f, 0.8f, 0.4f), barrel(1.3f, 0.4f, 0.4f), barrel2(1.9f, 0.4f, 0.4f)
{
    grip.setColor(1.0f, 0.9f, 0.0f);
    barrel.setColor(0.0f, 0.0f, 0.0f);
    barrel2.setColor(0.0f, 0.0f, 0.0f);
    points = 150;
    weapon = GUN;
    // Switching to the gun gives 5 shots, picking it up again 3 more
    startingShots = 5;
    extraShots = 3;
}

void GunBonus::drawWeapon()
{
    glPushMatrix();
    glRotatef(45, 0, 0, 1);  // pointing up
    glTranslatef(-0.8f, -0.8f, -0.2f);
    grip.glDraw();
    glTranslatef(-0.15f, 0.8f, 0.0f);
    barrel.glDraw();
    // Second barrel on top, tilted around its own X axis
    glTranslatef(0.0f, 0.6f, 0.2f);
    glRotatef(45, 1, 0, 0);
    glTranslatef(0.0f, -0.2f, -0.2f);
    barrel2.glDraw();
    glPopMatrix();
}

void TornadoBonus::glDraw()
{
    // The mini tornado grows immediately
    miniTwister.generate(0.02f);
    glPushMatrix();
    glTranslatef(0, height, 0);
    glRotatef(angle, 0, 1, 0);
    // Tilt it around its centre
    glTranslatef(0.0f, 1.5f, 0.0f);
    glRotatef(45, 0, 0, 1);
    glTranslatef(0.0f, -1.5f, 0.0f);
    glScalef(0.4f, 1, 0.4f);
    glColor3f(1.0f, 1.0f, 1.0f);
    miniTwister.glDraw();
    glPopMatrix();
}

BombBonus::BombBonus(float duration) : flashDuration(duration), fuse(0.15f, 0.15f, 0.6f)
{
    fuse.setColor(1.0f, 1.0f, 1.0f);
    points = 170;
    kind = BONUS_BOMB;
}

void BombBonus::glDraw()
{
    glPushMatrix();
    glTranslatef(0, height, 0);
    glRotatef(angle, 0, 1, 0);
    glRotatef(45, 0, 0, 1);
    glColor3f(0.0f, 0.0f, 0.0f);
    if (hasTexture) {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Texture coordinates from the object position
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
    }
    gl::drawSphere(1.0, 6, 6);
    if (hasTexture) {
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
    }
    // The black fuse holder...
    glRotatef(90, 0, 1, 0);
    glTranslatef(0, 0, 0.9f);
    gl::drawCylinder(0.2, 0.2, 0.7, 8, 1);
    // ...and the white fuse
    glTranslatef(0.0f, 0.0f, 0.6f);
    glRotatef(45, 0, 0, 1);
    fuse.glDraw();
    glPopMatrix();
}

} // namespace pang
