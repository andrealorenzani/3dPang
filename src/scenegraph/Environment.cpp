#include "Environment.h"

namespace pang {

// ---------------------------------------------------------------- Cell

#ifdef PANG3D_EDITOR
void Cell::glDraw()
{
    // Selected cells are highlighted in pink just above the floor
    if (!selected)
        return;
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef((row - 1) * size, thickness, (column - 1) * size);
    glColor3f(1.0f, 0.0f, 0.3f);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, size);
    glVertex3f(size, 0, size);
    glVertex3f(size, 0, 0);
    glEnd();
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
}
#endif

// ---------------------------------------------------------------- Floor

Floor::Floor(int numCellsX, int numCellsZ, float size, float thick)
    : cellsX(numCellsX), cellsZ(numCellsZ), cellSize(size), thickness(thick),
      slab(new Box(numCellsX * size, thick, numCellsZ * size))
{
    mainBox = slab;
#ifdef PANG3D_EDITOR
    createCells();
#endif
}

Floor::~Floor()
{
    if (useDisplayList && displayList != 0)
        glDeleteLists(displayList, 1);
    delete slab;
#ifdef PANG3D_EDITOR
    for (Cell* c : cells)
        delete c;
#endif
}

void Floor::setSlab(Box* box)
{
    delete slab;
    slab = box;
    mainBox = slab;
}

void Floor::glDraw()
{
    glPushMatrix();
    if (!textured)
        glDisable(GL_TEXTURE_2D);
#ifdef PANG3D_EDITOR
    // Floors change while editing: never cache them, and show the
    // selection by halving the opacity
    useDisplayList = false;
    slab->alpha = selectedCells > 0 ? alpha / 2 : alpha;
#else
    slab->alpha = alpha;
#endif
    if (useDisplayList)
        glCallList(displayList);
    else
        slab->glDraw();
    if (!textured)
        glEnable(GL_TEXTURE_2D);
    glPopMatrix();
#ifdef PANG3D_EDITOR
    for (Cell* c : cells) {
        glPushMatrix();
        c->glDraw();
        glPopMatrix();
    }
#endif
}

void Floor::genList()
{
    if (displayList == 0)
        displayList = glGenLists(1);
    useDisplayList = true;
    glNewList(displayList, GL_COMPILE);
    slab->glDraw();
    glEndList();
}

#ifdef PANG3D_EDITOR
void Floor::createCells()
{
    for (int i = 1; i <= cellsX; i++)
        for (int j = 1; j <= cellsZ; j++)
            cells.push_back(new Cell(cellSize, i, j, thickness + 0.1f));
}

void Floor::pick(Point3d p)
{
    const Point3f pos = getPos();
    const Point3f dim = getDim();
    if (pos[1] < (p[1] - (thickness / 2)) && dim[1] > p[1] - (thickness / 2) && pos[2] < p[2] && dim[2] > p[2] &&
        pos[0] < p[0] && dim[0] > p[0]) {
        // Find the cell under the point (cells are stored row by row)
        const Point3d local = p - Point3d(pos[0], pos[1], pos[2]);
        const int row = int(local[0] / cellSize);
        const int column = int(local[2] / cellSize);
        const int index = cellsZ * row + column;
        if (index < 0 || index >= int(cells.size()))
            return;
        auto it = cells.begin();
        std::advance(it, index);
        (*it)->pick(p);
        selectedCells += (*it)->selected ? 1 : -1;
    }
}
#endif

// ---------------------------------------------------------------- Wall

Wall::Wall(float sizeX, float sizeY, float sizeZ) : box(new Box(sizeX, sizeY, sizeZ))
{
    mainBox = box;
}

Wall::~Wall()
{
    if (useDisplayList && displayList != 0)
        glDeleteLists(displayList, 1);
    delete box;
}

void Wall::glDraw()
{
    if (!textured)
        glDisable(GL_TEXTURE_2D);
#ifdef PANG3D_EDITOR
    useDisplayList = false;
    box->alpha = selected ? alpha / 2 : alpha;
#else
    box->alpha = alpha;
#endif
    if (useDisplayList)
        glCallList(displayList);
    else
        box->glDraw();
    if (!textured)
        glEnable(GL_TEXTURE_2D);
}

void Wall::genList()
{
    if (displayList == 0)
        displayList = glGenLists(1);
    useDisplayList = true;
    glNewList(displayList, GL_COMPILE);
    box->glDraw();
    glEndList();
}

// ---------------------------------------------------------------- Ladder

namespace {
constexpr float kLadderBrown[3] = {0.47f, 0.33f, 0.0f};
} // namespace

Ladder::Ladder() : rung(new Box(2.8f, 0.2f, 0.2f)), side(new Box(0.2f, 0.7f, 0.5f))
{
    rung->setColor(kLadderBrown[0], kLadderBrown[1], kLadderBrown[2]);
    side->setColor(kLadderBrown[0], kLadderBrown[1], kLadderBrown[2]);
    mainBox = side;
}

Ladder::~Ladder()
{
    if (useDisplayList && displayList != 0)
        glDeleteLists(displayList, 1);
    delete rung;
    delete side;
}

void Ladder::addRungs(int count)
{
    rungs += count;
    if (rungs < 1)
        rungs = 1;
    delete side;
    side = new Box(0.2f, 0.7f * rungs, 0.5f);
    side->setColor(kLadderBrown[0], kLadderBrown[1], kLadderBrown[2]);
    mainBox = side;
    genList();
}

void Ladder::drawGeometry(float sideOffset)
{
    glPushMatrix();
    for (int i = 0; i < rungs; i++) {
        glColor3f(kLadderBrown[0], kLadderBrown[1], kLadderBrown[2]);
        glPushMatrix();
        glTranslatef(0.2f, (0.25f + 0.7f * i), 0.15f);
        glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
        rung->glDraw();
        glPopMatrix();
    }
    side->glDraw();
    glTranslatef(sideOffset, 0.0f, 0.0f);
    side->glDraw();
    glPopMatrix();
}

void Ladder::glDraw()
{
#ifdef PANG3D_PLAYER
    if (useDisplayList) {
        glCallList(displayList);
        return;
    }
#endif
    drawGeometry(3.0f);
}

void Ladder::genList()
{
    if (displayList == 0)
        displayList = glGenLists(1);
    useDisplayList = true;
    glNewList(displayList, GL_COMPILE);
    // (2005: the cached ladder had its second side 0.3 units closer)
    drawGeometry(2.7f);
    glEndList();
}

#ifdef PANG3D_EDITOR
void Ladder::pick(Point3d p)
{
    const Point3f pos = getPos();
    const Point3f dim = getDim();
    if (pos[2] < p[2] && p[2] < dim[2] && pos[0] < p[0] && p[0] < dim[0] && pos[1] < p[1] && p[1] < dim[1])
        selected = !selected;
    const float a = selected ? 0.5f : 1.0f;
    rung->alpha = a;
    side->alpha = a;
}
#endif

} // namespace pang
