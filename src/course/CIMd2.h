#pragma once
// Quake II MD2 animated model, from the "Costruzione di Interfacce" course
// library (Paolo Cignoni, ISTI-CNR Pisa). Ported to Qt 6 and the current vcglib, without GLU.

#include "GlInclude.h"

#include "VcgMath.h"

#include <QString>

#include <string>
#include <vector>

#define NUMVERTEXNORMALS 162

#define MD2_MAX_TRIANGLES		4096
#define MD2_MAX_vert		2048
#define MD2_MAX_TEXCOORDS		2048
#define MD2_MAX_FRAMES			512
#define MD2_MAX_SKINS			32
#define MD2_MAX_FRAMESIZE		(MD2_MAX_vert * 4 + 128)
namespace vcg {


class CIMd2
{
private: 
	typedef unsigned char byte;
class Header
{ public:
   int magic; 
   int version; 
   int skinWidth; 
   int skinHeight; 
   int frameSize; 
   int numSkins; 
   int numvert; 
   int numTexCoords; 
   int numTriangles; 
   int numGlCommands; 
   int numFrames; 
   int offsetSkins; 
   int offsetTexCoords; 
   int offsetTriangles; 
   int offsetFrames; 
   int offsetGlCommands; 
   int offsetEnd; 
} ;

class Md2CompressVertex
{ public:
   byte v[3];
   byte lightNormalIndex;
} ;

class Md2Vertex
{ public:
   Point3f v,n;
};

class Md2Triangle
{ public:
   short vertexIndices[3];
   short textureIndices[3];
};

class Md2TexCoord
{ public:
   short s, t;
};

class Md2CompressFrame
{ public:
   Point3f scale;
   Point3f translate;
   char name[16];
//	 Md2CompressVertex *vert;
} ;


class MD2Frame
{ public:
   char name[16];
	 std::vector<Md2Vertex> vert;
} ;
class MD2GLCommand
 {public:
   float s, t;
   int vertexIndex;
};

public:
	CIMd2(){ti=0;CurTime=0;FPS=5;StartFrame=0;EndFrame=0;CurFrame=0;numFrames=0;curFrame=0;nextFrame=0;nextiFrame=0;interp=0;};
~CIMd2(){};
Header header;
// Animation data
float FPS;
int StartFrame;
int EndFrame;
float CurFrame;
float CurTime;



int			numFrames, curFrame, nextFrame, nextiFrame;
//int			animStatus, animSleep, animSpeed, curAnim;
int			interp;
//
	std::vector<std::string> skins;
	std::vector<Md2TexCoord> texCoords;
	std::vector<Md2Triangle> triangles;
	std::vector<MD2Frame> frames;
	std::vector<int> glcommands;
	GLuint ti; // texture index
	std::string tex_name;
	void SetNormal(const int i, Point3f &n) const;
	bool Load(const QString& modelFile, const QString& textureFile);
	void DrawFrame(int frm);
	void DrawFrame(int curFrame, int nextFrame, float pol);
	void DrawTimedFrame(float time);
	void SetAnim(int sf, int ef){CurFrame=float(sf); StartFrame=sf; EndFrame=ef; }
	void SetFPS(float fps) {FPS=fps;};
	void LoadTextures();
};

} // namespace vcg
