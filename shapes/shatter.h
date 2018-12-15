#ifndef SHATTER_H
#define SHATTER_H

#include "triangle.h"

namespace CS123 { namespace GL {
    class CS123Shader;
    class Texture2D;
}}

struct TriangleNode {
   glm::vec3 velocity;
   glm::vec3 axis;
   float omega;
};

class Shatter
{
public:
    Shatter();
    void draw(float time, glm::vec3 pos, CS123::GL::CS123Shader *shader);

private:
    void init();

    std::unique_ptr<Triangle> m_triangle;
    std::vector<TriangleNode> m_triangles;

    // CONSTANTS
    int M_NUMTRIANGLES = 20;
    glm::vec3 M_GRAVITY = glm::vec3(0, -3.0f, 0);
};

#endif // SHATTER_H
