#include "TextWriter.h"

#include "DataPaths.h"

namespace pang {

TextWriter::TextWriter() : fontFile(QStringLiteral("fonts/verdanab.bmf")) {}

TextWriter::~TextWriter()
{
    // The OpenGL context may already be gone at exit: just forget the font.
    // BMF_Free() would call glDeleteTextures without a context.
    font = nullptr;
}

void TextWriter::init()
{
    if (font == nullptr)
        font = BMF_Load(paths::native(paths::resolve(fontFile)).constData());
    if (font == nullptr)
        qWarning("3dPang: cannot load font '%s'", qPrintable(fontFile));
}

void TextWriter::setFont(const QString& dataRelativePath)
{
    fontFile = dataRelativePath;
    if (font != nullptr)
        BMF_Free(font);
    font = nullptr;
    init();
}

void TextWriter::prepare(float red, float green, float blue, float alpha, int positionX, int positionY,
                         float rotationAngle, float sx, float sy)
{
    r = red;
    g = green;
    b = blue;
    a = alpha;
    posX = positionX;
    posY = positionY;
    angle = rotationAngle;
    scaleX = sx;
    scaleY = sy;
}

void TextWriter::enter2D()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, videoWidth, 0, videoHeight + 10, -1, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void TextWriter::exit2D()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void TextWriter::write(const QString& text)
{
    if (font == nullptr)
        init();
    if (font == nullptr)
        return;
    enter2D();
    BMF_Begin(font);
    BMF_SetScale(scaleX, scaleY);
    BMF_SetRotation(angle);
    if (vCentered)
        BMF_SetVAlign(BMF_ALIGN_CENTER);
    if (hCentered)
        BMF_SetHAlign(BMF_ALIGN_CENTER);
    BMF_SetColor(r, g, b, a);
    // Never start writing outside the window
    if (posX > videoWidth || posX < 0)
        posX = 0;
    if (posY > videoHeight || posY < 0)
        posY = 0;
    BMF_SetPos(float(posX), float(posY));
    // BMF_Print is printf-like: never pass user text as the format
    BMF_Print("%s", text.toLatin1().constData());
    BMF_End();
    exit2D();
}

} // namespace pang
