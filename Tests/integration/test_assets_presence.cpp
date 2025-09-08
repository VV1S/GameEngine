#include <gtest/gtest.h>
#include <filesystem>

TEST(Integration, AssetsArePresent)
{
    namespace fs = std::filesystem;
    const fs::path base = fs::current_path() / "assets";
    ASSERT_TRUE(fs::exists(base)) << "Lack of folder: " << base.string();

    const fs::path shader0 = base / "shaders" / "Texture.glsl";
    EXPECT_TRUE(fs::exists(shader0)) << "Lack of shader: " << shader0.string();

    const fs::path shader1 = base / "shaders" / "FlatColor.glsl";
    EXPECT_TRUE(fs::exists(shader1)) << "Lack of shader: " << shader1.string();

    const fs::path texture0 = base / "textures" / "Checkerboard.png";
    EXPECT_TRUE(fs::exists(texture0)) << "Lack of texture: " << texture0.string();

    const fs::path texture1 = base / "textures" / "Ship.png";
    EXPECT_TRUE(fs::exists(texture1)) << "Lack of texture: " << texture1.string();

    const fs::path texture2 = base / "textures" / "TestCanalAlfa.png";
    EXPECT_TRUE(fs::exists(texture2)) << "Lack of texture: " << texture2.string();

    const fs::path texture3 = base / "textures" / "Triangle.png";
    EXPECT_TRUE(fs::exists(texture3)) << "Lack of texture: " << texture3.string();

    const fs::path font = base / "OpenSans-Regular.ttf";
    EXPECT_TRUE(fs::exists(font)) << "Lack of font: " << font.string();
}
