#include "EditorView.h"

#include "GlUtil.h"

#include <QMouseEvent>
#include <QWheelEvent>

namespace pang {

EditorView::EditorView(QWidget* parent)
    : QOpenGLWidget(parent), scene(new SceneGraph()), center(35, 2, 35), eye(35.0f, 60.0f, 140.0f),
      ballColor(1, 1, 1), floorColor(1, 1, 1), wallColor(1, 1, 1)
{
    // Edit at full detail
    scene->detail = DETAIL_HIGH;
    rotationMatrix.SetIdentity();
    trackball.Init(width(), height());
    setFocusPolicy(Qt::StrongFocus);
    // The scene is animated: redraw continuously
    connect(&timer, &QTimer::timeout, this, qOverload<>(&QWidget::update));
    timer.start(16);
}

EditorView::~EditorView()
{
    // OpenGL objects must be freed with the context current
    makeCurrent();
    delete scene;
    doneCurrent();
}

void EditorView::resetCamera()
{
    center = Point3i(35, 2, 35);
    eye = Point3f(35.0f, 60.0f, 140.0f);
    rotationMatrix.SetIdentity();
    zoom = 1.0f;
}

Point2i EditorView::toPixels(const QPoint& p) const
{
    // OpenGL works in device pixels, Qt events in logical pixels
    const qreal r = devicePixelRatioF();
    return Point2i(int(p.x() * r), int(p.y() * r));
}

void EditorView::initializeGL()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    const float light[4] = {0, 30, 30, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, light);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glClearStencil(0);
    // The editor always shows the sky box
    scene->cubeMap->setTexture(skybox);
    glReady = true;
}

void EditorView::setSkybox(const QString& dataRelativeBaseName)
{
    skybox = dataRelativeBaseName;
    if (glReady) {
        makeCurrent();
        scene->cubeMap->setTexture(skybox);
        doneCurrent();
    }
}

void EditorView::paintGL()
{
    const float dt = scene->tick();
    if (animate)
        scene->update(std::min(dt, 0.1f));
    scene->collide();

    const qreal r = devicePixelRatioF();
    glViewport(0, 0, int(width() * r), int(height() * r));
    glClearColor(0.0f, 0.6f, 1.0f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gl::lookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);
    // Trackball rotation and zoom around the centre of the view
    glTranslatef(float(center[0]), float(center[1]), float(center[2]));
    glMultMatrix(rotationMatrix);
    glScalef(zoom, zoom, zoom);
    glTranslatef(-float(center[0]), -float(center[1]), -float(center[2]));
    scene->glDraw();

    // Mouse actions that need the depth buffer of this frame
    if (placing != nullptr && placeAt) {
        const Point3d p = scene->get3dPos(*placeAt);
        placing->posx = float(p[0]);
        placing->posy = float(p[1]) + 0.05f;
        placing->posz = float(p[2]);
        placing->storeStartPosition();
        placeAt.reset();
        if (finishPlacing) {
            placing = nullptr;
            finishPlacing = false;
            scene->recomputeFixedShadows = true;
        }
    }
    if (pickAt) {
        scene->pick(*pickAt);
        pickAt.reset();
    }
    if (centerAt) {
        const Point3d p = scene->get3dPos(*centerAt);
        center = Point3i(int(p[0]), int(p[1]), int(p[2]));
        centerAt.reset();
    }
    if (dragDelta != Point2i(0, 0)) {
        scene->translateSelected(dragDelta);
        dragDelta = Point2i(0, 0);
    }
}

void EditorView::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gl::perspective(45, float(w) / float(std::max(1, h)), 10, 200);
    glMatrixMode(GL_MODELVIEW);
    scene->regenerateDisplayList();
    trackball.Resize(w, h);
}

void EditorView::mousePressEvent(QMouseEvent* e)
{
    const QPoint p = e->position().toPoint();
    if (e->button() == Qt::RightButton) {
        // Right click turns the selected ladders by 90 degrees
        for (EnvGroup* a : scene->environment) {
            if (a->env->getType() == LADDER && a->env->selected) {
                a->rotation->angle = (a->rotation->angle == 90) ? 0 : 90;
                static_cast<Ladder*>(a->env)->rotate90();
                scene->recomputeFixedShadows = true;
            }
        }
    } else if (placing != nullptr) {
        // Click: the object stays here
        placeAt = toPixels(p);
        finishPlacing = true;
        setMouseTracking(false);
    } else if (trackballMode) {
        trackball.MouseDown(p.x(), p.y(), 0);
    } else {
        pickAt = toPixels(p);
        dragLast = Point2i(p.x(), p.y());
        dragging = true;
    }
    update();
}

void EditorView::mouseMoveEvent(QMouseEvent* e)
{
    const QPoint p = e->position().toPoint();
    if (placing != nullptr) {
        placeAt = toPixels(p);
    } else if (trackballMode) {
        trackball.CalcRotMatrix(rotationMatrix, p.x(), p.y());
    } else if (dragging) {
        const Point2i now(p.x(), p.y());
        dragDelta = dragDelta + (now - dragLast);
        dragLast = now;
    }
    update();
}

void EditorView::mouseReleaseEvent(QMouseEvent* e)
{
    const QPoint p = e->position().toPoint();
    if (trackballMode)
        trackball.MouseUp(p.x(), p.y());
    dragging = false;
    update();
}

void EditorView::wheelEvent(QWheelEvent* e)
{
    e->accept();
    const int delta = e->angleDelta().y();
    if (trackballMode) {
        // The wheel turns the trackball vertically
        const QPoint p = e->position().toPoint();
        const int d = delta / 60;
        trackball.MouseDown(p.x(), p.y(), 0);
        trackball.CalcRotMatrix(rotationMatrix, p.x(), p.y() + d);
        trackball.MouseUp(p.x(), p.y() + d);
    } else {
        // ...or raises/lowers the selection (adds/removes ladder rungs)
        scene->moveUp(float(delta) / 360.0f);
    }
    update();
}

void EditorView::mouseDoubleClickEvent(QMouseEvent* e)
{
    // Double click: centre the view on the point
    if (e->button() == Qt::LeftButton)
        centerAt = toPixels(e->position().toPoint());
}

void EditorView::startPlacingPlayer()
{
    PlayerGroup*& g = playerOne ? scene->player1 : scene->player2;
    if (g == nullptr)
        g = new PlayerGroup(playerOne);
    g->player->setMarker(playerOne ? static_cast<PlayerMarker*>(new MarkerOne()) : new MarkerTwo());
    placing = g;
    setMouseTracking(true);
}

void EditorView::setPlayerModelVisible(int showModel)
{
    for (PlayerGroup* g : {scene->player1, scene->player2})
        if (g != nullptr)
            g->player->showModel = showModel != 0;
}

void EditorView::applyColor(Point3f& color, int channel, int value, int target)
{
    color[channel] = float(value) / 255.0f;
    if (target == 0)
        scene->setBallColor(color[0], color[1], color[2]);
    else if (target == 1)
        scene->setFloorColor(color[0], color[1], color[2]);
    else
        scene->setWallColor(color[0], color[1], color[2]);
    update();
}

void EditorView::setBallRed(int v) { applyColor(ballColor, 0, v, 0); }
void EditorView::setBallGreen(int v) { applyColor(ballColor, 1, v, 0); }
void EditorView::setBallBlue(int v) { applyColor(ballColor, 2, v, 0); }
void EditorView::setFloorRed(int v) { applyColor(floorColor, 0, v, 1); }
void EditorView::setFloorGreen(int v) { applyColor(floorColor, 1, v, 1); }
void EditorView::setFloorBlue(int v) { applyColor(floorColor, 2, v, 1); }
void EditorView::setWallRed(int v) { applyColor(wallColor, 0, v, 2); }
void EditorView::setWallGreen(int v) { applyColor(wallColor, 1, v, 2); }
void EditorView::setWallBlue(int v) { applyColor(wallColor, 2, v, 2); }

void EditorView::setBallTexture(bool textured, const QString& file)
{
    makeCurrent();
    scene->setBallTexture(textured, file);
    doneCurrent();
}

void EditorView::setFloorTexture(bool textured, const QString& file)
{
    makeCurrent();
    scene->setFloorTexture(textured, file);
    doneCurrent();
}

void EditorView::setWallTexture(bool textured, const QString& file)
{
    makeCurrent();
    scene->setWallTexture(textured, file);
    doneCurrent();
}

void EditorView::deleteBalls()
{
    makeCurrent();
    scene->deleteSelectedBalls();
    doneCurrent();
}

void EditorView::deleteFloors()
{
    makeCurrent();
    scene->deleteSelectedFloors();
    doneCurrent();
}

void EditorView::deleteWalls()
{
    makeCurrent();
    scene->deleteSelectedWalls();
    doneCurrent();
}

void EditorView::deleteLadders()
{
    makeCurrent();
    scene->deleteSelectedLadders();
    doneCurrent();
}

void EditorView::newBalls(int radius, int children, int startHeight, int maxHeight, float gravity, float dirX,
                          float dirZ, float red, float green, float blue, bool textured, const QString& texture)
{
    makeCurrent();
    scene->newBalls(radius, children, startHeight, maxHeight, gravity, dirX, dirZ, red, green, blue, textured, texture);
    doneCurrent();
}

void EditorView::newFloor(bool destructible, bool isMain, int cellsX, int cellsZ, int h, int thickness, float red,
                          float green, float blue, bool textured, const QString& texture)
{
    if (isMain) {
        scene->numCellsX = cellsX;
        scene->numCellsZ = cellsZ;
        scene->floorThickness = thickness;
    }
    // Destructible floors are half transparent
    const float alpha = destructible ? 0.5f : 1.0f;
    makeCurrent();
    scene->newFloor(destructible, isMain, cellsX, cellsZ, h, thickness, red, green, blue, alpha, textured, texture);
    doneCurrent();
}

void EditorView::newWall(bool destructible, bool isMain, int length, int width, int h, int elevation, float red,
                         float green, float blue, bool textured, const QString& texture)
{
    const float alpha = destructible ? 0.5f : 1.0f;
    makeCurrent();
    scene->newWall(destructible, isMain, length, width, h, elevation, red, green, blue, alpha, textured, texture);
    doneCurrent();
}

void EditorView::newLevel(int cellsX, int cellsZ, int thickness)
{
    makeCurrent();
    scene->clearLevel();
    scene->powerUpChance = scene->bonusChance = 0;
    scene->weaponWeight = scene->lifeWeight = scene->fruitWeight = scene->bombWeight = scene->tornadoWeight = 0;
    for (View& v : scene->views)
        v = View();
    scene->player1 = new PlayerGroup(true);
    scene->player2 = new PlayerGroup(false);
    doneCurrent();
    fileName.clear();
    newFloor(false, true, cellsX, cellsZ, 0, thickness, 1.0f, 1.0f, 1.0f, false, QString());
    scene->recomputeFixedShadows = false;
}

void EditorView::pause(bool paused)
{
    animate = !paused;
}

void EditorView::rewind()
{
    scene->rewind();
}

void EditorView::setTrackball(bool enabled)
{
    trackballMode = enabled;
}

void EditorView::zoomOut()
{
    zoom *= 0.9f;
}

void EditorView::zoomIn()
{
    zoom *= 1.1f;
}

bool EditorView::save(const QString& file)
{
    scene->cubeMap->textureBaseName = skybox;
    return scene->saveLevel(file);
}

bool EditorView::load(const QString& file)
{
    makeCurrent();
    const bool ok = scene->loadLevel(file);
    if (ok && !scene->cubeMap->textureBaseName.isEmpty())
        skybox = scene->cubeMap->textureBaseName;
    else
        scene->cubeMap->setTexture(skybox);
    doneCurrent();
    if (ok)
        fileName = file;
    return ok;
}

void EditorView::setShadows(bool enabled)
{
    scene->shadows = enabled;
    update();
}

void EditorView::setView(int index, int x, int y, int z)
{
    if (index >= 0 && index < 6)
        scene->views[index].eye = Point3i(x, y, z);
    update();
}

void EditorView::addLadder()
{
    // The new ladder follows the mouse until the next click
    auto* g = new EnvGroup(DETAIL_MEDIUM);
    g->env = new Ladder();
    scene->environment.push_back(g);
    placing = g;
    setMouseTracking(true);
    update();
}

void EditorView::setProbabilities(int powerUp, int bonus, int weapons, int fruit, int lives, int tornado, int bombs)
{
    scene->powerUpChance = powerUp;
    scene->bonusChance = bonus;
    scene->weaponWeight = weapons;
    scene->fruitWeight = fruit;
    scene->lifeWeight = lives;
    scene->tornadoWeight = tornado;
    scene->bombWeight = bombs;
    update();
}

} // namespace pang
