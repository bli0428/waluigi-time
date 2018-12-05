#include "cone.h"
#include "OpenGLShape.h"
#include "glm/trigonometric.hpp"
#include "math.h"
#include <iostream>

Cone::Cone(int p1, int p2, int p3) :
    OpenGLShape(p1, p2 < 3 ? 3 : p2, p3, 1),
    m_triangleDrawer(TriangleStripDrawer()),
    m_ringDrawer(SquareRingDrawer())
{
    this->initialize();
}

void Cone::initialize() {
    this->generateVertices();
    OpenGLShape::initialize();
}

/**
 * @brief Cone::generateVertices Generates all the vertices for the cone and pushes them to m_coordinates.
 */
void Cone::generateVertices() {
    // p1 is number of horizontal strips, and p2 is number of sides for the base
    // the theta-intervals between sides is 2*PI/p2
    m_coordinates.reserve(2*(2*(m_p1+1) * m_p2 * 6)); // 2(m_p1+1) vertices per face, m_p2 faces, and 6 elements for each, and double that amount for the base

    // interval of angles the sides are divided into
    float thetaInterval = 2 * M_PI / m_p2;

    for (int side = 0; side < m_p2; side++) {
        // generates one side for each interval segment
        this->generateSide(thetaInterval*side, thetaInterval*(side+1));
    }

    // lastly, generates the base vertices.
    this->generateBase();
}

/**
 * @brief Cone::generateStrip Generates a single triangle strip of the cone's sides
 * @param startArcAngle
 * @param endArcAngle
 * @param isBase whether this is for the base or the side of the cone
 */
void Cone::generateSide(float startArcAngle, float endArcAngle) {
    // calculates the Cartesian coordinates for the vertices of the strip we're drawing
    glm::vec3 tip = glm::vec3(0, 0.5, 0);
    glm::vec3 bottomLeft = glm::vec3(glm::cos(startArcAngle) * 0.5, -0.5, glm::sin(startArcAngle) * 0.5);
    glm::vec3 bottomRight = glm::vec3(glm::cos(endArcAngle) * 0.5, -0.5, glm::sin(endArcAngle) * 0.5);

    // these are used for normal calculation
    float normalY = 1 / std::sqrt(5);
    float normalXZ = 2 / std::sqrt(5);

    // get the coordinates of the whole strip from m_drawer, the helper class
    std::vector<glm::vec3> coords = m_triangleDrawer.draw(tip, bottomLeft, bottomRight, m_p1);

    // push them into m_coordinates along with their calculated normals
    for (int i = 0; i < static_cast<int>(coords.size()); i += 2) {
        OpenGLShape::pushCoord(coords[i]);
        OpenGLShape::pushCoord(glm::vec3(glm::cos(startArcAngle) * normalXZ, normalY, glm::sin(startArcAngle) * normalXZ));
        OpenGLShape::pushCoord(glm::vec2(0, 0));

        OpenGLShape::pushCoord(coords[i+1]);
        OpenGLShape::pushCoord(glm::vec3(glm::cos(endArcAngle) * normalXZ, normalY, glm::sin(endArcAngle) * normalXZ));
        OpenGLShape::pushCoord(glm::vec2(0, 0));
    }
}

/**
 * @brief Cone::generateBase Generates the base of the cone.
 */
void Cone::generateBase() {
    // uses m_drawer, a triangle strip drawer to do this
    std::vector<glm::vec3> coords = m_triangleDrawer.drawCircle(glm::vec3(0, -0.5, 0), 0.5, m_p2, m_p1, false);

    // pushes them into m_coordinates, along with their normals
    for (glm::vec3 coord : coords) {
        OpenGLShape::pushCoord(coord);
        OpenGLShape::pushCoord(glm::vec3(0, -1, 0));
        OpenGLShape::pushCoord(glm::vec2(0, 0));
    }
}
