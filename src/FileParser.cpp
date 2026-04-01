#include "FileParser.h"
#include "VulkanApplication.h"
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

std::string FileParser::current_material = "";
std::string FileParser::current_group = "";
std::string FileParser::current_object = "";

VkVertexInputBindingDescription Vertex::getBindingDescription() {
  VkVertexInputBindingDescription bindingDescription{};
  bindingDescription.binding = 0;
  bindingDescription.stride = sizeof(Vertex);
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 4>
Vertex::getAttributeDescriptions() {
  std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

  // INFO: Position attribute
  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(Vertex, pos);

  // INFO: Color attribute
  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(Vertex, color);

  // INFO: Normal attribute
  attributeDescriptions[2].binding = 0;
  attributeDescriptions[2].location = 2;
  attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[2].offset = offsetof(Vertex, normal);

  // INFO: Texture coordinate attribute
  attributeDescriptions[3].binding = 0;
  attributeDescriptions[3].location = 3;
  attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[3].offset = offsetof(Vertex, texCoord);

  return attributeDescriptions;
}

std::map<std::string, std::vector<Triangle>> FileParser::groupedTriangles;
std::vector<SubMesh> FileParser::subMeshes;

struct VertexIndexHash {
  size_t operator()(const VertexIndex &v) const {
    size_t h = 0;
    auto hash_combine = [&h](size_t v_hash) {
      h ^= v_hash + 0x9e3779b9 + (h << 6) + (h >> 2);
    };
    hash_combine(std::hash<int>()(v.v_idx));
    hash_combine(std::hash<int>()(v.vt_idx));
    hash_combine(std::hash<int>()(v.vn_idx));
    hash_combine(std::hash<std::string>()(v.material_name));
    return h;
  }
};

struct VertexIndexEqual {
  bool operator()(const VertexIndex &a, const VertexIndex &b) const {
    return a.v_idx == b.v_idx && a.vt_idx == b.vt_idx && a.vn_idx == b.vn_idx &&
           a.material_name == b.material_name;
  }
};

const std::unordered_map<std::string_view, ObjLineType> FileParser::prefixMap =
    {{"v", ObjLineType::VERTEX},
     {"vt", ObjLineType::TEXTURE_COORDINATE},
     {"vn", ObjLineType::VERTEX_NORMAL},
     {"f", ObjLineType::FACE},
     {"g", ObjLineType::GROUP},
     {"o", ObjLineType::OBJECT},
     {"#", ObjLineType::COMMENT},
     {"mtllib", ObjLineType::MATERIAL_LIBRARY},
     {"usemtl", ObjLineType::USE_MATERIAL}};

static std::vector<glm::vec3> temp_positions;
static std::vector<glm::vec3> temp_normals;
static std::vector<glm::vec2> temp_texcoords;

std::unordered_map<std::string, Material> FileParser::materials;

static VertexIndex parseVertexChunk(const char **ptr) {
  VertexIndex result;
  char *end;

  result.v_idx = std::strtol(*ptr, &end, 10);
  *ptr = end;

  if (**ptr == '/') {
    (*ptr)++;
    if (**ptr != '/') {
      result.vt_idx = std::strtol(*ptr, &end, 10);
      *ptr = end;
    }
    if (**ptr == '/') {
      (*ptr)++;
      result.vn_idx = std::strtol(*ptr, &end, 10);
      *ptr = end;
    }
  }
  return result;
}

static void parse_vertex(const char *&cursor) {
  Vertex vert;
  char *end;

  vert.pos.x = std::strtof(cursor, &end);
  cursor = end;
  vert.pos.y = std::strtof(cursor, &end);
  cursor = end;
  vert.pos.z = std::strtof(cursor, &end);
  cursor = end;

  vert.color = {1.f, 1.f, 1.f};
  temp_positions.push_back(vert.pos);
}

void FileParser::parse_face(const char *&cursor) {
  std::vector<VertexIndex> parsedIndices;

  while (*cursor != '\n' && *cursor != '\r' && *cursor != '\0') {
    while (*cursor == ' ' || *cursor == '\t')
      cursor++;
    if (*cursor == '\n' || *cursor == '\r' || *cursor == '\0')
      break;

    VertexIndex vi = parseVertexChunk(&cursor);
    vi.material_name = current_material;

    if (vi.v_idx > 0) {
      parsedIndices.push_back(vi);
    }
  }

  if (parsedIndices.size() < 3)
    return;

  std::string active_mesh_name =
      current_group.empty()
          ? (current_object.empty() ? "default" : current_object)
          : current_group;

  for (size_t i = 1; i < parsedIndices.size() - 1; ++i) {
    Triangle tri;
    tri.vertices[0] = parsedIndices[0];
    tri.vertices[1] = parsedIndices[i];
    tri.vertices[2] = parsedIndices[i + 1];

    FileParser::groupedTriangles[active_mesh_name].push_back(tri);
  }
}

static void parse_texture_coord(const char *&cursor) {
  glm::vec2 texCoord(0.0f);
  char *end;

  texCoord.x = std::strtof(cursor, &end);
  cursor = end;
  texCoord.y = std::strtof(cursor, &end);
  cursor = end;

  temp_texcoords.push_back(texCoord);
}

static void parse_normal(const char *&cursor) {
  glm::vec3 normal;
  char *end;

  normal.x = std::strtof(cursor, &end);
  cursor = end;
  normal.y = std::strtof(cursor, &end);
  cursor = end;
  normal.z = std::strtof(cursor, &end);
  cursor = end;

  temp_normals.push_back(normal);
}

void FileParser::load_material_library(std::string &line,
                                       const std::string &base_dir) {
  std::istringstream iss(line);
  std::string token, filename;

  iss >> token >> filename;

  if (filename.empty()) {
    return;
  }

  std::string full_path = base_dir + '/' + filename;
  std::ifstream file(full_path);

  if (!file.is_open()) {
    throw std::runtime_error(
        std::format("File at {} coult not be opened", full_path));
  }

  std::string mtl_line;
  Material current_mtl;
  bool parsing_material = false;

  while (std::getline(file, mtl_line)) {
    std::istringstream mtl_iss(mtl_line);
    std::string type;

    mtl_iss >> type;

    // INFO: If were already parsing a material, save it before going to the
    // next one
    if (type == "newmtl") {
      if (parsing_material) {
        materials[current_mtl.name] = current_mtl;
      }

      current_mtl = Material();
      mtl_iss >> current_mtl.name;
      parsing_material = true;
    } else if (type == "Kd") {
      mtl_iss >> current_mtl.diffuse[0] >> current_mtl.diffuse[1] >>
          current_mtl.diffuse[2];
    } else if (type == "Ka") {
      mtl_iss >> current_mtl.ambient[0] >> current_mtl.ambient[1] >>
          current_mtl.ambient[2];
    } else if (type == "Ks") {
      mtl_iss >> current_mtl.specular[0] >> current_mtl.specular[1] >>
          current_mtl.specular[2];
    } else if (type == "Ns") {
      mtl_iss >> current_mtl.shininess;
    } else if (type == "map_Kd") {
      mtl_iss >> current_mtl.diffuse_map;
    }
  }

  if (parsing_material) {
    materials[current_mtl.name] = current_mtl;
  }

  std::cout << "Succesfully loaded " << materials.size() << " materials from "
            << filename << std::endl;
}

void FileParser::set_current_material(std::string &line) {
  std::istringstream iss(line);
  std::string token, material_name;

  iss >> token >> material_name;

  if (!material_name.empty()) {
    if (materials.find(material_name) != materials.end()) {
      current_material = material_name;
    } else {
      throw std::runtime_error(
          std::format("Warning: OBJ requested material ', but it was not found "
                      "in the MTL file.\n",
                      material_name));
      current_material = "default";
    }
  }
}

void FileParser::parse_group(std::string &line) {
  std::istringstream iss(line);
  std::string token, group_name;

  iss >> token >> group_name;

  FileParser::current_group = group_name.empty() ? "default" : group_name;
}

void FileParser::parse_object(std::string &line) {
  std::istringstream iss(line);
  std::string object_name, token;

  iss >> token >> object_name;

  current_object = object_name.empty() ? "default" : object_name;
}

void FileParser::readFileToBuffer(const char *filePath,
                                  std::vector<char> &buffer) {
  std::ifstream file_stream(filePath, std::ios::ate | std::ios::binary);
  if (!file_stream.is_open()) {
    throw std::runtime_error("Failed to open file stream, check file path");
  }
  size_t fileSize = (size_t)file_stream.tellg();
  buffer.resize(fileSize + 1);
  file_stream.seekg(0);
  file_stream.read(buffer.data(), fileSize);
  buffer[fileSize] = '\0';
}

void FileParser::parseBuffer(const std::vector<char> &buffer,
                             const std::string &base_dir) {
  size_t fileSize = buffer.size() - 1;

  temp_positions.reserve(fileSize / 50);
  temp_normals.reserve(fileSize / 50);
  temp_texcoords.reserve(fileSize / 50);
  VulkanApplication::vertices.reserve(fileSize / 20);
  VulkanApplication::indices.reserve(fileSize / 15);

  const char *cursor = buffer.data();

  while (*cursor != '\0') {
    while (*cursor == ' ' || *cursor == '\t')
      cursor++;

    if (*cursor == '\n' || *cursor == '\r' || *cursor == '#') {
      while (*cursor != '\n' && *cursor != '\0')
        cursor++;
      if (*cursor == '\n')
        cursor++;
      continue;
    }

    if (cursor[0] == 'v') {
      if (cursor[1] == ' ') {
        cursor += 2;
        parse_vertex(cursor);
      } else if (cursor[1] == 'n' && cursor[2] == ' ') {
        cursor += 3;
        parse_normal(cursor);
      } else if (cursor[1] == 't' && cursor[2] == ' ') {
        cursor += 3;
        parse_texture_coord(cursor);
      }
    } else if (cursor[0] == 'f' && cursor[1] == ' ') {
      cursor += 2;
      parse_face(cursor);
    } else {
      const char *lineEnd = strchr(cursor, '\n');
      if (!lineEnd)
        lineEnd = buffer.data() + fileSize;
      std::string line(cursor, lineEnd - cursor);

      if (strncmp(cursor, "mtllib", 6) == 0)
        load_material_library(line, base_dir);
      else if (strncmp(cursor, "usemtl", 6) == 0)
        set_current_material(line);
      else if (cursor[0] == 'g' && cursor[1] == ' ')
        parse_group(line);
      else if (cursor[0] == 'o' && cursor[1] == ' ')
        parse_object(line);

      cursor = lineEnd;
    }

    while (*cursor != '\n' && *cursor != '\0')
      cursor++;
    if (*cursor == '\n')
      cursor++;
  }
}

void FileParser::assembleMeshes() {
  std::unordered_map<VertexIndex, uint32_t, VertexIndexHash, VertexIndexEqual>
      uniqueVertices;

  for (const auto &[objectName, triangles] : FileParser::groupedTriangles) {
    SubMesh currentSubMesh;
    currentSubMesh.name = objectName;
    currentSubMesh.first_idx =
        static_cast<uint32_t>(VulkanApplication::indices.size());

    for (const auto &tri : triangles) {
      glm::vec3 p0 = temp_positions[tri.vertices[0].v_idx - 1];
      glm::vec3 p1 = temp_positions[tri.vertices[1].v_idx - 1];
      glm::vec3 p2 = temp_positions[tri.vertices[2].v_idx - 1];
      glm::vec3 edge1 = p1 - p0;
      glm::vec3 edge2 = p2 - p0;
      glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

      glm::vec3 faceColor = {1.0f, 1.0f, 1.0f};
      std::string mat_name = tri.vertices[0].material_name;
      if (materials.find(mat_name) != materials.end()) {
        const Material &mat = materials[mat_name];
        faceColor = glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
      }

      for (int i = 0; i < 3; ++i) {
        VertexIndex vi = tri.vertices[i];
        Vertex vertex{};

        vertex.color = faceColor;
        vertex.pos = temp_positions[vi.v_idx - 1];

        if (vi.vn_idx > 0 && vi.vn_idx <= temp_normals.size()) {
          vertex.normal = temp_normals[vi.vn_idx - 1];

          if (uniqueVertices.count(vi) == 0) {
            uniqueVertices[vi] =
                static_cast<uint32_t>(VulkanApplication::vertices.size());
            VulkanApplication::vertices.push_back(vertex);
          }
          VulkanApplication::indices.push_back(uniqueVertices[vi]);
        } else {
          vertex.normal = faceNormal;
          VulkanApplication::indices.push_back(
              static_cast<uint32_t>(VulkanApplication::vertices.size()));
          VulkanApplication::vertices.push_back(vertex);
        }
      }
    }
    currentSubMesh.idx_count =
        static_cast<uint32_t>(VulkanApplication::indices.size()) -
        currentSubMesh.first_idx;
    FileParser::subMeshes.push_back(currentSubMesh);
  }
}

void FileParser::parse_OBJ(const char *filePath) {
  std::string base_dir = std::filesystem::path(filePath).parent_path().string();

  std::vector<char> buffer;
  readFileToBuffer(filePath, buffer);

  FileParser::groupedTriangles.clear();
  FileParser::subMeshes.clear();
  temp_positions.clear();
  temp_normals.clear();
  temp_texcoords.clear();
  VulkanApplication::vertices.clear();
  VulkanApplication::indices.clear();
  current_group = "";
  current_object = "";

  parseBuffer(buffer, base_dir);
  assembleMeshes();
}
