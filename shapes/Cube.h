#ifndef __CUBE_H__
#define __CUBE_H__

#include "OpenGLShape.h"
#include "glm/geometric.hpp"

/**
 * @class Cube
 *
 * Cube, which inherits from OpenGLShape.
 */
class Cube : public OpenGLShape
{

public:
    Cube(int p1, int p2, int p3);

protected:
    void initialize() override;

private:
    void generateFace(glm::vec3 start, glm::vec3 downStep, glm::vec3 rightStep, glm::vec3 normal);
    void generateVertices();
};

#endif // CUBE_H
