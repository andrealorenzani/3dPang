#include "CIMd2.h"

#include "DataPaths.h"
#include "GlUtil.h"

#include <QImage>

#include <cmath>
#include <cstdio>
#include <cstring>

using namespace vcg;
using namespace std;

bool CIMd2::Load(const QString& modelFile, const QString& textureFile)
{
    tex_name = textureFile.toStdString();
    ti = 0;  // the texture is (re)loaded on the next draw
    triangles.clear();
    FILE* md2file = fopen(pang::paths::native(pang::paths::resolve(modelFile)).constData(), "rb");
    if (md2file == nullptr)
        return false;

    // Reads `count` items, false on a truncated file
    const auto read = [md2file](void* dst, size_t size, size_t count) {
        return count == 0 || fread(dst, size, count, md2file) == count;
    };
    if (!read(&header, sizeof(CIMd2::Header), 1)) {
        fclose(md2file);
        return false;
    }
    char ver[5];
    snprintf(ver, sizeof ver, "%c%c%c%c", header.magic, header.magic >> 8, header.magic >> 16, header.magic >> 24);
    if (strcmp(ver, "IDP2") != 0 || header.version != 8) {
        qWarning("3dPang: bad header for MD2 file '%s'", qPrintable(modelFile));
        fclose(md2file);
        return false;
    }
    numFrames = header.numFrames;

    fseek(md2file, header.offsetSkins, SEEK_SET);
    skins.resize(header.numSkins);
    for (int i = 0; i < header.numSkins; i++) {
        char buf[65] = {};
        if (!read(buf, 64, 1))
            break;
        skins[i] = buf;
    }

    fseek(md2file, header.offsetTexCoords, SEEK_SET);
    texCoords.resize(header.numTexCoords);
    bool ok = read(texCoords.data(), sizeof(Md2TexCoord), header.numTexCoords);

    fseek(md2file, header.offsetTriangles, SEEK_SET);
    triangles.resize(header.numTriangles);
    ok = ok && read(triangles.data(), sizeof(Md2Triangle), header.numTriangles);

    fseek(md2file, header.offsetFrames, SEEK_SET);
    frames.resize(header.numFrames);
    vector<Md2CompressVertex> tmpvert(header.numvert);
    for (int i = 0; i < header.numFrames; i++) {
        Md2CompressFrame tt;
        float scale[3], translate[3];
        ok = ok && read(scale, sizeof(float), 3) && read(translate, sizeof(float), 3) && read(tt.name, sizeof(char), 16);
        tt.name[15] = 0;
        ok = ok && read(tmpvert.data(), sizeof(Md2CompressVertex), header.numvert);
        if (!ok)
            break;
        memcpy(frames[i].name, tt.name, sizeof tt.name);
        frames[i].vert.resize(header.numvert);
        for (int j = 0; j < header.numvert; j++) {
            frames[i].vert[j].v[0] = float(int(tmpvert[j].v[0])) * scale[0] + translate[0];
            frames[i].vert[j].v[1] = -(float(int(tmpvert[j].v[1])) * scale[1] + translate[1]);
            frames[i].vert[j].v[2] = 24 + float(int(tmpvert[j].v[2])) * scale[2] + translate[2];
            SetNormal(tmpvert[j].lightNormalIndex, frames[i].vert[j].n);
            frames[i].vert[j].n[1] *= -1.0f;
        }
    }

    fseek(md2file, header.offsetGlCommands, SEEK_SET);
    glcommands.resize(header.numGlCommands);
    ok = ok && read(glcommands.data(), sizeof(int), header.numGlCommands);
    fclose(md2file);
    if (!ok) {
        qWarning("3dPang: truncated MD2 file '%s'", qPrintable(modelFile));
        triangles.clear();
        return false;
    }
    if (glcommands.empty())
        glcommands.push_back(0);
    return true;
}

void CIMd2::DrawFrame(int curFrame)
{
    if (triangles.empty() || curFrame < 0 || curFrame >= int(frames.size()))
        return;
    if (ti == 0)
        LoadTextures();

    int i = 0;
    int val = glcommands[i++];
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ti);
    glColor3f(1.f, 1.f, 1.f);

    while (val != 0) {
        int count;
        if (val > 0) {
            glBegin(GL_TRIANGLE_STRIP);
            count = val;
        } else {
            glBegin(GL_TRIANGLE_FAN);
            count = -val;
        }
        while (count--) {
            float s, t;
            memcpy(&s, &glcommands[i++], sizeof s);
            memcpy(&t, &glcommands[i++], sizeof t);
            t = 1.0f - t; // flip the texture vertically
            const int index = glcommands[i++];
            glTexCoord2f(s, t);
            glNormal(frames[curFrame].vert[index].n);
            glVertex(frames[curFrame].vert[index].v);
        }
        glEnd();
        val = glcommands[i++];
    }
}

void CIMd2::DrawTimedFrame(float curtime)
{
    if (triangles.empty())
        return;
    const float delta = curtime - CurTime;
    CurTime = curtime;
    CurFrame += delta * FPS;
    while (CurFrame > EndFrame + 1)
        CurFrame = CurFrame - (1 + EndFrame - StartFrame);
    if (CurFrame < StartFrame)
        CurFrame = float(StartFrame);
    int next = int(floor(CurFrame + 1));
    if (next > EndFrame)
        next = StartFrame;
    DrawFrame(int(floor(CurFrame)), next, float(fmod(CurFrame, 1)));
}

void CIMd2::LoadTextures()
{
    // Quake skins are PCX files, which Qt cannot read: 3dPang ships them as PNG
    QString texfilename = QString::fromStdString(tex_name);
    if (texfilename.endsWith(QLatin1String(".pcx"), Qt::CaseInsensitive))
        texfilename = texfilename.left(texfilename.size() - 4) + QLatin1String(".png");
    QImage tt(pang::paths::resolve(texfilename));
    if (tt.isNull()) {
        // Fall back to a white texture so that the model is still visible
        tt = QImage(2, 2, QImage::Format_RGBA8888);
        tt.fill(Qt::white);
    }
    glGenTextures(1, &ti);
    glBindTexture(GL_TEXTURE_2D, ti);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    pang::gl::uploadImage(GL_TEXTURE_2D, pang::gl::toGlImage(tt));
}

void CIMd2::DrawFrame(int curFrame, int nextFrame, float pol)
{
    if (triangles.empty() || curFrame < 0 || nextFrame < 0 || curFrame >= int(frames.size()) ||
        nextFrame >= int(frames.size()))
        return;
    if (ti == 0)
        LoadTextures();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ti);

    int i = 0;
    const float pol1 = 1 - pol;
    int val = glcommands[i++];
    glColor3f(1.f, 1.f, 1.f);

    while (val != 0) {
        int count;
        if (val > 0) {
            glBegin(GL_TRIANGLE_STRIP);
            count = val;
        } else {
            glBegin(GL_TRIANGLE_FAN);
            count = -val;
        }
        while (count--) {
            float s, t;
            memcpy(&s, &glcommands[i++], sizeof s);
            memcpy(&t, &glcommands[i++], sizeof t);
            t = 1.0f - t; // flip the texture vertically
            const int index = glcommands[i++];
            glTexCoord2f(s, t);
            glNormal(frames[curFrame].vert[index].n * pol1 + frames[nextFrame].vert[index].n * pol);
            glVertex(frames[curFrame].vert[index].v * pol1 + frames[nextFrame].vert[index].v * pol);
        }
        glEnd();
        val = glcommands[i++];
    }
}
