#include "shatter.h"
#include "glm.hpp"
#include "glm/gtx/transform.hpp"
#include "gl/shaders/CS123Shader.h"
#include <iostream>
#include "algorithm"
#include "trigonometric.hpp"

using namespace CS123::GL;

Shatter::Shatter()
{
    this->init();
}

void Shatter::init() {
    m_triangle = std::make_unique<Triangle>();
    for (int i = 0; i < M_NUMTRIANGLES; i++) {
        float y = static_cast<float>(rand()) / RAND_MAX * 2 - 1;
        float x = std::sqrt(1 - y * y);
        float scale = static_cast<float>(rand()) / RAND_MAX * 2 - 1;
        m_triangles.push_back(TriangleNode{glm::vec3(y * scale, x * glm::clamp(scale, 0.7f, 1.f), 0), glm::vec3(0, y, x), y * 5});
    }
}

void Shatter::draw(float time, glm::vec3 pos, CS123::GL::CS123Shader *shader) {
    for (TriangleNode t : m_triangles) {
        glm::vec3 newPos = glm::vec3(pos + t.velocity * time + 0.5f * M_GRAVITY * time * time);
        glm::mat4x4 translate = glm::translate(newPos);

        float rad = glm::atan(pos.x, pos.z);
        glm::mat4x4 rotate = glm::rotate(t.omega * time, t.axis) * glm::rotate(rad, glm::vec3(0, 1, 0)) * glm::rotate(3.141592f / 2, glm::vec3(1, 0, 0));
        glm::mat4x4 scale = glm::scale(glm::vec3(0.07f, 1, 0.07f));
        shader->setUniform("m", translate * rotate * scale);
        m_triangle->draw();
    }
}
