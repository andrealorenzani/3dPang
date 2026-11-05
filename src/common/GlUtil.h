#pragma once
// Small replacements for the GLU functions the 2005 code used, plus
// QImage -> OpenGL texture helpers (they replace QGLWidget::convertToGLFormat
// and gluBuild2DMipmaps).

#include "GlInclude.h"

class QImage;
class QString;

namespace pang::gl {

// gluPerspective
void perspective(double fovyDegrees, double aspect, double zNear, double zFar);

// gluLookAt
void lookAt(double eyeX, double eyeY, double eyeZ,
            double centerX, double centerY, double centerZ,
            double upX, double upY, double upZ);

// gluUnProject: maps window coordinates to object coordinates.
bool unProject(double winX, double winY, double winZ,
               const double model[16], const double proj[16], const int viewport[4],
               double* objX, double* objY, double* objZ);

// gluSphere with smooth normals (sphere centred at the origin, poles on Z).
void drawSphere(double radius, int slices, int stacks);

// gluCylinder with smooth normals along +Z, without caps.
void drawCylinder(double baseRadius, double topRadius, double height, int slices, int stacks);

// Converts a QImage to tightly packed, bottom-up RGBA8888, the layout OpenGL expects.
QImage toGlImage(const QImage& image);

// Uploads an image into the texture currently bound to `target`
// (GL_TEXTURE_2D or a cube map face) and asks the driver to build mipmaps.
void uploadImage(GLenum target, const QImage& glImage, bool generateMipmaps = true);

// Loads an image file into a new 2D texture with mipmaps.
// Returns 0 if the file cannot be read.
GLuint loadTexture2D(const QString& fileName);

// True when the context supports cube map textures (OpenGL 1.3+).
bool hasCubeMapSupport();

} // namespace pang::gl
