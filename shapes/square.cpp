#include "square.h"

Square::Square() : OpenGLShape(1, 1, 1, 1)
{
    this->initialize();
}

void Square::initialize() {
    m_coordinates.reserve(4);
    this->addVertex(glm::vec3(0.5, 0.5, 0), glm::vec3(0, 0, -1), glm::vec2(1, 0));
    this->addVertex(glm::vec3(0.5, -0.5, 0), glm::vec3(0, 0, -1), glm::vec2(1, 1));
    this->addVertex(glm::vec3(-0.5, 0.5, 0), glm::vec3(0, 0, -1), glm::vec2(0, 0));
    this->addVertex(glm::vec3(-0.5, -0.5, 0), glm::vec3(0, 0, -1), glm::vec2(0, 1));
    OpenGLShape::initialize();
}

void Square::addVertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texture) {
    OpenGLShape::pushCoord(pos);
    OpenGLShape::pushCoord(norm);
    OpenGLShape::pushCoord(texture);
}
