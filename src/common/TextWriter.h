#pragma once
// 2D text on top of the OpenGL scene, using the BMF bitmap fonts.
//
// Usage: call prepare() to set colour, position, rotation and scale,
// then write() the text.

#include "BMF_font.h"
#include "Definitions.h"

namespace pang {

class TextWriter {
public:
    TextWriter();
    ~TextWriter();
    TextWriter(const TextWriter&) = delete;
    TextWriter& operator=(const TextWriter&) = delete;

    // Loads the fonts. Needs a current OpenGL context; safe to call more than once.
    void init();
    void setFont(const QString& dataRelativePath);

    void setVideoSize(int width, int height)
    {
        videoWidth = width;
        videoHeight = height;
    }
    int getVideoWidth() const { return videoWidth; }
    int getVideoHeight() const { return videoHeight; }

    // Horizontal/vertical centring of the text around the position
    void setHCentered(bool centered) { hCentered = centered; }
    void setVCentered(bool centered) { vCentered = centered; }

    void prepare(float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f,
                 int positionX = 0, int positionY = 0, float rotationAngle = 0,
                 float scaleX = 1, float scaleY = 1);
    void write(const QString& text);

private:
    void enter2D();
    void exit2D();

    QString fontFile;
    BMF_Font* font = nullptr;
    bool hCentered = false;
    bool vCentered = false;
    float r = 1, g = 1, b = 1, a = 1;
    int posX = 0, posY = 0;
    float angle = 0;
    float scaleX = 1, scaleY = 1;
    int videoWidth = 640, videoHeight = 480;
};

} // namespace pang
