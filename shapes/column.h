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
    void generateCap();
    void generateRing(int floor);
    void generateOffsets();
    glm::vec3 getPosition(int level, int wedge);
    glm::vec3 getNormal(int level, int wedge);
    void addVertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texture);

    std::vector<glm::vec2> m_offsets;
};

#endif // COLUMN_H
