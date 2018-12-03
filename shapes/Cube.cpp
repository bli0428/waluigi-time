#include "Cube.h"
#include "OpenGLShape.h"
#include <iostream>
#include "glm/geometric.hpp"

Cube::Cube(int p1, int p2, int p3) :
    OpenGLShape(p1, p2, p3, 0)
{
    this->initialize();
}

void Cube::initialize() {
    this->generateVertices();
    OpenGLShape::initialize();
}

/**
 * @brief Cube::generateVertices Calculates all the vertices for each cube and adds them to m_coordinates.
 */
void Cube::generateVertices() {
    m_coordinates.reserve(2*(m_p1 + 1) * m_p1 * 6 * 6); // each face has m_p1 strips of 2*(m_p1+1) vertices, and there are 6 faces and 6 elements for each vertice

    float stepInterval = 1.0 / m_p1;
    // front face
    this->generateFace(glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0.0, -stepInterval, 0.0), glm::vec3(stepInterval, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));

    // right face
    this->generateFace(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, -stepInterval, 0.0), glm::vec3(0.0, 0.0, -stepInterval), glm::vec3(1.0, 0.0, 0.0));

    // down face
    this->generateFace(glm::vec3(0.5, -0.5, -0.5), glm::vec3(0.0, 0.0, stepInterval), glm::vec3(-stepInterval, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));

    // left face
    this->generateFace(glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0.0, stepInterval, 0.0), glm::vec3(0.0, 0.0, -stepInterval), glm::vec3(-1.0, 0.0, 0.0));

    // up face
    this->generateFace(glm::vec3(-0.5, 0.5, 0.5), glm::vec3(stepInterval, 0.0, 0.0), glm::vec3(0.0, 0.0, -stepInterval), glm::vec3(0.0, 1.0, 0.0));

    // back face
    this->generateFace(glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.0, -stepInterval, 0.0), glm::vec3(-stepInterval, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
}

/**
 * @brief Cube::generateFace Adds an entire face's coordinates to m_coordinates.
 * @param start The coordinate to start from.
 * @param downStep Which way to move "downwards" when drawing the triangles.
 * @param rightStep Which way to move "right" when drawing the triangles.
 * @param normal Which way the normal points for this face. (Given that this is a cube, all the normals on a face point the same way.)
 */
void Cube::generateFace(glm::vec3 start, glm::vec3 downStep, glm::vec3 rightStep, glm::vec3 normal) {
    for (int row = 0; row < m_p1; row++) {
        for (int col = 0; col <= m_p1; col++) {
            this->pushCoord(start + rightStep * static_cast<float>(col) + downStep * static_cast<float>(row));
            this->pushCoord(normal);
            this->pushCoord(start + rightStep * static_cast<float>(col) + downStep * static_cast<float>(row + 1));
            this->pushCoord(normal);
        }
    }
}
