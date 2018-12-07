#include "column.h"
#include "glm/trigonometric.hpp"
#include "stdlib.h"
#include <iostream>
#include "algorithm"

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
        float randX = static_cast<float>(rand()) / RAND_MAX * 2;
        float randY = static_cast<float>(rand()) / RAND_MAX * 2;
        m_offsets.push_back(glm::vec2(randX - 1, randY - 1)); // results in two numbers between -1 and 1
    }
}

glm::vec3 Column::getPosition(int level, int wedge, bool raw = false) {
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

    if (!raw || raw) {
        return pos;
    }

    glm::vec2 offset = m_offsets[level * m_p2 + wedge];
    offset.y /= m_p1 * 2;
    offset.x = (offset.x * 2 - 1) / M_SMOOTHNESS;

    glm::vec3 projected = glm::vec3(pos.x, 0.0f, pos.z);
    pos += projected * offset.x;
    pos.y += offset.y;

    return pos;
}

/**
 * @brief Column::getNormal Returns the normal given a level number and # of wedges rotated.
 * @param level
 * @param wedge
 * @return
 */
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
}

/**
 * @brief Column::generateCap Generates a cap (a flat circle) at either the top or bottom of the cylinder
 * @param y The y-value/height of the cap
 * @param reverse Whether to draw the triangles in reverse order (to account for counter-clockwise order)
 */
void Column::generateCap() {
    glm::vec3 center = glm::vec3(0, 0.5, 0);

    for (int i = 0; i < m_p2; i++) {
        this->addVertex(center, glm::vec3(0, 1, 0), getUV(glm::vec3(0, 1, 0))); // draw back to center
        this->addVertex(this->getPosition(0, i), this->getNormal(0, i), getUV(this->getPosition(0, i, true))); // draw out to circumference
        this->addVertex(this->getPosition(0, i+1), this->getNormal(0, i+1), getUV(this->getPosition(0, i+1, true))); // draw one wedge over on circumference
    }

    this->addVertex(center, glm::vec3(0, 1, 0), getUV(glm::vec3(0, 1, 0))); // draw back to center to close
}

/**
 * @brief Column::generateRing Generates a ring along the barrel of the cylinder
 * @param upperFloor
 */
void Column::generateRing(int floor) {
    // push to m_coordinates, along with their normals
    for (int i = 0; i <= m_p2; i++) {
        this->addVertex(this->getPosition(floor, i), this->getNormal(floor, i), getUV(this->getPosition(floor, i, true))); // draw on this floor
        this->addVertex(this->getPosition(floor + 1, i), this->getNormal(floor + 1, i), getUV(this->getPosition(floor + 1, i, true))); // draw one floor down
    }
}

void Column::addVertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texture) {
    OpenGLShape::pushCoord(pos);
    OpenGLShape::pushCoord(norm);
    OpenGLShape::pushCoord(texture);
}

glm::vec2 Column::getUV(glm::vec3 point) {
    float epsilon = 0.001f;

    // top cap
    if (std::abs(point.y - 0.5) < epsilon) {
        return glm::vec2(point.x + 0.5f, point.z + 0.5f);

    // bottom cap
    } else if (std::abs(point.y + 0.5) < epsilon) {
        return glm::vec2(point.x + 0.5f, 0.5f - point.z);

    // body
    } else {
        float theta = atan2(point.z, point.x);
        if (theta < 0) {
            return glm::vec2(-theta / (2.f * 3.14159f), 0.5f - point.y);
        } else {
            return glm::vec2(1.f - theta / (2.f * 3.14159f), 0.5f - point.y);
        }
    }
}
