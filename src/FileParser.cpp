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
    // Validate the vertex index exists
    if (vi.v_idx > 0 && vi.v_idx <= temp_positions.size()) {
      parsedIndices.push_back(vi);
    } else {
      std::cerr << "Warning: Invalid vertex index " << vi.v_idx << " in face" << std::endl;
    }
  }

  if (parsedIndices.size() < 3) {
    std::cerr << "Warning: Face with less than 3 vertices" << std::endl;
    return;
  }

  // Triangulate the face (handles both triangles and quads)
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

// Helper struct for Map lookups
struct VertexIndexComparator {
  bool operator()(const VertexIndex &a, const VertexIndex &b) const {
    if (a.v_idx != b.v_idx)
      return a.v_idx < b.v_idx;
    if (a.vn_idx != b.vn_idx)
      return a.vn_idx < b.vn_idx;
    return a.vt_idx < b.vt_idx;
  }
};

void FileParser::parse_OBJ(const char *filePath) {
  std::ifstream file_stream(filePath);
  if (!file_stream.is_open()) {
    throw std::runtime_error("Failed to open file stream, check file path");
  }

  // Clear previous data
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

  // Clear previous data
  VulkanApplication::vertices.clear();
  VulkanApplication::indices.clear();

  // Deduplication map
  std::map<VertexIndex, uint32_t, VertexIndexComparator> uniqueVertices;

  for (const auto &tri : FileParser::allTriangles) {
    // 1. Calculate geometric normal for the face (fallback if no normals in file)
    glm::vec3 p0 = temp_positions[tri.vertices[0].v_idx - 1];
    glm::vec3 p1 = temp_positions[tri.vertices[1].v_idx - 1];
    glm::vec3 p2 = temp_positions[tri.vertices[2].v_idx - 1];

    glm::vec3 edge1 = p1 - p0;
    glm::vec3 edge2 = p2 - p0;
    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

    for (int i = 0; i < 3; ++i) {
      VertexIndex vi = tri.vertices[i];
      Vertex vertex{};

      // Position
      vertex.pos = temp_positions[vi.v_idx - 1];
      vertex.color = {1.0f, 1.0f, 1.0f};

      // Normal
      if (vi.vn_idx > 0 && vi.vn_idx <= temp_normals.size()) {
        vertex.normal = temp_normals[vi.vn_idx - 1];

        // Use map for deduplication only if we have explicit normals (smooth shading)
        if (uniqueVertices.count(vi) == 0) {
          uniqueVertices[vi] = static_cast<uint32_t>(VulkanApplication::vertices.size());
          VulkanApplication::vertices.push_back(vertex);
        }
        VulkanApplication::indices.push_back(uniqueVertices[vi]);

      } else {
        // No normal in file, use calculated face normal (Flat shading)
        vertex.normal = faceNormal;

        // Cannot deduplicate effectively for flat shading without complex logic
        // (vertices at same position need different normals for different faces).
        VulkanApplication::indices.push_back(static_cast<uint32_t>(VulkanApplication::vertices.size()));
        VulkanApplication::vertices.push_back(vertex);
      }
    }
  }

  std::cout << "Final: " << VulkanApplication::vertices.size() << " vertices, "
            << VulkanApplication::indices.size() << " indices" << std::endl;

  auto app = VulkanApplication::getInstance();
}
