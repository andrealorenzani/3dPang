#pragma once
// The 3D view of the editor: shows the level, selects and moves objects.

#include "CITrackBall.h"
#include "SceneGraph.h"

#include <QOpenGLWidget>
#include <QTimer>

#include <optional>

namespace pang {

class EditorView : public QOpenGLWidget {
    Q_OBJECT

public:
    explicit EditorView(QWidget* parent = nullptr);
    ~EditorView() override;

    SceneGraph* scene;
    QString fileName;        // level being edited
    bool playerOne = true;   // player selected in the "Players" tab
    Point3i center;          // the camera looks here...
    Point3f eye;             // ...from here

    void newBalls(int radius, int children, int startHeight, int maxHeight, float gravity, float dirX, float dirZ,
                  float red, float green, float blue, bool textured, const QString& texture);
    void newFloor(bool destructible, bool isMain, int cellsX, int cellsZ, int h, int thickness, float red,
                  float green, float blue, bool textured, const QString& texture);
    void newWall(bool destructible, bool isMain, int length, int width, int h, int elevation, float red,
                 float green, float blue, bool textured, const QString& texture);
    // Starts an empty level with the given main floor
    void newLevel(int cellsX, int cellsZ, int thickness);
    bool save(const QString& file);
    bool load(const QString& file);
    void resetCamera();
    void setSkybox(const QString& dataRelativeBaseName);
    QImage snapshot() { return grabFramebuffer(); }

public slots:
    void startPlacingPlayer();
    void setPlayerModelVisible(int showModel);
    void setBallRed(int);
    void setBallGreen(int);
    void setBallBlue(int);
    void setFloorRed(int);
    void setFloorGreen(int);
    void setFloorBlue(int);
    void setWallRed(int);
    void setWallGreen(int);
    void setWallBlue(int);
    void deleteBalls();
    void deleteFloors();
    void deleteWalls();
    void pause(bool paused);
    void rewind();
    void setTrackball(bool enabled);
    void zoomIn();
    void zoomOut();
    void setBallTexture(bool textured, const QString& file);
    void setFloorTexture(bool textured, const QString& file);
    void setWallTexture(bool textured, const QString& file);
    void setShadows(bool enabled);
    void setView(int index, int x, int y, int z);
    void addLadder();
    void deleteLadders();
    void setProbabilities(int powerUp, int bonus, int weapons, int fruit, int lives, int tornado, int bombs);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;
    void mouseDoubleClickEvent(QMouseEvent* e) override;

private:
    Point2i toPixels(const QPoint& p) const;
    void applyColor(Point3f& color, int channel, int value, int target);

    bool animate = false;
    bool dragging = false;
    bool trackballMode = false;
    CITrackball trackball;
    Matrix44f rotationMatrix;
    float zoom = 1.0f;
    // Object being placed with the mouse (a player or a new ladder)
    Group* placing = nullptr;
    bool finishPlacing = false;          // the click that drops it happened
    std::optional<Point2i> placeAt;      // mouse position to place it at
    std::optional<Point2i> pickAt;       // click to resolve in the next frame
    std::optional<Point2i> centerAt;     // double click: new camera centre
    Point2i dragLast = Point2i(0, 0);
    Point2i dragDelta = Point2i(0, 0);
    Point3f ballColor, floorColor, wallColor;
    QString skybox = QStringLiteral("skyboxes/mount.jpg");
    QTimer timer;
    bool glReady = false;
};

} // namespace pang
