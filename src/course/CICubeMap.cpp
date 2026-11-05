#include "CICubeMap.h"

#include "GlUtil.h"

#include <QImage>

using namespace vcg;

static Point3f cube_vertices[8] = {
    Point3f(-1.0, 1.0, -1.0),  // l t f
    Point3f(-1.0, -1.0, -1.0), // l b f
    Point3f(1.0, -1.0, -1.0),  // r b f
    Point3f(1.0, 1.0, -1.0),   // r t f

    Point3f(-1.0, 1.0, 1.0),   // l t b
    Point3f(-1.0, -1.0, 1.0),  // l b b
    Point3f(1.0, -1.0, 1.0),   // r b b
    Point3f(1.0, 1.0, 1.0),    // r t b
};

static int cube_faces[6][4] = {
    {3, 2, 6, 7}, // right
    {4, 5, 1, 0}, // left
    {4, 0, 3, 7}, // top
    {1, 5, 6, 2}, // bottom
    {7, 6, 5, 4}, // back
    {0, 1, 2, 3}, // front
};

static float tex_vert[4][2] = {
    {1, 1}, {1, 0}, {0, 0}, {0, 1},
};

bool CICubeMap::useCubeMap() const
{
    if (cubeMapSupport < 0)
        cubeMapSupport = pang::gl::hasCubeMapSupport() ? 1 : 0;
    return cubeMapSupport == 1;
}

void CICubeMap::finalize()
{
    if (glIsTexture(ti))
        glDeleteTextures(1, &ti);
    for (int i = 0; i < 6; ++i) {
        if (glIsTexture(oti[i]))
            glDeleteTextures(1, &oti[i]);
        oti[i] = 0;
    }
    ti = 0;
}

void CICubeMap::DrawEnvCube()
{
    Matrix44f id;
    id.SetIdentity();
    DrawEnvCube(id);
}

void CICubeMap::DrawEnvCube(Matrix44f& tr)
{
    if (!useCubeMap())
        DrawEnvCubeOld(tr);
    else
        DrawEnvCubeExt(tr);
}

// Without hardware cube maps, draw a plain cube with six independent
// textures. The seams at the edges are visible
void CICubeMap::DrawEnvCubeOld(Matrix44f& tr)
{
    glMatrixMode(GL_MODELVIEW);
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_1D);
    glEnable(GL_TEXTURE_2D);
    glDepthMask(false);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glPushMatrix();
    glLoadIdentity();
    glMultMatrix(tr);
    glScalef(radius, radius, radius);

    for (int i = 0; i < 6; ++i) {
        glBindTexture(GL_TEXTURE_2D, oti[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);
        for (int j = 0; j < 4; ++j) {
            glTexCoord2f(tex_vert[j][0], tex_vert[j][1]);
            glVertex(cube_vertices[cube_faces[i][j]]);
        }
        glEnd();
    }

    glDepthMask(true);
    glPopMatrix();
    glPopAttrib();
}

void CICubeMap::DrawEnvCubeExt(Matrix44f& tr)
{
    Matrix44f ii = vcg::Inverse(tr);

    glMatrixMode(GL_MODELVIEW);
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);
    glPushMatrix();
    glLoadIdentity();
    glScalef(radius, radius, radius);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_1D);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ti);
    glDepthMask(false);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();
    glMultMatrix(ii);

    for (int i = 0; i < 6; ++i) {
        glBegin(GL_QUADS);
        for (int j = 0; j < 4; ++j) {
            glNormal(-cube_vertices[cube_faces[i][j]]);
            glVertex(cube_vertices[cube_faces[i][j]]);
        }
        glEnd();
    }
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_CUBE_MAP);
    glDepthMask(true);

    glPopMatrix();
    glPopAttrib();
}

bool CICubeMap::GetName(int i, const QString& basename, QString& filename)
{
    static const char* const exts[6] = {"_negx", "_posx", "_negy", "_posy", "_negz", "_posz"};
    const int dot = basename.lastIndexOf(QLatin1Char('.'));
    if (dot < 0)
        return false;
    filename = basename.left(dot) + QLatin1String(exts[i]) + basename.mid(dot);
    return true;
}

bool CICubeMap::Load(const QString& basename)
{
    if (basename.isEmpty())
        return false;
    if (!useCubeMap())
        return LoadOld(basename);
    return LoadExt(basename);
}

bool CICubeMap::LoadOld(const QString& basename)
{
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_TEXTURE_1D);
    glEnable(GL_TEXTURE_2D);
    if (oti[0] == 0)
        glGenTextures(6, &(oti[0]));
    QString filename;
    bool ok = true;
    for (int i = 0; i < 6 && ok; ++i) {
        QImage tt;
        ok = GetName(i, basename, filename) && tt.load(filename);
        if (!ok)
            break;
        glBindTexture(GL_TEXTURE_2D, oti[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        pang::gl::uploadImage(GL_TEXTURE_2D, pang::gl::toGlImage(tt), false);
    }
    glPopAttrib();
    return ok;
}

bool CICubeMap::LoadExt(const QString& basename)
{
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glDisable(GL_TEXTURE_1D);
    glDisable(GL_TEXTURE_2D);
    if (ti == 0)
        glGenTextures(1, &ti);

    glBindTexture(GL_TEXTURE_CUBE_MAP, ti);
    const GLenum targets[6] = {
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    };
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    bool ok = true;
    for (int i = 0; i < 6; ++i) {
        QString filename;
        QImage tt;
        if (!GetName(i, basename, filename) || !tt.load(filename)) {
            ok = false;
            break;
        }
        pang::gl::uploadImage(targets[i], pang::gl::toGlImage(tt), false);
    }

    glDisable(GL_TEXTURE_CUBE_MAP);
    glPopAttrib();
    return ok;
}
