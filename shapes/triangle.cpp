#include "triangle.h"

Triangle::Triangle() : OpenGLShape(1, 1, 1, 1)
{
    this->initialize();
}

void Triangle::initialize() {
    m_coordinates.reserve(6);

    // draw both ways to prevent the reverse culling thing
    this->addVertex(glm::vec3(0, 0, 0.5), glm::vec3(0, 1, 0), glm::vec2(1, 0));
    this->addVertex(glm::vec3(-0.5, 0, -0.5), glm::vec3(0, 1, 0), glm::vec2(1, 1));
    this->addVertex(glm::vec3(0.5, 0, -0.5), glm::vec3(0, 1, 0), glm::vec2(0, 0));
    this->addVertex(glm::vec3(-0.5, 0, -0.5), glm::vec3(0, 1, 0), glm::vec2(0, 1));
    this->addVertex(glm::vec3(0, 0, 0.5), glm::vec3(0, 1, 0), glm::vec2(1, 0));
    this->addVertex(glm::vec3(0.5, 0, -0.5), glm::vec3(0, 1, 0), glm::vec2(0, 0));
    OpenGLShape::initialize();
}

void Triangle::addVertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texture) {
    OpenGLShape::pushCoord(pos);
    OpenGLShape::pushCoord(norm);
    OpenGLShape::pushCoord(texture);
}
