#include "FileParser.h"
#include "VulkanApplication.h"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

VkVertexInputBindingDescription Vertex::getBindingDescription() {
  VkVertexInputBindingDescription bindingDescription{};
  bindingDescription.binding = 0;
  bindingDescription.stride = sizeof(Vertex);
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 4> Vertex::getAttributeDescriptions() {
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

std::vector<Triangle> FileParser::allTriangles;

const std::unordered_map<std::string_view, ObjLineType> FileParser::prefixMap = {
    {"v", ObjLineType::VERTEX},
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

static VertexIndex parseVertexChunk(const std::string &chunk) {
  VertexIndex result;
  std::istringstream chunkStream(chunk);
  std::string part;

  if (std::getline(chunkStream, part, '/')) {
    if (!part.empty())
      result.v_idx = std::stoi(part);
  }

  if (std::getline(chunkStream, part, '/')) {
    if (!part.empty())
      result.vt_idx = std::stoi(part);
  }

  if (std::getline(chunkStream, part)) {
    if (!part.empty())
      result.vn_idx = std::stoi(part);
  }

  return result;
}

static void parse_vertex(std::string &line) {
  Vertex vert;
  std::istringstream stream(line);
  std::string prefix;

  stream >> prefix;

  if (!(stream >> vert.pos.x >> vert.pos.y >> vert.pos.z)) {
    throw std::runtime_error("Error parsing vertex data");
    return;
  }
  vert.color = {1.f, 1.f, 1.f};
  temp_positions.push_back(vert.pos);
}

static void parse_face(std::string &line) {
  std::istringstream lineStream(line);
  std::string prefix;
  lineStream >> prefix;

  std::vector<VertexIndex> parsedIndices;
  std::string chunk;
  while (lineStream >> chunk) {
    VertexIndex vi = parseVertexChunk(chunk);
    if (vi.v_idx > 0 && vi.v_idx <= temp_positions.size()) {
      parsedIndices.push_back(vi);
    } else {
      std::cerr << "WARNING: Invalid vertex index " << vi.v_idx << " in face" << std::endl;
    }
  }

  if (parsedIndices.size() < 3) {
    std::cerr << "WARNING: Face with less than 3 vertices" << std::endl;
    return;
  }

  for (size_t i = 1; i < parsedIndices.size() - 1; ++i) {
    Triangle tri;
    tri.vertices[0] = parsedIndices[0];
    tri.vertices[1] = parsedIndices[i];
    tri.vertices[2] = parsedIndices[i + 1];
    FileParser::allTriangles.push_back(tri);
  }
}

static void parse_texture_coord(std::string &line) {
  // TODO: Implement texture coordinate parsing
}

static void parse_normal(std::string &line) {
  glm::vec3 normal;
  std::istringstream stream(line);
  std::string prefix;

  stream >> prefix;

  if (!(stream >> normal.x >> normal.y >> normal.z)) {
    throw std::runtime_error("Error parsing normal data");
    return;
  }
  temp_normals.push_back(normal);
}

static void load_material_library(std::string &line) {
  // TODO: Implement material library loading
}

static void set_current_material(std::string &line) {
  // TODO: Implement material setting
}

static void parse_group(std::string &line) {
  // TODO: Implement group parsing
}

static void parse_object(std::string &line) {
  // TODO: Implement object parsing
}

void FileParser::parse_OBJ(const char *filePath) {
  std::ifstream file_stream(filePath);
  if (!file_stream.is_open()) {
    throw std::runtime_error("Failed to open file stream, check file path");
  }

  FileParser::allTriangles.clear();
  temp_positions.clear();
  temp_normals.clear();
  VulkanApplication::vertices.clear();
  VulkanApplication::indices.clear();

  std::string line;
  while (std::getline(file_stream, line)) {
    if (line.empty())
      continue;

    ObjLineType line_type = FileParser::getLineType(line);

    switch (line_type) {
    case ObjLineType::VERTEX:
      parse_vertex(line);
      break;
    case ObjLineType::FACE:
      parse_face(line);
      break;
    case ObjLineType::TEXTURE_COORDINATE:
      parse_texture_coord(line);
      break;
    case ObjLineType::VERTEX_NORMAL:
      parse_normal(line);
      break;
    case ObjLineType::MATERIAL_LIBRARY:
      load_material_library(line);
      break;
    case ObjLineType::USE_MATERIAL:
      set_current_material(line);
      break;
    case ObjLineType::GROUP:
      parse_group(line);
      break;
    case ObjLineType::OBJECT:
      parse_object(line);
      break;
    case ObjLineType::COMMENT:
      break;
    case ObjLineType::UNKNOWN:
      std::cout << "Unknown line type encountered" << std::endl;
      break;
    }
  }

  VulkanApplication::vertices.clear();
  VulkanApplication::indices.clear();

  struct VertexIndexComparator {
    bool operator()(const VertexIndex &a, const VertexIndex &b) const {
      if (a.v_idx != b.v_idx)
        return a.v_idx < b.v_idx;
      if (a.vn_idx != b.vn_idx)
        return a.vn_idx < b.vn_idx;
      return a.vt_idx < b.vt_idx;
    }
  };

  std::map<VertexIndex, uint32_t, VertexIndexComparator> uniqueVertices;

  for (const auto &tri : FileParser::allTriangles) {
    for (int i = 0; i < 3; ++i) {
      VertexIndex vi = tri.vertices[i];

      if (uniqueVertices.find(vi) == uniqueVertices.end()) {
        Vertex vertex{};

        int p_idx = vi.v_idx - 1;
        if (p_idx >= 0 && p_idx < temp_positions.size()) {
          vertex.pos = temp_positions[p_idx];
        }

        int n_idx = vi.vn_idx - 1;
        if (n_idx >= 0 && n_idx < temp_normals.size()) {
          vertex.normal = temp_normals[n_idx];
        } else {
          vertex.normal = {0.0f, 1.0f, 0.0f};
        }

        vertex.color = {1.0f, 1.0f, 1.0f};
        vertex.texCoord = {0.0f, 0.0f, 0.0f};

        uniqueVertices[vi] = static_cast<uint32_t>(VulkanApplication::vertices.size());
        VulkanApplication::vertices.push_back(vertex);
      }

      VulkanApplication::indices.push_back(uniqueVertices[vi]);
    }
  }

  std::cout << std::format("Final: {} vertices, {} indices loaded", VulkanApplication::vertices.size(), VulkanApplication::indices.size()) << std::endl;

  auto app = VulkanApplication::getInstance();
}
