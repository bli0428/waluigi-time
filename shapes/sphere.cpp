#include "sphere.h"
#include "glm/trigonometric.hpp"

Sphere::Sphere(int p1, int p2, int p3) :
    OpenGLShape(p1 < 2 ? 2 : p1, p2 < 3 ? 3 : p2, p3, 2),
    m_ringDrawer(SquareRingDrawer()),
    m_radius(0.5f)
{
    this->initialize();
}

Sphere::Sphere(int p1, int p2, int p3, float radius) :
    OpenGLShape(p1 < 2 ? 2 : p1, p2 < 3 ? 3 : p2, p3, 2),
    m_ringDrawer(SquareRingDrawer()),
    m_radius(radius)
{
    this->initialize();
}


void Sphere::initialize() {
    this->generateVertices();
    OpenGLShape::initialize();
}

/**
 * @brief Sphere::generateVertices Generates all the vertices for the sphere
 */
void Sphere::generateVertices() {
    // calculates the angles & intervals
    float phiInterval = -M_PI / m_p1;
    float phiStart = M_PI / 2.0;

    // generates one strip for each phi interval we have
    for (int i = 0; i < m_p1; i++) {
        float phi = phiStart + phiInterval * i;
        float phiEnd = phi + phiInterval;
        this->generateStrip(phi, phiEnd);
    }
}

/**
 * @brief Sphere::generateStrip Generates the strip of rectangles (formed by triangles) along the sides of the sphere
 * @param phiStart the angle to start this strip at
 * @param phiEnd the angle to end this strip at
 */
void Sphere::generateStrip(float phiStart, float phiEnd) {
    // calculates the relevant radiuses and y-positions
    float lowerRadius = glm::cos(phiStart) * m_radius;
    float upperRadius = glm::cos(phiEnd) * m_radius;
    float lowerY = glm::sin(phiStart) * m_radius;
    float upperY = glm::sin(phiEnd) * m_radius;

    // gets coordinates from the helper m_ringDrawer
    std::vector<glm::vec3> coords = m_ringDrawer.draw(upperRadius, lowerRadius, upperY, lowerY, m_p2);

    // push them to m_coordinates
    for (glm::vec3 coord : coords) {
        OpenGLShape::pushCoord(coord);
        OpenGLShape::pushCoord(coord * 2.f);
    }
}

