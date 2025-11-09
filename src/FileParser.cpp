#include "FileParser.h"
#include "VulkanApplication.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

// Add Vertex method implementations
VkVertexInputBindingDescription Vertex::getBindingDescription() {
  VkVertexInputBindingDescription bindingDescription{};
  bindingDescription.binding = 0;
  bindingDescription.stride = sizeof(Vertex);
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 2> Vertex::getAttributeDescriptions() {
  std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(Vertex, pos);

  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(Vertex, color);

  return attributeDescriptions;
}

// Rest of your existing FileParser.cpp code...
std::vector<Triangle> FileParser::allTriangles;

const std::unordered_map<std::string_view, ObjLineType> FileParser::prefixMap = {
    {"v", ObjLineType::VERTEX},
    {"vt", ObjLineType::TEXTURE_COORDINATE},
    {"vn", ObjLineType::VERTEX_NORMAL},
    {"f", ObjLineType::FACE},
    {"g", ObjLineType::GROUP},
    {"o", ObjLineType::OBJECT},
    {"mtllib", ObjLineType::MATERIAL_LIBRARY},
    {"usemtl", ObjLineType::USE_MATERIAL}};

static std::vector<glm::vec3> temp_positions;

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
    parsedIndices.push_back(parseVertexChunk(chunk));
  }

  if (parsedIndices.size() < 3) {
    return; // Invalid face
  }

  const VertexIndex &v0 = parsedIndices[0];
  for (size_t i = 1; i < parsedIndices.size() - 1; ++i) {
    Triangle tri;
    tri.vertices[0] = v0;
    tri.vertices[1] = parsedIndices[i];
    tri.vertices[2] = parsedIndices[i + 1];
    FileParser::allTriangles.push_back(tri);
  }
}

static void parse_texture_coord(std::string &line) {
  // TODO: Implement texture coordinate parsing
}

static void parse_normal(std::string &line) {
  // TODO: Implement normal parsing
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

  // Clear previous data
  FileParser::allTriangles.clear();
  temp_positions.clear();

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
    case ObjLineType::UNKNOWN:
      // Skip comments or unsupported lines
      break;
    }
  }

  // Clear and populate VulkanApplication's static members
  VulkanApplication::vertices.clear();
  VulkanApplication::indices.clear();

  for (const auto &tri : FileParser::allTriangles) {
    for (int i = 0; i < 3; ++i) {
      const auto &vertex_indices = tri.vertices[i];

      Vertex final_vertex;
      final_vertex.pos = temp_positions[vertex_indices.v_idx - 1];
      final_vertex.color = {1.0f, 1.0f, 1.0f}; // Default color

      VulkanApplication::vertices.push_back(final_vertex);
      VulkanApplication::indices.push_back(VulkanApplication::vertices.size() - 1);
    }
  }

  // Clean up temporary data
  FileParser::allTriangles.clear();
  temp_positions.clear();
}
