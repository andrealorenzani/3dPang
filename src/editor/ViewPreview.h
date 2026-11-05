#pragma once
// Preview of a game view (F3-F8): a floor with a box standing for the
// player, seen from the chosen camera offset.

#include "SceneGraph.h"

#include <QOpenGLWidget>

namespace pang {

class ViewPreview : public QOpenGLWidget {
    Q_OBJECT

public:
    explicit ViewPreview(QWidget* parent = nullptr);
    ~ViewPreview() override;
    QSize sizeHint() const override { return QSize(220, 160); }

public slots:
    void setViewX(int);
    void setViewY(int);
    void setViewZ(int);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    Point3i eye;
    SceneGraph* scene;
};

} // namespace pang
