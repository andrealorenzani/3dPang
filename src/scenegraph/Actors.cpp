#include "Actors.h"

#include <cmath>
#include <cstdlib>

namespace pang {

// ---------------------------------------------------------------- Ball

Ball::~Ball()
{
    delete sphere;
    if (useDisplayList && displayList != 0)
        glDeleteLists(displayList, 1);
}

void Ball::glDraw()
{
    if (!textured) {
        glDisable(GL_TEXTURE_2D);
    } else {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // The texture is wrapped around the ball as a sphere map
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
    }
    if (useDisplayList)
        glCallList(displayList);
    else
        sphere->glDraw();
    if (!textured) {
        glEnable(GL_TEXTURE_2D);
    } else {
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
    }
}

void Ball::genList()
{
    if (displayList == 0)
        displayList = glGenLists(1);
    useDisplayList = true;
    glNewList(displayList, GL_COMPILE);
    sphere->glDraw();
    glEndList();
}

// ---------------------------------------------------------------- Player

Player::Player(int numLives, const QString& model3d, const QString& skin
#ifdef PANG3D_EDITOR
               , bool first
#endif
               )
    : lives(numLives), modelFile(model3d), skinFile(skin), model(new CIMd2())
{
    model->Load(modelFile.isEmpty() ? DEFAULT_PLAYER_MODEL : modelFile,
                skinFile.isEmpty() ? DEFAULT_PLAYER_SKIN : skinFile);
    model->SetFPS(5.0f);
    setStand();
#ifdef PANG3D_EDITOR
    marker = first ? static_cast<PlayerMarker*>(new MarkerOne()) : new MarkerTwo();
#endif
}

Player::~Player()
{
    delete model;
#ifdef PANG3D_EDITOR
    delete marker;
#endif
}

void Player::glDraw()
{
#ifdef PANG3D_EDITOR
    if (!showModel) {
        glPushMatrix();
        marker->glDraw();
        glPopMatrix();
        return;
    }
#endif
    glPushMatrix();
    // Orient the Quake model (Z up, X forward) in the game (Y up)
    glRotatef(-15, 1, 0, 0);
    glRotatef(-90, 0, 1, 0);
    glRotatef(-90, 1, 0, 0);
    glScalef(0.1f, 0.1f, 0.1f);
    if (!dead)
        model->DrawTimedFrame(animTime);
    else
        model->DrawFrame(183);  // last frame of the death animation
    glPopMatrix();
}

void Player::setModel(const QString& newSkin, const QString& newModel)
{
    if (!newModel.isEmpty())
        modelFile = newModel;
    skinFile = newSkin;
    model->Load(modelFile.isEmpty() ? DEFAULT_PLAYER_MODEL : modelFile,
                skinFile.isEmpty() ? DEFAULT_PLAYER_SKIN : skinFile);
    setStand();
    model->SetFPS(5.0f);
}

// ---------------------------------------------------------------- Twister

Twister::~Twister()
{
    if (generated && displayList != 0)
        glDeleteLists(displayList, 1);
}

void Twister::generate(float dt)
{
    // The tornado is built procedurally with the OpenGL matrix stack: each
    // vertical step is a ring of `horizontalSteps` positions, wider and
    // higher than the previous one, scattered with `intensity` random points.
    glPushMatrix();
    Matrix44d mm;
    internalTime += dt;
    const float currentTurns = internalTime / (creationTime / verticalSteps);
    if (generatedTurns < verticalSteps) {
        const float stepAngle = 360.0f / horizontalSteps;
        const float widthPerStep = width / verticalSteps;
        const float heightPerStep = height / verticalSteps;
        for (; generatedTurns < (currentTurns + 1) && generatedTurns < verticalSteps; generatedTurns++) {
            for (int j = 0; j < (horizontalSteps + 1); j++) {
                glLoadIdentity();
                glRotatef(stepAngle * j, 0, 1, 0);
                glTranslatef(widthPerStep * generatedTurns, heightPerStep * generatedTurns, 0);
                glGetModelViewMatrix(mm);
                for (int i = 0; i < intensity; i++) {
                    mesh.push_back(mm * Point3d(-1.0, 0.0, 0.0));
                    // This "random" scattering was found by trial and error in
                    // 2005: it is neither a straight line nor an explosion.
                    mesh.push_back(mm * Point3d(std::sqrt(double(std::abs(std::rand()))) / 60,
                                                std::sqrt(double(std::abs(std::rand()))) / 80,
                                                std::sqrt(double(std::abs(std::rand()))) / 95));
                }
            }
        }
    }
    glPopMatrix();
}

void Twister::glDraw()
{
    // No culling: the tornado must look round
    glDisable(GL_CULL_FACE);
    glRotatef(angle, 0, 1, 0);
    if (useDisplayList && !generated)
        generateDL();
    if (generated) {
        glCallList(displayList);
    } else {
        glBegin(GL_POINTS);
        for (size_t i = 0; i + 1 < mesh.size(); i += 2) {
            glNormal(mesh[i]);
            glVertex(mesh[i + 1]);
        }
        glEnd();
    }
    glEnable(GL_CULL_FACE);
}

void Twister::generateDL()
{
    // Only once the tornado has finished growing
    if (internalTime < creationTime)
        return;
    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);
    glBegin(GL_POINTS);
    for (size_t i = 0; i + 1 < mesh.size(); i += 2) {
        glNormal(mesh[i]);
        glVertex(mesh[i + 1]);
    }
    glEnd();
    glEndList();
    generated = true;
}

} // namespace pang
