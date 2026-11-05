#include "GlUtil.h"

#include <QImage>
#include <QString>
#include <QtGlobal>

#include <cmath>
#include <cstdio>
#include <cstring>

namespace pang::gl {

namespace {
constexpr double kPi = 3.14159265358979323846;

void normalize(double v[3])
{
    const double len = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (len > 0.0) {
        v[0] /= len;
        v[1] /= len;
        v[2] /= len;
    }
}

void cross(const double a[3], const double b[3], double out[3])
{
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}

// Column-major 4x4 product: out = a * b
void multiply(const double a[16], const double b[16], double out[16])
{
    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r) {
            double s = 0.0;
            for (int k = 0; k < 4; ++k)
                s += a[k * 4 + r] * b[c * 4 + k];
            out[c * 4 + r] = s;
        }
}

bool invert(const double m[16], double inv[16])
{
    double t[16];
    t[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
    t[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
    t[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
    t[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
    t[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
    t[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
    t[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
    t[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
    t[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
    t[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
    t[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
    t[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
    t[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
    t[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
    t[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
    t[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

    const double det = m[0] * t[0] + m[1] * t[4] + m[2] * t[8] + m[3] * t[12];
    if (det == 0.0)
        return false;
    for (int i = 0; i < 16; ++i)
        inv[i] = t[i] / det;
    return true;
}
} // namespace

void perspective(double fovyDegrees, double aspect, double zNear, double zFar)
{
    const double top = zNear * std::tan(fovyDegrees * kPi / 360.0);
    const double right = top * aspect;
    glFrustum(-right, right, -top, top, zNear, zFar);
}

void lookAt(double eyeX, double eyeY, double eyeZ,
            double centerX, double centerY, double centerZ,
            double upX, double upY, double upZ)
{
    double f[3] = {centerX - eyeX, centerY - eyeY, centerZ - eyeZ};
    double up[3] = {upX, upY, upZ};
    normalize(f);
    double s[3];
    cross(f, up, s);
    normalize(s);
    double u[3];
    cross(s, f, u);

    const double m[16] = {
        s[0], u[0], -f[0], 0.0,
        s[1], u[1], -f[1], 0.0,
        s[2], u[2], -f[2], 0.0,
        0.0,  0.0,  0.0,   1.0,
    };
    glMultMatrixd(m);
    glTranslated(-eyeX, -eyeY, -eyeZ);
}

bool unProject(double winX, double winY, double winZ,
               const double model[16], const double proj[16], const int viewport[4],
               double* objX, double* objY, double* objZ)
{
    double pm[16];
    double inv[16];
    multiply(proj, model, pm);
    if (!invert(pm, inv))
        return false;

    const double in[4] = {
        (winX - viewport[0]) / viewport[2] * 2.0 - 1.0,
        (winY - viewport[1]) / viewport[3] * 2.0 - 1.0,
        winZ * 2.0 - 1.0,
        1.0,
    };
    double out[4];
    for (int r = 0; r < 4; ++r)
        out[r] = inv[r] * in[0] + inv[4 + r] * in[1] + inv[8 + r] * in[2] + inv[12 + r] * in[3];
    if (out[3] == 0.0)
        return false;
    *objX = out[0] / out[3];
    *objY = out[1] / out[3];
    *objZ = out[2] / out[3];
    return true;
}

void drawSphere(double radius, int slices, int stacks)
{
    if (slices < 3)
        slices = 3;
    if (stacks < 2)
        stacks = 2;
    const double dRho = kPi / stacks;
    const double dTheta = 2.0 * kPi / slices;
    for (int i = 0; i < stacks; ++i) {
        const double rho = i * dRho;
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            const double theta = (j == slices) ? 0.0 : j * dTheta;
            for (int k = 0; k < 2; ++k) {
                const double r = rho + k * dRho;
                const double x = -std::sin(theta) * std::sin(r);
                const double y = std::cos(theta) * std::sin(r);
                const double z = std::cos(r);
                glNormal3d(x, y, z);
                glVertex3d(x * radius, y * radius, z * radius);
            }
        }
        glEnd();
    }
}

void drawCylinder(double baseRadius, double topRadius, double height, int slices, int stacks)
{
    if (slices < 3)
        slices = 3;
    if (stacks < 1)
        stacks = 1;
    const double dTheta = 2.0 * kPi / slices;
    // Normal z-component for a cone side, as GLU does
    const double len = std::sqrt(height * height + (baseRadius - topRadius) * (baseRadius - topRadius));
    const double nz = len > 0.0 ? (baseRadius - topRadius) / len : 0.0;
    const double nxy = len > 0.0 ? height / len : 1.0;
    for (int i = 0; i < stacks; ++i) {
        const double z0 = height * i / stacks;
        const double z1 = height * (i + 1) / stacks;
        const double r0 = baseRadius + (topRadius - baseRadius) * i / stacks;
        const double r1 = baseRadius + (topRadius - baseRadius) * (i + 1) / stacks;
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            const double theta = (j == slices) ? 0.0 : j * dTheta;
            const double sx = std::sin(theta);
            const double cy = std::cos(theta);
            glNormal3d(sx * nxy, cy * nxy, nz);
            glVertex3d(r0 * sx, r0 * cy, z0);
            glVertex3d(r1 * sx, r1 * cy, z1);
        }
        glEnd();
    }
}

QImage toGlImage(const QImage& image)
{
    const QImage rgba = image.convertToFormat(QImage::Format_RGBA8888);
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
    return rgba.flipped(Qt::Vertical);
#else
    return rgba.mirrored(false, true);
#endif
}

void uploadImage(GLenum target, const QImage& glImage, bool generateMipmaps)
{
    const GLenum paramTarget =
        (target >= GL_TEXTURE_CUBE_MAP_POSITIVE_X && target <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
            ? GLenum(GL_TEXTURE_CUBE_MAP)
            : target;
    if (generateMipmaps)
        glTexParameteri(paramTarget, GL_GENERATE_MIPMAP, GL_TRUE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(target, 0, GL_RGB, glImage.width(), glImage.height(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, glImage.constBits());
}

GLuint loadTexture2D(const QString& fileName)
{
    if (fileName.isEmpty())
        return 0;
    QImage image(fileName);
    if (image.isNull()) {
        qWarning("3dPang: cannot load texture '%s'", qPrintable(fileName));
        return 0;
    }
    GLuint name = 0;
    glGenTextures(1, &name);
    glBindTexture(GL_TEXTURE_2D, name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    uploadImage(GL_TEXTURE_2D, toGlImage(image));
    return name;
}

bool hasCubeMapSupport()
{
    const auto* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    if (version != nullptr) {
        int major = 0;
        int minor = 0;
        if (std::sscanf(version, "%d.%d", &major, &minor) == 2 && (major > 1 || (major == 1 && minor >= 3)))
            return true;
    }
    const auto* ext = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
    return ext != nullptr && std::strstr(ext, "GL_ARB_texture_cube_map") != nullptr;
}

} // namespace pang::gl
