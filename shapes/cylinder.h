#ifndef CYLINDER_H
#define CYLINDER_H

#include "OpenGLShape.h"
#include "trianglestripdrawer.h"
#include "squareringdrawer.h"

/**
 * @brief The Cylinder class, which inherits from OpenGLShape
 */
class Cylinder : public OpenGLShape
{

public:
    Cylinder(int p1, int p2, int p3);

protected:
    void initialize() override;

private:
    void generateVertices();
    void generateCap(float y, bool reverse);
    void generateRing(float upperY, float lowerY);

    TriangleStripDrawer m_stripDrawer;
    SquareRingDrawer m_ringDrawer;
};

#endif // CYLINDER_H
