#pragma once
// Level geometry: floors, walls and ladders.

#include "Definitions.h"
#include "Geometry.h"

#include <list>

namespace pang {

// Base class of environment objects.
class EnvObject {
public:
    virtual ~EnvObject() = default;

    bool useDisplayList = false;  // compiled into a display list?
    bool textured = false;
    GLuint displayList = 0;
    bool selected = false;        // editor selection
    Box* mainBox = nullptr;       // main box of the object

    virtual NodeType getType() const { return ENV_OBJECT; }
    // Minimum and maximum corners, relative to the group position
    virtual Point3f getPos() = 0;
    virtual Point3f getDim() = 0;
    virtual void glDraw() = 0;
    virtual void genList() = 0;
#ifdef PANG3D_EDITOR
    virtual void pick(Point3d p) = 0;
#endif
};

class Ladder : public EnvObject {
public:
    int rungs = 1;

    Ladder();
    ~Ladder() override;
    NodeType getType() const override { return LADDER; }
    Point3f getPos() override { return rotated ? Point3f(0.0f, 0.0f, -2.9f) : Point3f(0.0f, 0.0f, 0.0f); }
    Point3f getDim() override
    {
        return rotated ? Point3f(0.5f, 0.7f * rungs, 0) : Point3f(2.9f, 0.7f * rungs, 0.5f);
    }
    void glDraw() override;
    void rotate90() { rotated = !rotated; }
    bool isRotated() const { return rotated; }
    void genList() override;
    // Adds (or removes, if negative) rungs
    void addRungs(int count);
#ifdef PANG3D_EDITOR
    void pick(Point3d p) override;
#endif

private:
    void drawGeometry(float sideOffset);
    Box* rung;
    Box* side;
    bool rotated = false;
};

#ifdef PANG3D_EDITOR
// One cell of a floor. Floors are grids of cells in the editor:
// balls are created on top of the selected cells.
class Cell : public EnvObject {
public:
    float thickness;
    int row;     // 1-based position along X
    int column;  // 1-based position along Z
    float size;  // cells are squares

    Cell(float cellSize, int r, int c, float thick) : thickness(thick), row(r), column(c), size(cellSize) {}
    void glDraw() override;
    Point3f getPos() override { return Point3f((row - 1) * size, thickness, column * size); }
    Point3f getDim() override { return Point3f(row * size, thickness, (column - 1) * size); }
    void genList() override {}
    void pick(Point3d) override { selected = !selected; }
    NodeType getType() const override { return CELL; }
};
#endif

class Floor : public EnvObject {
public:
    float alpha = 1.0f;
#ifdef PANG3D_EDITOR
    std::list<Cell*> cells;
    int selectedCells = 0;
#endif
    int cellsX;           // cells along X
    int cellsZ;           // cells along Z
    float cellSize;
    float thickness;
    Box* slab;

    explicit Floor(int numCellsX = 10, int numCellsZ = 10, float size = 5, float thick = 0.1f);
    ~Floor() override;
    void glDraw() override;
    void genList() override;
    NodeType getType() const override { return FLOOR; }
    Point3f getPos() override { return slab->getPos(); }
    Point3f getDim() override { return slab->getDim(); }
    // Replaces the slab, e.g. with the one read from a level file
    void setSlab(Box* box);
#ifdef PANG3D_EDITOR
    void pick(Point3d p) override;
#endif

private:
#ifdef PANG3D_EDITOR
    void createCells();
#endif
};

class Wall : public EnvObject {
public:
    float alpha = 1.0f;
    Box* box;

    explicit Wall(float sizeX = 1, float sizeY = 1, float sizeZ = 1);
    ~Wall() override;
    void glDraw() override;
    void genList() override;
    NodeType getType() const override { return WALL; }
    Point3f getPos() override { return box->getPos(); }
    Point3f getDim() override { return box->getDim(); }
#ifdef PANG3D_EDITOR
    void pick(Point3d p) override
    {
        const Point3f pos = getPos();
        const Point3f dim = getDim();
        if (pos[1] <= p[1] && p[1] <= dim[1] && pos[2] <= p[2] && p[2] <= dim[2] && pos[0] <= p[0] && p[0] <= dim[0])
            selected = !selected;
    }
#endif
};

} // namespace pang
