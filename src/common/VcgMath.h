#pragma once
// vcglib (https://github.com/cnr-isti-vclab/vcglib, GPL-3.0) provides the
// points, matrices and OpenGL wrappers. In 2005 the game used a 2002 copy of
// the library from the "Costruzione di Interfacce" course. Version 2 uses
// the current upstream release, fetched by CMake.
//
// This header includes what the game needs, in the right order: the OpenGL
// headers must come before vcglib's wrap/gl helpers.

#include "GlInclude.h"

#include <vcg/math/matrix44.h>
#include <vcg/space/point2.h>
#include <vcg/space/point3.h>
#include <vcg/space/point4.h>
#include <wrap/gl/math.h>
#include <wrap/gl/space.h>

namespace vcg {

// The 2002 library had these helpers. vcglib spells them glGetv(GL_..., m).
inline void glGetModelViewMatrix(Matrix44f& m) { glGetv(GL_MODELVIEW_MATRIX, m); }
inline void glGetModelViewMatrix(Matrix44d& m) { glGetv(GL_MODELVIEW_MATRIX, m); }

} // namespace vcg
