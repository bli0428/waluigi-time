#ifndef SPHERE_H
#define SPHERE_H

#include "OpenGLShape.h"
#include "squareringdrawer.h"

/**
 * @brief The Sphere class, which inherits from OpenGLShape.
 */
class Sphere : public OpenGLShape
{
public:
    Sphere(int p1, int p2, int p3);

protected:
    void initialize() override;

private:
    void generateVertices();
    void generateStrip(float phiStart, float phiEnd);
    SquareRingDrawer m_ringDrawer;
};

#endif // SPHERE_H
