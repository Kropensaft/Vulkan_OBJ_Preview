#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include "VulkanApplication.h"
#include <string_view>
#include <unordered_map>
#include <vector>

struct Triangle {
  VertexIndex vertices[3];
};

enum class ObjLineType {
  VERTEX,
  TEXTURE_COORDINATE,
  VERTEX_NORMAL,
  FACE,
  GROUP,
  OBJECT,
  MATERIAL_LIBRARY,
  USE_MATERIAL,
  UNKNOWN,
};

constexpr double SCALE = 0.1;

class FileParser {
public:
  void read_OBJ();
  static ObjLineType getLineType(std::string_view line) {
    if (line.empty() || line[0] == '#')
      return ObjLineType::UNKNOWN;

    auto spacePos = line.find(' ');
    if (spacePos == std::string_view::npos)
      return ObjLineType::UNKNOWN;

    std::string_view prefix = line.substr(0, spacePos);
    auto it = prefixMap.find(prefix);
    return (it != prefixMap.end()) ? it->second : ObjLineType::UNKNOWN;
  }

  void parse_OBJ(const std::string &file_path);

  FileParser() {};
  ~FileParser() {}

private:
  std::vector<Triangle> allTriangles;
  static const std::unordered_map<std::string_view, ObjLineType> prefixMap;
};

#endif // !FILE_PARSER_H
