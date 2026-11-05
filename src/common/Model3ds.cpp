#include "Model3ds.h"

#include "DataPaths.h"

#include <QFile>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <map>

namespace pang {

namespace {

// Chunk identifiers of the 3DS format
enum : std::uint16_t {
    MAIN3DS = 0x4D4D,
    EDIT3DS = 0x3D3D,
    EDIT_OBJECT = 0x4000,
    OBJ_TRIMESH = 0x4100,
    TRI_VERTEXL = 0x4110,
    TRI_FACEL1 = 0x4120,
    TRI_MATERIAL = 0x4130,
    TRI_MAPPINGCOORS = 0x4140,
    TRI_SMOOTH = 0x4150,
    EDIT_MATERIAL = 0xAFFF,
    MAT_NAME = 0xA000,
    MAT_AMBIENT = 0xA010,
    MAT_DIFFUSE = 0xA020,
    MAT_SPECULAR = 0xA030,
    MAT_SHININESS = 0xA040,
    COLOR_F = 0x0010,
    COLOR_24 = 0x0011,
    LIN_COLOR_24 = 0x0012,
    LIN_COLOR_F = 0x0013,
    INT_PERCENTAGE = 0x0030,
    FLOAT_PERCENTAGE = 0x0031,
};

class Reader {
public:
    Reader(const std::uint8_t* data, size_t size) : p(data), end(data + size) {}
    bool has(size_t n) const { return size_t(end - p) >= n; }
    std::uint16_t u16()
    {
        if (!has(2)) { p = end; return 0; }
        const std::uint16_t v = std::uint16_t(p[0] | (p[1] << 8));
        p += 2;
        return v;
    }
    std::uint32_t u32()
    {
        if (!has(4)) { p = end; return 0; }
        const std::uint32_t v = std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
                                (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
        p += 4;
        return v;
    }
    float f32()
    {
        const std::uint32_t bits = u32();
        float f;
        std::memcpy(&f, &bits, sizeof f);
        return f;
    }
    std::uint8_t u8()
    {
        if (!has(1)) { p = end; return 0; }
        return *p++;
    }
    std::string cstring()
    {
        std::string s;
        while (p < end && *p != 0)
            s.push_back(char(*p++));
        if (p < end)
            ++p;
        return s;
    }
    const std::uint8_t* pos() const { return p; }
    const std::uint8_t* limit() const { return end; }
    void seek(const std::uint8_t* to) { p = std::min(to, end); }

private:
    const std::uint8_t* p;
    const std::uint8_t* end;
};

// Reads a colour sub-chunk (float or 24 bit, the first one found).
void readColor(Reader& r, const std::uint8_t* chunkEnd, std::array<float, 4>& out)
{
    bool done = false;
    while (r.pos() + 6 <= chunkEnd) {
        const auto* start = r.pos();
        const std::uint16_t id = r.u16();
        const std::uint32_t len = r.u32();
        const auto* next = start + len;
        if (len < 6 || next > chunkEnd)
            break;
        if (!done && (id == COLOR_F || id == LIN_COLOR_F)) {
            out = {r.f32(), r.f32(), r.f32(), 1.0f};
            done = true;
        } else if (!done && (id == COLOR_24 || id == LIN_COLOR_24)) {
            out = {r.u8() / 255.0f, r.u8() / 255.0f, r.u8() / 255.0f, 1.0f};
            done = true;
        }
        r.seek(next);
    }
}

float readPercentage(Reader& r, const std::uint8_t* chunkEnd)
{
    float value = 0.0f;
    while (r.pos() + 6 <= chunkEnd) {
        const auto* start = r.pos();
        const std::uint16_t id = r.u16();
        const std::uint32_t len = r.u32();
        const auto* next = start + len;
        if (len < 6 || next > chunkEnd)
            break;
        if (id == INT_PERCENTAGE)
            value = std::int16_t(r.u16()) / 100.0f;
        else if (id == FLOAT_PERCENTAGE)
            value = r.f32() / 100.0f;
        r.seek(next);
    }
    return value;
}

std::array<float, 3> sub(const std::array<float, 3>& a, const std::array<float, 3>& b)
{
    return {a[0] - b[0], a[1] - b[1], a[2] - b[2]};
}

std::array<float, 3> cross(const std::array<float, 3>& a, const std::array<float, 3>& b)
{
    return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
}

void normalize(std::array<float, 3>& v)
{
    const float l = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (l > 0.0f)
        for (float& c : v)
            c /= l;
}

} // namespace

Model3ds::~Model3ds()
{
    // Display lists belong to the GL context, which may be gone at exit.
    displayList = 0;
}

bool Model3ds::open(const QString& fileName)
{
    QFile file(paths::resolve(fileName));
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("3dPang: cannot open 3DS model '%s'", qPrintable(fileName));
        return false;
    }
    const QByteArray bytes = file.readAll();
    std::vector<std::uint8_t> data(bytes.begin(), bytes.end());
    materials.clear();
    meshes.clear();
    displayList = 0;
    if (!parse(data)) {
        qWarning("3dPang: '%s' is not a valid 3DS file", qPrintable(fileName));
        return false;
    }
    boundingRadius = 0.0f;
    for (const Mesh& m : meshes)
        for (const auto& p : m.points)
            boundingRadius = std::max(boundingRadius, std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]));
    return true;
}

bool Model3ds::parse(const std::vector<std::uint8_t>& data)
{
    Reader r(data.data(), data.size());
    if (r.u16() != MAIN3DS)
        return false;
    const std::uint32_t mainLen = r.u32();
    const std::uint8_t* mainEnd = data.data() + std::min<size_t>(mainLen, data.size());

    std::map<std::string, int> materialIndex;
    struct PendingMaterial {
        size_t mesh;
        std::string name;
        std::vector<std::uint16_t> faces;
    };
    std::vector<PendingMaterial> pending;

    // Walks the chunk tree iteratively: container chunks are entered,
    // leaf chunks are decoded, everything else is skipped.
    std::vector<const std::uint8_t*> ends{mainEnd};
    Mesh* mesh = nullptr;
    Material* material = nullptr;
    while (!ends.empty()) {
        if (r.pos() + 6 > ends.back()) {
            r.seek(ends.back());
            ends.pop_back();
            continue;
        }
        const auto* start = r.pos();
        const std::uint16_t id = r.u16();
        const std::uint32_t len = r.u32();
        const std::uint8_t* next = start + len;
        if (len < 6 || next > ends.back())
            return !meshes.empty();

        switch (id) {
        case EDIT3DS:
        case OBJ_TRIMESH:
            ends.push_back(next);
            continue;
        case EDIT_OBJECT:
            meshes.push_back(Mesh{});
            mesh = &meshes.back();
            mesh->name = r.cstring();
            ends.push_back(next);
            continue;
        case TRI_VERTEXL:
            if (mesh != nullptr) {
                const std::uint16_t n = r.u16();
                mesh->points.resize(n);
                for (auto& p : mesh->points)
                    p = {r.f32(), r.f32(), r.f32()};
            }
            break;
        case TRI_MAPPINGCOORS:
            if (mesh != nullptr) {
                const std::uint16_t n = r.u16();
                mesh->texels.resize(n);
                for (auto& t : mesh->texels)
                    t = {r.f32(), r.f32()};
            }
            break;
        case TRI_FACEL1:
            if (mesh != nullptr) {
                const std::uint16_t n = r.u16();
                mesh->faces.resize(n);
                for (Face& f : mesh->faces) {
                    f.v[0] = r.u16();
                    f.v[1] = r.u16();
                    f.v[2] = r.u16();
                    r.u16();  // edge flags
                }
                // Sub-chunks: face materials and smoothing groups
                while (r.pos() + 6 <= next) {
                    const auto* s = r.pos();
                    const std::uint16_t sid = r.u16();
                    const std::uint32_t slen = r.u32();
                    const auto* snext = s + slen;
                    if (slen < 6 || snext > next)
                        break;
                    if (sid == TRI_MATERIAL) {
                        PendingMaterial pm{size_t(mesh - meshes.data()), r.cstring(), {}};
                        const std::uint16_t count = r.u16();
                        for (std::uint16_t i = 0; i < count; ++i)
                            pm.faces.push_back(r.u16());
                        pending.push_back(std::move(pm));
                    } else if (sid == TRI_SMOOTH) {
                        for (Face& f : mesh->faces)
                            f.smoothing = r.u32();
                    }
                    r.seek(snext);
                }
            }
            break;
        case EDIT_MATERIAL:
            materials.push_back(Material{});
            material = &materials.back();
            mesh = nullptr;
            ends.push_back(next);
            continue;
        case MAT_NAME:
            if (material != nullptr) {
                material->name = r.cstring();
                materialIndex[material->name] = int(materials.size()) - 1;
            }
            break;
        case MAT_AMBIENT:
            if (material != nullptr)
                readColor(r, next, material->ambient);
            break;
        case MAT_DIFFUSE:
            if (material != nullptr)
                readColor(r, next, material->diffuse);
            break;
        case MAT_SPECULAR:
            if (material != nullptr)
                readColor(r, next, material->specular);
            break;
        case MAT_SHININESS:
            if (material != nullptr)
                material->shininess = readPercentage(r, next);
            break;
        default:
            break;
        }
        r.seek(next);
    }

    for (const PendingMaterial& pm : pending) {
        const auto it = materialIndex.find(pm.name);
        if (it == materialIndex.end())
            continue;
        Mesh& m = meshes[pm.mesh];
        for (std::uint16_t f : pm.faces)
            if (f < m.faces.size())
                m.faces[f].material = it->second;
    }
    // Drop degenerate meshes (cameras, lights, empty objects)
    meshes.erase(std::remove_if(meshes.begin(), meshes.end(),
                                [](const Mesh& m) { return m.points.empty() || m.faces.empty(); }),
                 meshes.end());
    return !meshes.empty();
}

void Model3ds::computeNormals(const Mesh& mesh, std::vector<std::array<float, 3>>& normals) const
{
    // Per-corner normals: the average of the normals of the faces sharing
    // the vertex and at least one smoothing group (as lib3ds did).
    std::vector<std::array<float, 3>> faceNormals(mesh.faces.size());
    std::vector<std::vector<size_t>> facesOfVertex(mesh.points.size());
    for (size_t i = 0; i < mesh.faces.size(); ++i) {
        const Face& f = mesh.faces[i];
        bool valid = true;
        for (int k = 0; k < 3; ++k)
            valid = valid && f.v[k] < mesh.points.size();
        if (!valid) {
            faceNormals[i] = {0, 0, 1};
            continue;
        }
        auto n = cross(sub(mesh.points[f.v[1]], mesh.points[f.v[0]]), sub(mesh.points[f.v[2]], mesh.points[f.v[0]]));
        normalize(n);
        faceNormals[i] = n;
        for (int k = 0; k < 3; ++k)
            facesOfVertex[f.v[k]].push_back(i);
    }
    normals.assign(mesh.faces.size() * 3, {0, 0, 1});
    for (size_t i = 0; i < mesh.faces.size(); ++i) {
        const Face& f = mesh.faces[i];
        for (int k = 0; k < 3; ++k) {
            if (f.v[k] >= mesh.points.size())
                continue;
            if (f.smoothing == 0) {
                normals[i * 3 + k] = faceNormals[i];
                continue;
            }
            std::array<float, 3> n{0, 0, 0};
            for (size_t other : facesOfVertex[f.v[k]]) {
                if ((mesh.faces[other].smoothing & f.smoothing) == 0)
                    continue;
                for (int c = 0; c < 3; ++c)
                    n[c] += faceNormals[other][c];
            }
            normalize(n);
            normals[i * 3 + k] = n;
        }
    }
}

void Model3ds::render()
{
    static const Material defaultMaterial;
    for (const Mesh& mesh : meshes) {
        std::vector<std::array<float, 3>> normals;
        computeNormals(mesh, normals);
        const bool hasTexels = mesh.texels.size() == mesh.points.size();
        int current = -2;
        for (size_t i = 0; i < mesh.faces.size(); ++i) {
            const Face& f = mesh.faces[i];
            if (f.material != current) {
                current = f.material;
                const Material& m = (current >= 0) ? materials[size_t(current)] : defaultMaterial;
                static const GLfloat black[4] = {0, 0, 0, 1};
                glMaterialfv(GL_FRONT, GL_AMBIENT, current >= 0 ? black : m.ambient.data());
                glMaterialfv(GL_FRONT, GL_DIFFUSE, m.diffuse.data());
                glMaterialfv(GL_FRONT, GL_SPECULAR, m.specular.data());
                const float s = std::min(128.0f, float(std::pow(2.0, 10.0 * m.shininess)));
                glMaterialf(GL_FRONT, GL_SHININESS, current >= 0 ? s : 0.0f);
            }
            glBegin(GL_TRIANGLES);
            for (int k = 0; k < 3; ++k) {
                if (f.v[k] >= mesh.points.size())
                    continue;
                glNormal3fv(normals[i * 3 + k].data());
                if (hasTexels)
                    glTexCoord2fv(mesh.texels[f.v[k]].data());
                glVertex3fv(mesh.points[f.v[k]].data());
            }
            glEnd();
        }
    }
}

void Model3ds::draw()
{
    if (meshes.empty())
        return;
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glDisable(GL_COLOR_MATERIAL);
    if (displayList == 0) {
        displayList = glGenLists(1);
        glNewList(displayList, GL_COMPILE);
        render();
        glEndList();
    }
    glCallList(displayList);
    glPopAttrib();
}

} // namespace pang
