#ifndef COLUMN_H
#define COLUMN_H

#include "OpenGLShape.h"
#include "trianglestripdrawer.h"
#include "squareringdrawer.h"

/**
 * @brief The Cylinder class, which inherits from OpenGLShape
 */
class Column : public OpenGLShape
{

public:
    Column(int p1, int p2);

protected:
    void initialize() override;

private:
    void generateVertices();
    void generateCap(float y, bool reverse);
    void generateRing(float upperY, float lowerY);
    void generateOffsets();

    TriangleStripDrawer m_stripDrawer;
    SquareRingDrawer m_ringDrawer;
    std::vector<glm::vec3> m_offsets;
};

#endif // COLUMN_H
