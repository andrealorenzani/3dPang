#include "Geometry.h"

#include "GlUtil.h"

#include <initializer_list>
#include <utility>

namespace pang {

namespace {
void setTextureParameters(GLuint texture)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
} // namespace

// ---------------------------------------------------------------- Box

void Box::glDraw()
{
    glDisable(GL_CULL_FACE);
    setTextureParameters(texSide);

    // When the stencil test is on (shadow pass) only the top face must
    // write into the stencil buffer: shadows are drawn on top faces only.
    const bool stencil = glIsEnabled(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (stencil)
        glDisable(GL_STENCIL_TEST);
    glColor4f(color[0], color[1], color[2], alpha);

    // Side faces
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0, dy, dz);
    glVertex3f(0, 0, dz);
    glVertex3f(dx, dy, dz);
    glVertex3f(dx, 0, dz);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(dx, dy, 0);
    glVertex3f(dx, 0, 0);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0, dy, 0);
    glVertex3f(0, 0, 0);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0, dy, dz);
    glVertex3f(0, 0, dz);
    glEnd();

    // Top face, the only one taking part in the stencil test
    if (stencil)
        glEnable(GL_STENCIL_TEST);
    setTextureParameters(texTop);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, dy, dz);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(dx, dy, dz);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(dx, dy, 0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0, dy, 0);
    glEnd();

    // Bottom face.
    // (The 2005 code switched stencil and culling inside glBegin/glEnd, where
    // OpenGL ignores state changes, so culling stayed off after every box.)
    if (stencil)
        glDisable(GL_STENCIL_TEST);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, dz);
    glVertex3f(dx, 0, dz);
    glVertex3f(dx, 0, 0);
    glEnd();

    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    if (stencil)
        glEnable(GL_STENCIL_TEST);
}

Point3f Box::setColor(float red, float green, float blue)
{
    const Point3f old = color;
    color = Point3f(red, green, blue);
    return old;
}

// ---------------------------------------------------------------- Sphere

void Sphere::glDraw()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(color[0], color[1], color[2], alpha);
    gl::drawSphere(radius, slices, stacks);
    glDisable(GL_BLEND);
}

Point3f Sphere::setColor(float red, float green, float blue)
{
    const Point3f old = color;
    color = Point3f(red, green, blue);
    return old;
}

// ---------------------------------------------------------------- Spiral

Point3f Spiral::getPos() const
{
    return Point3f(0, 0, 0);  // centre of the base
}

Point3f Spiral::getDim() const
{
    // X/Z: the radius, Y: current height
    return Point3f(radius, float(heightPerSec * objectTime), -radius);
}

void Spiral::glDraw()
{
    turns = int(objectTime * turnsPerSec);
    partialSteps = int((objectTime * turnsPerSec * steps) - (turns * steps));
    generate();
    // Need at least two normal/point/point triples to draw a strip
    if (mesh.size() + partialMesh.size() < 6)
        return;

    glPushMatrix();
    glDisable(GL_CULL_FACE);
    glColor3f(1.0f, 0.8f, 0.0f);  // rope colour
    glBegin(GL_QUAD_STRIP);
    for (size_t j = 0; j + 2 < mesh.size(); j += 3) {
        glNormal(mesh[j]);
        glVertex(mesh[j + 1]);
        glVertex(mesh[j + 2]);
    }
    glEnd();
    glBegin(GL_QUAD_STRIP);
    for (size_t k = 0; k + 2 < partialMesh.size(); k += 3) {
        glNormal(partialMesh[k]);
        glVertex(partialMesh[k + 1]);
        glVertex(partialMesh[k + 2]);
    }
    glEnd();
    glEnable(GL_CULL_FACE);
    glPopMatrix();
}

void Spiral::generate()
{
    // The points are computed procedurally with the OpenGL matrix stack:
    // for every step of a turn we rotate around Y and move out by the radius,
    // then emit the point at the height of every turn still to generate.
    const float stepAngle = 360.0f / steps;
    partialMesh.clear();
    glPushMatrix();
    Matrix44d mm;
    for (int i = 0; i < steps; i++) {
        glLoadIdentity();
        glRotatef(stepAngle * i, 0.0f, 1.0f, 0.0f);
        glTranslatef(radius, (i * (heightBetweenTurns / steps)), 0.0f);
        glGetModelViewMatrix(mm);
        const Point3d normal = mm * Point3d(-1.0, 0.0, 0.0);
        if (i < partialSteps) {
            partialMesh.push_back(normal);
            partialMesh.push_back(mm * Point3d(0, turns * heightBetweenTurns, 0));
            partialMesh.push_back(mm * Point3d(0, (turns * heightBetweenTurns) + thickness, 0));
        }
        for (int j = generatedTurns; j < turns; j++) {
            mesh.push_back(normal);
            mesh.push_back(mm * Point3d(0, j * heightBetweenTurns, 0));
            mesh.push_back(mm * Point3d(0, (j * heightBetweenTurns) + thickness, 0));
        }
    }
    generatedTurns = turns;
    glPopMatrix();
}

// ---------------------------------------------------------------- ShadowCircle

void ShadowCircle::generate()
{
    mesh.clear();
    const float stepAngle = 360.0f / sides;
    float angle = 0.0f;
    Matrix44d mm;
    glPushMatrix();
    mesh.push_back(Point3d(0, 0, 0));  // centre
    mesh.push_back(Point3d(0, 1, 0));  // normal
    for (int i = 0; i < sides; i++) {
        glLoadIdentity();
        glRotatef(angle, 0, 1, 0);
        angle += stepAngle;
        glGetModelViewMatrix(mm);
        mesh.push_back(mm * Point3d(radius, 0, 0));
    }
    mesh.push_back(Point3d(radius, 0, 0));  // close the fan
    glPopMatrix();
    generated = true;
}

void ShadowCircle::glDraw()
{
    if (!generated)
        generate();
    glPushMatrix();
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glBegin(GL_TRIANGLE_FAN);
    // Opaque black in the centre, transparent on the border: smooth shading blurs it
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    glVertex(mesh[0]);
    glNormal(mesh[1]);
    glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
    for (size_t i = 2; i < mesh.size(); i++)
        glVertex(mesh[i]);
    glEnd();
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glPopMatrix();
}

// ---------------------------------------------------------------- ShadowRect

void ShadowRect::glDraw()
{
    glPushMatrix();
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glTranslate(pos);
    // A unit square scaled to the rectangle
    glScalef(halfX, 0, halfZ);
    if (detail == DETAIL_HIGH) {
        // Inner square (75%) with a light fade, then the border fading out
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(0.0f, 0.0f, 0.0f, alpha);
        glVertex(Point3f(0, 0, 0));
        glNormal(Point3f(0, 1, 0));
        glColor4f(0.0f, 0.0f, 0.0f, 0.75f * alpha);
        glVertex(Point3f(0.75f, 0, -0.75f));
        glVertex(Point3f(0.75f, 0, 0.75f));
        glVertex(Point3f(-0.75f, 0, 0.75f));
        glVertex(Point3f(-0.75f, 0, -0.75f));
        glVertex(Point3f(0.75f, 0, -0.75f));
        glEnd();
        const Point3f inner[5] = {Point3f(0.75f, 0, -0.75f), Point3f(0.75f, 0, 0.75f), Point3f(-0.75f, 0, 0.75f),
                                  Point3f(-0.75f, 0, -0.75f), Point3f(0.75f, 0, -0.75f)};
        const Point3f outer[5] = {Point3f(1, 0, -1), Point3f(1, 0, 1), Point3f(-1, 0, 1), Point3f(-1, 0, -1),
                                  Point3f(1, 0, -1)};
        glBegin(GL_QUAD_STRIP);
        glNormal(Point3f(0, 1, 0));
        for (int i = 0; i < 5; ++i) {
            glColor4f(0.0f, 0.0f, 0.0f, 0.75f * alpha);
            glVertex(inner[i]);
            glColor4f(0.0f, 0.0f, 0.0f, 0.1f * alpha);
            glVertex(outer[i]);
        }
        glEnd();
    } else if (detail == DETAIL_MEDIUM) {
        // A plain, very light rectangle
        glColor4f(0.0f, 0.0f, 0.0f, 0.25f * alpha);
        glBegin(GL_POLYGON);
        glNormal(Point3f(0, 1, 0));
        glVertex(Point3f(1, 0, -1));
        glVertex(Point3f(1, 0, 1));
        glVertex(Point3f(-1, 0, 1));
        glVertex(Point3f(-1, 0, -1));
        glEnd();
    }
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glPopMatrix();
}

// ---------------------------------------------------------------- Hook / Harpoon

void Hook::glDraw()
{
    glPushMatrix();
    glRotatef(angle, 0, 1, 0);  // the hook spins while rising
    glDisable(GL_CULL_FACE);
    glBegin(GL_TRIANGLES);
    glColor3f(0.3f, 0.3f, 0.3f);
    // Two triangles in the XY plane, the upper one overlapping the lower one...
    glVertex3f(-size, 0, 0);
    glVertex3f(size, 0, 0);
    glVertex3f(0, 3 * size, 0);
    glVertex3f(-size, 2 * size, 0);
    glVertex3f(size, 2 * size, 0);
    glVertex3f(0, 5 * size, 0);
    // ...and the same two in the YZ plane
    glVertex3f(0, 0, -size);
    glVertex3f(0, 0, size);
    glVertex3f(0, 3 * size, 0);
    glVertex3f(0, 2 * size, -size);
    glVertex3f(0, 2 * size, size);
    glVertex3f(0, 5 * size, 0);
    glEnd();
    glEnable(GL_CULL_FACE);
    glPopMatrix();
}

void Harpoon::glDraw()
{
    const float angle = float(int(internalTime * rope.turnsPerSec * 360) % 360);
    const float height = internalTime * rope.heightPerSec;
    glPushMatrix();
    rope.glDraw();
    // The hook sits at the top of the rope and turns with it
    glTranslatef(0, height, 0);
    glRotatef(angle, 0, 1, 0);
    glTranslatef(rope.radius, 0.0f, 0.0f);
    hook.glDraw();
    glPopMatrix();
}

// ---------------------------------------------------------------- Player markers

#ifdef PANG3D_EDITOR
namespace {
void beginMarker(const Point3f& pos)
{
    glPushMatrix();
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_FLAT);
    glTranslate(pos);
}

void endMarker()
{
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glPopMatrix();
}

// Draws a flat polygon on the floor; coordinates are in quarters of a unit.
void polygon(const Point3f& color, float alpha, std::initializer_list<std::pair<int, int>> xz)
{
    constexpr float unit = 0.25f;
    glBegin(GL_POLYGON);
    glColor4f(color[0], color[1], color[2], alpha);
    glNormal3f(0, 1, 0);
    for (const auto& [x, z] : xz)
        glVertex3f(x * unit, 0, z * unit);
    glEnd();
}
} // namespace

void MarkerOne::glDraw()
{
    beginMarker(pos);
    polygon(color, alpha, {{-1, -3}, {-2, -2}, {-3, -2}, {-3, -3}, {-1, -5}});
    polygon(color, alpha, {{1, -5}, {1, 3}, {-1, 3}, {-1, -5}});
    endMarker();
}

void MarkerTwo::glDraw()
{
    beginMarker(pos);
    polygon(color, alpha, {{-4, -3}, {-4, -4}, {-2, -4}, {-2, -3}});
    polygon(color, alpha, {{-4, -4}, {-2, -7}, {-1, -5}, {-2, -4}});
    polygon(color, alpha, {{-2, -7}, {2, -7}, {1, -5}, {-1, -5}});
    polygon(color, alpha, {{2, -7}, {4, -4}, {2, -4}, {1, -5}});
    polygon(color, alpha, {{4, -4}, {4, -2}, {-2, 3}, {-4, 3}, {2, -2}, {2, -4}});
    polygon(color, alpha, {{-4, 3}, {5, 3}, {5, 5}, {-4, 5}});
    endMarker();
}
#endif

} // namespace pang
