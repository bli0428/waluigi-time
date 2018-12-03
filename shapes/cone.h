#ifndef CONE_H
#define CONE_H

#include "OpenGLShape.h"
#include "glm/geometric.hpp"
#include "trianglestripdrawer.h"
#include "squareringdrawer.h"

/**
 * @brief The Cone class, which inherits from OpenGLShape.
 */
class Cone : public OpenGLShape
{

public:
    Cone(int p1, int p2, int p3);

protected:
    void initialize() override;

private:
    void generateVertices();
    void generateSide(float startArcAngle, float endArcAngle);
    void generateBase();
    TriangleStripDrawer m_triangleDrawer;
    SquareRingDrawer m_ringDrawer;
};

#endif // CONE_H
