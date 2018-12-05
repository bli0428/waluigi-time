#include "column.h"
#include "glm/trigonometric.hpp"
#include "stdlib.h"
#include <iostream>

/**
 * @brief Column::Column
 * @param p1 - number of floors
 * @param p2 - number of wedges
 */
Column::Column(int p1, int p2) :
    OpenGLShape(p1, p2 < 3 ? 3 : p2, 1, 3)
{
    this->initialize();
}

void Column::initialize() {
    this->generateOffsets();
    this->generateVertices();
    OpenGLShape::initialize();
}

void Column::generateOffsets() {
    int numVertices = m_p2 * (m_p1 + 1);
    m_offsets.reserve(numVertices);
    for (int i = 0; i < numVertices; i++) {
        float randX = static_cast<float>(rand()) / RAND_MAX * 2.0f;
        float randY = static_cast<float>(rand()) / RAND_MAX * 2.0f;
        float randZ = static_cast<float>(rand()) / RAND_MAX * 2.0f;
        m_offsets.push_back(glm::vec3(randX - 1.0f, randY - 1.0f, randZ - 1.0f));
    }
}

glm::vec3 Column::getPosition(int level, int wedge) {
    // wrap around
    if (level > m_p1) {
        level = 0;
    }

    if (wedge >= m_p2) {
        wedge = 0;
    }

    glm::vec3 pos = glm::vec3();
    pos.y = 0.5f - static_cast<float>(level) / m_p1;

    float thetaDiff = (2 * 3.141592) / m_p2;
    float theta = thetaDiff * wedge;

    pos.x = glm::cos(theta) * 0.5;
    pos.z = glm::sin(theta) * 0.5;

    glm::vec3 offset = m_offsets[level * m_p2 + wedge];
    offset.y /= m_p1;
    offset.x /= m_p2 * 1.3;
    offset.z /= m_p2 * 1.3;
    pos += offset;

    return pos;
}

glm::vec3 Column::getNormal(int level, int wedge) {
    if (level > m_p1) {
        level = 0;
    }

    if (wedge >= m_p2) {
        wedge = 0;
    }

    glm::vec3 p = glm::vec3(getPosition(level, wedge));
    glm::vec3 n0 = glm::vec3(getPosition(level, wedge + 1) - p);
    glm::vec3 n1 = glm::vec3(getPosition(level - 1, wedge + 1) - p);
    glm::vec3 n2 = glm::vec3(getPosition(level - 1, wedge) - p);
    glm::vec3 n3 = glm::vec3(getPosition(level - 1, wedge - 1) - p);
    glm::vec3 n4 = glm::vec3(getPosition(level, wedge - 1) - p);
    glm::vec3 n5 = glm::vec3(getPosition(level + 1, wedge - 1) - p);
    glm::vec3 n6 = glm::vec3(getPosition(level + 1, wedge) - p);
    glm::vec3 n7 = glm::vec3(getPosition(level + 1, wedge + 1) - p);

    glm::vec3 norm1 = glm::normalize(glm::cross(n1, n0));
    glm::vec3 norm2 = glm::normalize(glm::cross(n2, n1));
    glm::vec3 norm3 = glm::normalize(glm::cross(n3, n2));
    glm::vec3 norm4 = glm::normalize(glm::cross(n4, n3));
    glm::vec3 norm5 = glm::normalize(glm::cross(n5, n4));
    glm::vec3 norm6 = glm::normalize(glm::cross(n6, n5));
    glm::vec3 norm7 = glm::normalize(glm::cross(n7, n6));
    glm::vec3 norm8 = glm::normalize(glm::cross(n0, n7));

    glm::vec3 average = (norm1 + norm2 + norm3 + norm4 + norm5 + norm6 + norm7 + norm8) / 8.f;

    return average;
}

/**
 * @brief Column::generateVertices Generates all the vertices for the column.
 */
void Column::generateVertices() {
    m_coordinates.reserve(2*(2 * m_p1 + 1) * m_p2 * 6 + m_p1 * 2 * (m_p2 + 1) * 6);

    // first draw the top cap
    this->generateCap();

    for (int i = 0; i < m_p1; i++) {
        this->generateRing(i);
    }

    // then draw the bottom cap... do we really need this lol
    // this->generateCap(-0.5, false);
}

/**
 * @brief Column::generateCap Generates a cap (a flat circle) at either the top or bottom of the cylinder
 * @param y The y-value/height of the cap
 * @param reverse Whether to draw the triangles in reverse order (to account for counter-clockwise order)
 */
void Column::generateCap() {
    glm::vec3 center = glm::vec3(0, 0.5, 0);

    for (int i = 0; i < m_p2; i++) {
        OpenGLShape::pushCoord(center);
        OpenGLShape::pushCoord(glm::vec3(0, 1, 0)); // normal

        OpenGLShape::pushCoord(this->getPosition(0, i));
        OpenGLShape::pushCoord(this->getNormal(0, i)); // normal

        OpenGLShape::pushCoord(this->getPosition(0, i+1));
        OpenGLShape::pushCoord(this->getNormal(0, i+1)); // normal
    }

    OpenGLShape::pushCoord(center);
    OpenGLShape::pushCoord(glm::vec3(0, 1, 0)); // normal
}

/**
 * @brief Column::generateRing Generates a ring along the barrel of the cylinder
 * @param upperFloor
 */
void Column::generateRing(int floor) {
    // push to m_coordinates, along with their normals
    for (int i = 0; i <= m_p2; i++) {
        OpenGLShape::pushCoord(this->getPosition(floor, i));
        OpenGLShape::pushCoord(this->getNormal(floor, i));

        OpenGLShape::pushCoord(this->getPosition(floor + 1, i));
        OpenGLShape::pushCoord(this->getNormal(floor + 1, i));
    }
}
