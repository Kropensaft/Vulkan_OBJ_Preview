#include "FileParser.h"
#include "VulkanApplication.h"
#include <gtest/gtest.h>

class FileParserTest : public ::testing::Test {
protected:
  void SetUp() override {
    FileParser::groupedTriangles.clear();
    FileParser::subMeshes.clear();
    VulkanApplication::vertices.clear();
    VulkanApplication::indices.clear();
  }

  void TearDown() override {}
  static void callParseBuffer(const std::vector<char> &buffer,
                              const std::string &base_dir) {
    FileParser::parseBuffer(buffer, base_dir);
  }
};

TEST_F(FileParserTest, ParseBufferExtractsVerticesAndFacesCorrectly) {
  std::string mockObjData = "v 1.0 1.0 1.0\n"
                            "v -1.0 -1.0 -1.0\n"
                            "v 1.0 -1.0 1.0\n"
                            "f 1 2 3\n";

  std::vector<char> buffer(mockObjData.begin(), mockObjData.end());
  buffer.push_back('\0');

  callParseBuffer(buffer, "dummy_dir");

  auto &triangles = FileParser::groupedTriangles;

  EXPECT_FALSE(triangles.empty());
  ASSERT_TRUE(triangles.find("default") != triangles.end());
  EXPECT_EQ(triangles["default"].size(), 1); // 1 face (Triangle)

  auto &tri = triangles["default"][0];
  EXPECT_EQ(tri.vertices[0].v_idx, 1);
  EXPECT_EQ(tri.vertices[1].v_idx, 2);
  EXPECT_EQ(tri.vertices[2].v_idx, 3);
}

TEST_F(FileParserTest, IdentifyLineTypesCorrectly) {
  EXPECT_EQ(FileParser::getLineType("v 1.0 2.0"), ObjLineType::VERTEX);
  EXPECT_EQ(FileParser::getLineType("vn 0.0 1.0 0.0"),
            ObjLineType::VERTEX_NORMAL);
  EXPECT_EQ(FileParser::getLineType("f 1/1/1 2/2/2"), ObjLineType::FACE);
  EXPECT_EQ(FileParser::getLineType("# This is a comment"),
            ObjLineType::COMMENT);
  EXPECT_EQ(FileParser::getLineType("mtllib material.mtl"),
            ObjLineType::MATERIAL_LIBRARY);
  EXPECT_EQ(FileParser::getLineType("invalid_prefix"), ObjLineType::UNKNOWN);
}
