#include "ViewPreview.h"

#include "GlUtil.h"

namespace pang {

ViewPreview::ViewPreview(QWidget* parent) : QOpenGLWidget(parent), eye(0, 20, 50), scene(new SceneGraph())
{
    scene->shadows = false;
    scene->detail = DETAIL_MEDIUM;  // no sky box

    auto* ground = new EnvGroup(DETAIL_MEDIUM);
    auto* floor = new Floor(10, 10, 5, 1);
    floor->slab->setColor(0.5f, 0.5f, 0.5f);
    ground->env = floor;
    ground->translation->move(-25.0f, -0.5f, -25.0f);

    // A box as big as a player, centred on the origin
    auto* player = new EnvGroup(DETAIL_MEDIUM);
    player->env = new Wall(5, 10, 5);
    player->translation->move(-2.5f, 0.0f, 2.5f);

    scene->environment.push_back(ground);
    scene->environment.push_back(player);
    setMinimumSize(160, 120);
}

ViewPreview::~ViewPreview()
{
    makeCurrent();
    delete scene;
    doneCurrent();
}

void ViewPreview::initializeGL()
{
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    const float light[4] = {0, 30, 30, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, light);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
}

void ViewPreview::paintGL()
{
    const qreal r = devicePixelRatioF();
    glViewport(0, 0, int(width() * r), int(height() * r));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gl::lookAt(eye[0], eye[1], eye[2], 0, 0, 0, 0, 1, 0);
    scene->glDraw();
}

void ViewPreview::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gl::perspective(45, float(w) / float(std::max(1, h)), 10, 200);
    glMatrixMode(GL_MODELVIEW);
}

void ViewPreview::setViewX(int v)
{
    eye[0] = v;
    update();
}

void ViewPreview::setViewY(int v)
{
    eye[1] = v;
    update();
}

void ViewPreview::setViewZ(int v)
{
    eye[2] = v;
    update();
}

} // namespace pang
