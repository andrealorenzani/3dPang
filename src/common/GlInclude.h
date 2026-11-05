#pragma once
// Platform OpenGL headers.
//
// 3dPang uses the legacy fixed-function pipeline (immediate mode, display
// lists, the matrix stack and texture coordinate generation). Both programs
// ask for an OpenGL *compatibility* context.
//
// The 2005 version needed GLEW for the cube map extension and GLU for the
// matrix and quadric helpers. Version 2 needs neither. The few post-1.1
// features in use are plain enum values, accepted by the OpenGL 1.1 entry
// points. The GLU helpers are reimplemented in GlUtil.h.

#ifdef _WIN32
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <windows.h>
#endif

#if defined(__APPLE__)
#  ifndef GL_SILENCE_DEPRECATION
#    define GL_SILENCE_DEPRECATION
#  endif
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

// Enums from OpenGL 1.2-1.4 that the Windows SDK <GL/gl.h> (1.1) lacks.
#ifndef GL_CLAMP_TO_EDGE
#  define GL_CLAMP_TO_EDGE 0x812F
#endif
#ifndef GL_GENERATE_MIPMAP
#  define GL_GENERATE_MIPMAP 0x8191
#endif
#ifndef GL_NORMAL_MAP
#  define GL_NORMAL_MAP 0x8511
#endif
#ifndef GL_TEXTURE_CUBE_MAP
#  define GL_TEXTURE_CUBE_MAP 0x8513
#endif
#ifndef GL_TEXTURE_CUBE_MAP_POSITIVE_X
#  define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#  define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#  define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#  define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#  define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#  define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
#endif
