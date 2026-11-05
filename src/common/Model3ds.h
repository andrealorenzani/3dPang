#pragma once
// Minimal Autodesk 3D Studio (.3ds) loader and renderer.
//
// It replaces lib3ds 1.2 (and the CI3ds wrapper from the course library),
// which is no longer maintained. Only what the bonus models need is read:
// meshes, texture coordinates, smoothing groups and material colours.
// Meshes are drawn as stored in the file, without keyframer animation.

#include "GlInclude.h"

#include <QString>

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace pang {

class Model3ds {
public:
    Model3ds() = default;
    ~Model3ds();
    Model3ds(const Model3ds&) = delete;
    Model3ds& operator=(const Model3ds&) = delete;

    // Loads a .3ds file given a data-relative or absolute path.
    bool open(const QString& fileName);
    // Draws the model (the first call compiles a display list).
    void draw();
    // Radius of the bounding sphere centred at the origin
    float radius() const { return boundingRadius; }

private:
    struct Material {
        std::string name;
        std::array<float, 4> ambient{0.2f, 0.2f, 0.2f, 1.0f};
        std::array<float, 4> diffuse{0.8f, 0.8f, 0.8f, 1.0f};
        std::array<float, 4> specular{0.0f, 0.0f, 0.0f, 1.0f};
        float shininess = 0.0f;  // 0..1 as in lib3ds
    };
    struct Face {
        std::uint16_t v[3];
        std::uint32_t smoothing = 0;
        int material = -1;
    };
    struct Mesh {
        std::string name;
        std::vector<std::array<float, 3>> points;
        std::vector<std::array<float, 2>> texels;
        std::vector<Face> faces;
    };

    bool parse(const std::vector<std::uint8_t>& data);
    void computeNormals(const Mesh& mesh, std::vector<std::array<float, 3>>& normals) const;
    void render();

    std::vector<Material> materials;
    std::vector<Mesh> meshes;
    float boundingRadius = 0.0f;
    GLuint displayList = 0;
};

} // namespace pang
