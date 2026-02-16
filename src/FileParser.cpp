#include "FileParser.h"
#include "VulkanApplication.h"
#include <filesystem>
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

void FileParser::parse_face(std::string &line) {
  std::istringstream lineStream(line);
  std::string prefix;
  lineStream >> prefix;

  std::vector<VertexIndex> parsedIndices;
  std::string chunk;
  while (lineStream >> chunk) {
    VertexIndex vi = parseVertexChunk(chunk);
    vi.material_name = current_material;

    if (vi.v_idx > 0 && vi.v_idx <= temp_positions.size()) {
      parsedIndices.push_back(vi);
    } else {
      std::cerr << "Warning: Invalid vertex index " << vi.v_idx << " in face"
                << std::endl;
    }
  }

  if (parsedIndices.size() < 3) {
    std::cerr << "Warning: Face with less than 3 vertices" << std::endl;
    return;
  }

  for (size_t i = 1; i < parsedIndices.size() - 1; ++i) {
    Triangle tri;
    tri.vertices[0] = parsedIndices[0];
    tri.vertices[1] = parsedIndices[i];
    tri.vertices[2] = parsedIndices[i + 1];

    FileParser::groupedTriangles[current_object].push_back(tri);
  }
}

static void parse_texture_coord(std::string &line) {
  glm::vec2 texCoord(0.0f);
  std::istringstream iss(line);
  std::string prefix;

  iss >> prefix;

  if (!(iss >> texCoord.x >> texCoord.y)) {
    // TODO: do stuff
  }
  temp_texcoords.push_back(texCoord);
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
      // Diffuse color (RGB)
      mtl_iss >> current_mtl.diffuse[0] >> current_mtl.diffuse[1] >>
          current_mtl.diffuse[2];
    } else if (type == "Ka") {
      // Ambient color (RGB)
      mtl_iss >> current_mtl.ambient[0] >> current_mtl.ambient[1] >>
          current_mtl.ambient[2];
    } else if (type == "Ks") {
      // Specular color (RGB)
      mtl_iss >> current_mtl.specular[0] >> current_mtl.specular[1] >>
          current_mtl.specular[2];
    } else if (type == "Ns") {
      // Shininess
      mtl_iss >> current_mtl.shininess;
    } else if (type == "map_Kd") {
      // Diffuse texture map filename
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

static void parse_group(std::string &line) {
  // TODO: Implement group parsing
}

void FileParser::parse_object(std::string &line) {
  std::istringstream iss(line);
  std::string object_name, token;

  iss >> token >> object_name;

  current_object = object_name.empty() ? "default" : object_name;
}

void FileParser::parse_OBJ(const char *filePath) {
  std::ifstream file_stream(filePath);
  if (!file_stream.is_open()) {
    throw std::runtime_error("Failed to open file stream, check file path");
  }

  std::string base_dir = std::filesystem::path(filePath).parent_path().string();

  // Clear previous data
  FileParser::groupedTriangles.clear();
  FileParser::subMeshes.clear();
  temp_positions.clear();
  temp_normals.clear();
  temp_texcoords.clear();
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
      load_material_library(line, base_dir);
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

  std::map<VertexIndex, uint32_t, VertexIndexComparator> uniqueVertices;

  for (const auto &[objectName, triangles] : FileParser::groupedTriangles) {

    SubMesh currentSubMesh;
    currentSubMesh.name = objectName;
    // INFO: Record where this object begins in the master index buffer
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

        vertex.pos = temp_positions[vi.v_idx - 1];
        vertex.color = faceColor;

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

    std::cout << "Parsed SubMesh: " << currentSubMesh.name
              << " (Indices: " << currentSubMesh.idx_count << ")\n";
  }

  std::cout << "Final: " << VulkanApplication::vertices.size() << " vertices, "
            << VulkanApplication::indices.size() << " indices" << std::endl;

  std::cout << std::format("Final count: {} vertices, {} indices",
                           VulkanApplication::vertices.size(),
                           VulkanApplication::indices.size());

  auto app = VulkanApplication::getInstance();
}
