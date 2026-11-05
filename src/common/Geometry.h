#pragma once
// Basic drawable shapes: boxes, spheres, shadows, the harpoon...

#include "Definitions.h"

#include <vector>

namespace pang {

// Axis-aligned box ("parallelepiped") with its minimum corner at the origin.
class Box {
public:
    GLuint texTop = 0;   // texture of the top (and bottom) face
    GLuint texSide = 0;  // texture of the side faces
    Point3f color;
    float alpha = 1.0f;
    float dx, dy, dz;    // size

    Box(float sizeX, float sizeY, float sizeZ)
        : color(1.0f, 1.0f, 1.0f), dx(sizeX), dy(sizeY), dz(sizeZ) {}

    // Sets the colour and returns the previous one
    Point3f setColor(float red, float green, float blue);
    void glDraw();
    // Minimum and maximum corner (the box is not centred on the origin)
    Point3f getPos() const { return Point3f(0, 0, 0); }
    Point3f getDim() const { return Point3f(dx, dy, dz); }
};

class Sphere {
public:
    Point3f color;
    float alpha = 1.0f;
    float radius;
    int slices, stacks;  // tessellation

    Sphere(float r, int numSlices, int numStacks)
        : color(1.0f, 0.0f, 0.0f), radius(r), slices(numSlices), stacks(numStacks) {}

    Point3f setColor(float red, float green, float blue);
    void glDraw();
};

// Soft rectangular shadow cast by a floor or wall on the objects below.
class ShadowRect {
public:
    float halfX = 1.0f;   // half size along X
    float halfZ = 1.0f;   // half size along Z
    float alpha = 1.0f;
    Point3f pos;          // centre
    int detail;

    explicit ShadowRect(int detailLevel) : detail(detailLevel) { pos.SetZero(); }
    void glDraw();
};

// Soft round shadow cast by a ball: a triangle fan that fades out at the border.
// Drawn by hand instead of with a GLU disk so that the border can be blurred.
class ShadowCircle {
public:
    float radius;

    explicit ShadowCircle(int numSides = 8, float r = 1.0f)
        : radius(r), sides(numSides < 4 ? 4 : numSides) {}
    void glDraw();

private:
    void generate();
    bool generated = false;
    std::vector<Point3d> mesh;  // centre, normal, then the border points
    int sides;
};

// The rope of the harpoon: a spiral that grows over time.
class Spiral {
public:
    double objectTime = 0.0;  // seconds since creation
    float radius;
    int steps;                // points per turn
    float heightPerSec;
    float turnsPerSec;
    float thickness;

    Spiral(float r, int numSteps, float heightPerSecond, float turnsPerSecond, float thick)
        : radius(r), steps(numSteps), heightPerSec(heightPerSecond), turnsPerSec(turnsPerSecond),
          thickness(thick), heightBetweenTurns(heightPerSecond / turnsPerSecond) {}

    Point3f getPos() const;
    Point3f getDim() const;
    void glDraw();
    void update(float dt) { objectTime += dt; }

private:
    void generate();
    int turns = 0;              // complete turns to draw
    int partialSteps = 0;       // steps of the turn being drawn
    float heightBetweenTurns;
    int generatedTurns = 0;
    // normal, point, point triples of the complete turns and of the current one
    std::vector<Point3d> mesh;
    std::vector<Point3d> partialMesh;
};

// Hook at the top of the harpoon: two pairs of crossed triangles.
class Hook {
public:
    Hook(float turnsPerSecond, float baseSize)
        : turnsPerSec(turnsPerSecond), size(baseSize) {}
    void glDraw();
    void update(float dt)
    {
        internalTime += dt;
        angle = float(int(internalTime * turnsPerSec * 360) % 360);
    }
    Point3f getPos() const { return Point3f(0, 0, 0); }
    Point3f getDim() const { return Point3f(size, 5 * size, size); }

private:
    float internalTime = 0.0f;
    float turnsPerSec;
    float angle = 0.0f;
    float size;
};

// Harpoon = spiral rope + hook on top.
class Harpoon {
public:
    float internalTime = 0.0f;

    Harpoon(float r, int numSteps, float heightPerSecond, float turnsPerSecond, float thick, float hookTurnsPerSecond)
        : rope(r, numSteps, heightPerSecond, turnsPerSecond, thick), hook(hookTurnsPerSecond, r / 5.0f) {}
    void update(float dt)
    {
        rope.update(dt);
        hook.update(dt);
        internalTime += dt;
    }
    void glDraw();
    Point3f getPos() const { return rope.getPos(); }
    Point3f getDim() const
    {
        Point3f d = rope.getDim();
        d[1] += hook.getDim()[1];
        return d;
    }

private:
    Spiral rope;
    Hook hook;
};

#ifdef PANG3D_EDITOR
// In the editor the start position of a player is shown with a big
// "1" or "2" drawn on the floor.
class PlayerMarker {
public:
    Point3f pos;
    Point3f color;
    float alpha = 1.0f;

    PlayerMarker() : color(0.0f, 1.0f, 0.0f) { pos.SetZero(); }
    virtual ~PlayerMarker() = default;
    virtual void glDraw() = 0;
    virtual int number() const = 0;
};

class MarkerOne : public PlayerMarker {
public:
    void glDraw() override;
    int number() const override { return 1; }
};

class MarkerTwo : public PlayerMarker {
public:
    void glDraw() override;
    int number() const override { return 2; }
};
#endif

} // namespace pang
