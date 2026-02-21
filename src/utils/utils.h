#include <raylib.h>
#include <rlgl.h>

#include <random>
#include <sstream>

// https://stackoverflow.com/questions/24365331/how-can-i-generate-uuid-in-c-without-using-boost-library
std::string generate_uuid_v4();

// https://www.raylib.com/examples/models/loader.html?name=models_textured_cube
void DrawCubeTexture(Texture2D texture, Vector3 position, float width,
    float height, float length, Color color);