#include "cylinder.h"
#include "glm/trigonometric.hpp"

Cylinder::Cylinder(int p1, int p2, int p3) :
    OpenGLShape(p1, p2 < 3 ? 3 : p2, p3, 3),
    m_stripDrawer(TriangleStripDrawer()),
    m_ringDrawer(SquareRingDrawer())
{
    this->initialize();
}

void Cylinder::initialize() {
    this->generateVertices();
    OpenGLShape::initialize();
}

/**
 * @brief Cylinder::generateVertices Generates all the vertices for the cylinder.
 */
void Cylinder::generateVertices() {
    m_coordinates.reserve(2*(2 * m_p1 + 1) * m_p2 * 6 + m_p1 * 2 * (m_p2 + 1) * 6);

    // first draw the top cap
    this->generateCap(0.5, true);

    // then draw the rings of squares along the barrels
    float heightStart = -0.5;
    float heightInterval = 1.0 / m_p1;

    for (int i = 0; i < m_p1; i++) {
        this->generateRing(heightStart + heightInterval * i, heightStart + heightInterval * (i+1));
    }

    // then draw the bottom cap
    this->generateCap(-0.5, false);
}

/**
 * @brief Cylinder::generateCap Generates a cap (a flat circle) at either the top or bottom of the cylinder
 * @param y The y-value/height of the cap
 * @param reverse Whether to draw the triangles in reverse order (to account for counter-clockwise order)
 */
void Cylinder::generateCap(float y, bool reverse) {
    // get the coordinates from the helper
    std::vector<glm::vec3> coords = m_stripDrawer.drawCircle(glm::vec3(0, y, 0), 0.5, m_p2, m_p1, reverse);

    // push to m_coordinates, along with their normals
    for (glm::vec3 coord : coords) {
        OpenGLShape::pushCoord(coord);
        OpenGLShape::pushCoord(glm::vec3(0, 2*y, 0));
    }
}

/**
 * @brief Cylinder::generateRing Generates a ring along the barrel of the cylinder
 * @param upperY y-pos of the ring's top
 * @param lowerY y-pos of the ring's bottom
 */
void Cylinder::generateRing(float upperY, float lowerY) {
    // get the coordinates from the helper
    std::vector<glm::vec3> coords = m_ringDrawer.draw(0.5, 0.5, upperY, lowerY, m_p2);

    // push to m_coordinates, along with their normals
    for (glm::vec3 coord : coords) {
        OpenGLShape::pushCoord(coord);
        OpenGLShape::pushCoord(glm::vec3(coord.x * 2.f, 0, coord.z * 2.f));
    }
}
