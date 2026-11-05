#pragma once
// Environment cube map, from the "Costruzione di Interfacce" course library
// (Paolo Cignoni, ISTI-CNR Pisa). Ported to Qt 6 and the current vcglib, without GLEW/GLU.

#include "GlInclude.h"

#include "VcgMath.h"

#include <QString>

namespace vcg {

class CICubeMap {
public:
    GLuint ti = 0;      // cube map texture (hardware cube maps)
    GLuint oti[6] = {}; // six 2D textures (fallback)
    float radius = 10;

    CICubeMap() = default;

    // basename is e.g. ".../mount.jpg": faces are mount_posx.jpg, mount_negx.jpg...
    bool Load(const QString& basename);
    bool LoadExt(const QString& basename);
    bool LoadOld(const QString& basename);
    void DrawEnvCubeExt(Matrix44f& tr);
    void DrawEnvCubeOld(Matrix44f& tr);
    void DrawEnvCube(Matrix44f& tr);
    void DrawEnvCube();
    static bool GetName(int i, const QString& basename, QString& filename);
    void finalize();

private:
    bool useCubeMap() const;
    mutable int cubeMapSupport = -1;  // -1 = not checked yet
};

} // namespace vcg
